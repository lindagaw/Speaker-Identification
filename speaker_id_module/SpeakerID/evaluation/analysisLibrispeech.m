%set result_folder, sessionSize first
mixtures = {'512', '1024', '2048'};

speakerResult =[];
result_index = 1;

correct_total = zeros(length(result_folder),3,3);
total_total = zeros(length(result_folder),3,3);

for folder_i = 1 : length(result_folder)
    for index = 1 : length(mixtures)
        mixture = mixtures{index};
        correct_by_file = load(strcat(result_folder{folder_i}, '\gmm_correct_by_file_',mixture,'_',num2str(sessionSize),'.txt'));
        total_by_file = load(strcat(result_folder{folder_i}, '\gmm_total_by_file_',mixture,'_',num2str(sessionSize),'.txt'));

        for i = 1 : size(correct_by_file,2)
           speakerResult(result_index+i-1, index,:) =...
               [sum(correct_by_file(:,i)) / sum(total_by_file(:,i))];
        end
        
        correct_total(folder_i,index,:) = sum(correct_by_file);
        total_total(folder_i,index,:) = sum(total_by_file);
        
    end
    result_index = result_index + size(correct_by_file,2);
end

% plotBarStackGroups(speakerResult, {'1','2','3'});
