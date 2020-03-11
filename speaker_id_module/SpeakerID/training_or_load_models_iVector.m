function [ models, nSpeakers ] = training_or_load_models_iVector (rootDirName, nMixtures)
% this function will be used by C++ program.
% if the model exists, it loads. if not, it trains.

fprintf(1, 'Training directory: %s\n', rootDirName);
% training
if exist(strcat(rootDirName, '\models_', num2str(nMixtures),'.mat'),'file') 
    models = load(strcat(rootDirName,'\models_', num2str(nMixtures),'.mat'));
    models = models.models;
else
    background_models = [];
    if exist(strcat(rootDirName, '\backgroud_models_', num2str(nMixtures),'.mat'),'file')
        background_models = load(strcat(rootDirName, '\background_models_', num2str(nMixtures),'.mat'));
        background_models = background_models.background_models;
    end
    models = training_iVector(rootDirName, nMixtures, background_models, 0 ,0);
end

nSpeakers = models.nSpeakers;

end
