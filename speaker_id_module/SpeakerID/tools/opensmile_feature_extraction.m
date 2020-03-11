function [  ] = opensmile_feature_extraction( folder, isUBMorDetection )

fileList = getAllFilesWithExtension(folder, '.wav');
parfor file_index = 1 : length(fileList)
    [path,filename,~] = fileparts(fileList{file_index});
    
    
    index = strfind(path, '\');
    last_foldername = path(index(end)+1:end);

    newpath = strcat(path(1:index(end-1)),'feature-func-1');
    if(~exist(newpath,'dir'))
        mkdir(newpath);
    end            
    
    command = 'C:\Dropbox\UVa_lab\Open-smile-feature-extraction\SMILExtract_Release.exe -I';
    command = strcat(command, {' '}, fileList{file_index}, ' -O');
    
    if(isUBMorDetection)
       % is ubm or detection so one wave file one feature file 
        command = strcat(command, {' '}, newpath, '\', filename,'.csv', ' -C');
    else
       % is not ubm, so one folder one feature file        
        command = strcat(command, {' '}, newpath, '\', last_foldername,'.csv', ' -C');
    end
        
    command = strcat(command, {' '}, 'C:\Dropbox\UVa_lab\Open-smile-feature-extraction\whole_features.conf');
    
    system(command{1});
end
    
end

