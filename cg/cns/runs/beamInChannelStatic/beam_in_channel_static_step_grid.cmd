#
# Create the initial grid for a flexible beam in a channel
# Use this grid with cgmp for a fluid-structure example.
#
# Usage:
#         ogen [noplot] beam_in_channel_static_step [options]
# where options are
#     -factor=<num>          : grid spacing factor
#     -interp=[e/i]          : implicit or explicit interpolation
#     -name=<string>         : over-ride the default name
#     -beamThickness=<float> : beam thickness
#     -sharpness=<num>       : sharpness parameter
#     -channelLength=<float> : length of the channel
#     -channelAspect=<float> : channel aspect ratio
#     -beamX=<float>         : location of the beam, from the start of the channel
#
# Examples:
#
#      ogen noplot beam_in_channel_static_step -interp=e -factor=2
#      ogen noplot beam_in_channel_static_step -interp=e -factor=4
#      ogen noplot beam_in_channel_static_step -interp=e -factor=8
#      ogen noplot beam_in_channel_static_step -interp=e -factor=16 -beamThickness=0.001
#
#
$factor=1; $name="";
$interp="i"; $interpType = "implicit for all grids";
$order=2; $orderOfAccuracy = "second order"; $ng=2;
$nExtra=0;
$sharpness=40;
$sharpness4=$sharpness/2;
$sharp=40;
$channelLength=0.32;
$step_height=0.015;
$channelAspectRatio=4.0;
$beamX=0.05;
$beamThickness=0.005;
$beamLength=0.04;
#
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,\
           "nExtra=i"=>\$nExtra,"width=f"=> \$width, "beamThickness=f"=> \$beamThickness, \
           "sharpness=f"=> \$sharpness, "channelLength=f"=> \$channelLength, \
           "channelAspect=f"=> \$channelAspectRatio, "beamX"=>\$beamX);
#
$channelHeight=$channelLength/$channelAspectRatio;
$ds0=0.1/32.0;
$ds=$ds0/$factor;
$nl=int((($beamThickness+0.01)+($beamLength*2+0.02))/$ds);
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $name eq "" ){$name = "beam_in_channel_static_step" . "$interp$factor" . ".hdf";}
create mappings 
  rectangle 
    $nx=int( ($channelLength/3.0000000-0.037)/$ds+1.5 );
*    $nx=int( $channelLength/$ds+1.5 );
    $ny=int( ($channelHeight-$stepHeight)/$ds+1.5 );
    $ya=0.0;#-$step_height;
    $yb=-$step_height+$channelHeight;
    $xb=$channelLength/2.99;
*    $xb=$channelLength;
    set corners 
    0.037,$xb,$ya,$yb
    lines 
    $nx,$ny 
*    share 
*    0,0,2,0 
*    boundary conditions 
*    3,0,2,2 
    share 
    0,0,5,0 
    boundary conditions 
    0,0,2,2 
    exit 
  exit this menu 
create mappings 
  rectangle 
    $nx=int( $channelLength/$ds/3.0000000/2.0+1.5 );
    $ny=int( ($channelHeight-$stepHeight)/$ds/2.0+1.5 );
    $ya=0.0;#-$step_height;
    $yb=-$step_height+$channelHeight;
    $xa=$channelLength/3.01;
    $xb=$xa+$channelLength/2.99;
    set corners 
    $xa,$xb,$ya,$yb
    lines 
    $nx,$ny 
    share 
    0,0,5,0 
    boundary conditions 
    0,0,2,2 
    mappingName
    rectangle2
    exit 
  exit this menu 
create mappings 
  rectangle 
    $nx=int( $channelLength/$ds/3.0000000/4.0+1.5 );
    $ny=int( ($channelHeight-$stepHeight)/$ds/4.0+1.5 );
    $ya=0.0;#-$step_height;
    $yb=-$step_height+$channelHeight;
    $xa=2.0*$channelLength/3.01;
    $xb=$channelLength;
    set corners 
    $xa,$xb,$ya,$yb
    lines 
    $nx,$ny 
    share 
    0,4,5,0 
    boundary conditions 
    0,4,2,2 
    mappingName
    rectangle3
    exit 
  exit this menu 
create mappings 
  rectangle 
    $nx=int( (0.039)/$ds+1.5 );
    $ny=int( ($channelHeight)/$ds+1.5 );
    $ya=-$step_height;
    $yb=-$step_height+$channelHeight;
    $xb=0.039;
    $xa=0.0;
    set corners 
    0.0,$xb,$ya,$yb
    lines 
    $nx,$ny 
    share 
    0,0,2,0 
    boundary conditions 
    3,0,2,2 
    mappingName
    rectangle4
    exit 
  exit this menu 
create mappings 
  smoothedPolygon  
    $halfT=$beamThickness*0.5;
    $beamXa=$beamX-$halfT;
    $beamXb=$beamX+$halfT;  
    $nx=$nl;
    $ny=int(7);
    $ndist=($ny-1)*$ds;
    vertices 
    4 
    $beamXa,0 
    $beamXa,$beamLength
    $beamXb,$beamLength 
    $beamXb,0 
    curve or area (toggle)
    lines 
    $nx
    sharpness
      $sharpness
      $sharpness
      $sharpness
      $sharpness
    t-stretch
      0,50
      0.1,10
      0.1,10
      0,50
*    n-stretch
*      1 5 0
*    boundary conditions 
*    2,2,1,0 
*    share 
*    5,5,1,0 
    exit 
  hyperbolic
    $stretchFactor=1.25;
    backward
    distance to march $ndist
    lines to march $ny
    points on initial curve $nx
    generate 
    mapping parameters
    Boundary Condition: left 2
    Boundary Condition: right 2
    Boundary Condition: bottom 1
    Boundary Condition: top 0
    Share Value: left 5
    Share Value: right 5
    Share Value: bottom 1
    Share Value: top 0
    close mapping dialog
    boundary condition options...
    BC: left fix y, float x and z
    BC: right fix y, float x and z
    generate 
    name hyp-smoothedPolygon
    exit
create mappings 
  smoothedPolygon
    $xa=$beamX-0.015;
    $stepnl=($step_height+0.005)/$ds;
    $nx=$stepnl;
    $ny=int(7);
    $ndist=($ny-1)*$ds*0.6;
    $ya=-$step_height;
    $xb=$xa+0.005;
    vertices 
    3 
    $xa,$ya
    $xa,0
    $xb,0
    lines 
    $nx,$ny
    n-dist 
    fixed normal distance 
    $ndist
    sharpness
      $sharpness4
      $sharpness4
      $sharpness4
    t-stretch
      0,50
      0.1,10
      0.1,10
    n-stretch
      1 5 0
    boundary conditions 
    2,0,2,0 
    share 
    2,0,5,0 
    mappingName
    stepPolygon
    exit 
  exit this menu 
generate an overlapping grid
  square
  rectangle2
  rectangle3
  rectangle4
  hyp-smoothedPolygon
  stepPolygon
  done
  change parameters
    specify a domain
      fluidDomain
      square
      hyp-smoothedPolygon
      stepPolygon
      rectangle2
      rectangle3
      rectangle4
      done
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all 
      $ng $ng $ng $ng $ng $ng
  exit
  compute overlap
  exit
save an overlapping grid
$name
beam_in_channel_static_step
exit
