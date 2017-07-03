% Read file
clc; clear all; close all

%% Low speed logger 1st Run
% logger frequency 100 HZ, ssytem beahves smoothly
% has 3 disturbance signals

F = readtable('lowSpeedLog2_60.txt','Delimiter', ',');

loadCellraw =F{:, 1};
Dutyraw = F{:, 2};
Errorraw = F{:, 3};

%% Data Smoothing
% Duty Cycle
Dutyraw(~any(~isnan(Dutyraw), 2),:)=0;
% t = [1:1:length(Dutyraw)];
Dutyraw = smooth(Dutyraw(:));

% remove noise
% i = 0;
% for i=1:length(Dutyraw)
% %     if i == 2939
% %         break;
% %     end
%     if (Dutyraw(i) < -25)
%         Dutyraw(i) = -20;
%     end
% end

t = [1:1:length(Dutyraw)];
D =fit(t', Dutyraw, 'smoothingspline');

% Load cell
loadCellraw(~any(~isnan(loadCellraw), 2),:)=0;
t2 = [1:1:length(loadCellraw)];
loadCellraw = smooth(loadCellraw(:));

% remove noise
i = 0;
for i=1:500
%     if i == 2939
%         break;
%     end
    if (loadCellraw(i) < 35) 
        loadCellraw(i) = 35;
    end
    
    if (loadCellraw(i) > 40) 
        loadCellraw(i) = 35;
    end
end
t2 = [1:1:length(loadCellraw)];
L =fit(t2', loadCellraw, 'smoothingspline');

% Error
Errorraw(~any(~isnan(Errorraw), 2),:)=0;
t3 = [1:1:length(Errorraw)];
Errorraw = smooth(Errorraw(:));

% remove noise
%i = 0;
%for i=1:length(Errorraw)
%     if i == 2939
%         break;
%     end
%     if (Errorraw(i) < 45)
%         Errorraw(i) = 50;
%     end
%end
t3 = [1:1:length(Errorraw)];
E =fit(t3', Errorraw, 'smoothingspline');

%% Plot Data

% Load Cell
figure
subplot(3, 1, 1)
plot(L, t2, loadCellraw);
% hold on
% hline(50, 'g');
% axis([1000 1500 30 80]);
legend('Load Cell');
xlabel('Samples');
ylabel('Load Cell ADC');

% Duty
subplot(3,1, 2)
plot(D, t, Dutyraw);
% hline(0, 'g');
% vline(1182, 'g');
% legend('Duty Cycle');
% axis([1000 1500 -25 5]);
xlabel('Samples');
ylabel('Duty Cycle');

subplot(3,1, 3)
plot(E, t3, Errorraw);
% hline(0, 'g');
% legend('Error');
% axis([1000 1500 -30 10]);
xlabel('Samples');
ylabel('Error');



