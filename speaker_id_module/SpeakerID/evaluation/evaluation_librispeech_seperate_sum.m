function [ correct_by_file, total_by_file, cannotdecide_by_file, correct_by_speaker, total_by_speaker, cannotdecide_by_speaker ] ...
    = evaluation_librispeech_seperate_sum( trainingDirName, detectionDirName, nMixtures, wavmicros )

model_count_per_speaker = 81;
sessionSize = 5000;

[~, nSpeakers ] = training_or_load_models (trainingDirName, nMixtures);
fileList = getAllFilesWithExtension(detectionDirName, '.wav');
correct_by_file = zeros(length(fileList),wavmicros + 1);
total_by_file = zeros(length(fileList),wavmicros + 1);  
cannotdecide_by_file = zeros(length(fileList),wavmicros + 1); 
correct_by_speaker = zeros(nSpeakers/model_count_per_speaker,wavmicros + 1);
total_by_speaker = zeros(nSpeakers/model_count_per_speaker,wavmicros + 1);
cannotdecide_by_speaker = zeros(nSpeakers/model_count_per_speaker,wavmicros + 1);
load(strcat(detectionDirName,'/gmm_', num2str(nMixtures),'_',num2str(sessionSize),'.mat'),'gmmResultsAll');

[~,filename,~] = fileparts(fileList{1});
index = strfind(filename,'-');
index = index(1);
firstSpeakerName = filename(1:index-1);

extension = '.flac';
fileList = getAllFilesWithExtension(detectionDirName, extension);

oldSpeakerName = firstSpeakerName;
speakerIndex = 1;
oldSpeakerStart = 1;
for i = 1:length(fileList)
    disp(fileList{i});
    gmmResults = gmmResultsAll{i,1};
    
    for session_i = 1:size(gmmResults,1)
        for channel_i = 1:size(gmmResults,2)
            values = gmmResults{session_i,channel_i};
            
            if(sum(values==0) == length(values))
                % all values are 0
                speakerIds(session_i,channel_i) = 0;
            else
                % sum up those positives
                sums(1) = sum(values(1:model_count_per_speaker).*(values(1:model_count_per_speaker)>0));
                sums(2) = sum(values(model_count_per_speaker+1:model_count_per_speaker*2).*(values(model_count_per_speaker+1:model_count_per_speaker*2)>0));
                sums(3) = sum(values(model_count_per_speaker*2+1:model_count_per_speaker*3).*(values(model_count_per_speaker*2+1:model_count_per_speaker*3)>0));
                [sorted, index] = sort(sums,'descend');
                if(sorted(1) == 0)
                    % all are negative
                    speakerIds(session_i,channel_i) = -1;
                else
                    speakerIds(session_i,channel_i) = index(1);
                end
            end
        end
    end
    
    [~,filename,~] = fileparts(fileList{i});
    index = strfind(filename,'-');
    index = index(1);
    speakerName = filename(1:index-1);
    
    if(~strcmp(speakerName, oldSpeakerName) || i == length(fileList))
        correct_by_speaker(speakerIndex,1) = sum(correct_by_file(oldSpeakerStart:i-1,1));
        total_by_speaker(speakerIndex,1) = sum(total_by_file(oldSpeakerStart:i-1,1));
        cannotdecide_by_speaker(speakerIndex,1) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,1));
        oldSpeakerStart = i;
        oldSpeakerName = speakerName;
        speakerIndex = speakerIndex + 1;
    end

    correct_by_file(i,1) = sum((ceil(speakerIds(:)))==speakerIndex);
    cannotdecide_by_file(i,1) = sum(speakerIds(:)==-1);
    total_by_file(i,1) = sum(speakerIds(:)~=0);
end

extension = '.wav';
fileList = getAllFilesWithExtension(detectionDirName, extension);
oldSpeakerName = firstSpeakerName;
speakerIndex = 1;
oldSpeakerStart = 1;
micro_index = 2;
for i = 1:length(fileList)
    disp(fileList{i});
    gmmResults = gmmResultsAll{i,2};
    
    for session_i = 1:size(gmmResults,1)
        for channel_i = 1:size(gmmResults,2)
            values = gmmResults{session_i,channel_i};
            
            if(sum(values==0) == length(values))
                % all values are 0
                speakerIds(session_i,channel_i) = 0;
            else
                % sum up those positives
%                 sums(1) = sum(values(1:model_count_per_speaker).*(values(1:model_count_per_speaker)>0));
%                 sums(2) = sum(values(model_count_per_speaker+1:model_count_per_speaker*2).*(values(model_count_per_speaker+1:model_count_per_speaker*2)>0));
%                 sums(3) = sum(values(model_count_per_speaker*2+1:model_count_per_speaker*3).*(values(model_count_per_speaker*2+1:model_count_per_speaker*3)>0));

                % mean those positives
%                 sums(1) = mean(values(1:model_count_per_speaker).*(values(1:model_count_per_speaker)>0));
%                 sums(2) = mean(values(model_count_per_speaker+1:model_count_per_speaker*2).*(values(model_count_per_speaker+1:model_count_per_speaker*2)>0));
%                 sums(3) = mean(values(model_count_per_speaker*2+1:model_count_per_speaker*3).*(values(model_count_per_speaker*2+1:model_count_per_speaker*3)>0));

                % sum up all
%                 sums(1) = sum(values(1:model_count_per_speaker));
%                 sums(2) = sum(values(model_count_per_speaker+1:model_count_per_speaker*2));
%                 sums(3) = sum(values(model_count_per_speaker*2+1:model_count_per_speaker*3));

%                 sum up top n positive
                n = 10;
                s1 = sort(values(1:model_count_per_speaker),'descend');
                sums(1) = sum(s1(1:n).*(s1(1:n)>0));
                s2 = sort(values(model_count_per_speaker+1:model_count_per_speaker*2),'descend');
                sums(2) = sum(s2(1:n).*(s2(1:n)>0));
                s3 = sort(values(model_count_per_speaker*2+1:model_count_per_speaker*3),'descend');
                sums(3) = sum(s3(1:n).*(s3(1:n)>0));
                
%                 sum up top n 
%                 n = 10;
%                 s1 = sort(values(1:model_count_per_speaker),'descend');
%                 sums(1) = sum(s1(1:n));
%                 s2 = sort(values(model_count_per_speaker+1:model_count_per_speaker*2),'descend');
%                 sums(2) = sum(s2(1:n));
%                 s3 = sort(values(model_count_per_speaker*2+1:model_count_per_speaker*3),'descend');
%                 sums(3) = sum(s3(1:n));
                
%                 % top 1
%                 s1 = sort(values(1:model_count_per_speaker),'descend');
%                 sums(1) = s1(1);
%                 s2 = sort(values(model_count_per_speaker+1:model_count_per_speaker*2),'descend');
%                 sums(2) = s2(1);
%                 s3 = sort(values(model_count_per_speaker*2+1:model_count_per_speaker*3),'descend');
%                 sums(3) = s3(1);

                  % vote
%                   sums(1) = sum(values(1:model_count_per_speaker) > 0);
%                   sums(2) = sum(values(model_count_per_speaker+1:model_count_per_speaker*2) > 0);
%                   sums(3) = sum(values(model_count_per_speaker*2+1:model_count_per_speaker*3) > 0);

                [sorted, index] = sort(sums,'descend');
                if(sorted(1) <= 0)
                    % all are negative
                    speakerIds(session_i,channel_i) = -1;
                else
                    speakerIds(session_i,channel_i) = index(1);
                end
            end
        end
    end
    
    [~,filename,~] = fileparts(fileList{i});
    index = strfind(filename,'-');
    index = index(1);
    speakerName = filename(1:index-1);

    if(~strcmp(speakerName, oldSpeakerName))
        for k = 2 : wavmicros + 1
            correct_by_speaker(speakerIndex,k) = sum(correct_by_file(oldSpeakerStart:i-1,k));
            total_by_speaker(speakerIndex,k) = sum(total_by_file(oldSpeakerStart:i-1,k));
            cannotdecide_by_speaker(speakerIndex,k) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,k));
        end
        oldSpeakerStart = i;
        oldSpeakerName = speakerName;
        speakerIndex = speakerIndex + 1;
    end
    
    if(exist('speakerIds','var'))
        for j = 1 : size(speakerIds,2)
            correct_by_file(i,micro_index+j-1) = sum(ceil(speakerIds(:,j))==speakerIndex);
            cannotdecide_by_file(i,micro_index+j-1) = sum(speakerIds(:,j)==-1);
            total_by_file(i,micro_index+j-1) = sum(speakerIds(:,j)~=0);
        end
    end
    
    if (i == length(fileList))
        for k = 2 : wavmicros + 1
            correct_by_speaker(speakerIndex,k) = sum(correct_by_file(oldSpeakerStart:i-1,k));
            total_by_speaker(speakerIndex,k) = sum(total_by_file(oldSpeakerStart:i-1,k));
            cannotdecide_by_speaker(speakerIndex,k) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,k));
        end
    end
end


save(strcat(detectionDirName,'/sum_correct_by_file_', num2str(nMixtures), '_', num2str(sessionSize), '.txt'), 'correct_by_file', '-ascii');
save(strcat(detectionDirName,'/sum_total_by_file_', num2str(nMixtures), '_', num2str(sessionSize),'.txt'), 'total_by_file', '-ascii');
save(strcat(detectionDirName,'/sum_canotdecide_by_file_', num2str(nMixtures), '_', num2str(sessionSize),'.txt'), 'cannotdecide_by_file', '-ascii');
save(strcat(detectionDirName,'/sum_correct_by_speaker_', num2str(nMixtures), '_', num2str(sessionSize),'.txt'), 'correct_by_speaker', '-ascii');
save(strcat(detectionDirName,'/sum_total_by_speaker_', num2str(nMixtures), '_', num2str(sessionSize),'.txt'), 'total_by_speaker', '-ascii');
save(strcat(detectionDirName,'/sum_canotdecide_by_speaker_', num2str(nMixtures), '_', num2str(sessionSize),'.txt'), 'cannotdecide_by_speaker', '-ascii');

end

