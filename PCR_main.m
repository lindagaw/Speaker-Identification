% This serves as the main function for the acoustic pipeline in PCR.
% This calls training_or_load_models and M2FED_detection.m for speaker identification.

function identity = PCR_main(fname)

    display(fname)

    % turn off the warnings
    identity = 'not registered speaker';
    
    warning('off','all');

    addpath(genpath('speaker_id_module'));

    % default setting for parameters
    nMixtures = 1024;
    rootDirName = 'speaker_id_module//SpeakerID';
    session_speaking_percentage = 0.5;
    sessionSize = 5;
    
    sampling_rate = 44100;
    channels = 1;
    
    gmm_threshold = 0.6;
    
    [ models, nSpeakers ] = training_or_load_models (rootDirName, nMixtures);

    %[gmmResults, session_speaking, speakerIds, values, num_sessions] ...
    %    = M2FED_detection(fname, nSpeakers, models, session_speaking_percentage, sessionSize);
    
    [speakerIds] = M2FED_realtime_detection(fname, sampling_rate, channels, ...
                        nSpeakers, models, session_speaking_percentage, ...
                        sessionSize, gmm_threshold);
    

    sid_mode = speakerIds
    identity = sid_mode;
    
    disp('raw identification - the nth simulation that has had the highiest probability:');
    disp(sid_mode);

    if sid_mode <= 28 && sid_mode >= 1
        identity = 1;
    elseif sid_mode <= 56 && sid_mode >= 29
        identity = 2;
    else
        identity = 0;
    end
end