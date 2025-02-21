 # -*- coding: utf-8 -*-
"""
Created on Tue Nov 28 13:32:30 2023

@author: Maurice Roots, Arthur Perng, John T. Sullivan
"""

import requests
import pandas as pd
import numpy as np
import datetime

import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.units as munits

from dateutil import tz

from tqdm import tqdm

from concurrent.futures import ThreadPoolExecutor, as_completed

#%% Function Space

# pd.set_option('future.no_silent_downcasting', True) # Remove warning about future behavior


class filter_files:
    def __init__(self, df, ptypes):
        self.df = df
        self.processing_types = ptypes
        return
        
    
    def daterange(self, min_date: str = None, max_date: str = None, **kwargs) -> pd.DataFrame:
        try:
            self.df = self.df[(self.df["start_data_date"] >= min_date) & (self.df["start_data_date"] <= max_date)]
        except:
            pass
        return self

    def instrument_group(self, instrument_group: list = None, **kwargs) -> pd.DataFrame:
        try:
            self.df = self.df[self.df["instrument_group_id"].isin(instrument_group)]
        except:
            pass
        return self

    def product_type(self, product_type: list = None, **kwargs) -> pd.DataFrame:
        try:
            self.df = self.df[self.df["product_type_id"].isin(product_type)]
        except:
            pass
        return self

    def file_type(self, file_type: list = None, **kwargs) -> pd.DataFrame:
        try:
            self.df = self.df[self.df["file_type_id"].isin(file_type)]
        except:
            pass
        return self
    
    def processing_type(self, processing_type: list = None, **kwargs) -> pd.DataFrame:
        """
        id 1 = centrally proccessed
        id 2 = in-house 
        id 3 = unprocessed
        """
        try:
            processing_type_names = []
            types = self.processing_types
            for process in processing_type:
                processing_type_names.append(
                    list(types["processing_type_name"][types["id"] == process])[0])
            # print(processing_type_names)
            self.df = self.df[self.df["processing_type_name"].isin(processing_type_names)]
        except:
            pass
        return self

class utilities:
    def __init__(self):
        self.data = {}
        self.troubleshoot = {}
        return
    
    def O3_curtain_colors(self):
        """
        Returns
        -------
        The color scheme used in the O3 curtain plots on the TOLNet website.

        """
        ncolors = [np.array([255,  140,  255]) / 255.,
           np.array([221,  111,  242]) / 255.,
           np.array([187,  82,  229]) / 255.,
           np.array([153,  53,  216]) / 255.,
           np.array([119,  24,  203]) / 255.,
           np.array([0,  0,  187]) / 255.,
           np.array([0,  44,  204]) / 255.,
           np.array([0,  88,  221]) / 255.,
           np.array([0,  132,  238]) / 255.,
           np.array([0,  165,  255]) / 255.,
           np.array([0,  235,  255]) / 255.,
           np.array([39,  255,  215]) / 255.,
           np.array([99,  255,  150]) / 255.,
           np.array([163,  255,  91]) / 255.,
           np.array([211,  255,  43]) / 255.,
           np.array([255,  255,  0]) / 255.,
           np.array([250,  200,  0]) / 255.,
           np.array([255,  159,  0]) / 255.,
           np.array([255,  111,  0]) / 255.,
           np.array([255,  63,  0]) / 255.,
           np.array([255,  0,  0]) / 255.,
           np.array([216,  0,  15]) / 255.,
           np.array([178,  0,  31]) / 255.,
           np.array([140,  0,  47]) / 255.,
           np.array([102,  0,  63]) / 255.,
           np.array([200,  200,  200]) / 255.,
           np.array([140,  140,  140]) / 255.,
           np.array([80,  80,  80]) / 255.,
           np.array([52,  52,  52]) / 255.,
           np.array([0,0,0]) ]

        ncmap = mpl.colors.ListedColormap(ncolors)
        ncmap.set_under([1,1,1])
        ncmap.set_over([0,0,0])
        bounds =   [0.001, *np.arange(5, 110, 5), 120, 150, 200, 300, 600]
        nnorm = mpl.colors.BoundaryNorm(bounds, ncmap.N)
        return ncmap, nnorm
    
    
    def curtain_plot(self, X, Y, Z, use_countourf=False, **kwargs):
        
        params = {"ylabel": "Altitude (km ASL)",
                  "xlabel": "Datetime (UTC)",
                  "fontsize_label": 18,
                  "fontsize_ticks": 16,
                  "fontsize_title": 20,
                  "title": r"$O_3$ Mixing Ratio Profile",
                  "savefig": False,
                  "savename": None,
                  "ylims": [0, 15],
                  "xlims": [X[0], X[-1]],
                  "yticks": np.arange(0, 15.1, 0.5),
                  "figsize": (15, 8),
                  "layout": "tight",
                  "cbar_label": 'Ozone ($ppb_v$)',
                  "fontsize_cbar": 16,
                  "grid": True,  # Add a parameter for grid
                  }
        
        params.update(kwargs)
        ncmap, nnorm = self.O3_curtain_colors()
        fig, ax = plt.subplots(1, 1, figsize=params["figsize"], layout=params["layout"])
        
        if use_countourf:
            # Plotting
            ncmap, nnorm = self.O3_curtain_colors()
            levels = nnorm.boundaries  # Use the same boundaries as for pcolormesh
            im = ax.contourf(X, Y, Z, levels=levels, cmap=ncmap, norm=nnorm)
            
        else:
            im = ax.pcolormesh(X, Y, Z, cmap=ncmap, norm=nnorm, shading="nearest", alpha=1)
            
        cbar = fig.colorbar(im, ax=ax, pad=0.01, ticks=[0.001, *np.arange(10, 101, 10), 200, 300])
        cbar.set_label(label=params["cbar_label"], size=16, weight="bold")
        
        plt.setp(ax.get_xticklabels(), fontsize=params["fontsize_ticks"])
        plt.setp(ax.get_yticklabels(), fontsize=params["fontsize_ticks"])
        
        cbar.ax.tick_params(labelsize=params["fontsize_ticks"])
        plt.title(params["title"], fontsize=params["fontsize_title"])

        ax.set_ylabel(params["ylabel"], fontsize=params["fontsize_label"])
        ax.set_xlabel(params["xlabel"], fontsize=params["fontsize_label"])

        ax.set_xlim(
            [np.datetime64(params["xlims"][0]), 
            np.datetime64(params["xlims"][1])]
            )

        ax.set_yticks(params["yticks"])
        
        ax.set_ylim(params["ylims"])
        
        converter = mdates.ConciseDateConverter()
        munits.registry[datetime.datetime] = converter
        ax.xaxis_date()
        
        if params["grid"]:
            ax.grid(color=params.get("grid_color", 'gray'),
                    linestyle=params.get("grid_linestyle", '--'),
                    linewidth=params.get("grid_linewidth", 0.5))
        
        if params["savename"]:
            plt.savefig(params["savename"], dpi=350)
            
        plt.show()
        
        return
    
class GEOS_CF(utilities):
    # https://dphttpdev01.nccs.nasa.gov/data-services/cfapi/assim/chm/v72/O3/39x-77/20230808/20230811
    # https://dphttpdev01.nccs.nasa.gov/data-services/cfapi/assim/met/v72/MET/39x-77/20230808/20230811
    def __init__(self, internal=True):
        super().__init__()
        if internal == False: 
            self.base_url = r"https://dphttpdev01.nccs.nasa.gov/data-services"
        else: 
            self.base_url = r"https://fluid.nccs.nasa.gov/cfapi"

        self.data[("GEOS_CF", "Replay")] = {}
        self.troubleshoot["GEOS_CF"] = []
        return
    
    def _get_geos_data(self, lat_lon, date_start, date_end, collection="assim", molecule="O3"):
        ozone_query = f'{self.base_url}/{collection}/chm/v72/{molecule}/{lat_lon}/{date_start}/{date_end}'
        heights_query = f'{self.base_url}/{collection}/met/v72/{molecule}/{lat_lon}/{date_start}/{date_end}'

        ozone_response = requests.get(ozone_query).json()
        met_response = requests.get(heights_query).json()
        times = pd.to_datetime(ozone_response['time'], utc=True, format="%Y-%m-%dT%H:%M:%S")

        
        ozone = pd.DataFrame(ozone_response['values']['O3'], index=times)
        ozone.columns = pd.to_numeric(ozone.columns)
        ozone.sort_index(axis=1, inplace=True)
        
        heights = pd.DataFrame(met_response['values']['ZL'], index=times)
        heights.columns = pd.to_numeric(heights.columns)
        heights.sort_index(axis=1, inplace=True)
        
        
        times = np.tile(times, (72, 1))
        data = {"height": heights, "ozone": ozone, "time": times.T}
        
        return (ozone_response, met_response), (date_start, date_end), data
    
    def get_geos_data_multithreaded(self, lat_lon, start_date, end_date):
        
        def fetch_geos_data_for_date(self, lat_lon, date):
            date_str = date.strftime('%Y%m%d')
            return self._get_geos_data(lat_lon, date_str, date_str)
        
        date_range = pd.date_range(start=start_date, end=end_date)
        
        with ThreadPoolExecutor(max_workers=2) as executor:
            futures = [executor.submit(fetch_geos_data_for_date, self, lat_lon, date) for date in date_range]
            
            for future in tqdm(as_completed(futures), total=len(futures), desc=f"Downloading GEOS_CF Data for {lat_lon}"):
                try:
                    response, dates, data = future.result()  
                    
                    key = ("GEOS_CF", "Replay", f"{lat_lon}")
                    if key not in self.data.keys(): 
                        self.data[key] = {}
                        
                    self.data[("GEOS_CF", "Replay", f"{lat_lon}")][dates[0]] = data
        
                except Exception as e:
                    self.troubleshoot["GEOS_CF"].append(f"{e}")
    
        return self
    

class TOLNet(GEOS_CF):

    def __init__(self):
        super().__init__()
        self.products = self.get_product_types()
        self.file_types = self.get_file_types()
        self.instrument_groups = self.get_instrument_groups()
        self.processing_types = self.get_processing_types()
        self.data = {}
        self.troubleshoot["TOLNet"] = []
        return
    
    def print_product_types(self): 
        """
        Prints out all products and their respective IDs.
        """
        print("\n TOLNET product IDs:")
        print(self.products[["id", "product_type_name", "description"]].to_string(index=False), '\n')
        return 
    
    
    def print_file_types(self):
        """
        Prints out all available filetypes and their respective IDs.
        """
        print("\n File Types:")
        print(self.file_types[["id", "file_type_name"]].to_string(index=False), '\n')
        return

    def print_instrument_groups(self):
        """
        Prints out all recorded instrument groups and their respective IDs.
        """
        print("\n Instrument Groups:")
        print(self.instrument_groups[["id", "instrument_group_name", "description"]].to_string(index=False), '\n')
        return
    
    def print_processing_types(self):
        """
        Prints out all present processing types and their respective IDs.
        """
        print("\n Processing Types:")
        print(self.processing_types[["id", "processing_type_name"]].to_string(index=False), '\n')
        return
    
    @staticmethod
    def get_product_types():
        """
        Returns a DataFrame containing all product types. 
        The returned DataFrame contains the columns id, processing_type_name, description, 
        display_order, public, and show_on_graph_page.
        """
        return pd.DataFrame(requests.get("https://tolnet.larc.nasa.gov/api/data/product_types").json()).sort_values(by=["id"])

    @staticmethod
    def get_file_types():
        """
        Returns a DataFrame containing all file types.
        The returned DataFrame contains the columns id, file_type_name, description, display_order, and public.
        """
        return pd.DataFrame(requests.get("https://tolnet.larc.nasa.gov/api/data/file_types").json()).sort_values(by=["id"])

    @staticmethod
    def get_instrument_groups():
        """
        Returns a DataFrame containing all instrument groups.
        The returned DataFrame contains the columns id, instrument_group_name, folder_name, description,
        display_order, current_pi(Principle Investigator), doi, and citation_url.
        """
        return pd.DataFrame(requests.get("https://tolnet.larc.nasa.gov/api/instruments/groups").json()).sort_values(by=["id"])
    
    @staticmethod
    def get_processing_types():
        """
        Returns a DataFrame containing all processing types.
        The returned DataFrame contains the columns id, processing_type_name, description, display_order,
        public, and show_on_graph_page.
        """
        return pd.DataFrame(requests.get("https://tolnet.larc.nasa.gov/api/data/processing_types").json()).sort_values(by=["id"])

    @staticmethod
    def get_files_list(min_date, max_date):
        """
        Parameters
        ----------
        min_date : STR
            The starting date for the query, in YYYY-MM-DD format.
        max_date : STR
            The ending date for the query, in YYYY-MM-DD format.

        Returns
        -------
        A DataFrame containing all files from the TOLNet API that fall between the two provided dates.
        The DataFrame contains each file name as well as various descriptors.
        """
        dtypes = {"row": "int16",
                 "count": "int16",
                 "id": "int16",
                 "file_name": "str",
                 "file_server_location": "str",
                 "author": "str",
                 "instrument_group_id": "int16",
                 "product_type_id": "int16",
                 "file_type_id":"int16",
                 "start_data_date": "datetime64[ns]",
                 "end_data_date":"datetime64[ns]",
                 "upload_date":"datetime64[ns]",
                 "public": "bool",
                 "instrument_group_name": "str",
                 "folder_name": "str",
                 "current_pi": "str",
                 "doi": "str",
                 "citation_url": "str",
                 "product_type_name": "str",
                 "processing_type_name": "str",
                 "file_type_name": "str",
                 "revision": "int16",
                 "near_real_time": "str",
                 "file_size": "int16",
                 "latitude": "int16",
                 "longitude": "int16",
                 "altitude": "int16",
                 "isAccessible": "bool"
                 }

        i = 1
        url = f"https://tolnet.larc.nasa.gov/api/data/1?min_date={min_date}&max_date={max_date}&order=data_date&order_direction=desc"
        response = requests.get(url)
        data_frames = []
        while response.status_code == 200:
            data_frames.append(pd.DataFrame(response.json()))
            i += 1
            url = f"https://tolnet.larc.nasa.gov/api/data/{i}?min_date={min_date}&max_date={max_date}&order=data_date&order_direction=desc"
            response = requests.get(url)

        df = pd.concat(data_frames, ignore_index=True)
        df["start_data_date"] = pd.to_datetime(df["start_data_date"])
        df["end_data_date"] = pd.to_datetime(df["end_data_date"])
        df["upload_date"] = pd.to_datetime(df["upload_date"])
        return df.astype(dtypes)

    def _add_timezone(self, time):
        return [utc.replace(tzinfo=tz.gettz('UTC')) for utc in time]

    def change_timezone(self, timezone: str):
        to_zone = tz.gettz(timezone)

        for key in self.data.keys():
            for filename in self.data[key].keys():
                time = self.data[key][filename].index.to_list()
                self.data[key][filename].index = [t.astimezone(to_zone) for t in time]

        return self

    def _json_to_dict(self, file_id: int) -> pd.DataFrame:
        """
        Parameters
        ----------
        file_id : INT
            The ID of the file to retrieve from the API.

        Returns
        -------
        A dictionary containing the file's ozone values and metadata.
        """
        try:
            url = f"https://tolnet.larc.nasa.gov/api/data/json/{file_id}"
            response = requests.get(url).json()
        except:
            self.troubleshoot["TOLNet"].append(f"Error with pulling {file_id}")
        return response
    
    def _unpack_data(self, meta_data):
        """
        Parameters
        ----------
        meta_data : A dictionary of a file's metadata

        Returns
        -------
        A DataFrame containing that same metadata
        
        """
        df = pd.DataFrame(meta_data["value"]["data"], 
                          columns = meta_data["altitude"]["data"],
                          index = pd.to_datetime(meta_data["datetime"]["data"])
                          )
        df = df.apply(pd.to_numeric)
        df[df.isnull()] = np.nan
        df.sort_index(inplace=True)
        return df
    
    def import_data(self, min_date, max_date, **kwargs):
        """
        Parameters
        ----------
        min_date : String
            The starting date to take from. Formatted as YYYY-MM-DD.
        max_date: String
            The ending date to take data from. Formatted as YYYY-MM-DD.

        """ 
        params = {"GEOS_CF": False}
        params.update(kwargs)
        
        def process_file(file_name, file_id):
            meta_data = self._json_to_dict(file_id)
            data = self._unpack_data(meta_data)
            data.index = self._add_timezone(data.index.to_list())
            return file_name, meta_data, data
        
        files = self.get_files_list(min_date, max_date)
        self.files = filter_files(files, self.processing_types).daterange(**kwargs).instrument_group(**kwargs).product_type(**kwargs).file_type(**kwargs).processing_type(**kwargs).df

        self.request_dates = (min_date, max_date)
        self.meta_data = {}
        
        # Use ThreadPoolExecutor for multithreading
        with ThreadPoolExecutor(max_workers=2) as executor:
            future_to_file = {
                executor.submit(process_file, file_name, file_id): file_name
                for file_name, file_id in zip(self.files["file_name"], self.files["id"])
            }
    
            for future in tqdm(as_completed(future_to_file), total=len(future_to_file), desc="Downloading TOLNet Data for"):
                file_name = future_to_file[future]
                try:
                    file_name, meta_data, data = future.result()
                    
                    lat_lon = (str(meta_data["LATITUDE.INSTRUMENT"]) + "x" + str(meta_data["LONGITUDE.INSTRUMENT"]))
                    date = meta_data["fileInfo"]["start_data_date"].split(" ")[0]
                    key = (meta_data["fileInfo"]["instrument_group_name"], 
                           meta_data["fileInfo"]["processing_type_name"],
                           lat_lon)
                    
                    if key not in self.data.keys(): 
                        self.data[key] = {}
                        self.meta_data[key] = {}
                        
                    self.data[key][date] = data
                    self.meta_data[key][file_name] = meta_data

                except Exception as e:
                    self.troubleshoot["TOLNet"].append(f"Error processing file {file_name}: {e}")
        
        if params["GEOS_CF"]:
            keys = list(self.data.keys())
            for key in keys:
                lat_lon = key[2]
                self.get_geos_data_multithreaded(lat_lon, self.request_dates[0], self.request_dates[1])
            
        return self

    def tolnet_curtains(self, **kwargs):
        """
        Parameters
        ----------
        timezone : INT, optional
            The timezone to display the x-axis times in. Defaults to None.
        **kwargs : TYPE
            Parameters for the plots. The accepted fields are title, xlabel, ylabel,
            xlims, ylims, yticks, surface, sonde.

        Returns
        -------
        A reference to the TOLNet object.

        """
        
        ncmap, nnorm = self.O3_curtain_colors()
        for key in self.data.keys():
            
            lim = self.request_dates
            xlims = [np.datetime64(lim[0]), np.datetime64(lim[-1])]
            
            if key[0] == "GEOS_CF":
                location = "GEOS-CF data"
            else:
                first_file = list(self.meta_data[key])[0]
                location = self.meta_data[key][first_file]['attributes']['DATA_LOCATION'].replace(".", ", ")
                
            title = f"{location} ({key[0]} / {key[1]}) \n {str(xlims[0])} - {str(xlims[1])}"
            
            plotname = f"{key[0]}_{key[1]}_{key[2]}_{str(xlims[0])}_{str(xlims[-1])}.png"
            savename = plotname.replace(' ', '_').replace('-', '_').replace('\\', '').replace('/', '')
            
            params = {"title": title, "savefig": False, "savename": savename, "xlims": xlims}
            
            
        
            if "GEOS_CF" not in key[0]:
                df = []
                for filename in self.data[key].keys():
                    
                    df.append(self.data[key][filename])
                
                df = pd.concat(df); df.sort_index(inplace=True)
                timedelta = min([(df.index[i] - df.index[i-1]).seconds for i in range(1, len(df))])
                df = df.resample(f"{timedelta}s").mean()
                
                X, Y, Z = (df.index, df.columns, df.to_numpy().T,)
                
            else: 
                T = []
                h = []
                o = []
                for date in self.data[key]:
                    t = self.data[key][date]["time"]
                    h.append(self.data[key][date]["height"])
                    o.append(self.data[key][date]["ozone"])
                    T.append(pd.DataFrame(t))
                    
                time = pd.concat(T).set_index([0], drop=False).sort_index()
                ozone = pd.concat(o).sort_index()
                height = pd.concat(h).sort_index()
                
                X = time.sort_index(axis=1, ascending=False).values
                Y = height.sort_index(axis=1, ascending=False).values / 1000
                Z = ozone.sort_index(axis=1, ascending=False).values
                params["use_countourf"] = True
            
            params.update(kwargs)
            self.curtain_plot(X, Y, Z, **params)
                
        return self


#%% Example

if __name__ == "__main__":
    
    date_start = "2023-08-08"
    date_end = "2023-08-11"
    product_IDs = [4]


    tolnet = TOLNet()
    print("Created TOLNET instance")
    
    tolnet.print_product_types()
    tolnet.print_processing_types()
    tolnet.print_instrument_groups()
    tolnet.print_file_types()
    
    data = tolnet.import_data(min_date=date_start, max_date=date_end, product_type=product_IDs, GEOS_CF=True)
    
    #%% 
    
    data.tolnet_curtains()

#%% 

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        