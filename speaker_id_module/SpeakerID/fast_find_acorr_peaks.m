function [peakvals, peaklocs] = fast_find_acorr_peaks(acorr_slice)
% function peakvals = fast_find_acorr_peaks(acorr_slice)
%
% finds peaks in an acorr slice.  starts at beginning, 
% requires dip below zero before first peak and successive peaks
% strategy: find zero crossings, then find max in between zero
% crossings

len = length(acorr_slice);

peakvals = [0];
peaklocs = [0];

% skip the first positive part
i = 1;
while acorr_slice(i) >= 0
    if i >= len
        return;
    end
    i = i + 1;
end

numpeaks = 0;

while i <= len
    % At this point, acorr_slice(i) < 0
    
    % Skip the negative correlations
    i = i + 1;
    while i <= len
        if (acorr_slice(i) > 0)
            break;
        end
        i = i + 1;
    end
    
    % Find the peaks within the positive correlations
    bestval = -1;
    while i <= len
        if acorr_slice(i) > bestval
            bestval = acorr_slice(i);
            bestloc = i;
        end
        if acorr_slice(i) < 0
            break;
        end
        i = i + 1;
    end
    
%    if i == len & acorr_slice(i) > 0
%        return;
%    end
    
    if (bestval > 0)
        numpeaks = numpeaks+1;
        peakvals(numpeaks) = bestval;
        peaklocs(numpeaks) = bestloc;
    end
end