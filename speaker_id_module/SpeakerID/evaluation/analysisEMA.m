%set result_folder first
mixtures = {'64', '128', '256'};

moodresult =[];
index = 1;
for index = 1 : length(mixtures)

    mixture = mixtures{index};
    cannotdecide_by_file = load(strcat(result_folder, '\canotdecide_by_file_',mixture,'.txt'));
    correct_by_file = load(strcat(result_folder, '\correct_by_file_',mixture,'.txt'));
    total_by_file = load(strcat(result_folder, '\total_by_file_',mixture,'.txt'));
    cannotdecide_by_speaker = load(strcat(result_folder, '\canotdecide_by_speaker_',mixture,'.txt'));
    correct_by_speaker = load(strcat(result_folder, '\correct_by_speaker_',mixture,'.txt'));
    total_by_speaker = load(strcat(result_folder, '\total_by_speaker_',mixture,'.txt'));

    cannot_decide_ratio = cannotdecide_by_speaker ./ total_by_speaker;
    correct_ratio = correct_by_speaker ./ total_by_speaker;
    
    if(sum(cannotdecide_by_speaker(1:4:end)) ~= 0 || ...
        sum(cannotdecide_by_speaker(2:4:end)) ~= 0 || ...
        sum(cannotdecide_by_speaker(3:4:end)) ~= 0 || ...
        sum(cannotdecide_by_speaker(4:4:end)) ~= 0)

        moodresult(1,index,:) = [sum(correct_by_speaker(1:4:end)) / sum(total_by_speaker(1:4:end)) sum(cannotdecide_by_speaker(1:4:end)) / sum(total_by_speaker(1:4:end))];
        moodresult(2,index,:) = [sum(correct_by_speaker(2:4:end)) / sum(total_by_speaker(2:4:end)) sum(cannotdecide_by_speaker(2:4:end)) / sum(total_by_speaker(2:4:end))]; 
        moodresult(3,index,:) = [sum(correct_by_speaker(3:4:end)) / sum(total_by_speaker(3:4:end)) sum(cannotdecide_by_speaker(3:4:end)) / sum(total_by_speaker(3:4:end))]; 
        moodresult(4,index,:) = [sum(correct_by_speaker(4:4:end)) / sum(total_by_speaker(4:4:end)) sum(cannotdecide_by_speaker(4:4:end)) / sum(total_by_speaker(4:4:end))]; 
    else
        moodresult(1,index,:) = [sum(correct_by_speaker(1:4:end)) / sum(total_by_speaker(1:4:end))];
        moodresult(2,index,:) = [sum(correct_by_speaker(2:4:end)) / sum(total_by_speaker(2:4:end))]; 
        moodresult(3,index,:) = [sum(correct_by_speaker(3:4:end)) / sum(total_by_speaker(3:4:end))]; 
        moodresult(4,index,:) = [sum(correct_by_speaker(4:4:end)) / sum(total_by_speaker(4:4:end))]; 

    end
end

result = moodresult(:,:,1);
%plotBarStackGroups(moodresult, {'angry','happy','neutral','sad'});
