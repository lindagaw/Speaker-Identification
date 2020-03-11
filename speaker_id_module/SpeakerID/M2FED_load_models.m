function [ models, nSpeakers, isOK ] = M2FED_load_models (rootDirName, nMixtures)
% this function will be used by C++ program.
% if the model exists, it loads. if not, it trains.

fprintf(1, 'Training directory: %s\n', rootDirName);
isOK = 0;

% training
if exist(strcat(rootDirName, '\models_', num2str(nMixtures),'.mat'),'file') 
    models = load(strcat(rootDirName,'\models_', num2str(nMixtures),'.mat'));
    models = models.models;
    nSpeakers = size(models.gmm, 1);
    isOK = 1;
end


end
