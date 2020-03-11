%set result_folder first
result_folder = 'C:\OneDrive\UVa-Lab\SpeakerIDPaper\results\emotion-mfcc-cmn-warp-no-revb-distance\12';
mixtures = {'64', '128', '256'};

moodresult = zeros(4,3,4,2);
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

        for i = 1 : 2
            moodresult(1,index,i,:) = [sum(correct_by_speaker(1:4:end,i)) / sum(total_by_speaker(1:4:end,i)) sum(cannotdecide_by_speaker(1:4:end,i)) / sum(total_by_speaker(1:4:end,i))];
            moodresult(2,index,i,:) = [sum(correct_by_speaker(2:4:end,i)) / sum(total_by_speaker(2:4:end,i)) sum(cannotdecide_by_speaker(2:4:end,i)) / sum(total_by_speaker(2:4:end,i))]; 
            moodresult(3,index,i,:) = [sum(correct_by_speaker(3:4:end,i)) / sum(total_by_speaker(3:4:end,i)) sum(cannotdecide_by_speaker(3:4:end,i)) / sum(total_by_speaker(3:4:end,i))]; 
            moodresult(4,index,i,:) = [sum(correct_by_speaker(4:4:end,i)) / sum(total_by_speaker(4:4:end,i)) sum(cannotdecide_by_speaker(4:4:end,i)) / sum(total_by_speaker(4:4:end,i))];
        end
    else
        for i = 1 : 2
            moodresult(1,index,i,:) = [sum(correct_by_speaker(1:4:end,i)) / sum(total_by_speaker(1:4:end,i))];
            moodresult(2,index,i,:) = [sum(correct_by_speaker(2:4:end,i)) / sum(total_by_speaker(2:4:end,i))]; 
            moodresult(3,index,i,:) = [sum(correct_by_speaker(3:4:end,i)) / sum(total_by_speaker(3:4:end,i))]; 
            moodresult(4,index,i,:) = [sum(correct_by_speaker(4:4:end,i)) / sum(total_by_speaker(4:4:end,i))]; 
        end
    end
end

result = moodresult(:,:,:,1);
%plotBarStackGroups(moodresult, {'angry','happy','neutral','sad'});
