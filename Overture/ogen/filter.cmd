*
*  Create a grid for a "filter" -- a body with multiple legs ---
*
* $name = "filterWith4Legs.hdf"; $factor=1.; $numberOfLegs=4;
$name = "filterWith6Legs.hdf"; $factor=1.; $numberOfLegs=6;
* $name = "filterWith8Legs.hdf"; $factor=1.; $numberOfLegs=8;
* 
create mappings
*
* 
$ds=.1/$factor;     # target grid spacing
$radius=.25; # radius of the wire
$wireShare=5;    # shared boundary flag for the wire surface
$bodyShare=6; # shared boundary flag for the body
$nr=8;       # number of lines in the radial direction 
$rDist=($nr-1-4)*$ds;  # normal distance for grids  (allow extra points for stretching)
$pi=4.*atan2(1.,1.); 
* 
  annulus
    inner and outer radii
      $deltaRad=$rDist; $outerRad=$radius+$deltaRad; 
      $radius $outerRad
    lines
      $nTheta= int( 2.*$pi*($radius+$outerRad)*.5/$ds +1.5 );
      * $nTheta= int( 2.*$pi*$radius/$ds +1.5 );
      $nTheta $nr
    boundary conditions
      -1 -1 1 0
    mappingName
      wire-cross-section
    exit
*
* --- Here is the curve that defines center line of the wire ---
*       The end of the wire should straighten out to a fixed angle so a rounded cap can be more easily added 
* 
  nurbs (curve)
    set range dimension
    3
    enter points
     7 3
      .5 0. 0.
     1.2 -.1 0.
     1.5 -.2 0.
     2.2  -2. 0
     2.8  -3.8 0.
     2.9  -3.9
     3.0  -4. 0. 
* -- record the end location of the wire and the tangent angle ---
* 
    lines
      51
    mappingName
      sweep-curve
   exit
* 
  sweep
    choose reference mapping
    wire-cross-section
    choose sweep curve
    sweep-curve
    lines
      $arcLength = 5.5; # guess arclength of the sweep-curve 
      $nSweep = int( $arcLength/$ds +1.5 );
      $nTheta $nr $nSweep
    mappingName
      wire1
   exit
* The Join mapping expects the radial direction to be the last axis, so we need
* to change the ordering 
  reparameterize
    transform which mapping?
      wire1
    reorient domain coordinates
      0 2 1
    mappingName
     wire
  exit
*
* --  cross-section of the body --
* 
  smoothedPolygon
    vertices
    $bodyRad=1.;  # radius of body 
    7
   -.5 -1.
   -.7 -1.
   -1. -1.
   -1. 0.
   -1. 1.
   -.7 1.
   -.5 1.
* 
    n-dist
    fixed normal distance
     $rDistBody = $rDist;
     $rDistBody
    sharpness
      15 
      15
      15
      15
      15
      15
      15
    t-stretch
    0 50
    0 50
    .2 10
    0. 20
    .2 10.
    0. 20
    0. 20
    boundary conditions
    0 0 1 0
    mappingName
      body-cross-section
* use
    exit
* 
  body of revolution
    revolve which mapping?
      body-cross-section
    tangent of line to revolve about
      0 1 0
    choose a point on the line to revolve about
      0 0 0
    lines
      $nrBody=$nr;
      $nTheta= int( 2.*$pi*$bodyRad/$ds + 1.5 );
      $nAxial = int( $bodyRad*2.75/$ds + 1.5 );
      $nAxial $nrBody $nTheta
    share 
      0 0 $bodyShare 0 0 0
    mappingName
      body
    exit
* 
  join
    choose curves
    * wire (side=0,axis=1)
    wire 
    body (side=0,axis=1)
    compute join
    boundary conditions
      -1 -1  1 0 2 0
    boundary conditions
      -1 -1 1 0 2 0 
    share 
      0 0  $bodyShare 0  $wireShare 0
    mappingName
     wireFitToBody
    exit
*
include wireEndCap.cmd
*
  rotate/scale/shift
    transform which mapping?
      end-cap
    rotate
       $endAngle=-45.; 
      $endAngle 2
      0 0 0
    shift
      * $xEnd=3.+.2; $yEnd=-4.-.2; 
      * Shift the end-cap/end-join to overlap near the end of the wire
      * (the factor of 2 comes from the end "cylinder" being 2*$radius long )
      $xEnd=3.+2.*$radius*cos($endAngle*$pi/180.); $yEnd=-4.+2.*$radius*sin($endAngle*$pi/180.); 
      $xEnd $yEnd 0.
    mappingName
     wireEndCap
   exit
*
  rotate/scale/shift
    transform which mapping?
      end-join
    rotate
      $endAngle 2
      0 0 0
    shift
      $xEnd $yEnd 0.
    mappingName
     wireEndSection
   exit
*
* Here is box for the top of the body
*
Box
  $xa=-.5; $xb=-$xa; $ya=1.; $yb=1.+$rDist; $za=$xa; $zb=$xb;
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = $nr;
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    0 0 2 0 0 0
  share
    0 0 $bodyShare 0 0  0  
  mappingName
    topBoxForBody
  exit
*
* Here is box for the bottom of the body
* 
Box
  $xa=-.5; $xb=-$xa; $ya=-1.-$rDist; $yb=-1.; $za=$xa; $zb=$xb;
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = $nr;
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    0 0 0 2 0 0
  share
    0 0 0 $bodyShare 0 0  
  mappingName
    bottomBoxForBody
  exit
* 
*
* Here is the backGround
*
Box
  $xa=-4.; $xb=4.; $ya=-5.5; $yb=3.5; $za=-4.; $zb=4.;
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  mappingName
    backGround
  exit
*
* --- add stretching in the radial direction ---
*    
* Define a perl routine to stretch the grid
*     stretch(mappingName,newName,[r1,r2,r3],a,b,c);
*     $commands 
sub stretch\
{ local($mappingName,$newName,$rDir,$a,$b,$c)=@_; \
  $commands = "stretch coordinates\n" . "transform which mapping?\n" . "$mappingName\n" . \
              "Stretch $rDir:itanh\n" . "STP:stretch $rDir itanh: layer 0 $a $b $c (id>=0,weight,exponent,position)\n" .\
              "stretch grid\n" . "STRT:name $newName\n" . "exit\n"; \
}
stretch(wireFitToBody,"stretched-wireFitToBody",r3,1.,5.,0.);
$commands
stretch(wireEndSection,"stretched-wireEndSection",r3,1.,5.,0.);
$commands
stretch(wireEndCap,"stretched-wireEndCap",r3,1.,5.,0.);
$commands
stretch(topBoxForBody,"stretched-topBoxForBody",r2,1.,5.,0.);
$commands
stretch(bottomBoxForBody,"stretched-bottomBoxForBody",r2,1.,5.,1.);
$commands
stretch(body,"stretched-body",r2,1.,5.,0.);
$commands
*   stretch coordinates
*     transform which mapping?
*       wireFitToBody
*     Stretch r3:itanh
*     STP:stretch r3 itanh: layer 0 1 5 0 (id>=0,weight,exponent,position)
*     stretch grid
*     STRT:name stretched-wireFitToBody
*   exit
* 
*  -- convert to Nurbs for faster evaluation, allow a rotation about the y-axis ---
* 
*     convertToNurbs(oldMappingName,newMappingName,angle)
* 
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
convertToNurbs("stretched-body",body0,0.);
$commands
convertToNurbs("stretched-topBoxForBody",topBoxForBody0,0.);
$commands
convertToNurbs("stretched-bottomBoxForBody",bottomBoxForBody0,0.);
$commands
* 
*  --- now make a number of legs ----
* 
$legCommands=""; $legGrids=""; 
for( $i=0; $i<$numberOfLegs; $i++ ){ $angle= 360.*$i/$numberOfLegs; \
convertToNurbs("stretched-wireFitToBody","wireFitToBody$i","$angle"); $legCommands = $legCommands . $commands; \
convertToNurbs("stretched-wireEndSection","wireEndSection$i","$angle"); $legCommands = $legCommands . $commands; \
convertToNurbs("stretched-wireEndCap","wireEndCap$i","$angle"); $legCommands = $legCommands . $commands; \
$legGrids = $legGrids . "wireFitToBody$i\n wireEndSection$i\n wireEndCap$i\n"; }
* 
$legCommands
* 
  exit this menu
*
* ----- generate the overlapping grid ---------
*
generate an overlapping grid
   backGround
  bottomBoxForBody0
  topBoxForBody0
  body0
* -- here is the list of leg grids:
  $legGrids
*  wireEndSection
*  wireEndCap
  done choosing mappings
* 
  compute overlap
  * pause
exit
* save an overlapping grid
save a grid (compressed)
$name
filter
exit



open graphics
plot
  change the plot
     toggle grid 0 0
     plot block boundaries 1
  exit
  * display intermediate results

  

  change the plot
    toggle grid 0 0
  compute overlap



open graphics
  view mappings
    body
    wireFitToBody
    wireEndCap
    wireEndSection
    topBoxForBody
    bottomBoxForBody
