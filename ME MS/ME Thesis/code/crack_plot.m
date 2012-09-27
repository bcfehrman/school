function []= crack_plot(foldernameUncracked, foldernameCracked)

font_s = 15;

time_step = 54/40000000;

for(i = 0:4)
filename = strcat(foldernameUncracked,  num2str(i), '_Signals', '.lvm');
uncracked(i+1,:,:) = load(filename);
	
filename = strcat(foldernameCracked,  num2str(i), '_Signals', '.lvm');
cracked(i+1,:,:) = load(filename);
end


for(i = 1:4)
difference(i) = 0;
	for(j = 1:size(cracked)(2))
	unsquared = uncracked(1,j,2) - uncracked(i+1,j,2) ;
	difference(i) = difference(i) + unsquared^2;
	end
end

for(i = 1:5)
difference(i+5) = 0;
	for(j = 1:size(cracked)(2))
	unsquared = uncracked(1,j,2) - cracked(i,j,2) ;
	difference(i+5) = difference(i+5) + unsquared^2;
	end
end

figure(1)
plot(difference)

axis tight
xlabel('Iteration Number', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Sum of Squared Differences', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Sum of Squared Differences from First Iteration','to Successive Iterations with the defect introduced', 'on the 6th iteration for the steel rod'}, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'steelDifferences');

uncrackedAvg = sum(uncracked) / 5;
crackedAvg = sum(cracked) / 5;

t = 0:time_step:time_step * length(uncrackedAvg) - time_step;

figure(2)
plot(t,uncrackedAvg(1,:,2))

axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Averaged undamaged steel rod signal response'}, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'steelUncracked');

figure(3)
plot(t,crackedAvg(1,:,2))

axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Averaged cracked steel rod signal response'}, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'steelCracked');