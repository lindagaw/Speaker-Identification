results = cell(5,1);
index = 1;
for distance = [1, 2, 3, 5, 8]
    result = [];
    for group = 1 : 4
        gmmResultsAll = load(strcat('K:\Audio-Dataset\ControlLab\USED-MFCC-CMN-Rev-Warping', ...
                                    '\ControlLab-final-close-training-all-detection-group', ...
                                    num2str(group), '\detection-', num2str(distance),'\gmm_1024_5000.mat'));
        gmmResultsAll = gmmResultsAll.gmmResultsAll(:,2);
        for i = 1:length(gmmResultsAll)
            for j = 1:length(gmmResultsAll{i})
                gmmResult = gmmResultsAll{i}{j};
                maxValue = max(gmmResult,[],2);
                result = [result; maxValue];
            end
        end
    end
    results{index} = result;
    index = index + 1;
end

figure();
hold on;
for i = 1:5
    plot(results{i});
end


resultswrong = cell(5,1);
index = 1;
for distance = [1, 2, 3, 5, 8]
    resultwrong = [];
    for group = 1 : 4
        gmmResultsAll = load(strcat('K:\Audio-Dataset\ControlLab\USED-MFCC-CMN-Rev-Warping', ...
                                    '\ControlLab-final-close-training-all-detection-group', ...
                                    num2str(group), '\detection-', num2str(distance),'\gmm_wrong1024_5000.mat'));
        gmmResultsAll = gmmResultsAll.gmmResultsAll(:,2);
        for i = 1:length(gmmResultsAll)
            for j = 1:length(gmmResultsAll{i})
                gmmResult = gmmResultsAll{i}{j};
                maxValue = max(gmmResult,[],2);
                resultwrong = [resultwrong; maxValue];
            end
        end
    end
    resultswrong{index} = resultwrong;
    index = index + 1;
end

figure();
hold on;
for i = 1:5
    plot(resultswrong{i});
end

