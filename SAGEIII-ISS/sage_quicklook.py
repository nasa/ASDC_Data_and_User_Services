import requests
import pandas as pd
import os
import PIL
from PIL import Image
import datetime

"""
By: Cheyenne Land
Tested using Python 3.10.5

This code retrieves Monthly Zonal Mean images from SAGE III-ISS' Browse Imagery Service, Quicklook:
https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/index.php

"""

def pull_h2o_no2_o3(parameter, start_date, end_date):
    current_working_directory = os.getcwd()
    valid_answer = False
    while not valid_answer:
        try:
            os.mkdir(f'{parameter}_{start_date}_{end_date}')
            break
        except FileExistsError:
            print(f'\nThis file already exists: \n{current_working_directory}/{parameter}_{start_date}_{end_date}')
            answer = input('Would you like to conintue? (y/n) \n:')
            if answer.lower() == 'y':
                break
            elif answer.lower() == 'n':
                quit()
            else:
                print('Sorry did not recongnize your answer. \nPlease enter y for yes OR n for no.')
    month_list = pd.period_range(start=start_date, end=end_date, freq='M')
    month_list = [month.strftime("%Y-%m") for month in month_list]
    years = []
    months = []
    for x in month_list:
        years.append(x[:4])
        months.append(x[-2:])

    for y,m in zip(years, months):
        with open(f'{parameter}_{start_date}_{end_date}/{parameter}_{y}_{m}.jpg', 'wb') as images:
            images.write(requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}_{y}{m}.jpg').content)
            if requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}_{y}{m}.jpg').status_code == 404:
                print(f'\n404 error for {parameter}_{y}_{m}.jpg \nCannot open image.') 
                print('Please double check date.')
                print(f'File located here: \n{current_working_directory}/{parameter}_{start_date}_{end_date}')

    frames = []
    dir_list = sorted(os.listdir(f'{parameter}_{start_date}_{end_date}')) 
    try:
        for list in dir_list:
            new_frame = Image.open(f'{parameter}_{start_date}_{end_date}/{list}')
            frames.append(new_frame)
        frames[0].save(f"{parameter}_{start_date}_{end_date}/{parameter}_{start_date}_{end_date}.gif", format="GIF", append_images=frames,
                    save_all=True, duration=700, loop=0) 
        print(f'\nComplete! \nImage(s) have been saved here: \n{current_working_directory}/{parameter}_{start_date}_{end_date}') 
        print('If you would like to visit SAGE III-ISS Quicklook Browse Imagery services, please go to the link provided below. \nhttps://sage.nasa.gov/sageiii-iss/browse_images/quicklook/index.php')
    except PIL.UnidentifiedImageError:
        quit()



def pull_single_h2o_no2_o3(parameter, date):
    current_working_directory = os.getcwd()
    valid_answer = False
    while not valid_answer:
        try:
            os.mkdir(f'{parameter}_{date}')
            break
        except FileExistsError:
            print(f'\nThis file already exists: \n{current_working_directory}/{parameter}_{date}')
            answer = input('Would you like to conintue? (y/n) \n:')
            if answer.lower() == 'y':
                break
            elif answer.lower() == 'n':
                quit()
            else:
                print('Sorry did not recongnize your answer. \nPlease enter y for yes OR n for no.')
    y = date[:4]
    m = date[-2:]
    with open(f'{parameter}_{date}/{parameter}_{y}_{m}.jpg', 'wb') as image:
        image.write(requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}_{y}{m}.jpg').content)
        if requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}_{y}{m}.jpg').status_code == 404:
                print(f'\n404 error for {parameter}_{y}_{m}.jpg \nCannot open image.') 
                print('Please double check date.')
                print(f'File located here: \n{current_working_directory}/{parameter}_{date}')
        else:
            print(f'\nComplete! \nImage(s) have been saved here: \n{current_working_directory}/{parameter}_{date}') 
            print('If you would like to visit SAGE III-ISS Quicklook Browse Imagery services, please go to the link provided below. \nhttps://sage.nasa.gov/sageiii-iss/browse_images/quicklook/index.php')



def pull_aer_atm(parameter, wavelength, start_date, end_date):
    current_working_directory = os.getcwd()
    valid_answer = False
    while not valid_answer: 
        try:
            os.mkdir(f'{parameter}_{wavelength}_{start_date}_{end_date}')
            break
        except FileExistsError:
            print(f'\nThis file already exists: \n{current_working_directory}/{parameter}_{wavelength}_{start_date}_{end_date}')
            answer = input('Would you like to conintue? (y/n) \n:')
            if answer.lower() == 'y':
                break
            elif answer.lower() == 'n':
                quit()
            else:
                print('Sorry did not recongnize your answer. \nPlease enter y for yes OR n for no.')
    month_list = pd.period_range(start=start_date, end=end_date, freq='M')
    month_list = [month.strftime("%Y-%m") for month in month_list]
    years = []
    months = []
    for x in month_list:
        years.append(x[:4])
        months.append(x[-2:])

    for y,m in zip(years, months):
        with open(f'{parameter}_{wavelength}_{start_date}_{end_date}/{parameter}_{wavelength}_{y}_{m}.jpg', 'wb') as images:
            images.write(requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}{wavelength}_{y}{m}.jpg').content)
            if requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}{wavelength}_{y}{m}.jpg').status_code == 404:
                print(f'\n404 error for {parameter}_{y}_{m}.jpg \nCannot open image.') 
                print('Please double check date.')
                print(f'File located here: \n{current_working_directory}/{parameter}_{wavelength}{start_date}_{end_date}')

    frames = []
    dir_list = sorted(os.listdir(f'{parameter}_{wavelength}_{start_date}_{end_date}')) 
    try:
        for list in dir_list:
            new_frame = Image.open(f'{parameter}_{wavelength}_{start_date}_{end_date}/{list}')
            frames.append(new_frame)
        frames[0].save(f"{parameter}_{wavelength}_{start_date}_{end_date}/{parameter}_{wavelength}_{start_date}_{end_date}.gif", format="GIF", append_images=frames,
                    save_all=True, duration=700, loop=0)
        print(f'\nComplete! \nImage(s) have been saved here: \n{current_working_directory}/{parameter}_{wavelength}_{start_date}_{end_date}') 
        print('If you would like to visit SAGE III-ISS Quicklook Browse Imagery services, please go to the link provided below. \nhttps://sage.nasa.gov/sageiii-iss/browse_images/quicklook/index.php')
    except PIL.UnidentifiedImageError:
        quit()



def pull_single_aer_atm(parameter, wavelength, date):
    current_working_directory = os.getcwd()
    valid_answer = False
    while not valid_answer:
        try:
            os.mkdir(f'{parameter}_{wavelength}_{date}')
            break
        except FileExistsError:
            print(f'\nThis file already exists: \n{current_working_directory}/{parameter}_{wavelength}_{date}')
            answer = input('Would you like to conintue? (y/n) \n:')
            if answer.lower() == 'y':
                break
            elif answer.lower() == 'n':
                quit()
            else:
                print('Sorry did not recongnize your answer. \nPlease enter y for yes OR n for no.')
  
    y = date[:4]
    m = date[-2:]
    with open(f'{parameter}_{wavelength}_{date}/{parameter}_{y}_{m}.jpg', 'wb') as image:
        image.write(requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}{wavelength}_{y}{m}.jpg').content)
        if requests.get(f'https://sage.nasa.gov/sageiii-iss/browse_images/quicklook/SAGEWeb/Quicklook/monthly_zonal_mean/{y}/{m}//QL_MONTHLY_ZMEAN_{parameter}{wavelength}_{y}{m}.jpg').status_code == 404:
                print(f'\n404 error for {parameter}_{y}_{m}.jpg \nCannot open image.') 
                print('Please double check date.')
                print(f'File located here: \n{current_working_directory}/{parameter}_{wavelength}_{date}')
        else:
            print(f'\nComplete! \nImage(s) have been saved here: \n{current_working_directory}/{parameter}_{wavelength}_{date}') 
            print('If you would like to visit SAGE III-ISS Quicklook Browse Imagery services, please go to the link provided below. \nhttps://sage.nasa.gov/sageiii-iss/browse_images/quicklook/index.php')



def input_parameter():
    valid_answer = False
    while not valid_answer:
        answer_parameter = input("""Please select a measurement of intrest 
        H2O (Water Vaping Mixing Ratio)
        NO2 (Nitrogen Dioxide Concentration)
        O3 (Ozone Concentration)
        AER (Aerosol Extinction Coefficient)
        ATM (Aerosol-to-Molecular Extinction Ratio)
        : """)
        if(answer_parameter.upper() == 'H2O' or answer_parameter.upper() == 'NO2' or answer_parameter.upper() == 'O3' or 
           answer_parameter.upper() == 'AER' or answer_parameter.upper() == 'ATM'):
           break
        else:
            print(f'\nResponse not recognized: {answer_parameter}')
            answer = input('Would you like to try again? (y/n) \n:')
            if answer.lower() == 'n':
                quit()
    return answer_parameter.upper()



def input_wavlength():
    valid_answer = False
    while not valid_answer:
        answer_wavelength = input('\nPlease select a wavelength (nm) of interest \n0384, 0449 , 0520 , 0602 , 0676 , 0756 , 0869 , 1021 , 1544 \n:')   
        if(answer_wavelength == '0384' or answer_wavelength == '0449' or answer_wavelength == '0520' 
        or answer_wavelength == '0602'  or answer_wavelength == '0676' or answer_wavelength == '0756' 
        or answer_wavelength == '0869' or answer_wavelength == '1021' or answer_wavelength == '1544'):
                    break
        else:
            print(f'\nResponse not recognized: {answer_wavelength}')
            print('You may have forgotten to add a zero in front of the three digit number.')
            answer = input('Would you like to try again? (y/n) \n:')
            if answer.lower() == 'n':
                quit()
    return answer_wavelength



def input_start_date():
    valid_answer_start = False
    while not valid_answer_start:
        answer_start_date = input('\nPlease enter a start date of interest \nFormat YYYY-MM \n:')
        start_date_format = '%Y-%m'
        try:
            datetime.datetime.strptime(answer_start_date, start_date_format)
        except ValueError:
            print(f'\n{answer_start_date} is an invalid format. \nFormat should be YYYY-MM')
            answer = input('Would you like to try again? (y/n) \n:')
            if answer.lower() == 'n':
                quit()
        else:
            break
    return answer_start_date



def input_end_date():
    valid_answer = False
    while not valid_answer:
        answer_end_date = input('\nPlease enter an end date of interest \nFormat YYYY-MM \n:')
        end_date_format = '%Y-%m'
        try:
            datetime.datetime.strptime(answer_end_date, end_date_format)
        except ValueError:
            print(f'\n{answer_end_date} is an invalid format. \nFormat should be YYYY-MM')
            answer = input('Would you like to try again? (y/n) \n:')
            if answer.lower() == 'n':
                quit()
        else:
            break
    return answer_end_date



def main():
    valid_answer = False
    while not valid_answer:
        print('\nHello, please select A or B to pull an image for one date or a date range of images.')
        print('(A) One Date \n(B) Date Range')
        answer_one = input('\n:')

        if answer_one.upper() == 'A':
            parameter = input_parameter()
            if (parameter.upper() == 'H2O' or parameter.upper() == 'NO2' or parameter.upper() == 'O3'):
                date = input_start_date()
                pull_single_h2o_no2_o3(parameter, date)

            elif (parameter.upper == 'AER' or parameter.upper() == 'ATM'):
                wavelength = input_wavlength()
                date = input_start_date()
                pull_single_aer_atm(parameter, wavelength, date)
            break

        elif answer_one.upper() == 'B':
            parameter = input_parameter()
            if (parameter.upper() == 'H2O' or parameter.upper() == 'NO2' or parameter.upper() == 'O3'):
                start_date = input_start_date()
                end_date = input_end_date()
                pull_h2o_no2_o3(parameter, start_date, end_date)

            elif (parameter.upper() == 'AER' or parameter.upper() == 'ATM'):
                wavelength = input_wavlength()
                start_date = input_start_date()
                end_date = input_end_date()
                pull_aer_atm(parameter, wavelength, start_date, end_date)
            break

        else: 
            print(f'Sorry, did not recognize your answer: {answer_one}')
            answer_two = input('Would you like to try again? (y/n) \n:')
            if answer_two.lower() == 'n':
                quit()


if __name__ == '__main__':
    main()
