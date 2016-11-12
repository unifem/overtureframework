#
#  Self convergence for the falling cylinder 
#
#  comp compFallingDrop -solution=<i> -numGrids=<i> -logFile=<s>
# 
$solution=-1; $numGrids=3; $logFileName=""; $show="fallingDrop"; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "logFile=s"=>\$logFile,"show=s"=>\$show, "numGrids=i"=>\$numGrids,"solution=i"=>\$solution);
#
if( $logFile eq "" ){ $logFile =$show; $logFile =~ s/^(\w)/\U$1/; $logFile="comp" . $logFile . ".log"; }
printf("logFile=$logFile\n");
# 
output file name: $logFile
specify files
  $cmd= $show . "G2";
  if( $numGrids > 1 ){ $cmd.="\n" . $show . "G4"; }
  if( $numGrids > 2 ){ $cmd.="\n" . $show . "G8"; }
  if( $numGrids > 3 ){ $cmd.="\n" . $show . "G16"; }
  $cmd
exit
choose a solution
  $solution
compute errors

exit