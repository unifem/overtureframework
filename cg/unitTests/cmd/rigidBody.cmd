#
# trb command file for testing the RigidBody class
#
# Usage:
#     trb -cmd=rigidBody.cmd -motion=[cxa|cya|cza|fr1|fr2|fr3|cwxa|cwya|cwza|trig|poly|...] ...
#                            -order=[2|4|..] -ts=[leapFrogTrapezoidal|implicitRungeKutta] ...
#                            -dt=<f>
# Examples:
#   trb -cmd=rigidBody.cmd -motion=cxa -go=go
#
$motion="cxa"; $tf=1.; $order=2; $ts="leapFrogTrapezoidal"; $cfl=.9; $dt=.1; 
$go="halt"; $useVariableTimeStep=0; $polyDegree=2; 
GetOptions( "motion=s"=>\$motion,"ts=s"=>\$ts,"dt=f"=>\$dt,"cfl=f"=>\$cfl,"tf=f"=>\$tf,"polyDegree=i"=>\$polyDegree,\
            "order=i"=>\$order,"useVariableTimeStep=i"=>\$useVariableTimeStep,"go=s"=>\$go );
#
if( $motion eq "cxa" ){ $motion="constant x acceleration"; }
if( $motion eq "cya" ){ $motion="constant y acceleration"; }
if( $motion eq "cza" ){ $motion="constant z acceleration"; }
if( $motion eq "cwxa" ){ $motion="constant omega_x acceleration"; }
if( $motion eq "cwya" ){ $motion="constant omega_y acceleration"; }
if( $motion eq "cwza" ){ $motion="constant omega_z acceleration"; }
if( $motion eq "fr1" ){ $motion="free rotation 1"; }
if( $motion eq "fr2" ){ $motion="free rotation 2"; }
if( $motion eq "fr3" ){ $motion="free rotation 3"; }
if( $motion eq "trig" ){ $motion="trigonometric motion"; }
if( $motion eq "poly" ){ $motion="polynomial motion"; }
#
if( $go eq "go" ){ $go="solve\n exit" }; 
if( $go eq "halt" ){ $go="#"; }; 
# 
polyDegree: $polyDegree
$motion
$ts
tFinal: $tf
dt0: $dt
order of accuracy: $order
use variable time step $useVariableTimeStep
solve
exit