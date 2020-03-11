function features = speech_features(sig, framesize, framestep, sampling_rate)
% Features:
% 1 - energy in frame

% (use normalized signal data)
% 2 - formant frequency (Hz)
% 3 - confidence in formant frequency
% 4 - spectral entropy
% 5 - value of largest autocorrelation peak
% 6 - location of largest autocorrelation peak
% 7 - number of autocorelation peaks
% 8 - time derivative of energy in frame (normalized)
% 9 - ZCR
% 10 - Normalized Energy
% 11:23 - Mel frequency cepstrum coefficients
% 24:36 - MFCC delta
% 37:49 - MFCC delta delta

% (not used, so not calculated)
% 50 - kurtosis
% 51 - skewness
% 52:54 - quartile 0.25,0.5,0.75
% 55 - interquartile

if isempty(sig)
    features = [];
else
    num_frames = max(floor( (length(sig) - (framesize-framestep)) /framestep ), 0);
    features = zeros(55, num_frames);

    % remove DC component
    sig = sig - sum(sig) / length(sig);

%     for f = 1:num_frames
%         frame = sig((f-1)*framestep+1:(f-1)*framestep+framesize);
% 
%         %%%%%%%%%%  Energy
%         % non-normalized energy, used to detect speaking/non-speaking
%         features(1, f) = 10 * log10(sum(frame.^2) / framesize);   
%     end

    %normalize
    sig = sig / max(abs(sig));

    %%%%%%%%%%  MFCC
    features(11:23, :) = mfcc(sig, sampling_rate);
end

% no need to calculate here, because it will be cacluated outside
%%%%%%%%%%  MFCC-delta
% features(24:36, :) = mfcc2delta(features(11:23, :),2);
%%%%%%%%%%  MFCC-delta-delta
% features(37:49, :) = mfcc2delta(features(24:36, :),1);

% %std_dev
% std_dev = sum(sig.^2) / length(sig);
% 
% %%%%%%%%%%  Spectrogram computation
% % Use hanning window, to match Matlab's specgram() function.
% h = hanning(framesize);
% nfft = framesize;
% num_formant_fft = framesize*2;
% 
% % Minimum and maximum formant frequency, in Hz.
% min_freq = 50;
% max_freq = 500;
% 
% max_index = round(num_formant_fft/(min_freq / sampling_rate * framesize) + 1);
% min_index = round(num_formant_fft/(max_freq / sampling_rate * framesize) + 1);
% 
% %%%%%%%%%%  Autocorrelation computation:
% nacorr = framesize/2;
% % compensate for frame effects: scale by 1/( 1/framesize -- framesize/framesize -- 1/framesize)
% % compensating multiplier
% comp = [framesize:-1:framesize-nacorr+1]/framesize;
% comp = 1./comp';
% 
% %%%%%%%%%%  Energy computation
% energy_deriv_filter = ((0:(framesize-1)) - (framesize-1)/2)' / sum(abs((0:(framesize-1)) - (framesize-1)/2));
% 
% for f = 1:num_frames
%     % Grab frame and remove DC component
%     frame = sig((f-1)*framestep+1:(f-1)*framestep+framesize);
%     frame = frame - sum(frame) / framesize;
%     
%     %%%%%%%%%%  Spectrogram
%     % Compute normalized spectrogram
%     tmp = fft(frame .* h,nfft); 
%     spec = abs(tmp(1:(nfft/2)));
%     normspec = spec / (sum(spec) + 1e-5);
% 
%     % Find the formant frequency
%     big_fft = fft(sqrt(normspec), num_formant_fft);
%     big_fft = real(big_fft(min_index:max_index));
%     [features(3, f), maxloc] = max(big_fft);
%     maxloc = maxloc + (min_index - 1);
%     num_samples = num_formant_fft ./ (maxloc - 1);
%     features(2, f) = num_samples .* (sampling_rate / framesize);
% 
%     % Compute the spectral entropy
%     % Unscaled range is 0 to 5.5452.  Scale it to roughly 0..1.
%     normspec(normspec < 1e-5) = 1e-5;
%     features(4, f) = -sum(normspec .* log(normspec));
%     if ~isfinite(features(4, f))
%         % This becomes Inf on negotiation 109 somehow.
%         % Actually, I now think that was memory corruption.
%         error('Spectral entropy became infinite')
%     end
%     
%     %%%%%%%%%%  Autocorrelation
%     X = fft(frame,2*framesize);
%     c = ifft(X.*conj(X));
%     % Multiply by comp to compensate for frame effects
%     acorr = real(c(1:nacorr)).*comp/(sum(frame.^2)+std_dev^2*framesize);
%     
%     % Compute the values of the autocorrelation peaks
%     [peakvals, peaklocs] = fast_find_acorr_peaks(acorr);
%     [features(5, f), index] = max(peakvals);
%     features(6, f) = peaklocs(index);
%     features(7, f) = length(peakvals);
%     
%     % normalized time derivative
%     features(8, f) = sum(frame .^2 .* energy_deriv_filter); 
%     
%     %%%%%%%%%%  ZCR
%     features(9, f) = sum(abs(diff(frame>0)))/length(frame);
%     
%     %%%%%%%%%%  Normalized Energy
%     features(10, f) = sum(frame.^2) / framesize ;
%     
%     % 50 - kurtosis
% %    features(50, f) = kurtosis(frame);
% 
%     % 51 - skewness
% %    features(51, f) = skewness(frame);
%     
%     % 52:54 - quartile 0.25,0.5,0.75
% %    features(52:54, f) = quantile(frame,[0.25 0.5 0.75]);
%     
%     % 55 - interquartile
% %    features(55, f) = features(54, f) - features(52, f);
% end

