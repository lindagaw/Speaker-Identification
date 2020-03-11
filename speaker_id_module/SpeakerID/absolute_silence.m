function percent = absolute_silence(audio_dir, output)
    addpath(genpath('.'));

    [y,fs]=audioread(audio_dir);
    try
        y = (y(:,1)+y(:,2))/2;
    catch
        % Nothing to do
    end
    y = resample(y,16000,fs);
    audiowrite(audio_dir,y,16000);
    
    mode = 2;           % 0 : ACAM3, 1 : bDNN, 2 : DNN, 3 : LSTM
    threshold = 0.4;    % threshold for hard decision
    output_type = 1;    % 0 : frame based prediction, 1: sample based prediction
    is_default = 1;     % 0 : use trained model, 1: use default model
    
    [result, pp] = vad_func(audio_dir, mode, threshold, output_type, is_default);
    s = audioread(audio_dir);
    t = (1:length(s))./16000;
    
    start_flag = 0;
    endpt_flag = 0;
    start = 0;
    endpt = 0;
    
    invalid_frames = 0;
    for n = 1 : length(result)
        if result(n) == 0
            s(n) = 0;
            invalid_frames = invalid_frames + 1;
        end
    end
    
    % p1 = plot(t, s);
    % ylim([-0.3 0.6]);
    % xlim([0 t(end)]);
    
    percent = 1 - invalid_frames/length(result);
    
    audiowrite(output,s,16000);
    disp('Voice activity preserved...')

end