function [models] = training_load_feature(rootDirName, nMixtures, ubm, only_ubm, only_1_microphone)

fprintf(1, 'Training directory: %s\n', rootDirName);

% Set the parameters of the experiment
folders = dir(strcat(rootDirName,'\singles'));
nSpeakers = length(folders) - 2;
if nargin < 2
    nMixtures = 256;        % How many mixtures used to generate data
end
if nargin < 5
    only_1_microphone = 0;
end
nWorkers = feature('NumCores');           % Number of parfor workers, if available

models = {};

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = load(strcat(rootDirName,'\speaker_feature.mat'));
trainSpeakerData = trainSpeakerData.trainSpeakerData;

models.ubm = ubm;

if(~only_ubm)
    % Adapt the UBM to each speaker to create GMM speaker model.
    map_tau = 10.0;
    config = 'mwv';
    gmm = cell(nSpeakers - 1, 1);
    parfor s=1:nSpeakers - 1
        speakerFeature = [];
        for i = 1 : length(trainSpeakerData{s+1})
            speakerFeature = [speakerFeature trainSpeakerData{s+1}{i}];
        end
        cellFeature = cell(1,1);
        cellFeature{1} = speakerFeature;
        gmm{s} = mapAdapt(cellFeature, ubm, map_tau, config);
    end
    
    models.gmm = gmm;
    save(strcat(rootDirName,'/models_', num2str(nMixtures),'.mat'), 'models');
end

end
