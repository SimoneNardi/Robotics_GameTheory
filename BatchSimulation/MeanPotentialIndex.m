%% Nel file i primi 500 elementi appartengolo a DISL gli altri 500 a PIPIP
function [] = MeanPotentialIndex( NumAgenti, Scenario )
%Scenario = 1 per EXTERNAL, 2 per TRIG, 3 harbour

%Apro file in lettura
fileID = fopen('l_potentialValue.txt','r');

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
Value_potentialDISL = zeros(n_M, 1);
for j = 1:n_M;
    Value_potentialDISL(j, 1) = mean(A_disl(N1:N2, 1));
    N1 = N1 + n_testcase;
    N2 = N2 + n_testcase;
end

N1 = 1;
N2 = 50;
Value_potentialPIPIP = zeros(n_M, 1);
for j = 1:n_M;
    Value_potentialPIPIP(j, 1) = mean(A_pipip(N1:N2, 1));
    N1 = N1 + n_testcase;
    N2 = N2 + n_testcase;
end
    
x_axis = 1:n_M;
figure, plot(x_axis, Value_potentialDISL, 'o')
grid on, xlabel('M'), ylabel('Mean potential index DISL')
if(Scenario == 1)
title('Open Sea')
end
if(Scenario == 2)
title('Bay')
end
if(Scenario == 3)
title('Harbour')
end
figure, plot(x_axis, Value_potentialPIPIP, 'o')
grid on, xlabel('M'), ylabel('Mean potential index PIPIP')
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
    save('Ext_potentialDISL_N5', 'Value_potentialDISL')
    save('Ext_potentialPIPIP_N5', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 10)
    save('Ext_potentialDISL_N10', 'Value_potentialDISL')
    save('Ext_potentialPIPIP_N10', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 15)
    save('Ext_potentialDISL_N15', 'Value_potentialDISL')
    save('Ext_potentialPIPIP_N15', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 20)
    save('Ext_potentialDISL_N20', 'Value_potentialDISL')
    save('Ext_potentialPIPIP_N20', 'Value_potentialPIPIP')
    end

end

if Scenario == 2
   
    if(NumAgenti == 5)
    save('Trig_potentialDISL_N5', 'Value_potentialDISL')
    save('Trig_potentialPIPIP_N5', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 10)
    save('Trig_potentialDISL_N10', 'Value_potentialDISL')
    save('Trig_potentialPIPIP_N10', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 15)
    save('Trig_potentialDISL_N15', 'Value_potentialDISL')
    save('Trig_potentialPIPIP_N15', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 20)
    save('Trig_potentialDISL_N20', 'Value_potentialDISL')
    save('Trig_potentialPIPIP_N20', 'Value_potentialPIPIP')
    end
    
end

if Scenario == 3
   
    if(NumAgenti == 5)
    save('Road_potentialDISL_N5', 'Value_potentialDISL')
    save('Road_potentialPIPIP_N5', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 10)
    save('Road_potentialDISL_N10', 'Value_potentialDISL')
    save('Road_potentialPIPIP_N10', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 15)
    save('Road_potentialDISL_N15', 'Value_potentialDISL')
    save('Road_potentialPIPIP_N15', 'Value_potentialPIPIP')
    end

    if(NumAgenti == 20)
    save('Road_potentialDISL_N20', 'Value_potentialDISL')
    save('Road_potentialPIPIP_N20', 'Value_potentialPIPIP')
    end
    
end

end