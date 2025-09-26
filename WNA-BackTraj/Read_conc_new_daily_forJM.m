%test code for JM
function Read_conc_new_daily_forJM(yearselect,monselect)
%clear;clc;
%nargin=0;
if(nargin<1);yearselect=2006;end
if(nargin<2);monselect=1:12;end

addpath('/nobackup/ycui1/ERA5/Netcdf/mexcdf/mexnc');
addpath('/nobackup/ycui1/ERA5/Netcdf/mexcdf/snctools');
addpath('/nobackup/ycui1/ERA5/Netcdf/Mfiles');

flcsv='/nobackup/ycui1/ERA5/Netcdf/western_NAmerica_ozone_obs_1994_2021_new_from900to300_correcthr_v2.csv';
a = csvread(flcsv,1,0);

Pressure=a(:,1);
Years=a(:,2);
Month=a(:,3);
Day=a(:,4);
Hour=a(:,5);
Minute=a(:,6);
Sites=a(:,7);
Lats=a(:,8);
Lons=a(:,9);
O3all=a(:,10);

heights=[300,3000,8000,13000,20000];

for yr=yearselect
  if(rem(yr,4)>0)
    dd=[31,28,31,30,31,30,31,31,30,31,30,31];
  else
    dd=[31,29,31,30,31,30,31,31,30,31,30,31];
  end

  for mm=monselect
    yrname=sprintf('%2.2d',yr);
    mmname=sprintf('%2.2d',mm);

flxdir=['/nobackup/ycui1/ERA5/WUSA_',yrname,mmname,'_v2'];
flxdirout=['/nobackupp28/jryoo/p8_recovered/jryoo/accdam2020/pro/yuyan_nc/WUSA_',yrname,mmname,'_v2/'];
%flxdirout=['./WUSA_',yrname,mmname,'_v2/'];

[header,fail]=flex_header(flxdir,0,1,0);

backward_time=length(header.dates);

%yr=str2num(flxdir(9:12));
%mm=str2num(flxdir(13:14));

iselect=find(Years==yr & Month==mm);
mon=Month(iselect);
dy=Day(iselect);
hr=Hour(iselect);
min=Minute(iselect);
sit=Sites(iselect);
replat=Lats(iselect);
replon=Lons(iselect);
o3=O3all(iselect);
pre0=Pressure(iselect); pre=pre0.*100;

lat=-90:1:89; lat=lat';
lon=-180:1:179;lon=lon';

lat2d=repmat(lat,[1, 360]); lat2d=lat2d';
lon2d=(repmat(lon,[1, 180]))';lon2d=lon2d';

timest=datenum(yr,mm,1,0,0,0)-17;
timeed=datenum(yr,mm,dd(mm),23,0,0)+1/24;

timeselect0=timeed:-1:timest;

timeselect1=timeselect0(1:end-1);
timeselect2=timeselect0(2:end);

q=0;
for i=1:length(timeselect1)
    i
    vect=find(header.dates>timeselect2(i) & header.dates<=timeselect1(i));
    vect=vect';

    if(~isempty(vect))
        q=q+1;
        concall=zeros(header.numxgrid,header.numygrid,header.numzgrid,header.numpointspec);
        for k=vect
            [conc] = flex_read_V7(header,3,1,1:header.numpoint,1:header.nageclass,0,k);
            conc2=squeeze(conc); % this is the output for each hour
            concall=concall+conc2;
        end

        concnew=permute(concall,[4,3,2,1]); 
        if(q==1)
	    flout=[flxdirout,flxdir(22:end),'_info.nc']
            nc_create_empty(flout,'netcdf4-classic');
            nc_add_dimension(flout,'rep',header.numpoint);
            nc_add_dimension(flout,'latitude',180);
            nc_add_dimension(flout,'longitude',360);
            nc_add_dimension(flout,'alt',5);

            varselect='latitude';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'latitude'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','degrees_north');
            nc_varput(flout,varselect,lat);

            varselect='longitude';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'longitude'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','degrees_east');
            nc_varput(flout,varselect,lon);

            varselect='verticallayer';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'alt'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','m');
            nc_varput(flout,varselect,heights);

            varselect='receptorlon';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'rep'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','degrees_east');
            nc_varput(flout,varselect,replon);

            varselect='receptorlat';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'rep'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','degrees_north');
            nc_varput(flout,varselect,replat);

            varselect='receptoralt';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'rep'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','hpa');
            nc_varput(flout,varselect,pre0);

            varselect='receptoro3';
            varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'rep'};
            nc_addvar(flout,varstruct);
            nc_attput(flout,varselect,'units','ppbv');
            nc_varput(flout,varselect,o3);
        end

        timename=datestr(timeselect2(i),'yyyymmddHHMMSS');
        flout=[flxdirout,'Retroplume_',timename,'.nc'];
        nc_create_empty(flout,'netcdf4-classic');
        nc_add_dimension(flout,'rep',header.numpoint);
        nc_add_dimension(flout,'latitude',180);
        nc_add_dimension(flout,'longitude',360);
        nc_add_dimension(flout,'alt',5);

        varselect='latitude';
        varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'latitude'};
        nc_addvar(flout,varstruct);
        nc_attput(flout,varselect,'units','degrees_north');
        nc_varput(flout,varselect,lat);

        varselect='longitude';
        varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'longitude'};
        nc_addvar(flout,varstruct);
        nc_attput(flout,varselect,'units','degrees_east');
        nc_varput(flout,varselect,lon);

        varselect='retroplume';                                                                                                           
        varstruct.Name=varselect; varstruct.Datatype='double'; varstruct.Dimension={'rep','alt','latitude','longitude'};                 
        nc_addvar(flout,varstruct);                                                                                                       
        nc_attput(flout,varselect,'units','s kg-1 m-3');                                                                                  
        nc_varput(flout,varselect,concnew); 
    end
end
end
end
