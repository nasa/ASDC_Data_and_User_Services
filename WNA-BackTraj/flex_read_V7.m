function [grid, drydep, wetdep, fail] = flex_read(header,unit,nspec_ret,pspec_ret,age_ret,nest,time_ret,fin)
%===========================================
% flex_read_V7.m
%-------------------------------------------
% input
%   - header object
%   - optional: data units, (1==conc(default), 2==pptv, 3==time(bkwd)
%   4==footprint_total 9=column_total 10=drydep, 11=wetdep)
%   - optional: which species
%               which pointspecies
%               which ageclasse
%   - optional: nest (0==no(default)/1==yes)
%               which time
% output
%   - data
%   - dry deposition
%   - wet deposition
%   - fail indicator (-1=fail, 0=success)
%-------------------------------------------
% FLEXPART matlab import routines
%-------------------------------------------
% last changes: HSO, 15-Jun-2007
%===========================================

% preset output variables
fail=1;

% set default units
prefix=['grid_conc_          ';'grid_pptv_          '
    'grid_time_          ';'footprint_total     '; ...
    'grid_conc_nest_     ';'grid_pptv_nest_     '; ...
    'grid_time_nest_     ';'footprint_total_nest'; ...
    'column_total        ';'grid_drydep_        '; ...
    'grid_wetdep_        ';'grid_scav_          '; ...
    'grid_total_nest_    ';'grid_drydep_nest_   '; ...
    'grid_wetdep_nest_   ';'grid_scav_nest_     '];

if nargin==1
    unit=1;
end

if nargin<=2
    nest=0;
end

if nest==1
    unit=unit+4;
end

if nargin<=6 || time_ret(1)<=0
    time_ret=1:length(header.dates);
end % if times



if unit==4 | unit==9
    grid=zeros(header.numxgrid,header.numygrid,1,length(nspec_ret),length(pspec_ret),length(age_ret),1);
else
    grid=zeros(header.numxgrid,header.numygrid,header.numzgrid,length(nspec_ret),length(pspec_ret),length(age_ret),length(time_ret));
    if unit<10
        wetdep=zeros(header.numxgrid,header.numygrid,length(nspec_ret),length(pspec_ret),length(age_ret),length(time_ret));
        drydep=zeros(header.numxgrid,header.numygrid,length(nspec_ret),length(pspec_ret),length(age_ret),length(time_ret));
    end
end
%header.numpointspec=1;

dat_cnt=0;

%--------------------------------------------------
% Loop over all times, given in field header.dates
%--------------------------------------------------
for dat_i=time_ret,
    dat_cnt=dat_cnt+1;
    ks_cnt=0;
    
    for ks=nspec_ret,
        ks_cnt=ks_cnt+1;
        
        if nargin<8 || isempty(fin)
            % filename to use is not given
            if dat_i<=length(header.dates)
                if unit~=4 & unit~=9 %not footprint or tot_col
                    fin=[header.path deblank(prefix((unit),:)) datestr(header.dates(dat_i),'YYYYmmddHHMMSS') num2str(ks,'_%03d') ];
                else
                    fin=[header.path deblank(prefix((unit),:)) num2str(ks,'_%03d') ];
                end
            else
                fprintf('Date does not exist! \n');
            end
        end
        
        
        fprintf('Opening %s \n',fin);
        fid=fopen(fin);
        
        
        if fid<=0
            fprintf('******************************\n could not open file %s !\n******************************\n',fin);
        else
            
            
            rl=fread(fid,1,'int32');
            itime=fread(fid,1,'int32');
            rl=fread(fid,2,'int32');
            
            %----------------------------------------------
            % read all fields
            %----------------------------------------------
            kp_cnt=0;
            
            for kp=1:header.numpointspec
                %  fprintf('reading: %f \n',kp);
                kp_read=0;
                if (kp_cnt)<length(pspec_ret)
                    if find(pspec_ret(kp_cnt+1)==kp)
                        kp_cnt=kp_cnt+1;
                        kp_read=1;
                    end
                end
                nage_cnt=0;
                for nage=1:max(1,header.nageclass)
                    nage_read=0;
                    if (nage_cnt)<length(age_ret)
                        if find(age_ret(nage_cnt+1)==nage)
                            nage_cnt=nage_cnt+1;
                            nage_read=1;
                        end
                    end
                    %----------------------------------------------
                    % read wetgrid
                    %----------------------------------------------
                    sp_count_i=fread(fid,1,'int32');
                    rl=fread(fid,2,'int32');
                    sparse_dump_i=fread(fid,sp_count_i,'int32');
                    rl=fread(fid,2,'int32');
                    sp_count_r=fread(fid,1,'int32');
                    rl=fread(fid,2,'int32');
                    sparse_dump_r=fread(fid,sp_count_r,'float');
                    rl=fread(fid,2,'int32');
                    
                    if (nage_read & kp_read)
                        fact=1;
                        ii=1;
                        for ir=1:length(sparse_dump_r)
                            if sparse_dump_r(ir)*fact>0
                                n=sparse_dump_i(ii);
                                fact=fact*-1;
                                ii=ii+1;
                            else
                                n=n+1;
                            end
                            jy=floor(n/header.numxgrid);
                            ix=n-header.numxgrid*jy;
                            wetdep(ix+1,jy+1,ks_cnt,kp_cnt,nage_cnt,dat_cnt)=sparse_dump_r(ir)*fact*(-1);
                        end
                    end
                    
                    
                    %----------------------------------------------
                    % read drygrid
                    %----------------------------------------------
                    sp_count_i=fread(fid,1,'int32',8);
                    sparse_dump_i=fread(fid,sp_count_i,'int32');
                    rl=fread(fid,2,'int32');
                    sp_count_r=fread(fid,1,'int32',8);
                    sparse_dump_r=fread(fid,sp_count_r,'float');
                    rl=fread(fid,2,'int32');
                    
                    if (nage_read & kp_read)
                        fact=1;
                        ii=1;
                        for ir=1:length(sparse_dump_r)
                            if sparse_dump_r(ir)*fact>0
                                n=sparse_dump_i(ii);
                                fact=fact*-1;
                                ii=ii+1;
                            else
                                n=n+1;
                            end
                            jy=floor(n/header.numxgrid);
                            ix=n-header.numxgrid*jy;
                            drydep(ix+1,jy+1,ks_cnt,kp_cnt,nage_cnt,dat_cnt)=sparse_dump_r(ir)*fact*(-1);
                        end
                    end
                    
                    %----------------------------------------------
                    % read data grid
                    %----------------------------------------------
                    
                    sp_count_i=fread(fid,1,'int32');
                    rl=fread(fid,2,'int32');
                    sparse_dump_i=fread(fid,sp_count_i,'int32');
                    rl=fread(fid,2,'int32');
                    sp_count_r=fread(fid,1,'int32');
                    rl=fread(fid,2,'int32');
                    sparse_dump_r=fread(fid,sp_count_r,'float');
                    rl=fread(fid,2,'int32');
                    
                    if (nage_read & kp_read)
                        fact=1;
                        ii=1;
                        for ir=1:length(sparse_dump_r)
                            if sparse_dump_r(ir)*fact>0
                                n=sparse_dump_i(ii);
                                fact=fact*-1;
                                ii=ii+1;
                            else
                                n=n+1;
                            end
                            kz=floor(n/(header.numxgrid*header.numygrid));
                            if kz>length(header.zp)
                                fprintf('ERROR - number of vertical levels do not match!');
                            end
                            jy=floor((n-kz*header.numxgrid*header.numygrid)/header.numxgrid);
                            ix=floor(n-header.numxgrid*header.numygrid*kz-header.numxgrid*jy);
                            grid(ix+1,jy+1,kz,ks_cnt,kp_cnt,nage_cnt,dat_cnt)=sparse_dump_r(ir)*(-1)*fact;
                        end
                    end
                    
                end % for nage
            end % for numpoint
            fclose(fid);
        end % file succesfully opened
        
    end % for species
end % for dat_i



fail=0;

end % of function flex_read

%fin
