function extract_audio_from_video_inverse( inputfilename, outputfoldername, tagfilename )

% read audio signal and sample rate
sig = audioread(inputfilename);
info = audioinfo(inputfilename);
fs = info.SampleRate;

% for family 13 close
% lag = zeros(14.3*fs, 2);
% sig = cat(1, lag, sig);

% for family 14 close
% lag = zeros((2*60+47)*fs, 2);
% sig = cat(1, lag, sig);
% sig = sig((2*60+47)*fs : end);

% for family 12 close
% sig = sig((2*60+18)*fs : end);

% read tags
time_relative = xlsread(tagfilename,'H:H');
duration = xlsread(tagfilename,'I:I');

time_relative = [0; time_relative];
duration = [0; duration];

[~,subject_name,~] = xlsread(tagfilename,'L:L');
subject_name = subject_name(2:end);

[~,mood,~] = xlsread(tagfilename, 'M:M');
mood = mood(2:end);

[~,age,~] = xlsread(tagfilename,'T:T');
age = age(2:end);

[~,gender,~] = xlsread(tagfilename, 'U:U');
gender = gender(2:end);

% for family 08, 10-JR, 14
[~,age,~] = xlsread(tagfilename,'S:S');
age = age(2:end);

[~,gender,~] = xlsread(tagfilename, 'T:T');
gender = gender(2:end);

% for family 10-BB , 12
% [~,age,~] = xlsread(tagfilename,'R:R');
% age = age(2:end);
% 
% [~,gender,~] = xlsread(tagfilename, 'S:S');
% gender = gender(2:end);

start_time = 0;

for i = 1 : size(time_relative, 1)
    if duration(i) == 0
        start_time = time_relative(i);
        continue;
    else
        end_time = time_relative(i);
        if start_time == end_time
            continue;
        end
        speech = sig( ceil(start_time * fs) + 1 : floor(end_time * fs));
        
%         folderbyspeaker = strcat(outputfoldername, '\audio_by_speaker\', subject_name{i});
%         if(~exist(folderbyspeaker, 'dir'))
%             mkdir(folderbyspeaker);
%         end            
%         outfilenamebyspeaker = strcat(folderbyspeaker, '\', num2str(i), '_', ...
%             subject_name{i}, '_', mood{i}, '_', age{i}, '_', gender{i}, '.wav');
%         disp(outfilenamebyspeaker);
%         audiowrite(outfilenamebyspeaker, speech, fs);
        
        folderbymood = strcat(outputfoldername, '\audio_by_mood\', mood{i});
        if(~exist(folderbymood, 'dir'))
            mkdir(folderbymood);
        end 
        outfilenamebymood = strcat(folderbymood, '\', num2str(i), '_', mood{i}, '_', ...
            subject_name{i}, '_', age{i}, '_', gender{i}, '.wav');
        disp(outfilenamebymood);
        audiowrite(outfilenamebymood, speech, fs);
    end
end

end

