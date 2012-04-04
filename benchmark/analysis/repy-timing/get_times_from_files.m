function [ t ] = get_times_from_files( repypath, naclpath )
% Reads times from the analysis file at path
%   Returns times in a struct t with:
%       t.start = start times
%       t.end   = end times
%       t.elapsed = elapsed times
%       t.std_start = standerdized start times
%                   (from 1st start time)

    % read in csv data
    repydat = csvread(repypath);
    nacldat = csvread(naclpath);
    
    % start times (1) (nacl timer!)
    t.before = nacldat(:,1);
    t.beforeavg = mean(t.before);
    
    % pre (2) (repy timer)
    t.pre = repydat(:,1);
    t.preavg = mean(t.pre);
    
    % start (3) (repy timer)
    t.start = repydat(:,2);
    t.startavg = mean(t.start);
    
    % post (4) (repy timer)
    t.post = repydat(:,3);
    t.postavg = mean(t.post);
    
    % stop (5) (repy timer)
    t.stop = repydat(:,4);
    t.stopavg = mean(t.post);
    
    % precall (7) (repy timer)
    t.precall = repydat(:,5);
    t.precallavg = mean(t.precall);
    
    % postcall (8) (repy timer)
    t.postcall = repydat(:,6);
    t.postcallavg = mean(t.postcall);
    
    % stop times (6) (nacl timer)
    t.after = nacldat(:,2);
    t.afteravg = mean(t.after);
    
    