%Originally written by Dr. Korde, modified and appended
%by Brian Fehrman

%Clean slate
close all
clear all

%Load the wave that was initially sent
wave = load('initialWave.lvm');

%Set the basename for file opens/saves
basename = "steel-2-3_"

%Set the font size
font_s = 15;

%Get the correct time array for plotting
time_step = 54/40000000;

%Create time array for plotting
t_wave = 0:time_step:time_step * length(wave) - time_step;

%Plot initial wave
figure_count= 1;
figure(figure_count)
plot(t_wave, wave)
title('Wave used')
xlabel('Time (seconds)')
ylabel('Amplitude (volts)')
figure_count = figure_count + 1;


%Taking speed of sound in structural steel to be 4623 m/s and
%sample speed of FPGA to be 1.35e-6, determine time taken for
%the wave to reach the defect location
speed_sound = 4623;
l_rod = .437;
sample_speed = 1.35e-6;
travel_time = l_rod / speed_sound;
num_samples = travel_time / sample_speed;

%Wave sent
chirpin = [zeros(num_samples, 1)' wave(:,1)'];

%Get correct size time array for plotting
N = num_samples + length(wave);
t_theory = 0:time_step:time_step * N - time_step;

% Steel rod and piezo parameters
eps0 = 8.854e-12; eps33 = 450*eps0; d33 = 152.0e-12; c33 = 11.3e10;
R0 = 1e2; RL = R0; rho = 7.6e3; 
rhost = 7.894e3;c33st = 16.939e10; a = 19.05e-3;
a = 12.7e-3; l = 12e-3; L = 437e-3;
lc = L; lT = 310e-3;

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

%Terms in constitutive relations
d3 = 1 - d33^2/(s33*eps33);
c33bar = 1/(s33*d3);
d33bar = (-d33/eps33)/d3;
eps33bar = eps33/d3;
area = pi*a^2;
Cc = eps33bar*area/l;
alpha = 1/(RL*Cc);
Rterm1 = c33bar + (d33bar*d33)/(2*s33);
Rterm2 = c33st;

% R reflection coefficient
R = (Rterm1 - Rterm2)/(Rterm1 + Rterm2);
Rc = (c33st - c33stc)/(c33st + c33stc);
Rhat = (Rterm2-Rterm1)/(Rterm1+Rterm2);

%T reflection coefficient
T = 1-abs(R);
Tc = 1-abs(Rc);
That = 1 - abs(Rhat);

%Actuator amplitude coefficient
wactamp = abs(T)*(1+abs(T)*abs(That)*M/RL/Cc);

%Crack amplitude coefficient
wcrackamp = abs(T)*(abs(T)*abs(That)*M/RL/Cc)*(Rc+Tc);

%Determine response at the crack location
deltt = 54/40e6;
delt = deltt;
chirprevcrack = zeros(N,1);
vlongchirp = zeros(N,1);

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

%Response at crack on first iteration
vlongchirp1 = Pamp*amp*chirprevcrack;

%Repsponse at crack on second iteration
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

%Plot the theory vs experiments
t_theory2 = time_step*N:time_step: 2 * time_step*N - time_step;
figure(figure_count)
plot(t_theory, vlongchirp1,'-r','LineWidth',4, t_theory, defect(:, 1),'--b','LineWidth',4, t_theory2, vlongchirp2,'-r','LineWidth',4, t_theory2, defect(:, 2),'--b','LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title('Defect response for 437mm and 428mm steel rods', 'FontSize', font_s, 'FontWeight', 'bold')
thisLegend = legend('Theoretical Defect Response', 'Experimental Defect Response','Location','SouthWest')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc',  strcat(basename, 'Iter_th_exp'));

%Plot initial defect response
initial = load(strcat(basename, 'Initial.lvm'));
t = 0:time_step:length(initial) * time_step - time_step;
figure(figure_count)
plot(t, initial,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Response at defect PZT on initial iteration for the steel rods', 'of length 437mm and 428mm'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc',  strcat(basename, 'Initial'));
final = load(strcat(basename, 'Final.lvm'));

%Plot final defect response
figure(figure_count)
plot(t, final,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Response at defect PZT on the 150th iteration for the steel rods', 'of length 437mm and 428mm'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc',  strcat(basename, 'Final'));

%Gain plot
gain = load(strcat(basename, 'iterationVsGain.lvm'));
figure(figure_count)
plot(gain,'LineWidth',4)
axis tight
xlabel('Iteration Number', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Gain (currentAmp / initialAmp)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'The amplitude gain of the response seen at the defect on each', 'iteration for the 437mm and 428mm steel rods'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc',  strcat(basename, 'iterationVsGain'));

%Plot initial wave sent
initialWave = load('initialWave.lvm');
t = 0:time_step: length(initialWave) * time_step - time_step;
figure(figure_count)
plot(t, initialWave,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Multi-tone wave sent by PZT A on initial iteration', '115kHz center frequency with 30kHz bandwidth'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'initialWave');

%Plot signal read by ch0 on first iteration
ch0Read = load('ch0Read.lvm');
t = 0:time_step: length(ch0Read) * time_step - time_step;
figure(figure_count)
plot(t, ch0Read,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Signal read by PZT A on the initial iteration'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc',   'ch0Read');

%Plot signal read by ch1 on first iteration
ch1Read = load('ch1Read.lvm');
t = 0:time_step: length(ch1Read) * time_step - time_step;
figure(figure_count)
plot(t,ch1Read,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Signal read by PZT B on the initial iteration'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'ch1Read');

%Plot windowed RMS of signal read by ch0 on first iteration
ch0RMS = load('ch0RMS.lvm');
t = 0:time_step: length(ch0RMS) * time_step - time_step;
figure(figure_count)
plot(t,ch0RMS,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Windowed RMS of signal read by PZT A on the initial iteration'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'ch0RMS');

%Plot windowed RMS of signal read by ch1 on first iteration
ch1RMS = load('ch1RMS.lvm');
t = 0:time_step: length(ch1RMS) * time_step - time_step;
figure(figure_count)
plot(t,ch1RMS,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Windowed RMS of signal read by PZT B on the initial iteration'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc',  'ch1RMS');

%Plot ch0 chosen exctraction siganl
ch0Chosen = load('ch0Chosen.lvm');
t = 0:time_step: length(ch0Chosen) * time_step - time_step;
figure(figure_count)
plot(t,ch0Chosen,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Window of PZT A chosen for playback based on RMS'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'ch0Chosen');

%Plot ch1 chosen exctraction siganl
ch1Chosen = load('ch1Chosen.lvm');
t = 0:time_step: length(ch1Chosen) * time_step - time_step;
figure(figure_count)
plot(t, ch1Chosen,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Window of PZT B chosen for playback based on RMS'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'ch1Chosen');

%Plot ch0 replayed signal
ch0Scaled = load('ch0Scaled.lvm');
t = 0:time_step: length(ch0Scaled) * time_step - time_step;
figure(figure_count)
plot(t,ch0Scaled,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Recentered, rescaled, and time reversed signal with', 'delays inserted which will be played back by PZT A'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'ch0Scaled');

%Plot ch1 replayed signal
ch1Scaled = load('ch1Scaled.lvm');
t = 0:time_step: length(ch1Scaled) * time_step - time_step;
figure(figure_count)
plot(t,ch1Scaled,'LineWidth',4)
axis tight
xlabel('Time (Seconds)', 'FontSize', font_s, 'FontWeight', 'bold')
ylabel('Amplitude (Volts)', 'FontSize', font_s, 'FontWeight', 'bold')
set(gca, 'FontSize', font_s, 'FontWeight', 'bold')
title({'Recentered, rescaled, and time reversed signal with', 'delays inserted which will be played back by PZT B'}, 'FontSize', font_s, 'FontWeight', 'bold')
figure_count = figure_count + 1;
set(thisLegend, 'FontSize', font_s, 'FontWeight', 'bold')
print('-depsc', 'ch1Scaled');