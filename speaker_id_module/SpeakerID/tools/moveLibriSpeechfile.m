function [ ] = moveLibriSpeechfile( inputDirName, outputDirName )

% move the last 20 flac files from all speakers under inputDirName folder
% into outputDirName folder

folders = dir(inputDirName);
total_length = 0;

for folder_index = 1:length(folders)

    fileList = getAllFilesWithExtension(strcat(inputDirName,'/', folders(folder_index).name), '.flac');

    for i = length(fileList)-9 : length(fileList)

        [~,filename,~] = fileparts(fileList{i});

        [y,fs] = audioread(fileList{i});
        total_length = total_length + size(y,1) / fs;
        disp(strcat(num2str(i), '-', num2str(length(fileList)), '-', num2str(total_length)));
    
        copyfile(fileList{i}, strcat(outputDirName, '/', filename, '.flac'));
    end

end

end

