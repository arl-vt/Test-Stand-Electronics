% Read file
clc; clear all; close all
F = readtable('loadCell_tacuna_8HardAvg.csv');

samples = F.Sample_float;
temp_samples = str2double(samples);
data = F.Data_float;
temp_adc = str2double(data);

plot(temp_samples, temp_adc);
xlabel('Samples');
ylabel('ADC Read');
title('Load Cell reading with stationary leg and some random disturbance');
legend('Hardware filtering');