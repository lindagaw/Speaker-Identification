function [] = M2FED_training_script(rootDirName, nMixtures)

fprintf(1, 'Training directory: %s\n', rootDirName);

try
    M2FED_add_reverberation(strcat(rootDirName, 'tools'));
    
    try
        ubm = [];
        if exist(strcat(rootDirName, '\models_ubm', num2str(nMixtures),'.mat'),'file')
            ubm = load(strcat(rootDirName, '\models_ubm', num2str(nMixtures),'.mat'));
            ubm = ubm.ubm;
        end 
        
        M2FED_training(rootDirName, nMixtures, ubm, 0, 1);
    
    catch a
        disp(a);
        fprintf(1, 'Training failed\n');
    end
catch
    fprintf(1, 'Reverberation generation failed\n');
end

folders = dir(strcat(rootDirName, '\singles'));
for k = 1:length(folders)
    if ~folders(k).isdir
        continue
    end
    fname = folders(k).name;
    if isempty(strfind(fname, '-TMP')) == false
        rmdir(strcat(rootDirName, '\singles\',fname),'s');
    end
end

end
