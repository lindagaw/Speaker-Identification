rootDirName = 'K:\Audio-Dataset\EmotionalTest\OpenSmileFeatureTest\train';

% Set the parameters of the experiment
ubmfiles = dir(strcat(rootDirName, '\ubm-feature'));
speakerfiles = dir(strcat(rootDirName,'\feature-1'));

nSpeakers = length(speakerfiles) - 2;

nMixtures = 256;        % How many mixtures used to generate data

nWorkers = feature('NumCores');           % Number of parfor workers, if available

models = {};

% Get ubm feature data
trainUBMData = cell( length(ubmfiles) - 2, 1 );
times = length(ubmfiles) - 2;

parfor i = 1 : times
    file = strcat(rootDirName,'\ubm-feature\',ubmfiles(i+2).name);
    [numbers, ~] = swallow_csv(file);
    numbers = numbers';
    trainUBMData{i} = numbers([1:5 18:36] ,:);
end
% Create the universal background model from the first wav file's speaker data
nmix = nMixtures;           % In this case, we know the # of mixtures needed
final_niter = 10;
ds_factor = 1;

ubm = gmm_em(trainUBMData, nmix, final_niter, ds_factor, nWorkers);
save(strcat(rootDirName,'/models_ubm_manully_selected', num2str(nMixtures),'.mat'), 'ubm');

models.ubm = ubm;

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = cell(nSpeakers, 1);

times = length(speakerfiles) - 2;

parfor i = 1 : times
    file = strcat(rootDirName,'\feature-1\',speakerfiles(i+2).name);
    
    [numbers, ~] = swallow_csv(file);
    currSpeakerData = cell(1,1);  
    numbers = numbers';
    currSpeakerData{1} = numbers([1:5 18:36] ,:);
    
    trainSpeakerData{i} = currSpeakerData;
end

% Adapt the UBM to each speaker to create GMM speaker model.
map_tau = 10.0;
config = 'mwv';
gmm = cell(nSpeakers, 1);

parfor s=1:nSpeakers
    gmm{s} = mapAdapt(trainSpeakerData{s}, ubm, map_tau, config);
end

models.gmm = gmm;

save(strcat(rootDirName,'/models_manully_selected', num2str(nMixtures),'.mat'), 'models');

