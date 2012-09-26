signal = load('0_Signals.lvm')

t_step = 054 / 40000000;
t = 0:t_step:t_step * length(signal(:, 1)) - t_step;

figure(1)
plot(t, signal(:,2))

axis tight
xlabel('Time (Seconds)', 'FontSize', 13)
ylabel('Amplitude (Volts)', 'FontSize', 13)
set(gca, 'FontSize', 13)
title({'PZT B response without diode circuit'}, 'FontSize', 13)
print('-depsc', 'noCiruit');