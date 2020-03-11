% addpath('C:\libsvm\matlab');

load('K:\Audio-Dataset\ControlLab\USED-MFCC\ControlLab-final-close-training-all-detection-group1\training\modelIVsPerSegments_MFCC_512.mat')
load('K:\Audio-Dataset\ControlLab\USED-MFCC\ControlLab-final-close-training-all-detection-group1\training\modelIVsPerSegmentsLabel_MFCC_512.mat')


bestcv = 0;
bestc = 0;
bestg = 0;
for log2c = -1:3,
  for log2g = -4:1,
    cmd = ['-q -c ', num2str(2^log2c), ' -g ', num2str(2^log2g), '-b ', 1];
    
    % ovrtrain(double(modelIVsPerSegmentsLabel), double(modelIVsPerSegments), cmd);
    
    cv = get_cv_ac(double(modelIVsPerSegmentsLabel), double(modelIVsPerSegments), cmd, 5);
    
    if (cv >= bestcv),
      bestcv = cv; bestc = 2^log2c; bestg = 2^log2g;
    end
    fprintf('%g %g %g (best c=%g, g=%g, rate=%g)\n', log2c, log2g, cv, bestc, bestg, bestcv);
  end
end
