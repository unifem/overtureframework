#
#   Random cylinders in a channel for the IG problem
#
# Examples:
#   ogen -noplot multiCylIG -factor=1 -name=multiCylIG1.hdf
#   ogen -noplot multiCylIG -factor=2 -name=multiCylIG2.hdf
#
# -- bigger domain
#   ogen -noplot multiCylIG -xa=-.5 -factor=2 -name=multiCylIG2Big.hdf
#   ogen -noplot multiCylIG -xa=-.5 -factor=4 -name=multiCylIG4Big.hdf
#   ogen -noplot multiCylIG -xa=-.5 -factor=8 -name=multiCylIG8Big.hdf
#
create mappings
#
$factor=1; $name="multiCylIG1.hdf";
$xa=0.; $xb=4.; $ya=-1.; $yb=1.
# 
GetOptions( "factor=f"=>\$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name );
#
# $factor=1; $name="multiCylIG1.hdf";
# $factor=2; $name="multiCylIG2.hdf";
# $factor=4; $name="multiCylIG4.hdf";
# $factor=8; $name="multiCylIG8.hdf";
#
# $factor=2; $name="multiCylIGSmall2.hdf"; $xa=0.; $xb=1.; $ya=-.5; $yb=.5
#
# $factor=2; $name="multiCylIGSmaller2.hdf"; $xa=0.; $xb=1.; $ya=0.; $yb=.5
#
#----
#  $factor=2; $name="multiCylIG2Big.hdf"; $xa=-.5;
#  $factor=4; $name="multiCylIG4Big.hdf"; $xa=-.5;
#  $factor=8; $name="multiCylIG8Big.hdf"; $xa=-.5;
#---
# Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5);\
}
#
$ds=1./40./$factor;
#
rectangle
  set corners
 # -2. 2. -1.5 1.5
    $xa $xb $ya $yb   
  lines
 # 161  121  * 129 97  65 49 
 # getGridPoints(161,121);
    $nx =int(($xb-$xa)/$ds+1.5);  $ny =int(($yb-$ya)/$ds+1.5); 
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
    backGround
exit
# ======================================================
# Define a function to build and AnnulusMapping 
# usage:
#   makeAnnulus(radius,xCenter,yCenter)
# =====================================================
$count=0; # counts annulii
sub makeAnnulus\
{ local($radius,$xc,$yc)=@_; \
  $outerRadius=$radius+.15/$factor;\
  $nxr = int(2.*3.1415*($radius+.125/$factor)/$ds+1.5); \
  $count=$count+1; $aname="annulus" . "$count"; \
  $annulusMappingNames = $annulusMappingNames . "   $aname\n"; \
  $commands = \
  "Annulus\n" . \
  "lines\n" . \
  "  $nxr $ny\n" . \
  "inner and outer radii\n" . \
  "  $radius $outerRadius\n" . \
  "centre\n" . \
  "   $xc $yc\n" .   \
  "boundary conditions\n" . \
  "  -1 -1 1 0\n" . \
  "mappingName\n" . \
  " $aname\n" .  \
  "exit\n"; \
}
#
# $nx=81; $ny=7; # for the annulus
  getGridPoints(81,7);
  $ny=int(.15/$ds/$factor+1.5); # fix lines in the normal direction since we reduce the radius
#
#.****************
#.makeAnnulus(.10,.55,.3);
#.$commands
#.* *
#.*
#.makeAnnulus(.07,.35,.125);
#.$commands
#.*
#.makeAnnulus(.06,.8,.2);
#.$commands
#.* 
#.*
#.****************
#.* makeAnnulus(.10,.55,.3);
#.* $commands
#.* *
#.* makeAnnulus(.08,.8,.05);
#.* $commands
#.* *
#.* makeAnnulus(.07,.35,.125);
#.* $commands
#.* *
#.* makeAnnulus(.09,.75,-.3);
#.* $commands
#.* *
#.* makeAnnulus(.08,.40,-.2);
#.* $commands
#.* *
#.*
#****************
 makeAnnulus(.15,1,0.2);
 $commands
 #
 makeAnnulus(.15,2,0.3);
 $commands
 #
 makeAnnulus(.15,1.75,-.7);
 $commands
 #
 makeAnnulus(.1,1.5,-.25);
 $commands
 #
 makeAnnulus(.1,2.5,.35);
 $commands
 #
 makeAnnulus(.1,3.0,-.55);
 $commands
 #
 makeAnnulus(.1,.75,-.35);
 $commands
 #
 makeAnnulus(.1,1.25,.5);
 $commands
 #
 makeAnnulus(.1,3.5,.1);
 $commands
 #
 makeAnnulus(.1,2.75,-.05);
 $commands
 #
 # ---- 
 makeAnnulus(.08,.5,.5);
 $commands
 #
 makeAnnulus(.08,.7,.1);
 $commands
 #
 makeAnnulus(.08,1.2,-.6);
 $commands
 #
 makeAnnulus(.08,1.5,.2);
 $commands
 #
 makeAnnulus(.08,2.1,-.25);
 $commands
 #
 makeAnnulus(.08,2.4,-.7);
 $commands
 #
 makeAnnulus(.08,2.7,-.5);
 $commands
 #
 makeAnnulus(.09,1.6,.7);
 $commands
 #
 makeAnnulus(.09,.5,-.7);
 $commands
 #
 makeAnnulus(.09,2.4,.1);
 $commands
 #
 makeAnnulus(.09,3.,.6);
 $commands
 #
 makeAnnulus(.09,3.,.25);
 $commands
 #
 makeAnnulus(.11,3.3,-.25);
 $commands
 #
 makeAnnulus(.11,2.2,.75);
 $commands
 #
 makeAnnulus(.11,.7,.725);
 $commands
 #
 makeAnnulus(.11,1.1,-.1);
 $commands
#
#
# *
exit
generate an overlapping grid
    backGround
    $annulusMappingNames
  done
  change parameters
 # choose implicit or explicit interpolation
 # interpolation type
 #   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
#  display intermediate results
  compute overlap
#
#
# display computed geometry
  exit
#
save an overlapping grid
$name
multiCylRandom
exit

