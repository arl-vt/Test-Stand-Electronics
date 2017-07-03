%% Design of Force controller for SEA 
clear all; close all; clc

%% Define Plant
s = tf('s');
SEA = 11358.64/(s^2 + 3.823*s + 50.126);

%Pole Zero map
figure
pzmap(SEA);
grid on

% Bode
figure
bode(SEA);
grid on


%% Open loop Step response
u1 = 1; % pound
u2 = 2;
u3 = 3;
u4 = 4;
u5 = 5;

[out1, T] = step(u1*SEA*(100/225));
[out2, T] = step(u2*SEA*(100/225));
[out3, T] = step(u3*SEA*(100/225));
[out4, T] = step(u4*SEA*(100/225));
[out5, T] = step(u5*SEA*(100/225));

figure;
plot(T, out1);
hold on
grid on
plot(T, out2);
plot(T, out3);
plot(T, out4);
plot(T, out5);
xlabel('Time in second');
ylabel('Force in N');
legend('100 N ', '200 N', '300 N', '400 N', '500 N');
title('Open Loop Step Resnponse');
axis([0 1 0 800]);

%% PID controller
Kp = .01;
Ki = 0.05;
Kd = 0.001;

C = Kp + Ki/s + Kd*s;

SEA_cl = feedback(C*SEA, 1);

figure
bode(SEA_cl);
grid on

t = 0:0.01:2;
[out_cl1, T2] = step(t, u1*SEA_cl*100);
[out_cl2, T2] = step(t, u2*SEA_cl*100);
[out_cl3, T2] = step(t, u3*SEA_cl*100);
[out_cl4, T2] = step(t, u4*SEA_cl*100);
[out_cl5, T2] = step(t, u5*SEA_cl*100);

figure;
plot(T2, out_cl1);
hold on
grid on
plot(T2, out_cl2);
plot(T2, out_cl3);
plot(T2, out_cl4);
plot(T2, out_cl5);
xlabel('Time in second');
ylabel('Force in N');
legend('100 N ', '200 N', '300 N', '400 N', '500 N');
title('Closed Loop Step Resnponse');
axis([0 1 0 600]);