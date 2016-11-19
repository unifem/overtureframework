#
# Plot results from the solid beam in a channel
#
# -- INS+SM
#    plotStuff plotBeamInAChannel -show=bicInsG8.show
#    plotStuff plotBeamInAChannel -show=bilcInsG8.show  [ long channel
#    plotStuff plotBeamInAChannel -show=tbilcInsG16.show  [ thin beam
#
# -- Linear-elasticity
#   plotStuff plotBeamInAChannel -show=sbicLE8.show
#   plotStuff plotBeamInAChannel -show=sbicLE16.show
#
# -- Neo-Hookean
#   plotStuff plotBeamInAChannel -show=sbicNeo8.show
#   plotStuff plotBeamInAChannel -show=sbicNeo8a.show   [ lighter -scf=10
#   plotStuff plotBeamInAChannel -show=sbicNeo16.show
#
#   plotStuff plotBeamInAChannel -show=sbicLEThin16.show
#   plotStuff plotBeamInAChannel -show=sbicLEThin16a.show
#
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16.show
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16a.show
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16b.show   
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16c.show   
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16d.show   
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16e.show   
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16f.show   
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16g.show   
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16h.show    [ lighter solid
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16i.show    [ rhos=1 lambdas=10 
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16j.show    [ scf=100, SHOCK
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16k.show -movieName=sbicNeoScf10Shock1p1   [ scf=10, SHOCK
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16l.show  -movieName=sbicNeoRhos1  [ rhos=1 lambdas=10  REDO
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16m.show  [ rhos=1 lambdas=10  REDO
#
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32.show  [ scf=10, SHOCK *trouble*
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32a.show  [ scf=10, SHOCK
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32c.show  [ rhos=2, lam=mu=10 SHOCK
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32d.show  [ scf=10, SHOCK slope limiter
# 
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s100.show  [ scf=100, SHOCK 
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s50.show   [ scf=50, SHOCK 
#  -- ramped:
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16s10ramped.show
#
#  -- plot schlieren or contours:
#    plotStuff plotBeamInAChannel -show=sbicNeoThin32a.show -option=sch   [ scf=10, SHOCK
#    plotStuff plotBeamInAChannel -show=sbicNeoThin32a.show -option=contours   [ scf=10, SHOCK
#    plotStuff plotBeamInAChannel -show=sbicNeoThin32s10ramped.show -option=sl -root=sbicNeoThin32s10ramped -vMax=.5 -spMax=.2 -res=2048   [ scf=10,ramped -- plots for flunsi
#  -- hardcopy:
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32a.show -option=sch -root="sbic32_Sc_Sp"
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32a.show -option=sl  -root="sbic32_SL_Sp"
#  --- line plots
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32a.show -option=line -root="sbic32"
#
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32c.show -option=line -root="sbic32rs2l10"
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s100.show -option=line -root=sbic32rs100l100
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16s100.show -option=line -root=sbic16rs100l100
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s50.show -option=line -root=sbic32rs50l50 
#
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16s1ramped.show -option=line -root=sbic16rs1l10ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s1ramped.show -option=line -root=sbic32rs1l10ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16s10ramped.show -option=line -root=sbic16rs10l10ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s10ramped.show -option=line -root=sbic32rs10l10ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16s50ramped.show -option=line -root=sbic16rs50l50ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s50ramped.show -option=line -root=sbic32rs50l50ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin16s100ramped.show -option=line -root=sbic16rs100l100ramped
#   plotStuff plotBeamInAChannel -show=sbicNeoThin32s100ramped.show -option=line -root=sbic32rs100l100ramped
#  
$show="deformingEllipse4.show"; $name="deformingEllipse4"; $root=""; 
$vMin=0.; $vMax=-1.;  $spMin=0.; $spMax=-1.; 
$res=1024; 
$option="sl"; # sl=streamlines, sch=schlieren
$movieName="sbic"; 
$sc="stressNorm"; $fc="p"; # component names
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show,"root=s"=>\$root,"solution=i"=>\$solution,"vMin=f"=>\$vMin,"vMax=f"=>\$vMax,\
            "spMin=f"=>\$spMin,"spMax=f"=>\$spMax,"res=i"=>\$res, "movieName=s"=>\$movieName,"option=s"=>\$option );
#
$show
#
# -- set displacement components for Godunov 
frame series:solidDomain
derived types
speed
specify displacement components
  6 7 8
stressNorm
exit
# 
previous
# plot:stressNorm
plot:speed
contour
  adjust grid for displacement 1
  plot contour lines (toggle)
  vertical scale factor 0.
  coarsening factor 1 (<0 : adaptive)
  if( $spMax > $spMin ){ $cmd ="min max $spMin $spMax"; }else{ $cmd="#"; }
  $cmd 
  exit
frame series:fluidDomain
#
if( $vMax > $vMin ){ $minMax ="min max $vMin $vMax"; }else{ $minMax="#"; }
if( $option eq "sl" ){ $cmd="stream lines\n arrow size .025\n $minMax\n exit"; }else{ $cmd="#"; }
$cmd
pause
#
$plotSchlieren="derived types\n schlieren\n exit\n plot:schlieren\n contour\n gray\n vertical scale factor 0.\n coarsening factor 1 (<0 : adaptive)\n plot contour lines (toggle)\n min max .3 1\n exit";
if( $option eq "sch" ){ $cmd=$plotSchlieren; }else{ $cmd="#"; }
$cmd
#
$plotContours="plot:rho\n contour\n vertical scale factor 0.\n coarsening factor 1 (<0 : adaptive)\n plot contour lines (toggle)\n exit";
if( $option =~ "^contour" ){ $cmd=$plotContours; }else{ $cmd="#"; }
$cmd
#
# ============== Hard-copies =================
#
#
hardcopy colour:0 24bit
if( $res > 1024 ){ $lineWidth=6; }else{ $lineWidth=1;}
line width scale factor:0 $lineWidth
hardcopy vertical resolution:0 $res
hardcopy horizontal resolution:0 $res
#
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
set view:0 -0.0140254 -0.032026 0 1.69915 1 0 0 0 1 0 0 0 1
# 
sub hardcopy{ local($num)=@_; $time=sprintf("%3.1f",($num-1)/10.); $time=~ s/\./p/; $name = $root . "_t$time\.ps"; $cmd.="\nsolution: $num\n hardcopy file name: $name\n hardcopy save"; } 
#
$cmd="#"; 
foreach $n (6,11,16,21,31,41,51,61,71,81) { hardcopy($n); }
if( ($root eq "") || ($option eq "line") ){ $cmd="#"; }  # no hardcopy 
$cmd 
#
# ----- LINE PLOTS -----
frame series:solidDomain
contour
  adjust grid for displacement 0
exit
#
sub linePlot{ local($num)=@_; $time=sprintf("%3.1f",($num-1)/10.); $time=~ s/\./p/; $name = $root . "LinePlot_t$time\.m"; $cmd.="\nsolution: $num\n contour\n line plots\n specify lines\n 1 51\n 0. 0. 0. 1.\n v1\n add v2\n add s11\n add s12\n add s21\n add s22\n add u\n add v\n add speed\n add x0\n add x1\n save results to a matlab file\n $name\n erase and exit\n exit this menu\n exit"; } 
#
$cmd="#"; 
foreach $n (1,11,21,31,41,51,61,71,81,91,101,111,121,131,141,151,161) { linePlot($n); }
if( $option ne "line" ){ $cmd="#"; } # no line plots
$cmd


contour
  line plots
    specify lines
    1 51
    0. 0. 0. 1.
      v1
      add v2
      add s11
      add u
      add v
      add speed
      add x0
      add x1
      add stressNorm
      save results to a matlab file
      linePlot.m
      erase and exit
    exit this menu





solution: 6
$name = $root . "_tp5.ps";
hardcopy file name:0 $name
hardcopy save:0
#
solution: 11
$name = $root . "_t1p0.ps";
hardcopy file name:0 $name
hardcopy save:0
#
solution: 16
$name = $root . "_t1p5.ps";
hardcopy file name:0 $name
hardcopy save:0
#
solution: 21
$name = $root . "_t2p0.ps";
hardcopy file name:0 $name
hardcopy save:0
#
solution: 31
$name = $root . "_t3p0.ps";
hardcopy file name:0 $name
hardcopy save:0




plot:p
contour
#  plot contour lines (toggle)
  vertical scale factor 0.
  coarsening factor 1 (<0 : adaptive)
  # p: 
  # min max .5 1.0001 
 exit
if( $option eq "sc" ){ $cmd="derived types\n schlieren\n exit\n plot:schlieren\n coarsening factor 1 (<0 : adaptive)\n plot contour lines (toggle)\n min max .3 1\n exit"; }
derived types
schlieren
exit
plot:schlieren



#
# -- MOVIE --
#
bigger
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
DISPLAY LABELS:0 0
solution: 1
save movie files 1
movie file name: $movieName
pause
show movie



plot:p
contour
#  plot contour lines (toggle)
  vertical scale factor 0.
  coarsening factor 1 (<0 : adaptive)
  # p: 
  # min max .5 1.0001 
 exit
# #


# -- plot Sch
derived types
schlieren
exit
plot:schlieren
# plot:p
contour
#  plot contour lines (toggle)
  gray
  vertical scale factor 0.
  coarsening factor 1 (<0 : adaptive)
  # schlieren: 
  plot contour lines (toggle)
  min max .3 1
  # p: 
  # min max .5 1.0001 
 exit
# 


bigger
DISPLAY COLOUR BAR:0 0
DISPLAY AXES:0 0
#
# -- MOVIE --
#
DISPLAY LABELS:0 0
solution: 1
save movie files 1
movie file name: $movieName
pause
show movie

movie file name: deformingEllipseNeoHookean

#
# Hard-copies: 
#
if( $root eq "" ){ $cmd="\n\n"; }else{ $cmd="#"; } # stop with blank line if not root specified
$cmd
#
# DISPLAY LABELS:0 0
#
#
#
hardcopy colour:0 24bit
if( $res > 1024 ){ $lineWidth=4; }else{ $lineWidth=1;}
line width scale factor:0 $lineWidth
hardcopy vertical resolution:0 $res
hardcopy horizontal resolution:0 $res
# 
solution: 6
$name = $root . "_tp5.ps";
hardcopy file name:0 $name
hardcopy save:0
pause
#
solution: 21
$name = $root . "_t2p0.ps";
hardcopy file name:0 $name
hardcopy save:0
pause
#
solution: 41
$name = $root . "_t4p0.ps";
hardcopy file name:0 $name
hardcopy save:0
pause
#
###
#   extract boundary
###
#
frame series:outerDomain
solution: 6
contour
  line plots
    specify a boundary
    201 1 0 1
    save results to a text file
      $name = $root . "_int_tp5.dat";
      $name
      0
      6 x
      7 y
      done
      exit this menu
    exit this menu
  exit
#
solution: 11
contour
  line plots
    specify a boundary
    201 1 0 1
    save results to a text file
      $name = $root . "_int_t1p0.dat";
      $name
      0
      6 x
      7 y
      done
      exit this menu
    exit this menu
  exit
#
solution: 21
contour
  line plots
    specify a boundary
    201 1 0 1
    save results to a text file
      $name = $root . "_int_t2p0.dat";
      $name
      0
      6 x
      7 y
      done
      exit this menu
    exit this menu
  exit
#
solution: 31
contour
  line plots
    specify a boundary
    201 1 0 1
    save results to a text file
      $name = $root . "_int_t3p0.dat";
      $name
      0
      6 x
      7 y
      done
      exit this menu
    exit this menu
  exit
#
solution: 41
contour
  line plots
    specify a boundary
    201 1 0 1
    save results to a text file
      $name = $root . "_int_t4p0.dat";
      $name
      0
      6 x
      7 y
      done
      exit this menu
    exit this menu
  exit
#



solution: 81
contour
  line plots
    specify a boundary
    201 1 0 1
    save results to a text file
      $name = $root . "_int_t8p0.dat";
      $name
      0
      6 x
      7 y
      done
      exit this menu
    exit this menu
  exit


