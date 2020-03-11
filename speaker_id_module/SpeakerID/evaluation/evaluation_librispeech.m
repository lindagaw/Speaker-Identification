function [ correct_by_file, total_by_file, cannotdecide_by_file, correct_by_speaker, total_by_speaker, cannotdecide_by_speaker ] ...
    = evaluation_librispeech( trainingDirName, detectionDirName, nMixtures, wavmicros, sessionSize )
session_speaking_percentage = 0.2;
[models, nSpeakers ] = training_or_load_models (trainingDirName, nMixtures);

fileList = getAllFilesWithExtension(detectionDirName, '.wav');
correct_by_file = zeros(length(fileList),wavmicros + 1);
total_by_file = zeros(length(fileList),wavmicros + 1);  
cannotdecide_by_file = zeros(length(fileList),wavmicros + 1); 
correct_by_speaker = zeros(nSpeakers,wavmicros + 1);
total_by_speaker = zeros(nSpeakers,wavmicros + 1);
cannotdecide_by_speaker = zeros(nSpeakers,wavmicros + 1);
gmmResultsAll = {}; %file * type * session * channel * 1 * model

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
    [gmmResults, ~, speakerIds, ~, ~] = detection(fileList{i}, nSpeakers, models, session_speaking_percentage, sessionSize);

    gmmResultsAll{i,1} = gmmResults;
    
    [~,filename,~] = fileparts(fileList{i});
    index = strfind(filename,'-');
    index = index(1);
    speakerName = filename(1:index-1);
    
    if(~strcmp(speakerName, oldSpeakerName))
        correct_by_speaker(speakerIndex,1) = sum(correct_by_file(oldSpeakerStart:i-1,1));
        total_by_speaker(speakerIndex,1) = sum(total_by_file(oldSpeakerStart:i-1,1));
        cannotdecide_by_speaker(speakerIndex,1) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,1));
        oldSpeakerStart = i;
        oldSpeakerName = speakerName;
        speakerIndex = speakerIndex + 1;
    end

    correct_by_file(i,1) = sum(speakerIds(:)==speakerIndex);
    cannotdecide_by_file(i,1) = sum(speakerIds(:)==-1);
    total_by_file(i,1) = sum(speakerIds(:)~=0);
    
    if(i == length(fileList))
        correct_by_speaker(speakerIndex,1) = sum(correct_by_file(oldSpeakerStart:i-1,1));
        total_by_speaker(speakerIndex,1) = sum(total_by_file(oldSpeakerStart:i-1,1));
        cannotdecide_by_speaker(speakerIndex,1) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,1));
    end
end

extension = '.wav';
fileList = getAllFilesWithExtension(detectionDirName, extension);
oldSpeakerName = firstSpeakerName;
speakerIndex = 1;
oldSpeakerStart = 1;
micro_index = 2;
for i = 1:length(fileList)
    disp(fileList{i});
    [gmmResults, ~, speakerIds, ~, ~] = detection(fileList{i}, nSpeakers, models, session_speaking_percentage, sessionSize);

    gmmResultsAll{i,2} = gmmResults;
    
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
        
    for j = 1 : size(speakerIds,2)
        correct_by_file(i,micro_index+j-1) = sum(speakerIds(:,j)==speakerIndex);
        cannotdecide_by_file(i,micro_index+j-1) = sum(speakerIds(:,j)==-1);
        total_by_file(i,micro_index+j-1) = sum(speakerIds(:,j)~=0);
    end
    
    if(i == length(fileList))
        for k = 2 : wavmicros + 1
            correct_by_speaker(speakerIndex,k) = sum(correct_by_file(oldSpeakerStart:i-1,k));
            total_by_speaker(speakerIndex,k) = sum(total_by_file(oldSpeakerStart:i-1,k));
            cannotdecide_by_speaker(speakerIndex,k) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,k));
        end
    end
end

save(strcat(detectionDirName,'/gmm_', num2str(nMixtures),'_',num2str(sessionSize),'.mat'),'gmmResultsAll');
save(strcat(detectionDirName,'/gmm_correct_by_file_', num2str(nMixtures),'_',num2str(sessionSize),'.txt'), 'correct_by_file', '-ascii');
save(strcat(detectionDirName,'/gmm_total_by_file_', num2str(nMixtures),'_',num2str(sessionSize),'.txt'), 'total_by_file', '-ascii');
save(strcat(detectionDirName,'/gmm_canotdecide_by_file_', num2str(nMixtures),'_',num2str(sessionSize),'.txt'), 'cannotdecide_by_file', '-ascii');
save(strcat(detectionDirName,'/gmm_correct_by_speaker_', num2str(nMixtures),'_',num2str(sessionSize),'.txt'), 'correct_by_speaker', '-ascii');
save(strcat(detectionDirName,'/gmm_total_by_speaker_', num2str(nMixtures),'_',num2str(sessionSize),'.txt'), 'total_by_speaker', '-ascii');
save(strcat(detectionDirName,'/gmm_canotdecide_by_speaker_', num2str(nMixtures),'_',num2str(sessionSize),'.txt'), 'cannotdecide_by_speaker', '-ascii');

end

