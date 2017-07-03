%% MRAC Pendulum system
clc; clear all; close all

%% System Model
s = tf('s');
plant = 1.89/(s^2 + 0.0389*s + 10.77);

% open loop step response and pzmap
figure
subplot(2,1,1)
pzmap(plant);

subplot(2,1,2)
step(plant);
