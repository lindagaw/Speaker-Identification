function [ ] = movefiles( inputDirName, outputDirName )

% move all the flac files from all subfolders under inputDirName folder
% into outputDirName folder

fileList = getAllFilesWithExtension(inputDirName, '.wav');

parfor i = 1 : length(fileList)
        
    [~,filename,~] = fileparts(fileList{i});
    
    disp(strcat(num2str(i), '-', num2str(length(fileList))));

    copyfile(fileList{i}, strcat(outputDirName, '/', filename, '.wav'));
end

end

