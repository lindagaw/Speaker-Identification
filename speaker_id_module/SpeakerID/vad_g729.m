function voicing = vad_g729( speech, fs, step)

% vadInitCstParams has been modified to use 16000HZ
VAD_cst_param = vadInitCstParams;

clear vadG729

num_frames = floor(length(speech)/step);
voicing = zeros(num_frames,1);

for i = 1 : num_frames
    current_speech = speech( (i-1) * step + 1 : min(single(i * step), single(length(speech))));
    
    % make it faster
    % voicing(i) = vadG729(current_speech, VAD_cst_param);
    voicing(i) = vadG729_mex(current_speech, VAD_cst_param);
end

% ignore short slience run less than 0.5 second. 
short_slience_count = fs/step * 0.5;
start_index = 1;
for i = 1 : num_frames
    if(voicing(i) == 0)
        continue;
    else
        if(i-start_index < short_slience_count)
            voicing(start_index:i) = 1;
        end 
        start_index = i+1;
    end
end

% ignore short speech run less than 0.25 second. 
short_speech_count = fs/step * 0.25;
start_index = 1;
for i = 1 : num_frames
    if(voicing(i) == 1)
        continue;
    else
        if(i-start_index < short_speech_count)
            voicing(start_index:i) = 0;
        end
        start_index = i+1;
    end
end

end

