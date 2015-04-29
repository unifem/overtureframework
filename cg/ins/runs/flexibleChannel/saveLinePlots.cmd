#
# Case a:
#  plotStuff saveLinePlots -show=fc2a.show -name=fc2a
#  plotStuff saveLinePlots -show=fc4a.show -name=fc4a
#  plotStuff saveLinePlots -show=fc8a.show -name=fc8a
#  plotStuff saveLinePlots -show=fc16a.show -name=fc16a
#
# -- Case a: smaller nu: 
#  plotStuff saveLinePlots -show=fc2an.show -name=fc2an
#  plotStuff saveLinePlots -show=fc4an.show -name=fc4an
#  plotStuff saveLinePlots -show=fc8an.show -name=fc8an
#  plotStuff saveLinePlots -show=fc8an.show -name=fc16an
# 
# -- Case a: smaller nu, stretched grid
#  plotStuff saveLinePlots -show=fc2as.show -name=fc2as
#  plotStuff saveLinePlots -show=fc4as.show -name=fc4as
#  plotStuff saveLinePlots -show=fc8as.show -name=fc8as
#  plotStuff saveLinePlots -show=fc8as.show -name=fc16as
#
# Case b:
#  plotStuff saveLinePlots -show=fc2b.show -name=fc2b
#  plotStuff saveLinePlots -show=fc4b.show -name=fc4b
#  plotStuff saveLinePlots -show=fc8b.show -name=fc8b
#  plotStuff saveLinePlots -show=fc16b.show -name=fc16b
# 
#  plotStuff saveLinePlots -show=fc4c.show -name=fc4c -- for animation
#
$show="fc2.show"; $name="fc2";
GetOptions( "show=s"=>\$show,"vorMin=f"=>\$vorMin,"vorMax=f"=>\$vorMax,"name=s"=>\$name );
$show
#
# ---------------------
# Save line plots of the solution on the surface
# ---------------------
sub linePlot{  local($solution,$name,$time)=@_; \
 $matlab = $name . "_$time.m"; \
 $cmd .="\nsolution: $solution\n"; \
 $cmd .="contour\n"; \
 $cmd .="  line plots\n"; \
 $cmd .="    specify a boundary\n"; \
 $cmd .="    121 1 0 1\n"; \
 $cmd .="      p\n"; \
 $cmd .="      add u\n"; \
 $cmd .="      add v\n"; \
 $cmd .="      add x0\n"; \
 $cmd .="      add x1\n"; \
 $cmd .="     save results to a matlab file\n"; \
 $cmd .="      $matlab\n"; \
 $cmd .="      exit this menu\n"; \
 $cmd .="   exit\n"; \
 $cmd .=" exit";\
}
#
$cmd="#";
linePlot( 2,$name,"t0p005"); 
linePlot( 3,$name,"t0p010"); 
linePlot( 4,$name,"t0p015"); 
linePlot( 5,$name,"t0p020"); 
$cmd

$cmd="#";
linePlot( 3,$name,"t0p002"); 
linePlot( 5,$name,"t0p004"); 
linePlot( 7,$name,"t0p006"); 
linePlot( 9,$name,"t0p008"); 
linePlot(11,$name,"t0p010"); 
linePlot(13,$name,"t0p012"); 
linePlot(15,$name,"t0p014"); 
linePlot(17,$name,"t0p016"); 
linePlot(19,$name,"t0p018"); 
linePlot(21,$name,"t0p020"); 
$cmd




$cmd="#";
linePlot(2,$name,"t0p005"); 
linePlot(3,$name,"t0p010"); 
linePlot(4,$name,"t0p015"); 
$cmd
# 
open a new file
$name="fc4b"; 
fc4b.show
#
$cmd="#";
linePlot(2,$name,"t0p005"); 
linePlot(3,$name,"t0p010"); 
linePlot(4,$name,"t0p015"); 
$cmd
# 
open a new file
$name="fc8b"; 
fc8b.show
#
$cmd="#";
linePlot(2,$name,"t0p005"); 
linePlot(3,$name,"t0p010"); 
linePlot(4,$name,"t0p015"); 
$cmd
# 
open a new file
$name="fc16b"; 
fc16b.show
#
$cmd="#";
linePlot(2,$name,"t0p005"); 
linePlot(3,$name,"t0p010"); 
linePlot(4,$name,"t0p015"); 
$cmd


