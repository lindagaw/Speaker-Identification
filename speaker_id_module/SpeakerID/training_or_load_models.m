function [ models, nSpeakers ] = training_or_load_models (rootDirName, nMixtures)
% this function will be used by C++ program.
% if the model exists, it loads. if not, it trains.

fprintf(1, 'Training directory: %s\n', rootDirName);
% training
if exist(strcat(rootDirName, '//models_', num2str(nMixtures),'.mat'),'file') 
    disp('Found the GMM module in the working directory. Retriving the module ... ');
    models = load(strcat(rootDirName,'//models_', num2str(nMixtures),'.mat'));
    models = models.models;
else
    ubm = [];
    if exist(strcat(rootDirName, '//models_ubm', num2str(nMixtures),'.mat'),'file')
        disp('Found the UBM module in the working directory. Retriving the module ... ');
        ubm = load(strcat(rootDirName, '//models_ubm', num2str(nMixtures),'.mat'));
        ubm = ubm.ubm;
    end
    models = training(rootDirName, nMixtures, ubm, 0);
end
nSpeakers = size(models.gmm, 1);
%disp('nSpeakers in the model:');
%disp(nSpeakers);


end
