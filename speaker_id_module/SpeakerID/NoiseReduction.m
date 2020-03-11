function output = NoiseReduction(signal, noise, fs)
% it is a wrapper for function SSBoll79
% caculate noise length and append it before the signal, then call SSBoll79

noiseLength = length(noise) / fs;
output = SSBoll79([noise; signal], fs, noiseLength);

output = output(length(noise)+1:end,:);

end
