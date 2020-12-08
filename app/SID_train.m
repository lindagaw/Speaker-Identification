function identity = SID_train()
    cd '..'
    
    warning('off','all');

    addpath(genpath('speaker_id_module//'));
    % default setting for parameters
    nMixtures = 1024;
    rootDirName = 'speaker_id_module//SpeakerID';
    
    disp(strcat('the newly trained SID will be saved as ', '..', rootDirName, 'models_1024.mat'));
    session_speaking_percentage = 0.5;
    sessionSize = 5;
    
    sampling_rate = 44100;
    channels = 1;
    
    gmm_threshold = 0.6;

    M2FED_training_script(rootDirName, nMixtures)
    

end