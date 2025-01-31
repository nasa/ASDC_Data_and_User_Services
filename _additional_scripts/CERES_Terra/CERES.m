% Specify the URL of the data you want to download
symlinkURL = 'https://asdc.larc.nasa.gov/data/CERES/ES4/Aqua-Xtrk_Edition4/2023/02/CER_ES4_Aqua-Xtrk_Edition4_403409.202302.hdf';

% Set the output directory where you want to save the downloaded data
outputDir = '/MATLAB Drive/tolnet';

% Set the username and password for authentication
username = 'your_user_name';
password = 'your_password';


% Specify the URL of the symlink file

% Create the authenticated URL
authenticatedURL = [symlinkURL, username, ':', password, '@', symlinkURL(9:end)];

% Create the full output file path
outputFile = fullfile(outputDir, 'CER_ES4_Aqua-Xtrk_Edition4_403409.202302.hdf');

% Download the .hdf file from the symlink
try
    urlwrite(authenticatedURL, outputFile);
    disp('Data downloaded successfully.');
catch
    disp('Failed to download the data.');
end

