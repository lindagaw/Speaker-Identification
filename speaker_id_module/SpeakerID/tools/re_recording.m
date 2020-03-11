function [ ] = re_recording( inputDirName, outputDirName )

fileList = getAllFilesWithExtension(inputDirName, '.flac');

recObj = audiorecorder(44100, 24, 2);
folderindex = 0;
fileindex = 0;

for i = 1 : length(fileList)
    [y,Fs] = audioread(fileList{i});
    player = audioplayer(y, Fs);
    record(recObj);
    playblocking(player);
    stop(recObj);
    
    rec_y = getaudiodata(recObj);
    
    [~,filename,~] = fileparts(fileList{i});
    index = strfind(filename,'-');
    index = index(1);
    speakerName = filename(1:index-1);
    
    if(~exist(strcat(outputDirName, '/', speakerName), 'dir'))
        mkdir(strcat(outputDirName, '/', speakerName));
        folderindex = folderindex + 1;
        fileindex = 0;
    end
    fileindex = fileindex + 1;
    disp(strcat(num2str(folderindex),'-', num2str(fileindex),'-' ,num2str(i)));
    
    audiowrite(strcat(outputDirName, '/', speakerName, '/', filename, '.wav'), rec_y, 44100);
end

end

