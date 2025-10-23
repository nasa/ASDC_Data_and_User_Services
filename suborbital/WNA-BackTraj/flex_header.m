function [header, fail] = flex_header(pathname, nest, readp, calcarea, datespath)
%===========================================
% flex_header.m
%-------------------------------------------
% input
%   - pathname: directory path
%   - readp   : read release points (0/1)
% output
%   - header object
%   - fail indicator (-1=fail, 0=success)
%-------------------------------------------
% FLEXPART matlab import routines
%-------------------------------------------
% last changes: HSO, 06-Jun-2007
%===========================================
 
  % anonymous helper function to skip some bytes
  skip = @(fid) fread(fid,2,'int32');

  % define the output object
  if nargin==0
    pathname='.';
  elseif nargin==1
    nest=0;  
    readp=1; % default: do not read release points
    calcarea=0;
  end % if nargin
  
  % create file name for header
  if (pathname(length(pathname))~='/')
    pathname=[pathname '/'];
  end % if pathname


  if nargin<=4
     datespath=pathname;
  else
    datespath=deblank(datespath);
  end   
  
  header.path=pathname;
  if nest==0
    fhd=fopen([header.path 'header']);
    %fprintf('\nreading header');
    header.nest=0;
  else
    fhd=fopen([header.path 'header_nest']);
    header.nest=1;
    fprintf('\nreading nested header');
  end

  % default settings
  header.decayconstant=0;

  % start reading if header was opened
  if fhd~=-1
    rl=fread(fhd,1,'int32');  
    header.open=1;
    header.ibdate=fread(fhd,1,'int32');
    header.ibtime=fread(fhd,1,'int32');
    pivot_vec=(datevec(date));
    %header.simulationstart=datenum([num2str(header.ibdate)  num2str(header.ibtime,'%06d')],'yyyymmddhhMMss',pivot_vec(1)-50);
    header.simulationstart=datenum([num2str(header.ibdate)  num2str(header.ibtime,'%06d')],'yyyymmddhhMMss');
    header.ver=char(transpose(fread(fhd,rl-8,'char'))); 
    %header.ver=char(transpose(fread(fhd,13,'char'))); 
    %header.ver=char(transpose(fread(fhd,27,'char'))); new FLEXPART version
    skip(fhd);
    header.loutstep=fread(fhd,1,'int32');
    header.loutaver=fread(fhd,1,'int32');
    header.loutsample=fread(fhd,1,'int32');
    skip(fhd);
    header.outlon0=fread(fhd,1,'float');
    header.outlat0=fread(fhd,1,'float');
    header.numxgrid=fread(fhd,1,'int32');
    header.numygrid=fread(fhd,1,'int32');    
    header.dxout=fread(fhd,1,'float');
    header.dyout=fread(fhd,1,'float');
    skip(fhd);
    header.numzgrid=fread(fhd,1,'int32');
    header.outheight=fread(fhd,header.numzgrid,'float');
    header.outheight=cat(1,[0],header.outheight);
    skip(fhd);
    header.jjjjmmdd=fread(fhd,1,'int32');    
    header.hhmmss=fread(fhd,1,'int32');    
    skip(fhd);
    header.nspec=fread(fhd,1,'int32')/3;
    %%%%%%%%%%%%%%%%%%%%%%% diff verisons
%     skip(fhd);
    header.numpointspec=fread(fhd,1,'int32');
    skip(fhd);
    %%%%%%%%%%%%%%%%%%%%%%% diff verisons
    % read the species
    for i=1:header.nspec
      one=fread(fhd,1,'int32'); % input skipped ??
      wd=fread(fhd,10,'char');  % input skipped ??
      skip(fhd);
      one=fread(fhd,1,'int32'); % input skipped ??
      dd=fread(fhd,10,'char');  % input skipped ??
      skip(fhd);
      header.numzgrid=fread(fhd,1,'int32');  
      header.species(i,1:10)=char(transpose(fread(fhd,10,'char')));
      skip(fhd);
    end % nspec
 
    % read the release points
    header.numpoint=fread(fhd,1,'int32');
      % initialise fields

    header.ireleasestart=zeros(header.numpoint,1);
    header.ireleaseend=zeros(header.numpoint,1);
    header.kindz=zeros(header.numpoint,1);
    header.xp1=zeros(header.numpoint,1);
    header.yp1=zeros(header.numpoint,1);
    header.xp2=zeros(header.numpoint,1);
    header.yp2=zeros(header.numpoint,1);
    header.zpoint1=zeros(header.numpoint,1);
    header.zpoint2=zeros(header.numpoint,1);
    header.npart=zeros(header.numpoint,1);
%    for i=1:header.numpoint,
%      header.compoint(i,1:45)='                                             ';
%    end
    header.xmass=zeros(header.numpoint,header.nspec);
    
    % read release info if requested
    if readp==1
      rl=fread(fhd,1,'int32');  
      for i=1:header.numpoint
        rl=fread(fhd,1,'int32');  
        %header.ireleasestart(i)=fread(fhd,1,'int32');
        %header.ireleaseend(i)=fread(fhd,1,'int32');
        header.ireleasestart(i)=header.simulationstart+(fread(fhd,1,'int32'))/24/3600;
        header.ireleaseend(i)=header.simulationstart+(fread(fhd,1,'int32'))/24/3600;
        %%%%%%%%%%%%%%%% 2 diffs!
%        header.kindz(i)=fread(fhd,1,'int32');
        header.kindz(i)=fread(fhd,1,'int16');
        %%%%%%%%%%%%%%%% 2 diffs!
        skip(fhd);
        header.xp1(i)=fread(fhd,1,'float');
        header.yp1(i)=fread(fhd,1,'float');
        header.xp2(i)=fread(fhd,1,'float');
        header.yp2(i)=fread(fhd,1,'float');
        header.zpoint1(i)=fread(fhd,1,'float');
        header.zpoint2(i)=fread(fhd,1,'float');
        skip(fhd);
        header.npart(i)=fread(fhd,1,'int32');
        header.mpart(i)=fread(fhd,1,'int32');
        rl=fread(fhd,2,'int32');
        header.compoint(i,:)=char(transpose(fread(fhd,45,'char')));
        rl=fread(fhd,1,'int32');
        for j=1:header.nspec,
          rl=fread(fhd,1,'int32');
          header.xmass(i,j)=fread(fhd,1,'float');
          skip(fhd);
          header.xmass(i,j)=fread(fhd,1,'float');
          skip(fhd);
          header.xmass(i,j)=fread(fhd,1,'float');
          rl=fread(fhd,1,'int32');
        end
      end % numpoint
    else
      % skip reading points, fill data structure with zeroes
      fseek(fhd,119*header.numpoint+(header.nspec*36)*header.numpoint+4,'cof');
    end % readp

    
    rl=fread(fhd,1,'int32');
    %%% check if ind_rel, ind_source is written out
    
    header.method=fread(fhd,1,'int32');
    header.lsubgrid=fread(fhd,1,'int32');
    header.lconvection=fread(fhd,1,'int32');
    if rl==20
    header.ind_source=fread(fhd,1,'int32');
    header.ind_receptor=fread(fhd,1,'int32');  
    end
    skip(fhd);
    header.nageclass=fread(fhd,1,'int32');    
    header.lage(1:header.nageclass)= fread(fhd,header.nageclass,'int32');
    skip(fhd);

    header.oroout=zeros(header.numxgrid,header.numygrid);
    for ix=1:header.numxgrid,
      header.oroout(ix,:)=fread(fhd,header.numygrid,'float');
      skip(fhd);
    end% fread

    fclose(fhd);
    fail=0;
 
    % try reading the dates file 
    f=([datespath 'dates']);
    if exist(f)
      dat=importdata([datespath 'dates']);
      header.dates=(datenum(num2str(dat),'yyyymmddHHMMss'));
    else
       header.dates=NaN;
       fprintf('Could not read dates file: %s\n',f);
    end
   
    header.lonp = [header.outlon0+header.dxout/2:header.dxout:header.outlon0+header.dxout*header.numxgrid];
    header.latp = [header.outlat0+header.dyout/2:header.dyout:header.outlat0+header.dyout*header.numygrid];
    header.zp= (header.outheight(1:length(header.outheight)-1)+header.outheight(2:length(header.outheight)))./2;
    %    header.lonp = [header.outlon0:header.dxout:header.outlon0+header.dxout*header.numxgrid-header.dxout];
%    header.latp = [header.outlat0:header.dyout:header.outlat0+header.dyout*header.numygrid-header.dyout];
   
    if calcarea==1
      header.area  = calculate_grid_area_eff(header);
    end
  else % if header file not opened
  
    fail=-1;
    header.open=0; % this is the only field in header!

  end % if header file opened

end % of function flex_header

%fin
