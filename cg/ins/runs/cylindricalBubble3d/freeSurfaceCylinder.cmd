#################################
# variables
#################################
# constants and inlines
$pi=4.*atan2(1.,1.);
$ml=0; 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
$factor=1;
$orderOfAccuracy = "second order";
# number of ghost points
$ng=2; 
# backGround grid dimensions
$xa=-1.1; $xb=1.1; $ya=-1.1; $yb=1.1; $za=-1.0; $zb=0.0;
# target grid spacing:
$ds0 = .1; 
# order of nurbs surface
$nurbsOrder=3;
$freeSurfaceShare=100; 
# 
$radx=1.0; $rady=1.0;
# 
$cx=0.0; $cy=0.0;
#
$interp="explicit";
#################################
# get command line arguments
#################################
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=>\$factor,"interp=s"=> \$interp,"case=s"=>\$case,\
           "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"nExtra=i"=>\$nExtra,"factor2=f"=>\$factor2,\
           "amp=f"=>\$amp,"freq=f"=>\$freq,"ml=i"=>\$ml,"prefix=s"=> \$prefix,\
           "radx=f"=>\$radx,"rady=f"=>\$rady );
#################################
# calculations
#################################
$ds = $ds0/$factor;
#################################
# generate points on a cylinder
#################################
$terrainFactor=1.2;
$nr1 = intmg( ($zb-$za)/$ds );
$nr2 = intmg( $terrainFactor*(2*$pi)/$ds + 1.5 );
$commands="";
for( $i=0; $i<$nr1; $i++ ) { \
  $z=$za+($zb-$za)*$i/($nr1-1); \
  for( $j=0; $j<$nr2; $j++ ) { \
    $theta=2.*$pi*$j/($nr2-1); \
    $x=$radx*cos($theta)+$cx; \
    $y=$rady*sin($theta)+$cy; \
    $commands = $commands . "$x $y $z \n"; \
  } \
}
#################################
# start ogen commands
#################################
create mappings
  3D Mappings...
  #################################
  box
    $nx=intmg( ($xb-$xa)/$ds+1.5 ); 
    $ny=intmg( ($yb-$ya)/$ds+1.5 ); 
    $nz=intmg( ($zb-$za)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx $ny $nz
    boundary conditions
      0 0 0 0 -1 -1
    share
      0 0 0 0 3 4
    mappingName 
    backGround
    exit
  3D Mappings...
  nurbs (surface)
    enter points
      $nr2 $nr1 $nurbsOrder
      $commands
    boundary conditions
      -1 -1 1 2 0 0
    mappingName
      interfaceCurve
    exit
  close 3D Mappings
  #################################
  builder
    add surface grid
      interfaceCurve
    create volume grid...
      backward
      boundary condition options...
        # march along normals 1
        BC: bottom fix z, float x and y
        BC: top fix z, float x and y
	apply boundary conditions to start curve 1
        close marching options
      #points on initial curve $nz, $nt	
      $linesToMarch = intmg( 9 );
      lines to march $linesToMarch
      $fudge = 0.5/($linesToMarch*$ds0);
      $dist = $fudge*$linesToMarch*$ds; 
      distance to march $dist
      generate
      mappingName freeSurface
      boundary conditions 
        -1 -1 -1 -1 5 0
      share
        0 0 3 4 $freeSurfaceShare 0
      exit
    exit
  exit
#################################
generate an overlapping grid
  backGround
  freeSurface
  done choosing mappings
  change parameters
    order of accuracy
    second order
    interpolation type
      $interp for all grids
    ghost points
      all
      2 2 2 2 2 2
    exit
  compute overlap
  exit
save an overlapping grid
$prefix = "FreeSurfaceCylinder";
if( $interp eq "explicit") {$interp="e";} else {$interp="i";}
$suffix = ".order$order"; 
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
$name
freeSurface
exit
