% Read file
clc; clear all; close all
F = readtable('capture3.txt');

Samples = F{:, 1};
LoadCell = F{:, 2};
Duty = F{:, 3};
Dir = F{:, 4};

indexDir = (Dir==0);
dirNegate = -1*indexDir;

finalDir = Dir+dirNegate;
finalDuty = Duty.*finalDir;

figure
subplot(2, 1, 1)
plot(Samples, LoadCell);
%axis([0 2000 2650 2750]);
legend('Load Cell');
xlabel('Samples');
ylabel('Load Cell ADC');

subplot(2,1, 2)
plot(Samples, finalDuty);
legend('PWM percent');
axis([0 2000 -3 3]);
xlabel('Samples');
ylabel('Duty Cycle');