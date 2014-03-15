***************************************************************************
*
*  Grid for the "cartoon" figure of a generic multi-domain/multi-physics problem
*
*  Usage:
*    ogen noplot multiDomain -factor=<> -interp=[e,i] -order=[2,4,6,8] -match=[yes,no]
*
* ogen noplot multiDomain -factor=1 -match=yes
* ogen noplot multiDomain -factor=2 -match=yes
* ogen noplot multiDomain -factor=4 -match=yes
*
***************************************************************************
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$deltaRadius0=.175; $matchGridLines="no"; 
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"match=s"=> \$matchGridLines,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "multiDomain" . "$interp$factor" . $suffix . ".hdf";}
*
*
* domain parameters:  
$ds = .1/$factor; # target grid spacing
*
*
$bcInterface1=100;  # bc for interfaces are numbered starting from 100 
$bcInterface2=101;
$bcInterface3=102;
$bcInterface4=103;
$bcInterface5=104;
* 
create mappings 
*
    $pi=4.*atan2(1.,1.);
*
  rectangle 
    mappingName
      backGround
    $xa=-1.75;  $xb=2.25;
    $ya=-1.5;  $yb=1.5; 
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    exit 
*
   $xe=-.5; $ye=0.;   # ellipse centre
   $ae=.75;           # ellipse a/b 
*
  rectangle 
    mappingName
      ellipseInnerSquare
    $xa=$xe-.3;  $xb=$xe+.3;
    $ya=$ye-.3/$ae;  $yb=$ye+.3/$ae;
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
  $r1=.5; $r2=.7; $r3=1.; 
  $nThetaCommon = int( (2.*$pi*$r2)/$ds+1.5 ); # use this for matching number of grid lines 
  $eps=.0;   # .02 offset boundaries between domains for plotting
  annulus
    ellipse ratio
      $ae
    inner and outer radii
      $r0 = .3; 
      $r0 $r1
    lines
      $nTheta=int( (2.*$pi*$r1)/$ds+1.5 );
      if( $matchGridLines eq "yes" ){ $nTheta=$nThetaCommon; }
      $nr=int( ($r1-$r0)/$ds+1.5 );
      $nTheta $nr
    centre for annulus
      $xe $ye
    boundary conditions
      -1 -1 0 $bcInterface1 
    share
     * material interfaces are marked by share>=100
      0 0 0 $bcInterface1
   mappingName
    ellipseInner
  exit
*
  annulus
    ellipse ratio
      $ae
    inner and outer radii
      $r1e=$r1+$eps;
      $r1e $r2 
    lines
      $nTheta=int( (2.*$pi*$r2)/$ds+1.5 );
      if( $matchGridLines eq "yes" ){ $nTheta=$nThetaCommon; }
      $nr=int( ($r2-$r1)/$ds+1.5 );
      $nTheta $nr
    centre for annulus
      $xe $ye
    boundary conditions
      -1 -1 $bcInterface1 $bcInterface2
    share
     * material interfaces are marked by share>=100
      0 0 $bcInterface1 $bcInterface2
   mappingName
    ellipseCenter
  exit
* 
  annulus
    ellipse ratio
      $ae
    inner and outer radii
      $r2e=$r2+$eps; 
      $r2e $r3
    lines
      $nTheta=int( (2.*$pi*$r3)/$ds+1.5 );
      if( $matchGridLines eq "yes" ){ $nTheta=$nThetaCommon; }
      $nr=int( ($r3-$r2)/$ds+1.5 );
      $nTheta $nr
    centre for annulus
      $xe $ye
    boundary conditions
      -1 -1 $bcInterface2 $bcInterface3
    share
     * material interfaces are marked by share>=100
      0 0 $bcInterface2 $bcInterface3
   mappingName
    ellipseOuter
  exit
* 
  annulus
    ellipse ratio
      $ae
    inner and outer radii
      $r3e=$r3+$eps;
      $r4 = $r3+5.*$ds; 
      $r3e $r4
    lines
      $nTheta=int( (2.*$pi*$r4)/$ds+1.5 );
      if( $matchGridLines eq "yes" ){ $nTheta=$nThetaCommon; }
      $nr=int( ($r4-$r3)/$ds+1.5 );
      $nTheta $nr
    centre for annulus
      $xe $ye
    boundary conditions
      -1 -1 $bcInterface3 0 
    share
     * material interfaces are marked by share>=100
      0 0 $bcInterface3 0
   mappingName
    ellipseOuterOuter
  exit
*
* --------------------  top right annulus -----------------------
  $x0=1.; $y0=.75; 
  $ishare=$ishare+5; 
  annulus 
    mappingName 
      innerAnnulus1 
    $deltaRadius=$deltaRadius0/$factor;
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    $rad2=$outerRadius+.05; 
    inner and outer radii 
      $innerRadius $outerRadius
*       $innerRadius $rad2
    centre for annulus
      $x0 $y0
    lines 
      $nTheta=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $nr=int( ($deltaRadius)/$ds+2.5 );
      $nTheta $nr 
    boundary conditions 
      -1 -1 0 $bcInterface4 
    share
     * material interfaces are marked by share>=100
      0 0 0 $bcInterface4
    exit 
*
  rectangle 
    mappingName
      innerSquare1
    $xa=$x0-$innerRadius-$ds;  $xb=$x0+$innerRadius+$ds; 
    $ya=$y0-$innerRadius-$ds;  $yb=$y0+$innerRadius+$ds; 
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
      outerAnnulus1 
    $innerRadius=$outerRadius; 
    $outerRadius=$innerRadius+$deltaRadius;
    inner and outer radii 
      $innerRadius $outerRadius
    centre for annulus
      $x0 $y0
    lines 
      $nr=int( ($deltaRadius)/$ds+2.5 );
      $nTheta $nr
    boundary conditions 
      -1 -1 $bcInterface4 0 
    share
     * material interfaces are marked by share>=100
      0 0 $bcInterface4 0   
    exit 
* --------------------  bottom right annulus -----------------------
  $x0=1.4; $y0=-.5; 
  $ishare=$ishare+10; 
  annulus 
    mappingName 
      innerAnnulus2 
    $deltaRadius=$deltaRadius0/$factor;
    $outerRadius=.4; $innerRadius=$outerRadius-$deltaRadius;
    $rad2=$outerRadius+.05; 
    inner and outer radii 
      $innerRadius $outerRadius
*       $innerRadius $rad2
    centre for annulus
      $x0 $y0
    lines 
      $nTheta=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $nr=int( ($deltaRadius)/$ds+2.5 );
      $nTheta $nr 
    boundary conditions 
      -1 -1 0 $bcInterface5
    share
     * material interfaces are marked by share>=100
      0 0 0 $bcInterface5
    exit 
*
  rectangle 
    mappingName
      innerSquare2
    $xa=$x0-$innerRadius-$ds;  $xb=$x0+$innerRadius+$ds; 
    $ya=$y0-$innerRadius-$ds;  $yb=$y0+$innerRadius+$ds; 
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
      outerAnnulus2 
    $innerRadius=$outerRadius; 
    $outerRadius=$innerRadius+$deltaRadius;
    inner and outer radii 
      $innerRadius $outerRadius
    centre for annulus
      $x0 $y0
    lines 
      $nr=int( ($deltaRadius)/$ds+2.5 );
      $nTheta $nr
    boundary conditions 
      -1 -1 $bcInterface5 0 
    share
     * material interfaces are marked by share>=100
      0 0 $bcInterface5 0   
    exit 
*
*
exit
generate an overlapping grid 
  backGround
  ellipseInnerSquare
  ellipseInner
  ellipseCenter
  ellipseOuter
  ellipseOuterOuter
  innerSquare1
  innerAnnulus1
  outerAnnulus1
  innerSquare2
  innerAnnulus2
  outerAnnulus2
 done 
*
  change parameters 
    * define the domains -- these will behave like independent overlapping grids
    specify a domain
      * domain name:
      ellipseInnerDomain
      * grids in the domain:
      ellipseInnerSquare
      ellipseInner
      done
    specify a domain
      * domain name:
      ellipseCenterDomain 
      * grids in the domain:
      ellipseCenter
      done
    specify a domain
      * domain name:
      ellipseOuterDomain 
      * grids in the domain:
      ellipseOuter
      done
    specify a domain
      * domain name:
      annulus1Domain
      * grids in the domain:
      innerSquare1
      innerAnnulus1
      done
    specify a domain
      * domain name:
      annulus2Domain
      * grids in the domain:
      innerSquare2
      innerAnnulus2
      done
    specify a domain
      * domain name:
      mainDomain
      * grids in the domain:
      backGround
      ellipseOuterOuter
      outerAnnulus1
      outerAnnulus2
      done
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
multiDomain
exit



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
      $ng $ng $ng $ng $ng $ng 
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
multiDomain
exit
