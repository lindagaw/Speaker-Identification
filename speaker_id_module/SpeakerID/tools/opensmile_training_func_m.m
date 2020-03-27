rootDirName = 'K:\Audio-Dataset\EmotionalTest\OpenSmileFeatureTest\train';

% Set the parameters of the experiment
ubmfiles = dir(strcat(rootDirName, '\ubm-feature-func-m-25'));
speakerfiles = dir(strcat(rootDirName,'\feature-func-m-25'));

nSpeakers = length(speakerfiles) - 2;

nMixtures = 256;        % How many mixtures used to generate data

nWorkers = feature('NumCores');           % Number of parfor workers, if available

models = {};

% Get ubm feature data
trainUBMData = cell( length(ubmfiles) - 2, 1 );
times = length(ubmfiles) - 2;

parfor i = 1 : times
    file = strcat(rootDirName,'\ubm-feature-func-m-25\',ubmfiles(i+2).name);
    [numbers, ~] = swallow_csv(file);
    trainUBMData{i} = numbers';
end
% Create the universal background model from the first wav file's speaker data
nmix = nMixtures;           % In this case, we know the # of mixtures needed
final_niter = 10;
ds_factor = 1;

ubm = gmm_em(trainUBMData, nmix, final_niter, ds_factor, nWorkers);
save(strcat(rootDirName,'/models_ubm_m_25_', num2str(nMixtures),'.mat'), 'ubm');

models.ubm = ubm;

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = cell(nSpeakers, 1);

times = length(speakerfiles) - 2;

parfor i = 1 : times
    file = strcat(rootDirName,'\feature-func-m-25\',speakerfiles(i+2).name);
    
    [numbers, ~] = swallow_csv(file);
    currSpeakerData = cell(1,1);  
    currSpeakerData{1} = numbers';
    
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

save(strcat(rootDirName,'/models_m_25_', num2str(nMixtures),'.mat'), 'models');



% Set the parameters of the experiment
ubmfiles = dir(strcat(rootDirName, '\ubm-feature-func-m-50'));
speakerfiles = dir(strcat(rootDirName,'\feature-func-m-50'));

nSpeakers = length(speakerfiles) - 2;

nMixtures = 256;        % How many mixtures used to generate data

nWorkers = feature('NumCores');           % Number of parfor workers, if available

models = {};

% Get ubm feature data
trainUBMData = cell( length(ubmfiles) - 2, 1 );
times = length(ubmfiles) - 2;

parfor i = 1 : times
    file = strcat(rootDirName,'\ubm-feature-func-m-50\',ubmfiles(i+2).name);
    [numbers, ~] = swallow_csv(file);
    trainUBMData{i} = numbers';
end
% Create the universal background model from the first wav file's speaker data
nmix = nMixtures;           % In this case, we know the # of mixtures needed
final_niter = 10;
ds_factor = 1;

ubm = gmm_em(trainUBMData, nmix, final_niter, ds_factor, nWorkers);
save(strcat(rootDirName,'/models_ubm_m_50_', num2str(nMixtures),'.mat'), 'ubm');

models.ubm = ubm;

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = cell(nSpeakers, 1);

times = length(speakerfiles) - 2;

parfor i = 1 : times
    file = strcat(rootDirName,'\feature-func-m-50\',speakerfiles(i+2).name);
    
    [numbers, ~] = swallow_csv(file);
    currSpeakerData = cell(1,1);  
    currSpeakerData{1} = numbers';
    
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

save(strcat(rootDirName,'/models_m_50_', num2str(nMixtures),'.mat'), 'models');
