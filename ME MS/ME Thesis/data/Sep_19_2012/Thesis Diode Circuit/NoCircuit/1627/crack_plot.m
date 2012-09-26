function []= crack_plot(foldernameUncracked, foldernameCracked)



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
xlabel('Iteration Number', 'FontSize', 13)
ylabel('Sum of Squared Differences', 'FontSize', 13)
set(gca, 'FontSize', 13)
title({'Sum of Squared Differences from First Iteration','to Successive Iterations with the defect introduced', 'on the 6th iteration for the steel rod'}, 'FontSize', 13)
print('-depsc', 'steelDifferences');

uncrackedAvg = sum(uncracked) / 5;
crackedAvg = sum(cracked) / 5;

figure(2)
plot(uncrackedAvg(1,:,2))

axis tight
xlabel('Time (Seconds)', 'FontSize', 13)
ylabel('Amplitude (Volts)', 'FontSize', 13)
set(gca, 'FontSize', 13)
title({'Averaged undamaged steel rod signal response'}, 'FontSize', 13)
print('-depsc', 'steelUncracked');

figure(3)
plot(crackedAvg(1,:,2))

axis tight
xlabel('Time (Seconds)', 'FontSize', 13)
ylabel('Amplitude (Volts)', 'FontSize', 13)
set(gca, 'FontSize', 13)
title({'Averaged cracked steel rod signal response'}, 'FontSize', 13)
print('-depsc', 'steelCracked');