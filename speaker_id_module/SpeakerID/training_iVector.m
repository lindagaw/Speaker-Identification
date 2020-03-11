function models = training_iVector(rootDirName, nMixtures, background_models, only_backgroundSpeaker, only_1_microphone)

fprintf(1, 'Training directory: %s\n', rootDirName);

if nargin < 2
    nMixtures = 256;        % How many mixtures used to generate data
end
if nargin < 4
    only_backgroundSpeaker = 0;
end
if nargin < 5
    only_1_microphone = 0;
end

% Set the parameters of the experiment
folders = dir(strcat(rootDirName,'\singles'));
nSpeakers = length(folders) - 2;
nWorkers = feature('NumCores');           % Number of parfor workers, if available

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = cell(nSpeakers, 1);

times = length(folders) - 2;
if(only_backgroundSpeaker)
    times = 1;
end

for i = 1 : times
    if(~isempty(background_models) && i == 1)
        continue;    % background_models exists, so no need to recalculate background speaker features
    end
    
    folder_name = folders(i+2).name;
    files = dir(strcat(rootDirName,'\singles\',folder_name));
    
    num_files = length(files) - 2;
    currSpeakerData = cell(num_files,1);    
    parfor j = 1 : length(files) - 2
        file_name = files(j+2).name;
        
        fprintf(1, 'Training input file %s\n', strcat(rootDirName, '\singles\', folder_name, '\', file_name));
        [Y,FS] = audioread(strcat(rootDirName, '\singles\', folder_name, '\', file_name));
        
        if only_1_microphone == 1
            Y = Y(:,1);
        end
        
        for channel = 1 : size(Y, 2)    % read every channel's data
            [b,a] = butter(3, [100/(FS/2), 3500/(FS/2)], 'bandpass');
            Y(:,channel) = filter(b,a,Y(:,channel));
            voicing = vad_g729(Y(:,channel), FS, FS*0.01);
            fprintf(1, '%d\n', sum(voicing) / length(voicing));

            speaker_signal = [];
            for k = 1 : size(voicing,1)
                if voicing(k) == 1
                   speaker_signal = [speaker_signal; Y((k-1)*FS*0.01 + 1: k*FS*0.01,channel)];
                end
            end            
            if ~isempty(speaker_signal)
                all_training_featuresSpeaking = wav_speech_features(speaker_signal, FS);
                currSpeakerData{j} = [currSpeakerData{j}, all_training_featuresSpeaking(11:49,:)];
            end
        end
    end
    trainSpeakerData{i} = currSpeakerData;
end

% Create the universal background model from the first wav file's speaker data
nmix = nMixtures;           % In this case, we know the # of mixtures needed
final_niter = 10;
ds_factor = 1;

if(isempty(background_models))
    % Create the universal background model
    ubm = gmm_em(trainSpeakerData{1}, nmix, final_niter, ds_factor, nWorkers);

    % Calculate the statistics needed for training T from background speakers.
    ubmSpeakerData = trainSpeakerData{1};
    totalnumberofbackgroundspeakers = size(ubmSpeakerData,1);    
    stats = cell(totalnumberofbackgroundspeakers,1);
    parfor s=1:totalnumberofbackgroundspeakers
        [N,F] = compute_bw_stats(ubmSpeakerData{s}, ubm);
        stats{s} = [N; F];
    end

    % Learn the total variability subspace from all the speaker data.
    tvDim = 400;
    niter = 5;
    T = train_tv_space(stats, ubm, tvDim, niter, nWorkers);    
    
    % Get development i-vectors
    devIVs = zeros(tvDim, totalnumberofbackgroundspeakers);
    parfor s=1:totalnumberofbackgroundspeakers
        devIVs(:, s) = extract_ivector(stats{s}, ubm, T);        
    end
    
    % Get speaker labs from background speakers, 10 recordings * 221 speakers
    speakerID = 1:221;
    for i = 1:9
        speakerID = cat(1, speakerID, 1:221);
    end    
    speakerID = reshape(speakerID, [], 1);

    % Now do LDA on the iVectors to find the dimensions that matter.
    ldaDim = 200;
    V = lda(devIVs, speakerID);
    finalDevIVs = V(:, 1:ldaDim)' * devIVs;

    % Now train a Gaussian PLDA model with development i-vectors
    nphi = ldaDim;                  % should be <= ldaDim
    niter = 10;
    pLDA = gplda_em(finalDevIVs, speakerID, nphi, niter);
    
    background_models.ubm = ubm;
    background_models.pLDA = pLDA;
    background_models.T = T;
    background_models.V = V;
    background_models.ldaDim = ldaDim;
    background_models.tvDim = tvDim;
    save(strcat(rootDirName,'/backgroud_models_', num2str(nMixtures),'.mat'), 'background_models');
end

if(~only_backgroundSpeaker)
    models = background_models;    
    ubm = background_models.ubm;
    T = background_models.T;
    V = background_models.V;
    ldaDim = background_models.ldaDim;
    tvDim = background_models.tvDim;
    
    % Now compute the ivectors for each speaker
    modelIVs = zeros(tvDim, nSpeakers-1);
    
    parfor s=1:nSpeakers-1     
        for i = 1:length(trainSpeakerData{s+1})            
            [n, f] = compute_bw_stats(trainSpeakerData{s+1}{i}, ubm);
            modelIVs(:, s) = modelIVs(:, s) + extract_ivector([n;f], ubm, T);
        end        
        modelIVs(:, s) = modelIVs(:, s) / length(trainSpeakerData{s+1});  % take the average
    end

    % Now do LDA on the iVectors to find the dimensions that matter.
    modelIVs = V(:, 1:ldaDim)' * modelIVs;

    % save the model
    models.modelIVs = modelIVs;
    models.nSpeakers = nSpeakers-1;

    save(strcat(rootDirName,'/models_', num2str(nMixtures),'.mat'), 'models');
end

end
