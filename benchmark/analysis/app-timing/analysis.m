format long

% paths to file locations 
lind_path = 'lind_results\';
c_path = 'c_results\';
plot_path = '';

% output format and extension for image file
img_format = 'dpng';
img_ext = '.png';

lind_colour = 'blue';
c_colour = 'red';

% file to analyze
file_name = 'grep';

% read in data for lind and native c
lind_times = get_times_from_file([lind_path file_name]);
c_times = get_times_from_file([c_path file_name]);

% bins for histogram
num_bins = 1000;

% lind histogram
hist(lind_times.elapsed, num_bins);
xlabel('Run Time (sec)');
ylabel('Frequency');
title([file_name ' lind histogram']);
print(img_format, [plot_path file_name '-lind-hist.pdf']);

% native c histogram
hist(c_times.elapsed, num_bins);
xlabel('Run Time (sec)');
ylabel('Frequency');
title([file_name ' c histogram']);
print(img_format, [plot_path file_name '-c-hist.pdf']); 

% lind scatterplot
scatter(lind_times.std_start, lind_times.elapsed, 4, lind_colour, 'filled');
xlabel('Start Time (sec)');
ylabel('Run Time (sec)');
title([file_name ' lind scatterplot']);
print(img_format, [plot_path file_name '-lind-scatter' img_ext]);

% c scatterplot
scatter(c_times.std_start, c_times.elapsed, 4, c_colour, 'filled');
xlabel('Start Time (sec)');
ylabel('Run Time (sec)');
title([file_name ' c scatterplot']);
print(img_format, [plot_path file_name '-c-scatter' img_ext]);

% both scatteplot
scatter(lind_times.std_start, lind_times.elapsed, 4, lind_colour, 'filled');
hold on
scatter(c_times.std_start, c_times.elapsed, 4, c_colour, 'filled');
xlabel('Start Time (sec)');
ylabel('Run Time (sec)');
title([file_name ' scatterplot']);
legend('lind', 'c');
print(img_format, [plot_path file_name '-both-scatter' img_ext]);
hold off

