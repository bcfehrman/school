data = load('../data/prog1_data_out.txt');

figure(1)
plot(data(:,1), data(:,2))

xlabel('X Position', 'FontSize', 14, 'interpreter', 'latex')
ylabel('Y Position', 'FontSize', 14, 'interpreter', 'latex')
set(gca, 'FontSize', 14)

%legend('Theoretical Defect Response (Initial Phase)', 'Experimental Defect Response (Initial Phase)', 'Theoretical Defect Response (TR Phase (I1))', 'Experimental Defect Response (TR Phase (I1))')
title({'Plot of the Robot Path Starting at (0,0,0)', 'and Ending at about (3159,-3410,11.73)'}, 'FontSize', 14)

print('-depsc',  '../data/prog1_plot.eps');