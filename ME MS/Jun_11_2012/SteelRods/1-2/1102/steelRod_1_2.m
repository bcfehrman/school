close all
clear all

wave = load('initialWave.lvm');
basename = "steel-1-2_"

%Get the correct time array for plotting
time_step = 54/40000000;

t_wave = 0:time_step:time_step * length(wave) - time_step;

figure_count= 1;

figure(figure_count)
plot(t_wave, wave)
title('Wave used')
xlabel('Time (seconds)')
ylabel('Amplitude (volts)')
figure_count = figure_count + 1;

%Taking speed of sound in structural steel to be 4512 m/s and the rod
%length to be .580m, we get that it will take about 1.28546e-4 seconds for
%the wave to reach the defect PZT. With a sampling time of 1.35e-6, we get
%that the wave should appear at the defect by about the 95th sample. Add
%the length of the initial wave to this and that will be our total wave
%signal length for graphing.

speed_sound = 4500;
l_rod = .579;
sample_speed = 1.35e-6;
travel_time = l_rod / speed_sound;
num_samples = travel_time / sample_speed;

chirpin = [zeros(num_samples, 1)' wave(:,1)'];

N = num_samples + length(wave);


t_theory = 0:time_step:time_step * N - time_step;


eps0 = 8.854e-12; eps33 = 450*eps0; d33 = 152.0e-12; c33 = 11.3e10; c33st = 10.8e10; 
R0 = 1e2; RL = R0;
rho = 7.6e3; rhost = 2.695e3; a = 12.25e-3; l = 10e-3; L = 580e-3; 
% Steel rod
rhost = 7.8e3;c33st = 20e10; a = 19.05e-3; L = 0.6096;
a = 12.7e-3; l = 12e-3; L = 580e-3;
lc = L; lT = 310e-3;
%
% M magnification applied in play-back
% Can M be set as the reciprocal of the maximum voltage of the reversed
% impulse response
%
M = 10; 
%
% Voltage V0
V0 = 20.0;
c = sqrt(c33/rho); cst = sqrt(c33st/rhost);
s33 = 1/c33; s33st = 1/c33st;
%
% Effect of crack is a localized change in c33st and rhost.
c33stc = 0.9*c33st;
rhostc = 0.9*rhost;

d3 = 1 - d33^2/(s33*eps33);
c33bar = 1/(s33*d3);
d33bar = (-d33/eps33)/d3;
eps33bar = eps33/d3;
%
%
area = pi*a^2;
Cc = eps33bar*area/l;
%
alpha = 1/(RL*Cc);

Rterm1 = c33bar + (d33bar*d33)/(2*s33);
Rterm2 = c33st;
% R reflection coefficient
R = (Rterm1 - Rterm2)/(Rterm1 + Rterm2);
Rc = (c33st - c33stc)/(c33st + c33stc);
Rhat = (Rterm2-Rterm1)/(Rterm1+Rterm2);
T = 1-abs(R);
Tc = 1-abs(Rc);
That = 1 - abs(Rhat);
%
wactamp = ((-2*s33*RL*Cc)/(d33*area))*(abs(T)*Cc^2+abs(T^2)*abs(That)*M)*V0;
wactamp = ((-2*s33*abs(T))/(d33*area*RL))*(1+(abs(T)*abs(That)*M/RL/Cc))*V0;
wactamp = abs(T)*(1+abs(T)*abs(That)*M/RL/Cc);
%wcrackamp = (4*s33*T^3*RL)/(d33*area*R0*Cc*c*cst)*V0;
wcrackamp = (-2*s33*abs(T)/(d33*area*RL))*(abs(T)*abs(That)*M/RL/Cc)*(Rc+Tc)*V0;
wcrackamp = abs(T)*(abs(T)*abs(That)*M/RL/Cc)*(Rc+Tc);
%

deltt = 54/40e6/1;
delt = deltt;
%chirpin = zeros(N+1,1);
%load chirpin.dat
chirprevcrack = zeros(N,1);
vlongchirp = zeros(N,1);
%
for i = 1:N;
    t(i) = (i-1)*deltt;
    wcrack(i) = wcrackamp*exp(-t(i)*alpha);
end


for k = 1:N;
    sum = 0;
    for i = 1:k;
        sum = sum + wcrack(i)*chirpin(k-i+1)*delt;
    end
    chirprevcrack(k) = sum;
end
%
Pamp = (d33*area*RL)/(2*s33);
amp = 1.0e-2; %scaling for transducer
vlongchirp1 = Pamp*amp*chirprevcrack;
vlongchirp2 = Pamp*amp*chirprevcrack * 1.2;

%Number of iteration samples to look at. There are 6 total available
num_iterations = 2;

defect = [];

%Load the data from each iteration
    
temp = load( '0_Signals.lvm');
temp2 = temp(:,3);
defect(:,1) = temp2(1:N);

temp = load( '1_Signals.lvm');
temp2 = temp(:,3);
defect(:,2) = temp2(1:N);


defect_avg = 0;

avg_start = 1;

%Get average value of end of signals. Used for trying to center signals around zero
for i = avg_start:N
   defect_avg = defect_avg + defect(i,1);
end

defect_avg = defect_avg / (N - avg_start);

%Subtract the average value of each signal from the signal. This attempts
%to center each signal around zero since the voltage readings aren't
%centered around zero.
for i = 1:num_iterations
    defect(:,i) = defect(:,i) - defect_avg; 
end

defect_cat = [];

for i = 1:num_iterations
  defect_cat = [defect_cat defect(:,i)'];
end

theory_cat = [vlongchirp1(:,1)' vlongchirp2(:,1)'];

t_theory_cat = 0:time_step:time_step * length(theory_cat) - time_step;

figure(figure_count)
plot(t_theory, vlongchirp1, t_theory, defect(:, 1))
title({'Theoretical and Experimental Defect Response Superimposed', '(Initial Phase)'})
xlabel('Time (seconds)')
ylabel('Amplitude (volts)')
legend('Theoretical Defect Response', 'Experimental Defect Response')
figure_count = figure_count + 1;

figure(figure_count)
plot(t_theory, vlongchirp2, t_theory, defect(:, 2))
title({'Theoretical and Experimental Defect Response Superimposed', '(Time Reversal Phase {First Iteration})'})
xlabel('Time (seconds)')
ylabel('Amplitude (volts)')
legend('Theoretical Defect Response', 'Experimental Defect Response')
figure_count = figure_count + 1;

figure(figure_count)
plot(t_theory_cat, theory_cat, t_theory_cat, defect_cat)
title({'Theoretical and Experimental Defect Responses Superimposed', 'Intial Phase and Time Reversal Phase {First Iteration}', '(actual time delay between phases not shown due to magnitude)'})
xlabel('Time (seconds)')
ylabel('Amplitude (volts)')
legend('Theoretical Defect Response', 'Experimental Defect Response')
figure_count = figure_count + 1;

t_theory2 = time_step*N:time_step: 2 * time_step*N - time_step;

%This plot just colors the two different phases so you can see where one
%ends and the next begins. Of course, the actual time delay is not shown
%since it was 1500ms in the program which is significantly larger than the
%time scale we have for these waves.

figure(figure_count)
plot(t_theory, vlongchirp1,'-r','LineWidth',4, t_theory, defect(:, 1),'--b','LineWidth',4, t_theory2, vlongchirp2,'-r','LineWidth',4, t_theory2, defect(:, 2),'--b','LineWidth',4)
axis tight
%title({'Theoretical and Experimental Defect Responses Superimposed', 'Intial Phase and Time Reversal Phase {First Iteration}', '(actual time delay between phases not shown due to magnitude)'})
xlabel('Time (Seconds)', 'FontSize', 13)
ylabel('Amplitude (Volts)', 'FontSize', 13)
set(gca, 'FontSize', 13)

%legend('Theoretical Defect Response (Initial Phase)', 'Experimental Defect Response (Initial Phase)', 'Theoretical Defect Response (TR Phase (I1))', 'Experimental Defect Response (TR Phase (I1))')
title('Defect response for 579mm and 437mm steel rods', 'FontSize', 13)
thisLegend = legend('Theoretical Defect Response', 'Experimental Defect Response','Location','NorthWest')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', 13')

print('-depsc',  strcat(basename, 'Iter_th_exp'));

initial = load(strcat(basename, 'Initial.lvm'));

t = 0:time_step:length(initial) * time_step - time_step;

figure(figure_count)
plot(t, initial,'LineWidth',4)
axis tight
%title({'Theoretical and Experimental Defect Responses Superimposed', 'Intial Phase and Time Reversal Phase {First Iteration}', '(actual time delay between phases not shown due to magnitude)'})
xlabel('Time (Seconds)', 'FontSize', 13)
ylabel('Amplitude (Volts)', 'FontSize', 13)
set(gca, 'FontSize', 13)

%legend('Theoretical Defect Response (Initial Phase)', 'Experimental Defect Response (Initial Phase)', 'Theoretical Defect Response (TR Phase (I1))', 'Experimental Defect Response (TR Phase (I1))')
title({'Response at defect PZT on initial iteration for the steel rods', 'of length 579mm and 437mm'}, 'FontSize', 13)

figure_count = figure_count + 1;
set(thisLegend, 'FontSize', 13')

print('-depsc',  strcat(basename, 'Initial'));

final = load(strcat(basename, 'Final.lvm'));

figure(figure_count)
plot(t, final,'LineWidth',4)
axis tight
%title({'Theoretical and Experimental Defect Responses Superimposed', 'Intial Phase and Time Reversal Phase {First Iteration}', '(actual time delay between phases not shown due to magnitude)'})
xlabel('Time (Seconds)', 'FontSize', 13)
ylabel('Amplitude (Volts)', 'FontSize', 13)
set(gca, 'FontSize', 13)

%legend('Theoretical Defect Response (Initial Phase)', 'Experimental Defect Response (Initial Phase)', 'Theoretical Defect Response (TR Phase (I1))', 'Experimental Defect Response (TR Phase (I1))')
title({'Response at defect PZT on the 150th iteration for the steel rods', 'of length 579mm and 437mm'}, 'FontSize', 13)

figure_count = figure_count + 1;
set(thisLegend, 'FontSize', 13')

print('-depsc',  strcat(basename, 'Final'));

gain = load(strcat(basename, 'iterationVsGain.lvm'));

figure(figure_count)
plot(gain,'LineWidth',4)
axis tight
%title({'Theoretical and Experimental Defect Responses Superimposed', 'Intial Phase and Time Reversal Phase {First Iteration}', '(actual time delay between phases not shown due to magnitude)'})
xlabel('Iteration Number', 'FontSize', 13)
ylabel('Gain (currentAmp / initialAmp)', 'FontSize', 13)
set(gca, 'FontSize', 13)

%legend('Theoretical Defect Response (Initial Phase)', 'Experimental Defect Response (Initial Phase)', 'Theoretical Defect Response (TR Phase (I1))', 'Experimental Defect Response (TR Phase (I1))')
title({'The amplitude gain of the response seen at the defect on each', 'iteration for the 579mm and 437mm steel rods'}, 'FontSize', 13)

figure_count = figure_count + 1;
set(thisLegend, 'FontSize', 13')

print('-depsc',  strcat(basename, 'iterationVsGain'));