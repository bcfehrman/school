signal = load('0_Signals.lvm')
font_s = 15;
t_step = 054 / 40000000;
t = 0:t_step:t_step * length(signal(:, 1)) - t_step;

figure(1)
plot(t, signal(:,2))

axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'PZT B response without diode circuit'}, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'noCircuit');