function models = training_get_and_save_feature(rootDirName)

fprintf(1, 'Training directory: %s\n', rootDirName);

% Set the parameters of the experiment
folders = dir(strcat(rootDirName,'/singles'));
nSpeakers = length(folders) - 2;
nWorkers = feature('NumCores');           % Number of parfor workers, if available

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = cell(nSpeakers, 1);
times = length(folders) - 2;

parfor i = 2 : times
    
    folder_name = folders(i+2).name;
    files = dir(strcat(rootDirName,'/singles/',folder_name));
    
    num_files = length(files) - 2;
    currSpeakerData = cell(num_files,1);    
    for j = 1 : length(files) - 2
        file_name = files(j+2).name;
        
        fprintf(1, 'Training input file %s\n', strcat(rootDirName, '/singles/', folder_name, '/', file_name));
        [Y,FS] = audioread(strcat(rootDirName, '/singles/', folder_name, '/', file_name));
        
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

save(strcat(rootDirName,'/speaker_feature.mat'), 'trainSpeakerData','-v7.3');

end
