function [gmmResults, speakerIds, values] = opensmile_detection(featureFile, nSpeakers, models)

% speakerId = -1 non-family
% speakerId = 0 non-speaking
% speakerID > 0 speakerID

speakerIds = zeros(1, 1);
values = zeros(1, 1);
gmmResults = cell(1, 1);

[numbers, ~] = swallow_csv(featureFile);
numbers = numbers';

%for i = 1 : num_sessions(1)
%    start_frame = (i - 1) * sessionSize / 10 + 1;
%    end_frame = min(i * sessionSize / 10, total_voicing_size(channel));

    % State 0 is "not voiced," state 1 is "voiced." 
    % if more than session_speaking_percentage speaking, we say it speaking.

    % this is 1 intentionally because detection_algorithm takes cell as the first input parameter

nonSlientData{1} = numbers;

[ gmmResults{1 ,1}, speakerIds(1 ,1), values(1 ,1) ] = ...
    detection_algorithm(nonSlientData(1), models.gmm, models.ubm, nSpeakers, 1);

%end


end
