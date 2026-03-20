function lidar_plot_narsto(fname)

% Language:  MatLab 6.5 (version 13) and 5.3 (version 12)
%            This code has been tested on PCs running Windows XP, Windows NT, and Windows 2000.

% Point of Contact:  If there are questions about using the JHU_LIDAR_MATLab_plot.m program, please contact: 
                     
%		     Mariana Adam 
%                    The Johns Hopkins University 
%                    Department of Geography and Environmental Engineering 
%                    Baltimore, MD 21218 
%                    U.S.A

%                    phone: 410-516-4483 
%                    email:  mariana@jhu.edu
 
% Important Data Quality Note:
% Due to an error in the LIDAR system’s telemetry data acquisition software (discovered later on) the range values as 
% written in the TDA files (line 13) are wrong. 
% After corrections to the software and calculations, we found out that in order to get the right range we need a
% correction factor (linear multiplication). This correction factor is 1.25. So, in order to properly use the TDA data (to 
% make profile color plots or other computations desired by the user), the actual range is multiplied with the factor 
% of 1.25. For example, instead of 0, 3, 6, 9 ...4490.89 m we have actually 0, 3.75, 7.5, 11.25 ...5613.6 m. 
% This correction is supplied in this Matlab script. See below.
% We mention that that correction was done in the case of the Mixing Height data file 
% (NARSTO_EPA_SS_BALTIMORE_LIDAR_MIX_HGT_20010522_20020814_V1.csv). We specify that all the existing plots have the wrong range 
% (vertical axis). When looking at the plots remember to multiply the vertical axis by a factor of 1.25. To use the right 
% Mixing Height plots, please use the data file supplied for the Mixing Height (which has correction applied already) and 
% do the plots again. In order to use the right profile color plots of the aerosol backscatter, please run the Matlab 
% script supplied to plot them again. We apologize for inconvenience, but it was not our fault. The lidar was delivered 
% with the software when we bought it. Unfortunately it contained this error which effected the telemetry.

% e.g. fname: write in Command Window:   fname='c:\lidar\LIDARPON020704018.TDA'
% then write:   lidar_plot_narsto(fname)

% 'lidar_plot_narsto' lidar plotting routine
%
%   'lidar_plot_narsto(fname)' reads input file designated by fname and plots data om appropriate axis
%
%      'fname' is a file name string.  This file comes directly from the lidar computer and had not been transformed 
%      in any way
%  
%      'lidar_plot_narsto' does the following
%         -read data from file designated by 'fname'
%         -subtract the backgroung radiation
%         -perform range correction
%         -perform some data smoothing with a binning level
%         -plots the data
%         -creates a .jpg image from the plot
%
%      no output arguments are defined

% read the data
fid = fopen(fname,'r');                 % open the file specified by fname
                             
scantype=fgets(fid);                    % read the comment line for the type of scan (Ist line)
z=length(scantype);
ftype=scantype(3:z);                    % take off the * in the line header
crap = fgets(fid);                      % blind read (IInd line)
crap = fgets(fid);                      % blind read (IIIrd line)
datetime=fgets(fid) ;                   % read line containing the date and time (IVth line)
z=length(datetime);
ftime=datetime(z-9:z-2);                % get the time string hh:mm:ss
fdate=datetime(z-20:z-16);              % get the date string mm/dd
fdate=[fdate '/2002'];                  % get the date string in mm/dd/yyyy

site=fgets(fid);                        % reads Vth line
fsite = site(19:length(site));          % get the experiment site string
for i=6:13
    crap=fgets(fid);                    % reads lines 6-13
end

range=str2num(crap(11:length(crap)));   % delete the place holder (-9999.99) at the beg. of 13th line 

% APPLY CORRECTION: MULTIPLY WITH CORRECTION FACTOR 1.25
range=range*1.25;

nsteps = length(range);                 % find the # of range increments (in vertical)
all=fscanf(fid, '%f');                  % read the rest of the data file
fclose(fid);                            % close the input file

ntimes = length(all)/(nsteps+1);        % find the # of time increments

hold = reshape(all,nsteps+1,ntimes);    % rewrite the lidar file in a matrix form
times = hold(1,:);                      % pull out the time-step info
lidardat = hold(2:nsteps+1,1:ntimes);   % pull out the back scatter info
clear hold all crap;                    % clear memory of large unused variables
fprintf('   file read \n');

% % PLOT ROW SIGNAL P
% figure
% plot(range,lidardat)
 
denoise=1
% note: in order to get better results, we have changed the initial
% de-noising procedure, replacing sym8 level 1 with db10 level 3
if denoise == 1
    [thr,sorh,keepapp] = ddencmp('den','wv',lidardat); % de-noise all LOS at once with db10 at level 3 (see Wavelets)
    xd = wdencmp('gbl',lidardat,'db10',3,thr,sorh,keepapp);
    lidardat=xd;
end

% determine the index of incomplete overlap
overlap = 300;                          % for now hard set it at 450m
[c,place] = min(abs(range-overlap));
fprintf('   incomplete overlap determined \n');

% perform the background subtraction
%   this background subtraction portion calculates the mean of the last 50 data points
%   in each line of sight, then finds mean and standard deviation of this quantity.
%   Lines of sight that have a mean 1.5*stddev away from the grand mean have the grand mean 
%   subtracted.  Otherwise, the line of sight has it's own background radiation subtracted. 

% note: for better results, we replaced the initial background range
% (nsteps-50:nsteps) with (nsteps-60:nsteps-10) to avoid the last bins
% which sometimes show large values
backrad = mean(lidardat(nsteps-60:nsteps-10,:));% the background rad for each line of sight

% backrad=mean(lidardat(1499-50:1499,:));   % this is used when another background substraction is specified
                                            % other then the last 50 steps (eg when we have clouds across the last 50 steps)
back = ones(nsteps,ntimes)*spdiags(backrad',0,ntimes,ntimes); %matrix of bacgrounnd 
                                        % radiation that will be subtracted from each point
lidardat = lidardat-back;               % subtract the background radiation
clear back backrad test mu              % clear unused variables
fprintf('   background subtracted \n');

% Perform the range correction
lidardat = ((lidardat')*spdiags(range.^2',0,nsteps,nsteps))'; % range correction
fprintf('   range correction complete \n');

hour=str2num(ftime(1:2));
minute=str2num(ftime(4:5));
second=str2num(ftime(7:8));
hrs_mins_secs=hour*60*60+minute*60+second;

% writting the x axis corresponding to the time
for i=1:length(times)
    ax_t(i)=datenum(0,0,0,0,0,hrs_mins_secs+times(i));
end
ax_t;
length(ax_t);
length(times);

% plot the results
fprintf('   starting to plot this might take a couple of minutes \n');

figure
pcolor(ax_t,range,lidardat);             % make the pcolor plot
datetick('x',15)%,'keepticks','keeplimits') 
shading interp;                          % attractive shading
axis([ax_t(1) ax_t(end) 375 range(end)])     % initial was 400 instead of 375
ylabel('z [m]');                         % plot annotation
xlabel('time (EST)');
caxis([1E7 3E8]);                        % change the color scale when necesary 
colorbar
z=length(fname);
title_str = strcat(fname(z-20:z-4),' - starttime:',ftime); 
title(title_str);

webname=[fname(1:z-21) fname(z-20:z-4)];

print(gco, '-djpeg85', '-r75', webname);
% saveas(gcf,webname,'fig')              % this is for saving as .fig file (matlab plot)

fprintf('   web image created \n');