function [ correct_by_file, total_by_file, cannotdecide_by_file, correct_by_speaker, total_by_speaker, cannotdecide_by_speaker ] ...
    = evaluation_EMA( trainingDirName, detectionDirName, nMixtures )
sessionSize = 2000;
session_speaking_percentage = 0.5;
[models, nSpeakers ] = training_or_load_models (trainingDirName, nMixtures);

fileList = getAllFilesWithExtension(detectionDirName, '.wav');
correct_by_file = zeros(length(fileList),1);
total_by_file = zeros(length(fileList),1);  
cannotdecide_by_file = zeros(length(fileList),1); 
correct_by_speaker = zeros(nSpeakers*4,1);
total_by_speaker = zeros(nSpeakers*4,1);
cannotdecide_by_speaker = zeros(nSpeakers*4,1);

[~,filename,~] = fileparts(fileList{1});
index = strfind(filename,'_');
firstSpeakerName = filename(index(1)+1:index(3)-1);

oldSpeakerName = firstSpeakerName;
speakerIndex = 1;
oldSpeakerStart = 1;
for i = 1:length(fileList)
    disp(fileList{i});
    [~, ~, speakerIds, ~, ~] = detection(fileList{i}, nSpeakers, models, session_speaking_percentage, sessionSize);

    [~,filename,~] = fileparts(fileList{i});
    index = strfind(filename,'_');
    speakerName = filename(index(1)+1:index(3)-1);

    if(~strcmp(speakerName, oldSpeakerName) || i == length(fileList))
        correct_by_speaker(speakerIndex) = sum(correct_by_file(oldSpeakerStart:i-1));
        total_by_speaker(speakerIndex) = sum(total_by_file(oldSpeakerStart:i-1));
        cannotdecide_by_speaker(speakerIndex) = sum(cannotdecide_by_file(oldSpeakerStart:i-1));
        oldSpeakerStart = i;
        oldSpeakerName = speakerName;
        speakerIndex = speakerIndex + 1;
    end
    
    correct_by_file(i) = sum(speakerIds(:)==ceil(speakerIndex/4));
    cannotdecide_by_file(i) = sum(speakerIds(:)==-1);
    total_by_file(i) = sum(speakerIds(:)~=0);
end

save(strcat(detectionDirName,'/correct_by_file_', num2str(nMixtures),'.txt'), 'correct_by_file', '-ascii');
save(strcat(detectionDirName,'/total_by_file_', num2str(nMixtures),'.txt'), 'total_by_file', '-ascii');
save(strcat(detectionDirName,'/canotdecide_by_file_', num2str(nMixtures),'.txt'), 'cannotdecide_by_file', '-ascii');
save(strcat(detectionDirName,'/correct_by_speaker_', num2str(nMixtures),'.txt'), 'correct_by_speaker', '-ascii');
save(strcat(detectionDirName,'/total_by_speaker_', num2str(nMixtures),'.txt'), 'total_by_speaker', '-ascii');
save(strcat(detectionDirName,'/canotdecide_by_speaker_', num2str(nMixtures),'.txt'), 'cannotdecide_by_speaker', '-ascii');

end

