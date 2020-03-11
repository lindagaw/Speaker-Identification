% This serves as the main function for the acoustic pipeline in PCR.
% This calls training_or_load_models and M2FED_detection.m for speaker identification.

function id = PCR_main (fname)
    % turn off the warnings
    id = 'not registered speaker';
    
    warning('off','all');
    addpath(genpath('.'));

    % default setting for parameters
    nMixtures = 1024;
    rootDirName = '/Users/yegao/Desktop/PCR_pipeline';
    session_speaking_percentage = 0.5;
    sessionSize = 5;
    
    sampling_rate = 44100;
    channels = 1;
    
    gmm_threshold = 0.6;
    
    [ models, nSpeakers ] = training_or_load_models (rootDirName, nMixtures);

    %[gmmResults, session_speaking, speakerIds, values, num_sessions] ...
    %    = M2FED_detection(fname, nSpeakers, models, session_speaking_percentage, sessionSize);
    
    [num_count, speakerIds] = M2FED_realtime_detection(fname, sampling_rate, channels, ...
                        nSpeakers, models, session_speaking_percentage, ...
                        sessionSize, gmm_threshold)
    
    %speakerIds = speakerIds(speakerIds ~= 0);
    %id = mode(speakerIds);
    %disp(id);
    
    if speakerIds == -1
        id = 'not registered speaker';
    end
    
    
    if speakerIds <= 28 && speakerIds >= 0
        speakerIds = 1;
        id = 'Ashley';
    end
    

    
end