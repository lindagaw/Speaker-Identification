function [] = seperate_channels(dirname)

files = getAllFilesWithExtension(dirname, '.wav');

for i = 1 : length(files)
    fprintf(1, 'Input file %s\n', files{i});
    [y,fs,bits] = wavread(files{i});
    
    % noise cancelling
%    [b,a] = butter(3, [100/(fs/2), 1200/(fs/2)], 'bandpass');
%    y = filter(b,a,y);
    
    [~, name, ext] = fileparts(files{i});
    
    positions = strfind(name,'-');
    position = positions(1) - 1;    
    personname = name( 1:position );
    
    for j = 1 : size(y,2)
        if ~exist(strcat(dirname, '\', personname , '-', num2str(j)), 'dir')
            mkdir (strcat(dirname, '\', personname , '-', num2str(j)));
        end
        audiowrite(strcat(dirname, '\', personname , '-', num2str(j), '\', ...
            personname , '-', num2str(j), name(position+1:end), ext),y(:,j),fs,'BitsPerSample',bits);
    end
end

end