function plot_VAD(audio_dir)
    [y,fs]=audioread(audio_dir);
    try
        y = (y(:,1)+y(:,2))/2;
    catch
        % Nothing to do
    end

    s = audioread(audio_dir);
    figure
    t = (1:length(s))./16000;
    p1 = plot(t, s);
    hold on
    
    ylim([-0.3 0.6]);
    xlim([0 t(end)]);
end
