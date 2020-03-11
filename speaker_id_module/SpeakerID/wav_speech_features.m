function features = wav_speech_features(signal, FS)

% >20 ms, and power of 2; 44.1kHz = 1024(~23ms), 16kHz = 512(~32ms)
%framesize = 2^nextpow2(FS * 0.02); 
framesize = 512;    % it is 16kHZ
framestep = framesize / 2;

sz = size(signal,1);

% Read not more than 5 minutes at a time.
samples_per_read = FS*60*5;

num_reads = ceil(sz(1) / samples_per_read);
total_num_frames = floor((sz(1) - framesize) / framestep) + 1;
frames_per_read = ceil(total_num_frames / num_reads);

features = zeros(55, total_num_frames);

for r = 1:num_reads
    start_frame = (r - 1) * frames_per_read + 1;
    end_frame = min(r * frames_per_read, total_num_frames);
    start_sample = (start_frame - 1) * framestep + 1;
    end_sample = (end_frame - 1) * framestep + framesize;
   
    sig = signal(start_sample:end_sample);
    
    features(:, start_frame:end_frame) ...
        = speech_features(sig, framesize, framestep, FS);
end

features(11:23,:) = cmvn(features(11:23,:));


features(11:23,:) = fea_warping(features(11:23,:));



% re-calculate delta and delta-delta
%%%%%%%%%%  MFCC-delta
features(24:36, :) = mfcc2delta(features(11:23, :),2);
%%%%%%%%%%  MFCC-delta-delta
features(37:49, :) = mfcc2delta(features(24:36, :),1);


% % cepstral mean normalization for mfcc
% cmn = mean(features(11:23,:),2);
% % csn = std(features(11:23,:),0,2);
% for i = 1 : size(features,2)
%     features(11:23,i) = features(11:23,i) - cmn;
%     %features(11:23,i) = features(11:23,i) ./ csn;
% end
% 
% % mfcc feature CDF-matching to standard normal distribution
% matchingsize = 300;
% padding = zeros(size(features,1), matchingsize/2);
% features_padded = [padding features padding];
% for i = 1 : size(features,2)
% 	for j = 11 : 23
% 		central_ranking = find(sort(features_padded(j, i:i+matchingsize-1),2) == features_padded(j, i+matchingsize/2 ));
% 		central_ranking = central_ranking(1);
% 		matched_ranking = (central_ranking - 1/2) / matchingsize;
% 		matched_feature(j,i) = icdf('Normal', matched_ranking, 0, 1);
% 	end
% end
% features(11:23,:) = matched_feature(11:23,:);
% 
% % re-calculate delta and delta-delta
% %%%%%%%%%%  MFCC-delta
% features(24:36, :) = mfcc2delta(features(11:23, :),2);
% %%%%%%%%%%  MFCC-delta-delta
% features(37:49, :) = mfcc2delta(features(24:36, :),1);

