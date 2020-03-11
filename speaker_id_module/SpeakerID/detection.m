function [gmmResults, session_speaking, speakerIds, values, num_sessions] ...
    = detection(fname, nSpeakers, models, session_speaking_percentage, sessionSize)

% speakerId = -1 non-family
% speakerId = 0 non-speaking
% speakerID > 0 speakerID

% in each session we detect 500 ms frames data to make decision
if (nargin < 5)
    sessionSize = 500;
end

% if there are more than 'session_speaking_percentage' speaking in a session, we say it speaking.
if (nargin < 4)
    session_speaking_percentage = 0.5;
end

% Get all detection data feature from file
[Y,FS] = audioread(fname);

if(FS ~= 16000)
    Y = resample(Y, 16000, FS);
    FS = 16000;
end

[b,a] = butter(3, [100/(FS/2), 3500/(FS/2)], 'bandpass');
channelCount = size(Y, 2);

for channel = 1 : channelCount    % filter and voicing by channels
    Y(:,channel) = filter(b,a,Y(:,channel));
    voicing(:,channel) = vad_g729(Y(:,channel), FS, FS*0.01);
    fprintf(1, 'Channel %d: %d\n', channel, sum(voicing(:,channel)) / length(voicing(:,channel)));
    total_voicing_size(channel) = size(voicing(:,channel),1);
    num_sessions(channel) = floor(total_voicing_size(channel) * 10 / sessionSize);
end

session_speaking = zeros(num_sessions(1), channelCount);
speakerIds = zeros(num_sessions(1), channelCount);
values = zeros(num_sessions(1), channelCount);
gmmResults = cell(num_sessions(1), channelCount);

for channel = 1 : channelCount
    for i = 1 : num_sessions(1)
        start_frame = (i - 1) * sessionSize / 10 + 1;
        end_frame = min(i * sessionSize / 10, total_voicing_size(channel));

        % State 0 is "not voiced," state 1 is "voiced." 
        % if more than session_speaking_percentage speaking, we say it speaking.
        if( sum(voicing(start_frame:end_frame ,channel)) / (end_frame - start_frame) >= session_speaking_percentage)
            session_speaking(i ,channel) = 1;
            speaker_signal = [];
            session_signal = Y( (start_frame-1)*FS*0.01+1 : end_frame*FS*0.01 ,channel );
            for k = 1 : end_frame-start_frame+1
                if voicing(k + start_frame - 1 ,channel) == 1
                    speaker_signal = ...
                        [speaker_signal; session_signal((k-1)*FS*0.01 + 1: k*FS*0.01)];
                end
            end
            % this is 1 intentionally because detection_algorithm takes cell as the first input parameter
            nonSlientData{1} = wav_speech_features(speaker_signal, FS);    
            nonSlientData{1} = nonSlientData{1}(11:49,:);
            
            [ gmmResults{i ,channel}, speakerIds(i ,channel), values(i ,channel) ] = ...
                detection_algorithm(nonSlientData(1), models.gmm, models.ubm, nSpeakers, session_speaking(i ,channel));
        else
            session_speaking(i ,channel) = 0;
            [ gmmResults{i ,channel}, speakerIds(i ,channel), values(i ,channel) ] = ...
                detection_algorithm([], models.gmm, models.ubm, nSpeakers, session_speaking(i ,channel));
        end
    end
end

end
