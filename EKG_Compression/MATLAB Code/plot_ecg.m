my = fopen('' ,'r');
%fp = csvread('/Users/alexandraertola/Desktop/Grad_School/Thesis/physionet/ecg_data/mit_a_fib/07879_ecg2_10.csv',3);

%samples = fp(:, 2);

%size_samples = [1 Inf];
samples = fscanf(my, '%f');
L = length(samples)
figure
plot(samples)
set(gca, 'FontSize', 16)
xlabel('Sample Number', 'FontSize',20)
ylabel('Vdiff','FontSize',20)
title('ECG Plot From Arduino','FontSize',20);



