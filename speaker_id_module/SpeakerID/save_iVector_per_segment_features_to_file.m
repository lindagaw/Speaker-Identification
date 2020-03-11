function models = save_iVector_per_segment_features_to_file(rootDirName, nMixtures)

fprintf(1, 'Training directory: %s\n', rootDirName);

background_models = load(strcat(rootDirName, '/background_models_', num2str(nMixtures),'.mat'));
background_models = background_models.background_models;

% Set the parameters of the experiment
folders = dir(strcat(rootDirName,'/singles'));
nSpeakers = length(folders) - 2;

% Get sound feature from every WAV file in the training folder.
trainSpeakerData = load(strcat(rootDirName,'/speaker_feature.mat'));
trainSpeakerData = trainSpeakerData.trainSpeakerData;

models = background_models;    
ubm = background_models.ubm;
T = background_models.T;
V = background_models.V;
ldaDim = background_models.ldaDim;

% Now compute the ivectors for each speaker
modelIVsPerSegments = [];
modelIVsPerSegmentsLabel = [];

ivectorStep = 300;    
parfor s=1:nSpeakers-1     
    totalnumberofSegments = 0;
    for i = 1:length(trainSpeakerData{s+1})
        for j = 1:ivectorStep:length(trainSpeakerData{s+1}{i})  

            j_end = min(j+ivectorStep,length(trainSpeakerData{s+1}{i}));
            if ( j_end - j < ivectorStep * 0.5) % the last part is too short, discard it.
                continue;
            end

            [n, f] = compute_bw_stats( trainSpeakerData{s+1}{i}( : , j : j_end), ubm);
            ivector = extract_ivector([n;f], ubm, T);
            modelIVsPerSegments = [modelIVsPerSegments ; ivector'];
            modelIVsPerSegmentsLabel = [modelIVsPerSegmentsLabel; s];
            totalnumberofSegments = totalnumberofSegments + 1;
        end
    end
end

% Now do LDA on the iVectors to find the dimensions that matter.
modelIVsPerSegments = modelIVsPerSegments * V(:, 1:ldaDim);

% save modelIVsPerSegments
save(strcat(rootDirName,'/modelIVsPerSegments_', num2str(nMixtures),'.mat'), 'modelIVsPerSegments');
save(strcat(rootDirName,'/modelIVsPerSegmentsLabel_', num2str(nMixtures),'.mat'), 'modelIVsPerSegmentsLabel');

end
