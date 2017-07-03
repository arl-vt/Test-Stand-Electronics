% Read file
clc; clear all; close all

%% Fisrt experiment - with logger

F = readtable('c1.txt','Delimiter', ',');

loadCellraw =F{:, 1};
Dutyraw = F{:, 2};

%Dutyraw(~any(~isnan(Dutyraw), 2),:)=[];
%loadCellraw(~any(~isnan(loadCellraw), 2),:)=[];
 
duty_s = length(Dutyraw);
Duty = zeros(size(Dutyraw));
j = 1;
for i=(1:duty_s)
    if (Dutyraw(i) < 70)
        Duty(j) = Dutyraw(i);
        j = j+1;
    end
end
D = medfilt1(Dutyraw,10);
duty_s = length(D);
Duty = zeros(size(D));
j = 1;
for i=(1:duty_s)
    if (D(i) < 70)
        Duty(j) = D(i);
        j = j+1;
    end
end


figure
subplot(2, 1, 1)
plot(loadCellraw);
hold on
vline(4826, 'r');
axis([4000 8000 -100 100]);
legend('Load Cell', '-');
xlabel('Samples');
ylabel('Load Cell ADC');

% step 
y = 0:0.1:duty_s;
x = ones(1, length(y));

% vert
y_st = -20;
y_end = 20;
subplot(2,1, 2)
plot(Dutyraw);
hold on
plot(y, x); 
%line([4826 -20], [4826 20]);
vline(4826, 'r');
legend('PWM percent');
axis([4000 8000 -30 30]);
xlabel('Samples');
ylabel('Duty Cycle');



%% Second experiment - separate logger
% F = readtable('c_log.txt','Delimiter', ',');
% 
% Samples = F{:, 1};
% loadCellraw =F{:, 2};
% Dutyraw = F{:, 3};
% 
% Dutyraw(~any(~isnan(Dutyraw), 2),:)=[];
% 
% duty_s = length(Dutyraw);
% Duty = zeros(size(Dutyraw));
% j = 1;
% for i=(1:duty_s)
%     if (Dutyraw(i) < 70)
%         Duty(j) = Dutyraw(i);
%         j = j+1;
%     end
% end
% D = medfilt1(Dutyraw,10);
% duty_s = length(D);
% Duty = zeros(size(D));
% j = 1;
% for i=(1:duty_s)
%     if (D(i) < 70)
%         Duty(j) = D(i);
%         j = j+1;
%     end
% end
% % loadCellraw(~any(~isnan(loadCellraw), 2),:)=[];
% 
% figure
% subplot(2, 1, 1)
% plot(loadCellraw);
% %axis([0 5000 20 100]);
% legend('Load Cell', '-');
% xlabel('Samples');
% ylabel('Load Cell ADC');
% 
% subplot(2,1, 2)
% plot(Duty);
% legend('PWM percent');
% %axis([0 5000 -30 30]);
% xlabel('Samples');
% ylabel('Duty Cycle');

