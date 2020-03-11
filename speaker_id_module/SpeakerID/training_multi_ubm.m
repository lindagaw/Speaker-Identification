function [models] = training_multi_ubm(rootDirName, nMixtures)

folders_all_ubm = dir(rootDirName);
num_ubms = length(folders_all_ubm) - 2;

for ubm_index = 1 : num_ubms
    fprintf(1, 'Training directory: %s\n', folders_all_ubm(ubm_index + 2).name);

    % Set the parameters of the experiment
    folders = dir(strcat(rootDirName,'/',folders_all_ubm(ubm_index + 2).name));
    nSpeakers = 1;
    nWorkers = feature('NumCores');           % Number of parfor workers, if available

    models = {};

    % Get sound feature from every WAV file in the training folder.
    trainSpeakerData = cell(nSpeakers, 1);
    
    files = dir(strcat(rootDirName,'/',folders_all_ubm(ubm_index + 2).name, '/', folders(3).name));
    num_files = length(files) - 2;
    currSpeakerData = cell(num_files,1);   
    
    curr_folder = strcat(rootDirName,'/',folders_all_ubm(ubm_index + 2).name, '/', folders(3).name);
    
    parfor j = 1 : length(files) - 2
        fprintf(1, 'Training input file %s\n', ...
            strcat(curr_folder, '/', files(j+2).name));
        [Y,FS] = audioread(strcat(curr_folder, '/', files(j+2).name));

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
    trainSpeakerData{1} = currSpeakerData;

    % Create the universal background model from the first wav file's speaker data
    nmix = nMixtures;           % In this case, we know the # of mixtures needed
    final_niter = 10;
    ds_factor = 1;

    ubm = gmm_em(trainSpeakerData{1}, nmix, final_niter, ds_factor, nWorkers);

    indexes = strfind(folders_all_ubm(ubm_index + 2).name, '-');
    model_name = strcat('models_ubm_', num2str(nMixtures), '_', folders_all_ubm(ubm_index + 2).name(indexes(1)+1:end),'.mat');

    save(strcat(rootDirName, '/../models/', model_name), 'ubm');

end

end
