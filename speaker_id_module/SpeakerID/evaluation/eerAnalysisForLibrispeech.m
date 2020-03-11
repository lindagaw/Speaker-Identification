%set result_folder (with the wav files), sessionSize first
mixtures = 512;
% model_count_per_speaker = 81;
model_count_per_speaker = 1;

% channel_index = 1; % for control lab

channel_index = 1; % change for librispeech
% channel_index = 2; % change for librispeech

score = [];
label = []; % 1 for target, 0 for non-target

for folder_i = 1 : length(result_folder)
    % load results
%     resultsAll = load(strcat(result_folder{folder_i}, '\gmm_',num2str(mixtures),'_',num2str(sessionSize),'.mat'));
%     resultsAll = resultsAll.gmmResultsAll;
    
    resultsAll = load(strcat(result_folder{folder_i}, '\iVector_',num2str(mixtures),'_',num2str(sessionSize),'.mat'));
    resultsAll = resultsAll.iVectorResultsAll;

    % generate labels
    fileList = getAllFilesWithExtension(result_folder{folder_i}, '.wav');

    [~,filename,~] = fileparts(fileList{1});
    t = strfind(filename,'-');
    t = t(1);
    firstSpeakerName = filename(1:t-1);
    oldSpeakerName = firstSpeakerName;

    speakerIndex = 1;    
    for i = 1 : size(resultsAll,1)            
        [~,filename,~] = fileparts(fileList{i});
        t = strfind(filename,'-');
        t = t(1);
        speakerName = filename(1:t-1);
        if(~strcmp(speakerName, oldSpeakerName))
            oldSpeakerName = speakerName;
            speakerIndex = speakerIndex + 1;
        end

        results = resultsAll{i,2};
        for seesion_index = 1 : size(results,1)
            scoreinResult = results{seesion_index, channel_index};
            scoreinResult = scoreinResult';

            for max_score_index = 1 : model_count_per_speaker : length(scoreinResult)
                score = [score; max(scoreinResult(max_score_index:max_score_index+model_count_per_speaker-1))];
            end

            labelinResult = zeros(length(scoreinResult)/model_count_per_speaker, 1);
            labelinResult(speakerIndex) = 1;
            label = [label;labelinResult];
        end
    end
end

[eer,value1,value2,dcf10] = compute_eer(score, label, true);
disp(eer)
