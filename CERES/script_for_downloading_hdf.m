% *Summary:*
% This MATLAB code is designed to download a CERES (Clouds and the Earth's Radiant Energy System) mission data file 
% from NASA's Atmospheric Science Data Center (ASDC).
% The script downloads a specific HDF (Hierarchical Data Format) file containing CERES ES4 data 
% from the Aqua satellite's Crosstrack measurements [https://asdc.larc.nasa.gov/project/CERES/CER_ES4_Terra+Aqua_Edition4]. 
% It constructs an authenticated URL by embedding user credentials directly into the URL string, 
% then uses MATLAB's urlwrite function to download the file to a specified local directory. 

% Specify the URL of the data you want to download
symlinkURL = 'https://asdc.larc.nasa.gov/data/CERES/ES4/Aqua-Xtrk_Edition4/2023/02/CER_ES4_Aqua-Xtrk_Edition4_403409.202302.hdf';

% Set the output directory where you want to save the downloaded data
outputDir = '/MATLAB Drive/tolnet';

% Set the username and password for authentication
username = 'your_user_name';
password = 'your_password';


% Specify the URL of the symlink file

% Create the authenticated URL
authenticatedURL = ['https://', username, ':', password, '@', symlinkURL(9:end)];

% Create the full output file path
outputFile = fullfile(outputDir, 'CER_ES4_Aqua-Xtrk_Edition4_403409.202302.hdf');

% Download the .hdf file from the symlink
try
    urlwrite(authenticatedURL, outputFile);
    disp('Data downloaded successfully.');
catch
    disp('Failed to download the data.');
end

