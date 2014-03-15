*# This ogen script generates a multidomain grid for 
*# the conjugate heat transfer problem of a flow bounded by
*# a conducting wall and an isothermal wall.  The geometry is
*# given by:
*#
*#
*# +================top_isoT_bc===========+ y = H
*# 
*# x-periodic   Fluid Domain                x-periodic
*#
*# +----------------interface_bc----------+ y = \alpha H
*#
*# x-periodic   Solid Domain                x-periodic
*#
*# +================bot_isoT_bc===========+ y = 0
*# x = 0                                    x = L
*#
*# kkc 080603: Initial version 
*# wdh 080710: 3d version from kkc's 2d version
*# 
*# set some global parameters to be treated as constants
$top_isoT_bc  = 1;
$interface_bc = 2;
$bot_isoT_bc  = 3;
$default_Ny   = 21; # total number of points spanning both domains with constant dy
if( $flatPlateIsPeriodic eq "" ){ $flatPlateIsPeriodic=1; } # be default the flatPlate is periodic in the x-direction
*#
*# get command line options
GetOptions("alpha=f"=>\$alpha, "H=f"=>\$H, "L=f"=>\$L, "factor=i"=>\$factor, "name=s"=>\$name, "accuracy=i"=>\$accuracy);
if ( !$alpha ) {$alpha = .5;};
if ( !$H ) { $H = 1.; };
if ( !$L ) { $L = 3.; };
if ( !$Lz ) { $Lz = .5; };
if ( !$factor ) { $factor=1;};
if ( !$name ) { $name = "flatPlates"; };
if ( !$accuracy ) { $accuracy = 2; };
*#
$orderOfAccuracy = "second order";
if    ( $accuracy==2 ) \
    { $orderOfAccuracy = "second order"; } \
elsif ( $accuracy==4 ) \
    { $orderOfAccuracy="fourth order"; } \
elsif ( $accuracy==6 ) \
    { $orderOfAccuracy="sixth order"; } \
else \
    { die "UNKNOWN ORDER OF ACCURACY = $accuracy\n"; };
use BoxMapping;
$fluid = BoxMapping->new(); $$fluid{MAPPINGNAME} = "fluid";
$solid = BoxMapping->new(); $$solid{MAPPINGNAME} = "solid";
$dy = $H/(($default_Ny-1.)*$factor);
$dx = $dy; $dz=$dy; 
$Nx = int($L/$dx + 1); # number of points in the x direction
$Ny = ($default_Ny-1)*$factor/2 + 1; # number of points in y for each domain
$Nz = int($Lz/$dx + 1); # number of points in the z direction
$$fluid{X_MIN} = $$solid{X_MIN} = 0;
$$fluid{X_MAX} = $$solid{X_MAX} = $L;
$$solid{Y_MIN} = 0;
$$solid{Y_MAX} = $$fluid{Y_MIN} = $alpha*$H;
$$fluid{Y_MAX} = $H;
$$fluid{Z_MIN} = 0; $$fluid{Z_MAX} = $lz;
$$solid{Z_MIN} = 0; $$solid{Z_MAX} = $lz;
if( $flatPlateIsPeriodic ){ $$solid{BC} = "-1 -1 $bot_isoT_bc $interface_bc -1 -1"; }else{ $$solid{BC} = "4 5  $bot_isoT_bc $interface_bc 4 5 "; }
if( $flatPlateIsPeriodic ){ $$fluid{BC} = "-1 -1 $interface_bc $top_isoT_bc -1 -1 "; }else{ $$fluid{BC} = "4 5 $interface_bc $top_isoT_bc 4 5 "; }
* $$fluid{PERIODICITY} = $$solid{PERIODICITY} = "periodicity: axis 0 derivative periodic";
$$solid{LINES} = $$fluid{LINES} ="$Nx $Ny $Nz";
*# CREATE THE MAPPINGS
create mappings
$cmd = $fluid->cmdString().$solid->cmdString();
$cmd
exit this menu
*# GENERATE THE OVERLAPPING GRID
generate an overlapping grid
$$fluid{MAPPINGNAME}
$$solid{MAPPINGNAME}
change parameters
  specify a domain
    fluidDomain
    $$fluid{MAPPINGNAME}
  done
  specify a domain
    solidDomain
    $$solid{MAPPINGNAME}
  done
  order of accuracy
    $orderOfAccuracy
  ghost points
    all
     $ng=2; 
     $ng $ng $ng $ng $ng $ng 
  exit
compute overlap
plot
* pause
exit
exit
