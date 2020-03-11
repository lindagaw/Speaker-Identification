clc
clear
close all

addpath(genpath('./lib'));

%% parameter setting
mode = 2;           % 0 : ACAM3, 1 : bDNN, 2 : DNN, 3 : LSTM
threshold = 0.4;    % threshold for hard decision
output_type = 1;    % 0 : frame based prediction, 1: sample based prediction
is_default = 1;     % 0 : use trained model, 1: use default model

audio_dir = 'C:\Users\yg9ca\Documents\Datasets\TESS\Angry\YAF_sub_angry.wav';
absolute_silence(audio_dir, audio_dir);

%%
function absolute_silence_folder_wide(folder, emotion)
    emotion_dir = strcat(folder, emotion, '//');
    files = dir(strcat(emotion_dir, '//*.wav'));
    for file = files'
        
        disp(strcat('processing', ' --> ', file.name));
        absolute_path = strcat(emotion_dir, file.name);
        absolute_silence(absolute_path, absolute_path);
    end
end

%% reduce all the frames classified as silence to absolute silence (volume = 0)
function absolute_silence(audio_dir, output)
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
    
    for n = 1 : length(result)
        if result(n) == 0
            s(n) = 0;
        end
    end

    audiowrite(output,s,16000);
    
end

%% plot (sample based)
function plot_VAD(audio_dir)
    [y,fs]=audioread(audio_dir);
    try
        y = (y(:,1)+y(:,2))/2;
    catch
        % Nothing to do
    end
    y = resample(y,16000,fs);
    audiowrite(audio_dir,y,16000);
    % audio_dir = './data/example/SNR103F3MIC021001_ch01.wav';
    mode = 2;           % 0 : ACAM3, 1 : bDNN, 2 : DNN, 3 : LSTM
    threshold = 0.4;    % threshold for hard decision
    output_type = 1;    % 0 : frame based prediction, 1: sample based prediction
    is_default = 1;     % 0 : use trained model, 1: use default model

    % prediction
    % result : binary decision
    % pp : posterior probability
    [result, pp] = vad_func(audio_dir, mode, threshold, output_type, is_default);
    s = audioread(audio_dir);
    figure
    t = (1:length(s))./16000;
    p1 = plot(t, s);
    hold on
    p3 = plot(t(1:length(result)), result*0.3, 'r');
    ylim([-0.3 0.6]);
    xlim([0 t(end)]);
end