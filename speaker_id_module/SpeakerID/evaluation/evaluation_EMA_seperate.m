function [ correct_by_file, total_by_file, cannotdecide_by_file, correct_by_speaker, total_by_speaker, cannotdecide_by_speaker ] ...
    = evaluation_EMA_seperate( trainingDirName, detectionDirName, nMixtures, wavmicros )
sessionSize = 2000;
session_speaking_percentage = 0.5;
[models, nSpeakers ] = training_or_load_models (trainingDirName, nMixtures);
model_count_per_speaker = 81;

fileList = getAllFilesWithExtension(detectionDirName, '.wav');
correct_by_file = zeros(length(fileList), wavmicros);
total_by_file = zeros(length(fileList), wavmicros);  
cannotdecide_by_file = zeros(length(fileList), wavmicros); 
correct_by_speaker = zeros(nSpeakers*4, wavmicros);
total_by_speaker = zeros(nSpeakers*4, wavmicros);
cannotdecide_by_speaker = zeros(nSpeakers*4, wavmicros);
gmmResultsAll = {}; %file * session * channel * 1 * model

[~,filename,~] = fileparts(fileList{1});
index = strfind(filename,'_');
firstSpeakerName = filename(index(1)+1:index(3)-1);

oldSpeakerName = firstSpeakerName;
speakerIndex = 1;
oldSpeakerStart = 1;
for i = 1:length(fileList)
    disp(fileList{i});
    [gmmResults, ~, speakerIds, ~, ~] = detection(fileList{i}, nSpeakers, models, session_speaking_percentage, sessionSize);

    gmmResultsAll{i} = gmmResults;
    [~,filename,~] = fileparts(fileList{i});
    index = strfind(filename,'_');
    speakerName = filename(index(1)+1:index(3)-1);

    if(~strcmp(speakerName, oldSpeakerName))
        for k = 1 : wavmicros
            correct_by_speaker(speakerIndex,k) = sum(correct_by_file(oldSpeakerStart:i-1,k));
            total_by_speaker(speakerIndex,k) = sum(total_by_file(oldSpeakerStart:i-1,k));
            cannotdecide_by_speaker(speakerIndex,k) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,k));
        end
        oldSpeakerStart = i;
        oldSpeakerName = speakerName;
        speakerIndex = speakerIndex + 1;
    end
    
    for j = 1 : size(speakerIds, 2)
        correct_by_file(i,j) = sum(ceil(speakerIds(:,j)./model_count_per_speaker)==ceil(speakerIndex/4));
        cannotdecide_by_file(i,j) = sum(speakerIds(:,j)==-1);
        total_by_file(i,j) = sum(speakerIds(:,j)~=0);
    end
    
    if (i == length(fileList))
        for k = 1 : wavmicros
            correct_by_speaker(speakerIndex,k) = sum(correct_by_file(oldSpeakerStart:i-1,k));
            total_by_speaker(speakerIndex,k) = sum(total_by_file(oldSpeakerStart:i-1,k));
            cannotdecide_by_speaker(speakerIndex,k) = sum(cannotdecide_by_file(oldSpeakerStart:i-1,k));
        end
    end
end

save(strcat(detectionDirName,'/gmm_', num2str(nMixtures),'_',num2str(sessionSize),'.mat'),'gmmResultsAll');
save(strcat(detectionDirName,'/correct_by_file_', num2str(nMixtures),'.txt'), 'correct_by_file', '-ascii');
save(strcat(detectionDirName,'/total_by_file_', num2str(nMixtures),'.txt'), 'total_by_file', '-ascii');
save(strcat(detectionDirName,'/canotdecide_by_file_', num2str(nMixtures),'.txt'), 'cannotdecide_by_file', '-ascii');
save(strcat(detectionDirName,'/correct_by_speaker_', num2str(nMixtures),'.txt'), 'correct_by_speaker', '-ascii');
save(strcat(detectionDirName,'/total_by_speaker_', num2str(nMixtures),'.txt'), 'total_by_speaker', '-ascii');
save(strcat(detectionDirName,'/canotdecide_by_speaker_', num2str(nMixtures),'.txt'), 'cannotdecide_by_speaker', '-ascii');

end

