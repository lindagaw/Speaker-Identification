folder = 'K:\Audio-Dataset\HomeNoiseDataset\NAR_dataset';
categories = dir(folder);

fs = 48000;
 
VAD_cst_param = vadInitCstParams;
VAD_cst_param.Fs = fs;
VAD_cst_param.L_WINDOW = fs * 0.03;
VAD_cst_param.L_FRAME = fs * 0.01;
LA = VAD_cst_param.L_NEXT;
LB = VAD_cst_param.L_WINDOW - VAD_cst_param.L_NEXT;
VAD_cst_param.hamwindow = [0.54 - 0.46*cos(2*pi*(0:LB-1)'/(2*LB-1)); cos(2*pi*(0:LA-1)'/(4*LA-1))];

[b,a] = butter(3, [100/(fs/2), 3500/(fs/2)], 'bandpass');

[background, F] = audioread('K:\Audio-Dataset\ControlLab\ControlLab-data\microphone-background\microphone_background\microphone_background-1.wav');
background = background(1:F,:);
background = resample(background, fs, F);
background = filter(b,a,background);           

[human, F] = audioread('K:\Audio-Dataset\LibriSpeech\different-ubm-training\microphone_recorded_background\rerecordedvoicesSample\307\307-127540-0027.wav');
human = human(1.5*F:2.5*F,1);
human = resample(human, fs, F);
human = filter(b,a,human);
                        
for i = 3:length(categories)
   
    category = categories(i).name;
    
    files = dir(strcat(folder, '\', category));
   
    Y = [background; human; background];
    for j = 3 : length(files)
        [y t] = audioread(strcat(folder, '\', category, '\', files(j).name));
        y = y / max(abs(y)) / 4;
        y = resample(y, fs, t);
        
        y = filter(b,a,y);
        Y = [Y;y;background];
    end
    
    clear vadG729

    num_frames = floor(length(Y)/(0.01*fs));
    voicing = zeros(num_frames,1);

    for index = 1 : num_frames
        current_speech = Y( (index-1) * 0.01*fs + 1 : min(single(index * 0.01*fs), single(length(Y))));
        voicing(index) = vadG729(current_speech, VAD_cst_param);
    end
    
end
