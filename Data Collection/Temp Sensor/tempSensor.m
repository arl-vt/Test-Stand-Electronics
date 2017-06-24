% Read file
clc; clear all; close all
F = readtable('hardwareAvg_8.csv');

samples = F.Sample_float;
temp_samples = str2double(samples);
data = F.Data_float;
temp_adc = str2double(data);

plot(temp_samples(20:end), temp_adc(20:end));
xlabel('Samples');
ylabel('ADC Read');
title('Temperature Sensor with hardware averaging of 8 samples');
legend('Hardware averaging, 8 samples');