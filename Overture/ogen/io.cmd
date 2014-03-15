***************************************************************************
*
*  Builds grids for regions exterior and interior to a boundary
*
* *  Usage:
*    ogen noplot io -factor=[1|2...] -interp=[e,i] -order=[2,4,6,8] -rgd=[fixed|var]
* 
*  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance* 
* Examples:
*   ogen noplot io -factor=2 -order=2 -interp=e
*   ogen noplot io -factor=4 -order=2 -interp=e
* 
*   ogen noplot io -order=4 -interp=e -factor=2
*   ogen noplot io -order=4 -interp=e -factor=4
*   ogen noplot io -order=4 -interp=e -factor=8
*   ogen noplot io -order=4 -interp=e -factor=16
*   ogen noplot io -order=4 -interp=e -factor=32
*
*   ogen noplot io -interp=e -deltaRadius0=.15 -rgd=fixed -factor=1 
*   ogen noplot io -interp=e -deltaRadius0=.15 -rgd=fixed -factor=2 
*   ogen noplot io -interp=e -deltaRadius0=.15 -rgd=fixed -factor=4 
*   ogen noplot io -interp=e -deltaRadius0=.15 -rgd=fixed -factor=8 
* 
*   ogen noplot io -order=4 -interp=e -rgd=fixed -deltaRadius0=.15 -factor=2 
*   ogen noplot io -order=4 -interp=e -rgd=fixed -deltaRadius0=.15 -factor=4 
*   ogen noplot io -order=4 -interp=e -rgd=fixed -deltaRadius0=.15 -factor=8 
* 
***************************************************************************
$order=2; $factor=1; $interp="i"; $name=""; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$rgd="var";   
$deltaRadius0=-1.;  # If set >0 then use this value instead of the default
GetOptions( "order=i"=>\$order,"factor=f"=>\$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "angle=f"=> \$pipeAngle,"startAngle=f"=> \$pipeAngleStart,"interp=s"=> \$interp,\
            "option=s"=> \$option,"rgd=s"=> \$rgd,"name=s"=> \$name,"deltaRadius0=f"=> \$deltaRadius0);
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
$prefix = "innerOuter"; 
if( $option ne "" ){ $prefix = $option . "CurvedPipe"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $name eq "" ){$name = "$prefix" . "$interp$factor" . $suffix . ".hdf";}
* 
if( $deltaRadius0 < 0 ){ $deltaRadius0=.175 +($order-2)*.1; }
$pi=4.*atan2(1.,1.);
*
*
* domain parameters:  
$ds = .05/$factor; # target grid spacing
*
*
$bcInterface=100;  # bc for interfaces
$ishare=100;
* 
create mappings 
*
  annulus 
    mappingName 
      innerAnnulus 
    boundary conditions 
      -1 -1 0 $bcInterface 
    share
     * material interfaces are marked by share>=100
      0 0 0 $ishare
    if( $rgd eq "fixed" ){ $deltaRadius=$deltaRadius0; }else{$deltaRadius=$deltaRadius0/$factor;}
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    $rad2=$outerRadius+.05; 
    inner and outer radii 
      $innerRadius $outerRadius
*       $innerRadius $rad2
    lines 
      $nx=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nTheta=$nx;
      $nx $ny 
    exit 
*
  rectangle 
    mappingName
      innerSquare
    $xa=-$innerRadius-$ds;  $xb=$innerRadius+$ds; 
    $ya=-$innerRadius-$ds;  $yb=$innerRadius+$ds; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      0 0 0 0
    exit 
*
  annulus 
    mappingName 
      outerAnnulus 
    $innerRadius=$outerRadius; 
    $outerRadius=$innerRadius+$deltaRadius;
    inner and outer radii 
      $innerRadius $outerRadius
    lines 
      $nx=$nTheta; 
      $ny=int( ($deltaRadius)/$ds+2.5 );
      $nx $ny
    boundary conditions 
      -1 -1 $bcInterface 0 
    share
     * material interfaces are marked by share>=100
      0 0 $ishare 0   
    exit 
*
  rectangle 
    mappingName
      outerSquare
    $xa=-1.;  $xb=1.0; 
    $ya=-1.;  $yb=1.0; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    exit 
  exit this menu 
*
generate an overlapping grid 
  outerSquare
  outerAnnulus 
  innerSquare
  innerAnnulus 
  done 
*
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      outerDomain 
      * grids in the domain:
      outerSquare
      outerAnnulus
      done
    specify a domain
      * domain name:
      innerDomain 
      * grids in the domain:
      innerSquare
      innerAnnulus
      done
    ghost points 
      all 
      2 2 2 2 
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    exit 
*    display intermediate results
* pause
    compute overlap
* pause
  exit
save a grid
$name
innerOuter
exit
