function features = M2FED_wav_speech_features(signal, FS)

% >20 ms, and power of 2; 44.1kHz = 1024(~23ms), 16kHz = 512(~32ms)
%framesize = 2^nextpow2(FS * 0.02);

features = speech_features(signal, 1024, 512, FS);

if ~isempty(features)
    % features(11:23,:) = cmvn(features(11:23,:));
    features(11:23,:) = fea_warping(features(11:23,:));

    % re-calculate delta and delta-delta
    %%%%%%%%%%  MFCC-delta
    features(24:36, :) = mfcc2delta(features(11:23, :),2);
    %%%%%%%%%%  MFCC-delta-delta
    features(37:49, :) = mfcc2delta(features(24:36, :),1);
end


