function [ score, speakerIds, values ] = ...
    detection_algorithm_iVector(testSpeakerData, pLDA, ubm, modelIVs, T, V, ldaDim, nSpeakers, session_speaking)
% get the detection results for single session data, only output the accept
% decisions. if all reject decisions, output speakerIds as -1 (ubm, others). if
% non-speaking, output 0. 

if session_speaking == 0    % non-speaking
    score = zeros(1,nSpeakers);
    speakerIds = 0;
    values = 0;
else                        % speaking
    % Step4.2: Now compute the ivectors for the test set 
    % and score the utterances against the models
    [N, F] = compute_bw_stats(testSpeakerData, ubm);
    testIVs = extract_ivector([N; F], ubm, T);

    finalTestIVs = V(:, 1:ldaDim)' * testIVs;
    score = score_gplda_trials(pLDA, modelIVs, finalTestIVs);

    score = reshape(score, 1, []);

    % if the score is negative, it means it is more likely as an
    % ubm than a speaker's model. So we get the reject decision. Otherwise
    % we get the positive number means the accept decision. 

    % find the maximum value of the log-likelihood
    [values, speakerIds] = max(score,[],2);
    
%     if values <= 0
%        % all score are negative which means all reject decisions.
%        % we cannot make decision in this situation. So we output others.
%        speakerIds = -1;
%        values = 0;
%     else
%        % at least one score are positive which means at least we get
%        % one accept decision. We make decision based on the largest
%        % log-likelihood which means the chances to be accepted is largest.
%        
%        % here we let the values, speakerIds to be as-is.       
%     end
end

end

