

write = get_times_from_files('rdwr-repy-write.txt', 'rdwr-startstop-write.txt');
read = get_times_from_files('rdwr-repy-read.txt', 'rdwr-startstop-read.txt');
open = get_times_from_files('open-rdwr-repy-open.txt', 'open-rdwr-startstop-open.txt');
close = get_times_from_files('open-rdwr-repy-close.txt', 'open-rdwr-startstop-close.txt');
seek = get_times_from_files('rdwr-repy-seek.txt', 'rdwr-startstop-seek.txt');


times = [ barrify(write); barrify(read); barrify(open); barrify(close); barrify(seek) ];

names = {'write', 'read', 'open', 'close', 'seek'};

b = bar( times, 'stacked' );
%colormap('prism');
%cols = {'r';'o';'b';'g';'p'};
%set(b, 'FaceColor',cols);

set(gca, 'XTickLabel', names)
ylabel('Elapsed Time (sec)');

% get(gca, 'YTick') returns y lables, we then convert to a string with
% num2str. This returns one big string separated with spaces, so split into
% an array. The resulting array contains our y lables represented as strings
ylab = regexp( num2str(get(gca, 'YTick')) ,'\ +','split');
set(gca, 'YTickLabel', ylab);
title('System Call Time Spent in RePy');
legend('(3) - (2)', '(7) - (3)', '(8) - (7)', '(4) - (8)','(5) - (4)');

print('-dpng', 'syscall-bar.png');