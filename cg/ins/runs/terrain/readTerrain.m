% ============================================================================================
% 
% Read GIS terrain data and export surface files for Ogen.
% 
% NOTE:
%     .flt and .hdr files come from USGS's National Map Viewer.
%
%
% Usage: s=string, f=float, i=integer (all options are optional and order does not matter):
% 
%    readTerrain -file=<s> -name=<s> -long0=<f> -lat0-<f> -xWidth=<f> -yWidth=<f> -plotOption=[0|1] ...
%                -smooth=[0|1] -crossSection=[0|1] -nxa=<i> -nxb=<> -nya=<i> -nyb=<i> ...
%                -yCrossSection=<f> -xCrossSection=<f> -nSmooth=<i> -nBoundarySmooth=<i>
%
% Options:
%  -file: GIS file name (i.e. file.flt and file.hdr should exist)
%  -name : output file name
%  -long0, -lat0 : center of sub-region (in degrees longitude [-180,180] and latitude [-90,90]  )
%  -xWidth : horizontal width (m) in x direction (longitudinal direction). By default use full size.
%  -yWidth : horizontal width (m) in y direction  (lateral direction) By defaut use full size.
%  -nxa  : number of buffer zones to add to the start in the x (longitude) direction.
%  -nxb  : number of buffer zones to add to the end in the x (longitude) direction.
%  -nya  : number of buffer zones to add to the start in the y (latitude) direction.
%  -nyb  : number of buffer zones to add to the end in the y (latitude) direction.
%  -smooth : 1=smooth surface 
%  -nSmooth : number of interior smoothing iterations
%  -nBoundarySmooth : number of boundary smooths PER interior smooth iteration
%  -xCrossSection=xVal : if xVal>=0, create a 2D cross section at xVal (m) from the left side x=0 (closest grid point).
%  -yCrossSection=yVal : if yVal>=0, create a 2D cross section at yVal (m) from the bottom y=0 (closest grid point).
%  -plotOption : 0=no plotting, 1=plot, 3=plot and save-hard-copies 
% 
% Notes:
%   Buffer Zones:
%     Buffer zones are extended from the terrain region in selected directions in order to provide a smooth terrain profile near the
%    boundaries of the computational regime. A buffer zone may be needed at outflow boundaries to prevent strong
%    recirculation regions (from steep terrain near the outflow) from hitting the outflow boundary and 
%    causing a locally strong inflow condition that may cause difficulties in the flow solver.
% 
% Examples: 
%  Altamont Pass: Extent : (-121.701, 37.667), (-121.576, 37.782)
%    readTerrain -file=AltamontPass -name=AltamontPass -plotOption=1 -smooth=1 -long0=-121.643519 -lat0=37.724444 -xWidth=3000 -yWidth=3000
%    
%  Initial site300 region from Kyle, 1500m x 1500m 
%    readTerrain -file=35707821 -name=site300Full  -plotOption=1 -nxa=10 -nxb=40 -nya=10 -nyb=10
%    readTerrain -file=35707821 -name=site300Full -long0=-121.554815 -lat0=37.645926 -xWidth=700 -yWidth=700 -plotOption=1
% 
%   Region around site300 from Charles:
%      - site 300 met tower (37.675, -121.541)
%      - lightning detector at Site 300 (37.674, -121.539)
%      - Site 300 Radio tower (37.652, -121.534)
%      - North of Building 834L ("Matthew Area") (37.64805, -121.502778)
%     -- 1000m x 1000m :
%    readTerrain -file=floatn38w122_1 -name=site300x1y1 -long0=-121.541 -lat0=37.675 -xWidth=1000 -yWidth=1000 -plotOption=1 
%      -- 10Km x 10Km : 
%    readTerrain -file=floatn38w122_1 -name=site300x10y10 -long0=-121.541 -lat0=37.675 -xWidth=10000 -yWidth=10000 -plotOption=1 
%
% Authors: 
%         2011/09/03 : WDH, initial version. This script came from runs/cgmx/bass/readAfm.m
%         2012/09    : Charles Reid -- read GIS directly, other enhancements
%         2012/11    : WDH : enhancements
%          
% ============================================================================================
function readTerrain(varargin)

% --- define default values for parameters ---


file = 'floatn38w122_1';
name ='site300_0.5km'; 

xCrossSection=-1; % if >0 create a x=constant curve
yCrossSection=-1; % if >0 create a y=constant curve

plotOption=1; 

smoothSurface = 1;   % 0: no smoothing
nSmooth=4;           % number of smoothing steps
nBoundarySmooth=10;  % smooth the boundary more

smoothSubPatch=0;    % set to 1 to turn on smoothing of a sub-patch
nSubPatchSmooth=20; 

% optionally add a buffer zone of this many cells on each edge.
% The buffer zone will transition the terrain to be approximately flat (?)
% *** FINISH ME ***
nxa=0; nxb=0; 
nya=0; nyb=0;




% pick a centerpoint for the bounding box
long0 = -121.541;
lat0 = 37.675;
%%% lat0 = 37.674;
%%% long0 = -121.541;
%lat0 = 37.774;
%long0 = -121.541;

% bounding box dimensions: [m]
xWidth = 1.e10; yWidth=1e10;

% utilitarian stuff
deg2rad = (pi/180.0);
r_earth = 6378.1e3;    % radius of the earth in meters


 % --- read command line args ---
 for i = 1 : nargin
   % fprintf ( 1, 'readTerrain: argument %d is [%s]\n', i, varargin{i} );
   line = varargin{i};

   if( strncmp(line,'-plotOption=',12) )
     plotOption = sscanf(varargin{i},'-plotOption=%d'); 
   elseif( strncmp(line,'-long0=',4) )
     long0 = sscanf(varargin{i},'-long0=%e'); 
   elseif( strncmp(line,'-lat0=',4) )
     lat0 = sscanf(varargin{i},'-lat0=%e'); 
   elseif( strncmp(line,'-xWidth=',8) )
     xWidth = sscanf(varargin{i},'-xWidth=%e'); 
   elseif( strncmp(line,'-yWidth=',8) )
     yWidth = sscanf(varargin{i},'-yWidth=%e'); 
   elseif( strncmp(line,'-nSmooth=',9) )
     nSmooth = sscanf(varargin{i},'-nSmooth=%d'); 
   elseif( strncmp(line,'-name=',6) )
     name = sscanf(varargin{i},'-name=%s'); 
   elseif( strncmp(line,'-file=',6) )
     file = sscanf(varargin{i},'-file=%s'); 
   elseif( strncmp(line,'-nxa=',5) )
     nxa = sscanf(varargin{i},'-nxa=%d'); 
   elseif( strncmp(line,'-nxb=',5) )
     nxb = sscanf(varargin{i},'-nxb=%d'); 
   elseif( strncmp(line,'-nya=',5) )
     nya = sscanf(varargin{i},'-nya=%d'); 
   elseif( strncmp(line,'-nyb=',5) )
     nyb = sscanf(varargin{i},'-nyb=%d'); 
   elseif( strncmp(line,'-xCrossSection=',15) )
     xCrossSection = sscanf(varargin{i},'-xCrossSection=%e'); 
   elseif( strncmp(line,'-yCrossSection=',15) )
     yCrossSection = sscanf(varargin{i},'-yCrossSection=%e'); 
   elseif( strncmp(line,'-smooth=',8) )
     smoothSurface = sscanf(varargin{i},'-smooth=%d'); 
   elseif( strncmp(line,'-nSmooth=',9) )
     nSmooth = sscanf(varargin{i},'-nSmooth=%d'); 
   elseif( strncmp(line,'-nBoundarySmooth=',17) )
     nBoundarySmooth = sscanf(varargin{i},'-nBoundarySmooth=%d'); 
   else
      fprintf ( 1, 'readTerrain:WARNING: argument %d [%s] is UNKNOWN.\n', i, varargin{i} );
   end;
 end


 fprintf('readTerrain: file=%s, name=%s, (long0,lat0)=(%9.3e,%9.3e) xWidth=%9.3e, yWidth=%9.3e, plotOption=%d, nSmooth=%d,\n',...
       file,name,long0,lat0,xWidth,yWidth,plotOption,nSmooth);
fprintf('           : xCrossSection=%e, yCrossSection=%e, nSmooth=%d, nBoundarySmooth=%d.\n',...
          xCrossSection,yCrossSection,nSmooth,nBoundarySmooth);

% ---------------------------------------------------------------------------------
% ----------------- choose a subset of the surface by lat/long --------------------
% ---------------------------------------------------------------------------------


% 1 arcsecond = 1/3600 of a degree = 30 meters 
km2deg = (10^3)/(30*60*60);
m2deg = 1./(30*60*60);

% Convert region sizes in meters into (approximate sizes in degrees long/lat)
% Use distance between two points 1 degree apart near (long0,lat0): 
% phi = latitude
% lambda = longitude
phi_1=lat0*deg2rad;  lambda_1=long0*deg2rad;

phi_2=phi_1+deg2rad;  lambda_2=lambda_1;  % shift 1 degree in latitude
lat2m = 2*r_earth*asin( sqrt( sin((phi_2-phi_1)/2)^2 + cos(phi_1)*cos(phi_2)*sin((lambda_2-lambda_1)/2)^2 ) );

phi_2=phi_1;  lambda_2=lambda_1+deg2rad; % shift 1 degree in longitude
long2m = 2*r_earth*asin( sqrt( sin((phi_2-phi_1)/2)^2 + cos(phi_1)*cos(phi_2)*sin((lambda_2-lambda_1)/2)^2 ) );

fprintf(' m2deg=%10.3e lat2m=%10.3e 1/lat2m=%10.3e long2m=%10.3e, 1/long2m=%10.3e\n',...
          m2deg,lat2m,1/lat2m,long2m,1/long2m);
% pause

bbWidth = xWidth/long2m;
lng_start = long0 - (bbWidth/2);
lng_end   = long0 + (bbWidth/2);

bbHeight = yWidth/lat2m;
lat_start = lat0 - (bbHeight/2);
lat_end   = lat0 + (bbHeight/2);

if( xWidth ~= 1.e10 || yWidth ~= 1.e10 )
  fprintf('----------------------------------------\n');
  fprintf('------- Bounding Box Dimensions: -------\n');
  fprintf('Longitude: [%0.4f,%0.4f] degrees\n',lng_start,lng_end);
  fprintf('Latitude:  [%0.4f,%0.4f] degrees\n',lat_start,lat_end);
  fprintf('----------------------------------------\n');
  fprintf('\n');
end;
%pause()


% ---------------------------------------------------------------------
% ----------------------------- Load GIS Data -------------------------
% Load data from GridFloat files containing GIS data
% (.flt and .hdr files; these come from USGS's National Map Viewer)

floatfile = strcat(file,'.flt');
hdrfile = strcat(file,'.hdr');

H = fopen(hdrfile,'r');

l = fgetl(H); 
[s, ncols] = strtok(l); 
ncols = str2num(ncols);

l = fgetl(H); 
[s, nrows] = strtok(l); 
nrows = str2num(nrows);

l = fgetl(H); 
[s, xllcorner] = strtok(l); 
xllcorner = str2num(xllcorner);  % lower left corner -- longitude

l = fgetl(H); 
[s, yllcorner] = strtok(l); 
yllcorner = str2num(yllcorner);

l = fgetl(H); 
[s, cellsize] = strtok(l); 
cellsize = str2num(cellsize);

fclose(H);

F = fopen(floatfile,'r','ieee-le');
e=fread(F,ncols*nrows,'float32');
fclose(F);

lng = xllcorner + linspace(0,ncols-1,ncols)*cellsize;
lat = yllcorner + linspace(0,nrows-1,nrows)*cellsize;

fprintf('File: bounds: [%0.6f,%0.6f]x [%0.6f,%0.6f] degrees longitude x latitude\n',lng(1),lng(nrows),lat(1),lat(nrows));
xCenter=.5*(lng(1)+lng(nrows)); yCenter=.5*(lat(1)+lat(nrows));

xca = abs(xCenter); yca=abs(yCenter);

xDegrees = floor(xca); xMinutes = floor( (xca-xDegrees)*60. ); xSeconds=(xca-xDegrees-xMinutes/60.)*60*60; 
yDegrees = floor(yca); yMinutes = floor( (yca-yDegrees)*60. ); ySeconds=(yca-yDegrees-yMinutes/60.)*60*60; 

fprintf('      centre: [%0.6f,%0.6f] = [%3.0fo %2.0fm %fs, %3.0fo %2.0fm %fs] [long,lat] degrees\n',...
         xCenter,yCenter, xDegrees,xMinutes,xSeconds, yDegrees, yMinutes, ySeconds );

% phi = latitude
% lambda = longitude

% compute longitudinal locations (in m instead of longitudinal degrees)
%lng_m = zeros(length(lng));
kk = 1;
for i=1:length(lng)

    if( lng(i) > lng_start && lng(i) < lng_end )
        
        if( kk == 1 )
            i_start = i;
        else
            i_start = lng_indices(1);
        end

        phi_1 = lat(1)*deg2rad;
        phi_2 = lat(1)*deg2rad;

        lambda_1 = lng(i_start)*deg2rad;
        lambda_2 = lng(i)*deg2rad;

        d = 2*r_earth*asin( sqrt( sin((phi_2-phi_1)/2)^2 + cos(phi_1)*cos(phi_2)*sin((lambda_2-lambda_1)/2)^2 ) );

        lng_m(kk)=d;
        lng_indices(kk) = i;

        kk=kk+1;

    end

end

% compute lateral locations (in m instead of latitudinal degrees)
%lat_m = zeros(length(lat));
kk = 1;
for j=1:length(lat)

    if( lat(j) > lat_start && lat(j) < lat_end ) 

        if( kk == 1 )
            j_start = j;
        else
            j_start = lat_indices(1);
        end

        phi_1 = lat(j_start)*deg2rad;
        phi_2 = lat(j)*deg2rad;

        lambda_1 = lng(1)*deg2rad;
        lambda_2 = lng(1)*deg2rad;

        d = 2*r_earth*asin( sqrt( sin((phi_2-phi_1)/2)^2 + cos(phi_1)*cos(phi_2)*sin((lambda_2-lambda_1)/2)^2 ) );

        lat_m(kk) = d;
        lat_indices(kk) = j;

        kk=kk+1;

    end

end

nrows_inrange = length(lat_m);
ncols_inrange = length(lng_m);

xMinimum = lng_m(1); xMaximum=lng_m(ncols_inrange);
yMinimum = lat_m(1); yMaximum=lat_m(nrows_inrange);

if( 1==1 )
  fprintf('----------------------------------------\n');
  fprintf('-------- Actual Box Dimensions: --------\n');
  fprintf('Longitude: [%0.4f,%0.4f] m\n',xMinimum,xMaximum);
  fprintf('Latitude:  [%0.4f,%0.4f] m\n',yMinimum,yMaximum);
  fprintf('----------------------------------------\n');
  fprintf('\n');
end;


% Now populate the data array
%%% data = zeros(3,ncols,nrows);
%%% for lt=1:nrows
%%%     for lg=1:ncols
%%%         y = lat_m(lt);
%%%         x = lng_m(lg);
%%%         ii = (nrows-lt)*ncols + lg;
%%% 
%%%         data(:,lg,lt) = [x y e(ii)];
%%% 
%%%         %disp(sprintf('%18.10e %18.10e %18.10e',x,y,e(ii)))
%%%         %fprintf(target,'%18.10e %18.10e %18.10e \n',x,y,e(ii));
%%%     end
%%% end
data = zeros(3,ncols_inrange,nrows_inrange);
for lt=1:nrows_inrange
    for lg=1:ncols_inrange
        y = lat_m(lt);
        x = lng_m(lg);

        ii = (nrows-lat_indices(lt))*ncols + lng_indices(lg);

        data(:,lg,lt) = [x y e(ii)];

        %disp(sprintf('%18.10e %18.10e %18.10e',x,y,e(ii)))
        %fprintf(target,'%18.10e %18.10e %18.10e \n',x,y,e(ii));
    end
end

%save('gridFloatVars.mat','nrows','ncols','xllcorner','yllcorner','data');
%load('gridFloatVars.mat');

%i1a=1; i1b=ncols_inrange;
%i2a=1; i2b=nrows_inrange; 
%
%nx=i1b-i1a+1;
%ny=i2b-i2a+1;

%x = zeros(nx,ny);
%x(1:nx,1:ny) = data(1,i1a:i1b,i2a:i2b);
%y = zeros(nx,ny);
%y(1:nx,1:ny) = data(2,i1a:i1b,i2a:i2b);
%z = zeros(nx,ny);
%z(1:nx,1:ny) = data(3,i1a:i1b,i2a:i2b);

nx = length(lng_m);
ny = length(lat_m);

x = zeros(nx,ny);
x(:,:) = squeeze( data(1,:,:) );

y = zeros(nx,ny);
y(:,:) = squeeze( data(2,:,:) );

z = zeros(nx,ny);
z(:,:) = squeeze( data(3,:,:) );


if( nxa>0 || nxb>0 || nya>0 || nyb>0 )

  % ------------------------
  % -- add a buffer zone ---
  % ------------------------

  % new number of grid points
  nxp = nx + nxa + nxb;
  nyp = ny + nya + nyb;

 
  % copy interior points:
  tmp = zeros(nxp,nyp);
  tmp(nxa+1:nxa+nx,nya+1:nya+ny)=x;  x = zeros(nxp,nyp); x=tmp;
  tmp(nxa+1:nxa+nx,nya+1:nya+ny)=y;  y = zeros(nxp,nyp); y=tmp;
  tmp(nxa+1:nxa+nx,nya+1:nya+ny)=z;  z = zeros(nxp,nyp); z=tmp;


  % -- set buffer zones equal to values from the last real points
  for j=nya+1:nyp-nyb
    if( nxa>0 )
      dx =x(nxa+2,j)-x(nxa+1,j); 
      for i=1:nxa
        x(i,j)=x(nxa+1,j)-dx*(nxa-i+1);  
      end;
      y(1:nxa,j)=y(nxa+1,j); 
      z(1:nxa,j)=z(nxa+1,j);
    end;
    if( nxb>0 )
      dx=x(nxa+nx,j)-x(nxa+nx-1,j);
      for i=1:nxb
        x(nxa+nx+i,j)=x(nxa+nx,j)+dx*i;
      end; 
      y(nxa+nx+1:nxp,j)=y(nxa+nx,j); 
      z(nxa+nx+1:nxp,j)=z(nxa+nx,j); 
    end;
  end;
  %   NOTE: points in corners get set here (loop over i includes buffer cells)
  for i=1:nxp
    if( nya>0 )
      x(i,1:nya)=x(i,nya+1);  
      dy = y(i,nya+2)-y(i,nya+1);
      for j=1:nya
        y(i,j)=y(i,nya+1)-dy*(nya-j+1);  
      end;
      z(i,1:nya)=z(i,nya+1); 
    end;
    if( nyb>0 )
      x(i,nya+ny+1:nyp)=x(i,nya+ny); 
      dy = y(i,nya+ny)-y(i,nya+ny-1);
      for j=1:nyb
        y(i,nya+ny+j)=y(i,nya+ny)+dy*j; 
      end;
      z(i,nya+ny+1:nyp)=z(i,nya+ny); 
    end;
  end;

  nx=nxp;
  ny=nyp;

  if( 1==0 )
    figure(3);
    surf(x,y,z);
    title(sprintf('Surface after adding buffer zones : %s',name));
    axis equal;  % equal aspect ratio
    xlabel('x (long)')
    ylabel('y (lat)');
    zlabel('z');
    pause
  end;

end;


% -------------------------------------------------------------------------------------------------
% ---------------------------------- 3D surface smoothing  ----------------------------------------
if( smoothSurface==1  )

  omega=.5; 
  
  % We need to smooth more near the boundary so the surface becomes flat there.
  % Define a smooth function that is 1 in the interior and zero near the boundary
  
  phi1 = zeros(nx);
  phi2 = zeros(ny);
  iShift=10; % half width of transition zone -- tanh(5)=.9999
  beta=5./iShift; 

  % We always smooth at least iShift points near the boundary. 
  % We always smooth all of the buffer points
  mxa=max(0,nxa-iShift); mxb=max(0,nxb-iShift);
  mya=max(0,nya-iShift); myb=max(0,nyb-iShift);
  fprintf('smooth: iShift=%d, mxa=%d, mxb=%d, mya=%d, myb=%d\n',iShift,mxa,mxb,mya,myb);

  for i=1:nx
    phi1(i) = .25*(tanh(beta*(i-1-iShift-mxa))+1.)*(tanh(beta*(nx-i-iShift-mxb))+1.);
  end;
  for i=1:ny
    phi2(i) = .25*(tanh(beta*(i-1-iShift-mya))+1.)*(tanh(beta*(ny-i-iShift-myb))+1.);
  end;
  
  if 1==0
    x1=x(1:nx,1);
    plot(x1,phi1,'b-+'); 
    title('phi1');
    %pause;
    x2=y(1,1:ny)';
    plot(x2,phi2,'b-+'); 
    title('phi2');
    %pause;
  end;
  
  z1=z;
  z2=z;
  
  if smoothSubPatch == 1 
    fprintf('First smooth the sub patch (%i,%i)(%i,%i) [%e,%e][%e,%e],  %i times...\n',j1a,j1b,j2a,j2b,...
                  x(j2a),x(j2b),y(j1a),y(j1b),nSubPatchSmooth);
    for n=1:nSubPatchSmooth
    
      for i2=j2a:j2b
      for i1=j1a:j1b
        % fourth order filter: 
        if 0==1 
          z2(i1,i2)= z1(i1,i2) + (omega/12.)*(-z1(i1-2,i2) + 4.*z1(i1-1,i2) -6.*z1(i1,i2) + 4.*z1(i1+1,i2) -z1(i1+2,i2) ...
                                              -z1(i1,i2-2) + 4.*z1(i1,i2-1) -6.*z1(i1,i2) + 4.*z1(i1,i2+1) -z1(i1,i2+2) );
        % second order filter
        else
          z2(i1,i2)= z1(i1,i2) + .25*omega*(z1(i1-1,i2) -4.*z1(i1,i2) + z1(i1+1,i2) ...
                                          + z1(i1,i2-1)               + z1(i1,i2+1) );
        end; 
      end;
      end;
      z1=z2;
    end; % end for n=1:nSmooth
  end;
  
  
  fprintf('Smooth the 3d surface (nSmooth=%d, nBoundarySmooth=%d)...\n',nSmooth,nBoundarySmooth);
  
  for n=1:nSmooth
  
    % fourth order filter: (plus 2nd order near boundary)
   for m=1:nBoundarySmooth
    for i2=3:ny-2
    for i1=3:nx-2
      if m==1 
        z2(i1,i2)= z1(i1,i2) + (omega/12.)*(-z1(i1-2,i2) + 4.*z1(i1-1,i2) -6.*z1(i1,i2) + 4.*z1(i1+1,i2) -z1(i1+2,i2) ...
                                            -z1(i1,i2-2) + 4.*z1(i1,i2-1) -6.*z1(i1,i2) + 4.*z1(i1,i2+1) -z1(i1,i2+2) );
      end; 
      % smooth the boundary ( phi1 and phi2 are 1 in the interior and 0 near the boundary)
      z2(i1,i2)= z2(i1,i2) + .25*omega*(1.-phi1(i1))*( z2(i1-1,i2) +z2(i1,i2-1) -4.*z2(i1,i2) + z2(i1+1,i2) +z2(i1,i2+1) );
      z2(i1,i2)= z2(i1,i2) + .25*omega*(1.-phi2(i2))*( z2(i1-1,i2) +z2(i1,i2-1) -4.*z2(i1,i2) + z2(i1+1,i2) +z2(i1,i2+1) );
    end;
    end;
    % end conditions: 
    for i2=1:ny
      z2(1,i2)=z2(3,i2);
      z2(2,i2)=z2(3,i2);
      z2(nx-1,i2)=z2(nx-2,i2);
      z2(nx,i2)  =z2(nx-2,i2);
    end;
    for i1=1:nx
      z2(i1,1)=z2(i1,3);
      z2(i1,2)=z2(i1,3);
      z2(i1,ny-1)=z2(i1,ny-2);
      z2(i1,ny)  =z2(i1,ny-2);
    end;
   end; % nBoundarySmooth
   z1=z2;
  end;

end; % end if( smoothSurface==1 )

% -------------------------------------End surface smoothing----------------------------------------------------------------------------------
% -------------------------------------------------------------------------------------------------




% ----------------------------------------------------------------------------
% ----------------- Extract a cross section at a fixed y value -----------------
if( yCrossSection >= 0 )

  %iy = round(nrows/2);  % y index of the cross section
  % iy = round( size(y,2)/2 ); 

  iy = round( 1. + (ny-1)*yCrossSection/(yMaximum-yMinimum) );
  fprintf(' yCrossSection=%e, Choosing grid line iy=%d, y=%e (yPrev=%e,yNext=%e)\n',yCrossSection,iy,y(1,iy),y(1,max(1,iy-1)),y(1,min(ny,iy+1)));

  name2d=sprintf('%sy%0d',name,iy );
  
  x2d = x(1:nx,iy);
  y2d = y(1:nx,iy);
  z2d = z2(1:nx,iy);  % Note: z2 is the smoothed surface
  
  %  z2d = z2d - z(1,iy);  % offset to z=0 at bottom ?
  
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  %%% hold on;
  %%% plot3(x2d,y2d,z2d,'r','LineWidth',4); 
  %%% title(sprintf('Site300 surface : %s (cross section at iy=%d y=%e)',name,iy,y(1,iy)));
  %%% 
  %%% %zoom out; % reset zoom 
  %%% %zoom(1.5); 
  %%% %view(-38,22); % azimuth , elevation
  %%% axis equal;  % equal aspect ratio
  %%% % colorbar;
  %%% plotName = sprintf('%s.eps',name);
  %%% fprintf('saving plot: %s\n',plotName);
  %%% print('-depsc2',plotName);
  %%% 
  %%% %pause
  %%% hold off;
  
  if( plotOption > 0  )
    figure(3);
    plot(x2d,z2d,'r','LineWidth',2); 
    title(sprintf('file=%s: %s cross section at iy=%d y=%e',file,name,iy,y(1,iy)));
    axis equal;  % equal aspect ratio
    xlabel('x (long)')    
    % pause
  end;
  
end;
% ----------------------------------------------------------------------------
% ----------------------------------------------------------------------------

  
% ----------------------------------------------------------------------------
% ----------------- Extract a cross section at a fixed x value -----------------
if( xCrossSection >= 0 )

  ix = round( 1. + (nx-1)*xCrossSection/(xMaximum-xMinimum) );
  fprintf(' xCrossSection=%e, Choosing grid line ix=%d, x=%e (xPrev=%e,xNext=%e)\n',xCrossSection,ix,x(ix,1),x(max(1,ix-1),1),x(min(nx,ix+1),1));

  name2dx=sprintf('%sx%0d',name,ix);
  
  x2dx(1:ny) = x(ix,1:ny);
  y2dx(1:ny) = y(ix,1:ny);
  z2dx(1:ny) = z2(ix,1:ny); % Note: z2 is the smoothed surface
  
  %  z2d = z2d - z(1,iy);  % offset to z=0 at bottom ?
  
  if( plotOption > 0  )
    figure(4);
    plot(y2dx,z2dx,'r','LineWidth',2); 
    title(sprintf('file=%s: %s cross section at ix=%d x=%e',file,name,ix,x(ix,1)));
    axis equal;  % equal aspect ratio
    xlabel('y (lat)')    
    % pause
  end;

end; 

% ----------------------------------------------------------------------------
% ----------------------------------------------------------------------------

  
  
%%%%%%%%%%%%%%%%%%%%%%%%%%
if( plotOption >0 )
  figure(1);
  surf(x,y,z);
  title(sprintf('Original Surface : %s',name));
  view(-4,32); % azimuth , elevation
  axis equal;  % equal aspect ratio
  xlabel('x (long)')
  ylabel('y (lat)');
  zlabel('z');

  if( yCrossSection >= 0 )
    hold on;
    plot3(x2d,y2d,z2d,'r','LineWidth',4); 
    hold off;
  end;

  if( xCrossSection >= 0 )
    hold on;
    plot3(x2dx,y2dx,z2dx,'r','LineWidth',4); 
    hold off;
  end;

  %xlim([0 m*1e3])
  %ylim([0 n*1e3])

  if( plotOption >1 )
    plotName = sprintf('%sOriginal.eps',name);
    fprintf('saving plot: %s\n',plotName);
    print('-depsc2',plotName);
  end;

  if( smoothSurface==1 )
    figure(2);
    z=z2; 
    surf(x,y,z);
    title(sprintf('Site300 Smoothed Surface : %s',name));
    view(-4,32); % azimuth , elevation
    axis equal;  % equal aspect ratio
    xlabel('x (long)')
    ylabel('y (lat)');
    zlabel('z');

    if( yCrossSection >= 0 )
      hold on;
      plot3(x2d,y2d,z2d,'r','LineWidth',4); 
      hold off;
    end;

    if( xCrossSection >= 0 )
      hold on;
      plot3(x2dx,y2dx,z2dx,'r','LineWidth',4); 
      hold off;
    end;

    if( plotOption >1 )
      plotName = sprintf('%sSmoothed.eps',name);
      fprintf('saving plot: %s\n',plotName);
      print('-depsc2',plotName);
    end;

    % pause; 
  end;
end;


% surf(x,y,z);
% title(sprintf('Site300 surface : %s',name));
% pause;
%%%%%%%%%%%%%%%%%%%%%%%%



if( yCrossSection >= 0 )
  % ----------------------------------------------------------------------------
  % --- save the 2D profile in a file that can be read into a NurbsMapping ---
  
  outputName = sprintf('%s.dat',name2d);
  fprintf('Saving file %s with the 2d profile (for creating a nurbs in terrainGrid2d.cmd)\n',outputName);
  fout = fopen(outputName,'w');
  if fout < 0
     error(['Could not open ',outputName,' for output']);
  end
  
  % -------------------- optionally scale data ----------------------------
  xScale=1.;
  yScale=xScale;
  xMin=min(min(x2d));  % min of a matrix is a vector of min's of each column so take min twice
  xMax=max(max(x2d));
  yMin=min(min(z2d));  % min of a matrix is a vector of min's of each column so take min twice
  yMax=max(max(z2d));
  fprintf(fout,'# File created by cg/ins/runs/readTerrain.m (matlab script)\n');
  fprintf(fout,'# This is a 2d cross section at iy=%d, y=%e\n',iy,y(1,iy));
  fprintf(fout,'# This file is included in the ogen script terrainGrid2d.cmd.\n');
  fprintf(fout,'# You should define the variable $degree to be the degree of the NURBS\n');
  fprintf(fout,'# xScale=%e yScale=%e\n',xScale,yScale);
  fprintf(fout,'$xMin=%e; $xMax=%e; $yMin=%e; $yMax=%e; # bounds\n',xMin*xScale,xMax*xScale,yMin*yScale,yMax*yScale);
  fprintf(fout,'%d $degree\n',nx); % leave $degree as a variable
  for i1=1:nx
    fprintf(fout,'%e %e\n',x2d(i1)*xScale,z2d(i1)*yScale);  
  end;
  
  fclose(fout);

end;

if( xCrossSection >= 0 )
  % ----------------------------------------------------------------------------
  % --- save the 2D profile in a file that can be read into a NurbsMapping ---
  
  outputName = sprintf('%s.dat',name2dx);
  fprintf('Saving file %s with the 2d profile (for creating a nurbs in terrainGrid2d.cmd)\n',outputName);
  fout = fopen(outputName,'w');
  if fout < 0
     error(['Could not open ',outputName,' for output']);
  end
  
  % -------------------- optionally scale data ----------------------------
  xScale=1.;
  yScale=xScale;
  xMin=min(min(y2dx));  % min of a matrix is a vector of min's of each column so take min twice
  xMax=max(max(y2dx));
  yMin=min(min(z2dx));  % min of a matrix is a vector of min's of each column so take min twice
  yMax=max(max(z2dx));
  fprintf(fout,'# File created by cg/ins/runs/readTerrain.m (matlab script)\n');
  fprintf(fout,'# This is a 2d cross section at ix=%d, x=%e\n',ix,x(ix,1));
  fprintf(fout,'# This file is included in the ogen script terrainGrid2d.cmd.\n');
  fprintf(fout,'# You should define the variable $degree to be the degree of the NURBS\n');
  fprintf(fout,'# xScale=%e yScale=%e\n',xScale,yScale);
  fprintf(fout,'$xMin=%e; $xMax=%e; $yMin=%e; $yMax=%e; # bounds\n',xMin*xScale,xMax*xScale,yMin*yScale,yMax*yScale);
  fprintf(fout,'%d $degree\n',nx); % leave $degree as a variable
  for i1=1:ny
    fprintf(fout,'%e %e\n',y2dx(i1)*xScale,z2d(i1)*yScale);  
  end;
  
  fclose(fout);

end;


% --------------------------------------------------------------------------
% --- save the 3D surface in a file that can be read into a NurbsMapping ---

outputName = sprintf('%s.dat',name);
fprintf('Saving file %s with the 3d profile (for creating a nurbs in site300Grid.cmd)\n',outputName);
fout = fopen(outputName,'w');
if fout < 0
   error(['Could not open ',outputName,' for output']);
end

% --- optionally scale data ---
xScale=1.; yScale=xScale; zScale=xScale; 
xMin=min(min(x));  % min of a matrix is a vector of min's of each column so take min twice
xMax=max(max(x));
yMin=min(min(y)); 
yMax=max(max(y));
zMin=min(min(z)); 
zMax=max(max(z));
fprintf(fout,'# File created by cg/ins/runs/readTerrain.m (matlab script)\n');
fprintf(fout,'# This is the 3D surface\n');
fprintf(fout,'# This file is included in the ogen script terrainGrid.cmd.\n');
fprintf(fout,'# You should define the variable $degree to be the degree of the NURBS\n');
fprintf(fout,'# xScale=%e yScale=%e zScale=%e\n',xScale,yScale,zScale);
fprintf(fout,'$xMin=%e; $xMax=%e; $yMin=%e; $yMax=%e; $zMin=%e; $zMax=%e;# bounds\n',xMin*xScale,xMax*xScale,yMin*yScale,yMax*yScale,zMin*zScale,zMax*zScale);
fprintf(fout,'%d %d $degree\n',nx,ny); % leave $degree as a variable
for i2=1:ny
for i1=1:nx
  fprintf(fout,'%e %e %e\n',x(i1,i2)*xScale,y(i1,i2)*yScale,z(i1,i2)*zScale);  
end;
end;

fclose(fout);


return

