import os
import h5py
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import pyplot, colors
import matplotlib.font_manager as fm
from matplotlib.ticker import FormatStrFormatter

current_working_directory = os.getcwd()


files_sr = []
files_ss = []
file_name = []


dsets = ['ao3', 'h2o', 'no2']


def plot_data(dates, file_lists, filename_each_day, dict_ss, dict_sr, collection, sage_profile_dir):   
    for date, file_list, filename_list, ss_values, sr_values in zip(sorted(dates), file_lists, filename_each_day, dict_ss.values(), dict_sr.values()):

        if len(ss_values) > 0 and len(sr_values) > 0:
            plot_two(date, file_list, ss_values, sr_values, filename_list, collection, sage_profile_dir)
        elif len(ss_values) == 0 and len(sr_values) > 0:
            color = pyplot.get_cmap("Blues", len(filename_list))
            title = 'Sunrise'
            plot_one(date, file_list, sr_values, color, title, collection, sage_profile_dir)
        elif len(sr_values) == 0 and len(ss_values) > 0:
            color = pyplot.get_cmap("Reds", len(filename_list))
            title = 'Sunset'
            plot_one(date, file_list, ss_values, color, title, collection, sage_profile_dir)
            


def plot_two(date, file_list, ss_values, sr_values, filename_list, collection, sage_profile_dir):
    blues = pyplot.get_cmap("Blues", len(filename_list))
    reds = pyplot.get_cmap("Reds", len(filename_list))
    
    for dset in dsets:
        for f, b, r in zip(file_list, range(blues.N), range(reds.N)):
            a = f.split('/')[-1]
            q = a.index('v')
            short_name = a[:q].rsplit('.')[2] 

            with h5py.File(f, mode='r') as f:
                if dset == 'ao3':
                    dset_var = f['/Altitude Based Data/Aerosol Ozone Profiles/Ozone_AO3']
                elif dset == 'h2o':
                    dset_var = f['/Altitude Based Data/Water Vapor Profiles/H2O']
                elif dset == 'no2':
                    dset_var = f['/Altitude Based Data/Nitrogen Dioxide Profiles/NO2']
                
                
                dset_pres = f['/Altitude Based Data/Altitude Information/Altitude']
                dset_time = f['/Event Information/Time']
                dset_date = f['/Event Information/Date']
                dset_lat = f['/Event Information/Latitude']
                dset_lon = f['/Event Information/Longitude']
    
            
                # Read the data.
                data = dset_var[:]
                altitude = dset_pres[:]
                time = dset_time[()]
                dt = dset_date[()]
                lat = dset_lat[()]
                lon = dset_lon[()]
                
                # Read the needed attributes.
                data_units = dset_var.attrs['units']
                pres_units = dset_pres.attrs['units']
                data_title = dset_var.attrs['long_name']
                time_title = dset_time.attrs['long_name']
                pres_title = dset_pres.attrs['long_name']
            
                fillvalue = dset_var.attrs['_FillValue']
                data[data == fillvalue] = np.nan
                max_value = np.nanmax(data)
            
                # The date is stored as a six-digit number, YYYYMM.  Convert it into
                # a string.
                dstr = str(dt)
                ymd_str = dstr[0:4] + "-" + dstr[4:6] + "-" + dstr[6:8]
            
                # Set lat/lon string.
                locstr = "Longitude = " + str(lon) + ", Latitude = " + str(lat)
                
                plt.gcf().set_size_inches(14, 12)
                plt.style.use('dark_background')
                
                plt.suptitle('{0} \n{1}'.format(ymd_str, data_title), fontsize=30)

                if 'SR' in short_name:
                                    
                    plt.subplot(1, 2, 1)
                
                    plt.plot(data, altitude, color = blues(b), linewidth=4)
                    plt.legend(sorted(sr_values), frameon=False, loc='center left', bbox_to_anchor=(-0.70, 0.5), fontsize=12)
                    
                    formatter = plt.ScalarFormatter(useMathText=True)
                    formatter.set_scientific(True)
                    formatter.set_powerlimits((-3, 4))
                    plt.gca().xaxis.set_major_formatter(formatter)
                    
                    
                    plt.xlabel('{0} ({1})'.format(data_title, data_units), fontsize=12)
                    plt.ylabel('{0} ({1})'.format(pres_title, pres_units), fontsize=12)
                    
                    
                    plt.title('Sunrise', fontsize=20 )
                    plt.gca().set_xlim(0, max_value)

                    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%g'))

                if 'SS' in short_name:
                    
                    plt.subplot(1, 2, 2)

                    plt.plot(data, altitude, color = reds(r), linewidth=4)
                    plt.legend(sorted(ss_values), frameon=False, loc='center left', bbox_to_anchor=(1.15, 0.5), fontsize=12)
        
                    formatter = plt.ScalarFormatter(useMathText=True)
                    formatter.set_scientific(True)
                    formatter.set_powerlimits((-3, 4))
                    plt.gca().xaxis.set_major_formatter(formatter)

                    plt.xlabel('{0} ({1})'.format(data_title, data_units), fontsize=12)
                    plt.ylabel('{0} ({1})'.format(pres_title, pres_units), fontsize=12)
                    
                    plt.title('Sunset', fontsize=20)
                    plt.gca().set_xlim(0, max_value)
                    
                    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%g'))
 
    
                fig = plt.gcf()
                pngfile = '{0}_{1}.png'.format(date, dset)
                fig.savefig(f'{sage_profile_dir}/{collection}/{date}/{pngfile}', dpi=500, bbox_inches='tight')
                
        print(f'Done. Plot for {data_title} on {date} located here: ~/sage_daily_profiles/{collection}/{date}')
        plt.close()
        


def plot_one(date, file_list, values, color, title, collection, sage_profile_dir):
    files = []
    
    for dset in dsets:
        for c, f in zip(range(color.N), file_list):
            with h5py.File(f, mode='r') as f:
                if dset == 'ao3':
                    dset_var = f['/Altitude Based Data/Aerosol Ozone Profiles/Ozone_AO3']
                elif dset == 'h2o':
                    dset_var = f['/Altitude Based Data/Water Vapor Profiles/H2O']
                elif dset == 'no2':
                    dset_var = f['/Altitude Based Data/Nitrogen Dioxide Profiles/NO2']
                
                
                dset_pres = f['/Altitude Based Data/Altitude Information/Altitude']
                dset_time = f['/Event Information/Time']
                dset_date = f['/Event Information/Date']
                dset_lat = f['/Event Information/Latitude']
                dset_lon = f['/Event Information/Longitude']
    
            
                # Read the data.
                data = dset_var[:]
                altitude = dset_pres[:]
                time = dset_time[()]
                dt = dset_date[()]
                lat = dset_lat[()]
                lon = dset_lon[()]
                
                # Read the needed attributes.
                data_units = dset_var.attrs['units']
                pres_units = dset_pres.attrs['units']
                data_title = dset_var.attrs['long_name']
                time_title = dset_time.attrs['long_name']
                pres_title = dset_pres.attrs['long_name']
            
                fillvalue = dset_var.attrs['_FillValue']
                data[data == fillvalue] = np.nan
                max_value = np.nanmax(data)
            
                # The date is stored as a six-digit number, YYYYMM.  Convert it into
                # a string.
                dstr = str(dt)
                ymd_str = dstr[0:4] + "-" + dstr[4:6] + "-" + dstr[6:8]        
            
                # Set lat/lon string.
                locstr = "Longitude = " + str(lon) + ", Latitude = " + str(lat)
                
                plt.gcf().set_size_inches(8, 12)
                plt.style.use('dark_background')
                
                plt.suptitle('{0} \n{1}'.format(ymd_str, data_title), fontsize=30)
                
                plt.plot(data, altitude, color = color(c), linewidth=4)

                plt.legend(sorted(values), frameon=False, loc='center left', bbox_to_anchor=(-0.55, 0.5), fontsize=12)
                
                # Save some screen space by using scientific notation for the xtick labels.
                formatter = plt.ScalarFormatter(useMathText=True)
                formatter.set_scientific(True)
                formatter.set_powerlimits((-3, 4))
                plt.gca().xaxis.set_major_formatter(formatter)
                
                
                plt.xlabel('{0} ({1})'.format(data_title, data_units), fontsize=12)
                plt.ylabel('{0} ({1})'.format(pres_title, pres_units), fontsize=12)
                
                
                plt.title(f'{title}', fontsize=20 )
                plt.gca().set_xlim(0, max_value)
            
                plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%g'))

                            
            fig = plt.gcf()
            pngfile = '{0}_{1}.png'.format(date, dset)
            fig.savefig(f'{sage_profile_dir}/{collection}/{date}/{pngfile}', dpi=500, bbox_inches='tight')
                    
        print(f'Done. Plot for {data_title} on {date} located here: ~/sage_daily_profiles/{collection}/{date}')
        plt.close()
        