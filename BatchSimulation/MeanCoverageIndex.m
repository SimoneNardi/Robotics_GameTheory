function [] = MeanCoverageIndex( NumAgenti, Scenario )

fileID = fopen('l_coverageValue.txt','r');

formatSpec = '%f';
A = fscanf(fileID,formatSpec);

A_disl = A(1:length(A)/2, 1);
A_pipip = A(length(A)/2+1:end, 1);

%inizializzo i vettori in cui salvo i valori
n_M = 10; % M va da 1 a 10
n_testcase = 50; %numero di cond iniziali random

N1 = 1;
N2 = 50;
Value_coverageDISL = zeros(n_M, 1);
for j = 1:n_M;
    Value_coverageDISL(j, 1) = mean(A_disl(N1:N2, 1));
    N1 = N1 + n_testcase;
    N2 = N2 + n_testcase;
end

N1 = 1;
N2 = 50;
Value_coveragePIPIP = zeros(n_M, 1);
for j = 1:n_M;
    Value_coveragePIPIP(j, 1) = mean(A_pipip(N1:N2, 1));
    N1 = N1 + n_testcase;
    N2 = N2 + n_testcase;
end

x_axis = 1:n_M;
figure, plot(x_axis, Value_coverageDISL, 'o')
grid on, xlabel('M'), ylabel('Mean coverage index')

if(Scenario == 1)
title('Open Sea')
end
if(Scenario == 2)
title('Bay')
end
if(Scenario == 3)
title('Harbour')
end
figure, plot(x_axis, Value_coveragePIPIP, 'o')
grid on, xlabel('M'), ylabel('Mean coverage index')
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
    save('Ext_coverageDISL_N5')
    save('Ext_coveragePIPIP_N5')
    end

    if(NumAgenti == 10)
    save('Ext_coverageDISL_N10')
    save('Ext_coveragePIPIP_N10')
    end

    if(NumAgenti == 15)
    save('Ext_coverageDISL_N15')
    save('Ext_coveragePIPIP_N15')
    end

    if(NumAgenti == 20)
    save('Ext_coverageDISL_N20')
    save('Ext_coveragePIPIP_N20')
    end

end

if Scenario == 2
   
    if(NumAgenti == 5)
    save('Trig_coverageDISL_N5')
    save('Trig_coveragePIPIP_N5')
    end

    if(NumAgenti == 10)
    save('Trig_coverageDISL_N10')
    save('Trig_coveragePIPIP_N10')
    end

    if(NumAgenti == 15)
    save('Trig_coverageDISL_N15')
    save('Trig_coveragePIPIP_N15')
    end

    if(NumAgenti == 20)
    save('Trig_coverageDISL_N20')
    save('Trig_coveragePIPIP_N20')
    end
    
end

if Scenario == 3
   
    if(NumAgenti == 5)
    save('Road_coverageDISL_N5')
    save('Road_coveragePIPIP_N5')
    end

    if(NumAgenti == 10)
    save('Road_coverageDISL_N10')
    save('Road_coveragePIPIP_N10')
    end

    if(NumAgenti == 15)
    save('Road_coverageDISL_N15')
    save('Road_coveragePIPIP_N15')
    end

    if(NumAgenti == 20)
    save('Road_coverageDISL_N20')
    save('Road_coveragePIPIP_N20')
    end
    
end

end