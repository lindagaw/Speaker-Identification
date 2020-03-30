function [  ] = M2FED_add_reverberation( folder )

%parameters = [0.3 0.7; 0.7 0.3];

parameters = [];
for i = 0.2:0.3:0.8
    for j = 0.2:0.2:0.6
        parameters = [parameters; i j];
    end
end

fileList = getAllFilesWithExtension(folder, '.wav');
disp('start adding reverberation');

parfor file_index = 1 : length(fileList)
    
    [path,filename,~] = fileparts(fileList{file_index});
    [signal_original,fs] = audioread(fileList{file_index});
    
    signal_original = signal_original(:,1);
    
    reverb = reverberator;
    reverb.SampleRate = fs;
    
    %To model a large room, use low decay factor, long reverb tail. 
    %To model a small room, use high decay factor, short reverb tail.
    for WetDry = [0.2 0.3 0.4]
        for i = 1 : size(parameters,1)
            
            %DecayFactor is proportional to the time it takes for 
            %reflections to run out of energy.
            DecayFactor = parameters(i,1);
            
            %Diffusion is proportional to the rate at which the reverb tail 
            %builds in density. Increasing Diffusion pushes the reflections 
            %closer together, thickening the sound. Reducing Diffusion 
            %creates more discrete echoes.
            Diffusion = parameters(i,2);
            
            reverb.WetDryMix = WetDry;
            reverb.DecayFactor = DecayFactor;
            reverb.Diffusion = Diffusion;

            signal_reverb = step(reverb, signal_original);

            if(size(signal_original,2) == 1)
                signal_reverb = signal_reverb(:,1);
            end
            
            newpath = strcat(path,'-',num2str(WetDry*10),'-',num2str(DecayFactor*10),'-',num2str(Diffusion*10),'-TMP');
            if(~exist(newpath,'dir'))
                mkdir(newpath);
            end
            
            newfilename = strcat(newpath,'\',filename,'-',num2str(WetDry*10), ...
                '-',num2str(DecayFactor*10),'-',num2str(Diffusion*10),'.wav');
            disp(newfilename);
            audiowrite(newfilename,signal_reverb,fs);
        end            
    end
end
    
end

