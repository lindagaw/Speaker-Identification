function [num_count, speakerIds] ...
    = M2FED_realtime_detection(dataStream, sampling_rate, channels, ...
                        nSpeakers, models, session_speaking_percentage, ...
                        sessionSize, gmm_threshold)
% this function will be used by C++ program.

sig = reshape(dataStream,channels,[]);
sig = sig';

% becasue all the filter-related process has been processed by c++, here
% only the non-zero signal is human-voice signal. So it becomes very simple
% to handle.

speakerIds = zeros(1, channels);
num_count = zeros(1,channels);
   
for channel = 1 : channels

        % speaker_signal = sig( : ,channel );

        [speaker_signal,Fs] = audioread(dataStream);
        
        % get rid of all zeros
        speaker_signal = speaker_signal(speaker_signal~=0);

        temp = M2FED_wav_speech_features(speaker_signal, 44100);

        if ~isempty(temp)
            [ ~, speakerIds(1 ,channel), ~] = ...
                detection_algorithm(num2cell(temp(11:49,:), [1 2]), models.gmm, models.ubm, nSpeakers, 1, gmm_threshold);

            num_count(channel) = 1;
            
            if speakerIds <= 28 && speakerIds >= 0
                speakerIds = 1;
            end

            dlmwrite(strcat('speaker_id_module//SpeakerID//Temp//Speaker-feature-', num2str(channel), '.txt'), temp(11:49, :));
        end
end


% commented all the bolow
% % if(sampling_rate ~= 16000 && channels == 1)
% %    sig = resample_mex1(sig, 16000, sampling_rate);
% % elseif (sampling_rate ~= 16000 && channels == 2)
% %    sig = resample_mex2(sig, 16000, sampling_rate);
% % elseif (sampling_rate ~= 16000 && channels == 4)
% %    sig = resample_mex4(sig, 16000, sampling_rate);
% % elseif (sampling_rate ~= 16000 && channels == 8)
% %    sig = resample_mex8(sig, 16000, sampling_rate);
% %end
% 
% %[b,a] = butter(3, [100/(FS/2), 3500/(FS/2)], 'bandpass');
% 
% % num_frames = floor(length(sig(:,1))/(sampling_rate*0.01));
% num_frames = floor(length(sig(:,1))/441);
% voicing = zeros(num_frames,channels);
% total_voicing_size = zeros(1,channels);
% num_sessions = zeros(1,channels);
% 
% for channel = 1 : channels    % filter and voicing by channels
%     if channel_map(channel) == 1
%         % sig(:,channel) = filter(b,a,sig(:,channel));
%         % voicing(:,channel) = vad_g729(sig(:,channel), sampling_rate, sampling_rate*0.01);
%         % voicing(:,channel) = M2FED_vad_g729(sig(:,channel), 16000, 160);
%         voicing(:,channel) = M2FED_vad_g729(sig(:,channel), 44100, 441);
%         total_voicing_size(channel) = size(voicing(:,channel),1);
% 
%         % only for realtime we change it to ceil not floor
%         %num_sessions(channel) = ceil(total_voicing_size(channel) * 10 / sessionSize);
%         num_sessions(channel) = 1;
%     end
% end
% 
% % max_num_sessions = max(num_sessions);
% % session_speaking = zeros(max_num_sessions, channels);
% % speakerIds = zeros(max_num_sessions, channels);
% speakerIds = zeros(1, channels);
% 
% try
%     num_count = zeros(1,channels);
%     nonSlientData = cell(1,channels);
%     parfor channel = 1 : channels
%         if channel_map(channel) == 1            
%             %for i = 1 : num_sessions(channel)
%             % i=1;    % when real time i always be 1, some modification below
%                %  start_frame = 1;
%                 end_frame = total_voicing_size(channel);
% 
%                 % State 0 is "not voiced," state 1 is "voiced." 
%                 % if more than session_speaking_percentage speaking, we say it speaking.
%                 if( sum(voicing(: ,channel)) / end_frame >= session_speaking_percentage)
%                     speaker_signal = [];
%                     session_signal = sig( : ,channel );
%                     for k = 1 : end_frame
%                         if voicing(k, channel) == 1
%                             speaker_signal = ...
%                                 [speaker_signal; session_signal((k-1)*44100*0.01 + 1: k*44100*0.01)];
%                                %[speaker_signal; session_signal((k-1)*16000*0.01 + 1: k*16000*0.01)];
%                                %[speaker_signal; session_signal((k-1)*sampling_rate*0.01 + 1: k*sampling_rate*0.01)];                                
%                         end 
%                     end
%                     % this is 1 intentionally because detection_algorithm takes cell as the first input parameter
%                     %nonSlientData{1,channel} = wav_speech_features(speaker_signal, sampling_rate); 
%                     % temp = wav_speech_features(speaker_signal, 16000);
%                     temp = M2FED_wav_speech_features(speaker_signal, 44100);
%                     
%                     [ ~, speakerIds(1 ,channel), ~] = ...
%                         detection_algorithm(num2cell(temp(11:49,:), [1 2]), models.gmm, models.ubm, nSpeakers, 1, gmm_threshold);
%                 else
%                     speakerIds(1 ,channel) = 0;
%                 end
%             %end
%             num_count(channel) = num_sessions(channel);
%         end
%     end
% catch
%     num_count = zeros(1,channels);
%     speakerIds = zeros(1,channels);
%     
% end

end
