function [models] = M2FED_training(rootDirName, nMixtures, ubm, only_ubm, only_1_microphone)

fprintf(1, 'Training directory: %s\n', rootDirName);

% Set the parameters of the experiment
folders = dir(strcat(rootDirName,'\singles'));
nSpeakers = length(folders) - 2;
if nargin < 2
    nMixtures = 256;        % How many mixtures used to generate data
end
if nargin < 5
    only_1_microphone = 0;
end
nWorkers = feature('NumCores');           % Number of parfor workers, if available

models = {};

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = cell(nSpeakers, 1);

times = length(folders) - 2;
if(only_ubm)
    times = 1;
end

parfor i = 1 : times
    if(~isempty(ubm) && i == 1)
        continue;    %ubm exists, so no need to recalculate ubm
    end
    
    files = dir(strcat(rootDirName,'\singles\',folders(i+2).name));
    
    num_files = length(files) - 2;
    currSpeakerData = cell(num_files,1);      
    for j = 1 : length(files) - 2
        fprintf(1, 'Training input file %s\n', strcat(rootDirName, '\singles\', folders(i+2).name, '\', files(j+2).name));
        [Y,FS] = audioread(strcat(rootDirName, '\singles\', folders(i+2).name, '\', files(j+2).name));
        
        [b,a] = butter(3, [100/(FS/2), 3500/(FS/2)], 'bandpass');
        if only_1_microphone == 1
        	Y = Y(:,1);
        end
        
        for channel = 1 : size(Y, 2)    % read every channel's data            
            Y(:,channel) = filter(b,a,Y(:,channel));
            voicing = M2FED_vad_g729(Y(:,channel), FS, FS*0.01);
            fprintf(1, '%d\n', sum(voicing) / length(voicing));

            speaker_signal = [];
            for k = 1 : size(voicing,1)
                if voicing(k) == 1
                   speaker_signal = [speaker_signal; Y((k-1)*FS*0.01 + 1: k*FS*0.01,channel)];
                end
            end            
            if ~isempty(speaker_signal)
                all_training_featuresSpeaking = M2FED_wav_speech_features(speaker_signal, FS);
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

if(isempty(ubm))
    ubm = gmm_em(trainSpeakerData{1}, nmix, final_niter, ds_factor, nWorkers);
    save(strcat(rootDirName,'/models_ubm', num2str(nMixtures),'.mat'), 'ubm');
end

models.ubm = ubm;

if(~only_ubm)
    % Adapt the UBM to each speaker to create GMM speaker model.
    map_tau = 10.0;
    config = 'mwv';
    gmm = cell(nSpeakers - 1, 1);
    parfor s=1:nSpeakers - 1
        speakerFeature = [];
        for i = 1 : length(trainSpeakerData{s+1})
            speakerFeature = [speakerFeature trainSpeakerData{s+1}{i}];
        end
        speakerFeatureCell = cell(1, 1);
        speakerFeatureCell{1} = speakerFeature;
        gmm{s} = mapAdapt(speakerFeatureCell, ubm, map_tau, config);
    end

    models.gmm = gmm;
    save(strcat(rootDirName,'/models_', num2str(nMixtures),'.mat'), 'models');
end

end
