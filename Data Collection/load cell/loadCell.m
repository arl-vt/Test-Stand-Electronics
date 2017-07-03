% Read file
clc; clear all; close all
F = readtable('loadCell_tac_4HardAvg_legSwing_slow.csv');

samples = F.Sample_float;
temp_samples = str2double(samples);
data = F.Data_float;
temp_adc = str2double(data);

plot(temp_samples, temp_adc);
xlabel('Samples');
ylabel('ADC Read');
title('Load Cell reading with Slow Leg Swing');
legend('Hardware filtering, 4 samples');