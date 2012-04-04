function [ t ] = get_times_from_file( path )
% Reads times from the analysis file at path
%   Returns times in a struct t with:
%       t.start = start times
%       t.end   = end times
%       t.elapsed = elapsed times
%       t.std_start = standerdized start times
%                   (from 1st start time)

    % read in csv data
    dat = csvread(path);

    t.start = dat(:,1); % col 1 has start time
    t.end = dat(:,2); % col 2 has end time
    t.elapsed = t.end - t.start;
    
    % start(1) will be the smallest (first) start time
    t.std_start = t.start - t.start(1); 
  
end

