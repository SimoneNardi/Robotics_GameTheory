%% Nel file i primi 500 elementi appartengolo a DISL gli altri 500 a PIPIP
function [] = MeanError( NumAgenti, Scenario )
%Scenario = 1 per EXTERNAL, 2 per TRIG, 3 harbour

%Apro file in lettura
fileID = fopen('l_benefitValue.txt','r');

%specifico formato e salvo i dati in A
formatSpec = '%f';
A = fscanf(fileID,formatSpec);

% divido in base agli algoritmi di learning
A_disl = A(1:length(A)/2, 1);
A_pipip = A(length(A)/2+1:end, 1);

%inizializzo i vettori in cui salvo i valori
n_M = 10; % M va da 1 a 10
n_testcase = 50; %numero di cond iniziali random

% elementi da 501 a 1000
N1 = 1;
N2 = 50;
Value_benefitDISL = zeros(n_M, 1);
for j = 1:n_M;
    Value_benefitDISL(j, 1) = mean(A_disl(N1:N2, 1));
    N1 = N1 + n_testcase;
    N2 = N2 + n_testcase;
end

N1 = 1;
N2 = 50;
Value_benefitPIPIP = zeros(n_M, 1);
for j = 1:n_M;
    Value_benefitPIPIP(j, 1) = mean(A_pipip(N1:N2, 1));
    N1 = N1 + n_testcase;
    N2 = N2 + n_testcase;
end

%DISL plot
x_axis = 1:n_M;
figure, plot(x_axis, Value_benefitDISL, 'o--')
grid on, xlabel('M'), ylabel('MeanError DISL'), legend(num2str(NumAgenti))
if(Scenario == 1)
title('Open Sea')
end
if(Scenario == 2)
title('Bay')
end
if(Scenario == 3)
title('Harbour')
end

% PIPIP plot
figure, plot(x_axis, Value_benefitPIPIP, 'o--')
grid on, xlabel('M'), ylabel('Mean Error, PIPIP'), legend(num2str(NumAgenti))
if(Scenario == 1)
title('Open Sea')
end
if(Scenario == 2)
title('Bay')
end
if(Scenario == 3)
title('Harbour')
end

% salva i file .mat con i valori processati
if Scenario == 1

    if(NumAgenti == 5)
    save('Ext_BenefitDISL_N5', 'Value_benefitDISL')
    save('Ext_BenefitPIPIP_N5', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 10)
    save('Ext_BenefitDISL_N10', 'Value_benefitDISL')
    save('Ext_BenefitPIPIP_N10', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 15)
    save('Ext_BenefitDISL_N15', 'Value_benefitDISL')
    save('Ext_BenefitPIPIP_N15', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 20)
    save('Ext_BenefitDISL_N20', 'Value_benefitDISL')
    save('Ext_BenefitPIPIP_N20', 'Value_benefitPIPIP')
    end

end

if Scenario == 2
   
    if(NumAgenti == 5)
    save('Trig_BenefitDISL_N5', 'Value_benefitDISL')
    save('Trig_BenefitPIPIP_N5', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 10)
    save('Trig_BenefitDISL_N10', 'Value_benefitDISL')
    save('Trig_BenefitPIPIP_N10', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 15)
    save('Trig_BenefitDISL_N15', 'Value_benefitDISL')
    save('Trig_BenefitPIPIP_N15', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 20)
    save('Trig_BenefitDISL_N20', 'Value_benefitDISL')
    save('Trig_BenefitPIPIP_N20', 'Value_benefitPIPIP')
    end
    
end

if Scenario == 3
   
    if(NumAgenti == 5)
    save('Road_BenefitDISL_N5', 'Value_benefitDISL')
    save('Road_BenefitPIPIP_N5', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 10)
    save('Road_BenefitDISL_N10', 'Value_benefitDISL')
    save('Road_BenefitPIPIP_N10', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 15)
    save('Road_BenefitDISL_N15', 'Value_benefitDISL')
    save('Road_BenefitPIPIP_N15', 'Value_benefitPIPIP')
    end

    if(NumAgenti == 20)
    save('Road_BenefitDISL_N20', 'Value_benefitDISL')
    save('Road_BenefitPIPIP_N20', 'Value_benefitPIPIP')
    end
    
end

end