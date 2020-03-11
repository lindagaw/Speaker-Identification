function [ gmmResults, speakerIds, values ] = ...
    detection_algorithm(testSpeakerData, gmm, ubm, nSpeakers, session_speaking, gmm_threshold)
% get the detection results for single session data, only output the accept
% decisions. if all reject decisions, output speakerIds as -1 (ubm, others). if
% non-speaking, output 0. 

if nargin < 6
    gmm_threshold = 0.6;
end

if session_speaking == 0    % non-speaking
    gmmResults = zeros(1,nSpeakers);
    speakerIds = 0;
    values = 0;
else                        % speaking
    % Calculate the score for each model versus each speaker's data.
    % Generate a list that tests each model against all the session.
    trials = zeros(nSpeakers, 2);
    for ix = 1 : nSpeakers
        % b = ix;
        % e = b;
        % trials(b:e, :)  = [ix * ones(1,1), (1:1)'];
        trials(ix, :)  = [ix, 1];
    end        
    
    gmmScores = score_gmm_trials(gmm, testSpeakerData, trials, ubm);
    gmmResults = reshape(gmmScores, 1, nSpeakers);

    % if the gmmResult is negative, it means it is more likely as an
    % ubm than a speaker's model. So we get the reject decision. Otherwise
    % we get the positive number means the accept decision. 

    % find the maximum value of the log-likelihood
    [values, speakerIds] = max(gmmResults,[],2);
    
    % disp(gmmResults);

    if values <= gmm_threshold
       % all gmmResults are negative which means all reject decisions.
       % we cannot make decision in this situation. So we output others.

       speakerIds = -1;
       values = 0;
    else
       % at least one gmmResult are positive which means at least we get
       % one accept decision. We make decision based on the largest
       % log-likelihood which means the chances to be accepted is largest.
       
       % here we let the values, speakerIds to be as-is.       
    end
end

end

