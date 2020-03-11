function [  ] = get_voice_segment_for_opensmile_feature_extraction( folder )

fileList = getAllFilesWithExtension(folder, '.wav');
parfor file_index = 1 : length(fileList)
    [path,filename,~] = fileparts(fileList{file_index});
    
    [signal_original,fs] = audioread(fileList{file_index});
    
    signal_original = resample(signal_original, 16000, fs);
    fs = 16000;
    
    [b,a] = butter(3, [100/(fs/2), 3500/(fs/2)], 'bandpass');
    
    for channel = 1 : size(signal_original, 2)    % read every channel's data        
    % for channel = 1 : 1   % for ubm only read the first channel        
        signal_original(:,channel) = filter(b,a,signal_original(:,channel));
        
        voicing = vad_g729(signal_original(:,channel), fs, fs*0.01);
        fprintf(1, '%d\n', sum(voicing) / length(voicing));

        speaker_signal = [];
        for k = 1 : size(voicing,1)
            if voicing(k) == 1
               speaker_signal = [speaker_signal; signal_original((k-1)*fs*0.01 + 1: k*fs*0.01,channel)];
            end
        end
        if ~isempty(speaker_signal)
            
            index = strfind(path, '\');
            last_foldername = path(index(end)+1:end);
   
            newpath = strcat(path(1:index(end-1)),'voice-', num2str(channel),'\', last_foldername);
            if(~exist(newpath,'dir'))
                mkdir(newpath);
            end
            
            newfilename = strcat(newpath,'\',filename, '.wav');
            
            audiowrite(newfilename,speaker_signal,fs);
        end
    end   
end
    
end

