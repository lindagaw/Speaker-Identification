function decisions = realtime_slience_filter(dataStream, sampling_rate, channels, session_speaking_percentage)

% this function will be used by C++ program.
FS = sampling_rate;

sig = reshape(dataStream,channels,[]);
sig = sig';

if(FS ~= 16000)
    sig = resample(sig, 16000, FS);
    FS = 16000;
end

[b,a] = butter(3, [100/(FS/2), 3500/(FS/2)], 'bandpass');

num_frames = floor(length(sig(:,1))/(FS*0.01));
voicing = zeros(num_frames,channels);

decisions = zeros(1,channels);

parfor channel = 1 : channels    % filter and voicing by channels
    sig(:,channel) = filter(b,a,sig(:,channel));
    voicing(:,channel) = vad_g729(sig(:,channel), FS, FS*0.01);
    if sum(voicing(:,channel)) / size(voicing(:,channel),1) > session_speaking_percentage
        decisions(channel) = 1;
    else
        decisions(channel) = 0;
    end
end



end