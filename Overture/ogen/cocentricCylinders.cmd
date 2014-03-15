*
* Two co-centric 3D cylinders (A two-domain grid for cgmp)
*
*
* usage: ogen [noplot] cocentricCylinders -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name= -ra= -rb= -rc= -za= -zb= -zPeriodic=[0|1]
* 
* examples:
*     ogen noplot cocentricCylinders -zPeriodic=1 -factor=1 
*     ogen noplot cocentricCylinders -zPeriodic=1 -factor=2 
*     ogen noplot cocentricCylinders -zPeriodic=1 -factor=4 
*     ogen noplot cocentricCylinders -zPeriodic=1 -factor=8 
* 
* -- set default parameter values:
$ra=.5; $rb=1.; $rc=1.5; $za=0.; $zb=1.; $zPeriodic=0; 
$order=2; $factor=1; $interp="i"; $name="";
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
* 
* get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=>\$factor,"interp=s"=>\$interp,"ra=f"=>\$ra,"rb=f"=>\$rb,"rc=f"=>\$rc,"za=f"=>\$za,"zb=f"=>\$zb,"name=s"=>\$name,"zPeriodic=i"=>\$zPeriodic);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $zPeriodic == 1 ){ $suffix = "p$suffix"; }
if( $name eq "" ){ $name = "cocentricCylinders" . "$interp$factor" . $suffix . ".hdf"; }
* 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.); 
$bcInterface=100;  # bc for interfaces
$ishare=100;
*
*
* Make the inner cylinder 
*
create mappings
*
  Cylinder
    mappingName
      innerCylinder
    orientation
      2 0 1 
    bounds on the radial variable
      $ra $rb 
    bounds on the axial variable
      $za $zb
    lines
      $nt = int( 2.*$pi*$rb/$ds + 1.5 ); # use this value for both grids so lines match
      $nr = int( ($rb-$ra)/$ds + 2.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
      $nt $nz $nr 
    boundary conditions
      if( $zPeriodic == 0 ){ $bcCommand="-1 -1 3 4 1 $bcInterface"; }else{ $bcCommand="-1 -1 -1 -1 1 $bcInterface"; }
      $bcCommand
    share
      0 0 1 2 0 $ishare
  exit
*
* Make the outer cylinder 
* 
  Cylinder
    mappingName
      outerCylinder
    orientation
      2 0 1 
    bounds on the radial variable
      $rb $rc 
    bounds on the axial variable
      $za $zb
    lines
      $nr = int( ($rc-$rb)/$ds + 2.5 );
      $nt $nz $nr 
    boundary conditions
      if( $zPeriodic == 0 ){ $bcCommand="-1 -1 3 4 $bcInterface 1"; }else{ $bcCommand="-1 -1 -1 -1 $bcInterface 1"; }
      $bcCommand
    share
      0 0 1 2 $ishare 0 
  exit
exit
*
*
generate an overlapping grid
    innerCylinder
    outerCylinder
  done
  change parameters
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
        innerCylinder
      done
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
        outerCylinder
      done
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
  exit
*  display intermediate results
* pause
  compute overlap
  exit
*
save an overlapping grid
$name
cocentricCylinders
exit


