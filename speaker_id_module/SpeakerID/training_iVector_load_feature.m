function models = training_iVector_load_feature(rootDirName, nMixtures, background_models, only_backgroundSpeaker, only_1_microphone)

fprintf(1, 'Training directory: %s\n', rootDirName);

if nargin < 2
    nMixtures = 256;        % How many mixtures used to generate data
end
if nargin < 4
    only_backgroundSpeaker = 0;
end
if nargin < 5
    only_1_microphone = 0;
end

% Set the parameters of the experiment
folders = dir(strcat(rootDirName,'\singles'));
nSpeakers = length(folders) - 2;

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = load(strcat(rootDirName,'\speaker_feature.mat'));
trainSpeakerData = trainSpeakerData.trainSpeakerData;

if(~only_backgroundSpeaker)
    models = background_models;    
    ubm = background_models.ubm;
    T = background_models.T;
    V = background_models.V;
    ldaDim = background_models.ldaDim;
    tvDim = background_models.tvDim;
    
    % Now compute the ivectors for each speaker
    modelIVs = zeros(tvDim, nSpeakers-1);
    
    parfor s=1:nSpeakers-1     
        for i = 1:length(trainSpeakerData{s+1})            
            [n, f] = compute_bw_stats(trainSpeakerData{s+1}{i}, ubm);
            modelIVs(:, s) = modelIVs(:, s) + extract_ivector([n;f], ubm, T);
        end        
        modelIVs(:, s) = modelIVs(:, s) / length(trainSpeakerData{s+1});  % take the average
    end

    % Now do LDA on the iVectors to find the dimensions that matter.
    modelIVs = V(:, 1:ldaDim)' * modelIVs;

    % save the model
    models.modelIVs = modelIVs;
    models.nSpeakers = nSpeakers-1;

    save(strcat(rootDirName,'/models_', num2str(nMixtures),'.mat'), 'models');
end

end
