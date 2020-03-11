function fileList = getAllFilesWithExtension(dirName, extension)

  dirData = dir(dirName);      %# Get the data for the current directory
  dirIndex = [dirData.isdir];  %# Find the index for directories
  fileList = {dirData(~dirIndex).name}';  %'# Get a list of the files
  if ~isempty(fileList)
    extFileList = {};
    for i = 1 : length(fileList)
        [~,~,ext] = fileparts(fileList{i});
        if(strcmp(ext, extension) == 1)
            if(isempty(extFileList))
                extFileList = {fileList{i}};
            else
                extFileList = [extFileList; fileList{i}];
            end
        end
    end
    fileList = extFileList;
    fileList = cellfun(@(x) fullfile(dirName,x),...  %# Prepend path to files
                       fileList,'UniformOutput',false);
  end
  subDirs = {dirData(dirIndex).name};  %# Get a list of the subdirectories
  validIndex = ~ismember(subDirs,{'.','..'});  %# Find index of subdirectories
                                               %#   that are not '.' or '..'
  for iDir = find(validIndex)                  %# Loop over valid subdirectories
    nextDir = fullfile(dirName,subDirs{iDir});    %# Get the subdirectory path
    fileList = [fileList; getAllFilesWithExtension(nextDir, extension)];  %# Recursively call getAllFiles
  end

end