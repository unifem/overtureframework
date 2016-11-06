eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#
# perl program to run different cases 
# 
# Examples:
#  runCases.p -option=cylDrop -tf=2 -rhos=5.

use Getopt::Long; use Getopt::Std;

$pi = 4.*atan2(1.,1.);

$numberOfParameters = @ARGV;
if ($numberOfParameters eq 0)
{
  
  printf("\n");
  printf("================================================================================\n");
  printf("This perl script will run different cases...\n");
  printf("  Usage: \n");
  printf("    runCases.p -option=[cylDrop] -tf=<f> -rhos=<f> -numResolutions=<i>\n");
  printf("    \n");
  printf("==============================================================================\n\n");
  exit;
  
}

$option="cylDrop";
$tf=1.; 
$rhos=.01; 
$projectVelocity=1;
$numResolutions=2; 
$move=1; 
$dt0=-1; 
$ts="im"; 

foreach $arg ( @ARGV )
{
  if( $arg =~ /-option=(.*)/ )
  {
    $option = $1;
    printf("Setting option=[%s]\n",$option);
  }
  elsif( $arg =~ /-tf=(.*)/ )
  {
    $tf=$1; printf("Setting tf=[%g]\n",$tf);
  }
  elsif( $arg =~ /-rhos=(.*)/ )
  {
    $rhos=$1;  printf("Setting rhos=[%g]\n",$rhos);
  }
  elsif( $arg =~ /-dt0=(.*)/ )
  {
    $dt0=$1;  printf("Setting dt0=[%g]\n",$dt0);
  }
  elsif( $arg =~ /-numResolutions=(.*)/ )
  {
    $numResolutions=$1;  printf("Setting numResolutions=[$numResolutions]\n");
  }
  elsif( $arg =~ /-move=(.*)/ )
  {
    $move=$1;  printf("Setting move=[$move]\n");
  }
  elsif( $arg =~ /-ts=(.*)/ )
  {
    $ts=$1;  printf("Setting ts=[$pc]\n");
  }
  elsif( $arg =~ /-instabilityErrorTol=(.*)/ )
  {
    $instabilityErrorTol=$1;  printf("Setting instabilityErrorTol=[%g]\n",$instabilityErrorTol);
  }
  elsif( $arg =~ /-projectVelocity=(.*)/ )
  {
    $projectVelocity=$1; printf("Setting projectVelocity=[%d]\n",$projectVelocity);
  }
}

$debug=0; # set to 1 for debug info 

# printf("------------ Option=[$option] rhos=$rhos tf=$tf projectVelocity=$projectVelocity--------------\n");
# printf("------------ instabilityErrorTol=[$instabilityErrorTol]                          --------------\n");
$CGBUILDPREFIX=$ENV{CGBUILDPREFIX};
$cginsCmd = "$CGBUILDPREFIX/ins/bin/cgins";  # command for cgins 



for( $i=1; $i <= $numResolutions; $i++ )
{

  $factor=2**$i; 

  if( $option eq "cylDrop" )
  {
    if( $dt0 < 0.  ){ $dt0=.05; }
    #    $dtMax=.05/$factor; 
    $dtMax=$dt0/$factor; 
    $fact = $factor**2; 
    $rtolp=1.e-8/$fact; $atolp=1.e-14; 
    $rtol =1.e-6/$fact; $atol=1.e-14; 

    $rampGravity=1;

   # $cmd = "$cginsCmd -noplot cylDrop -g=cylGridSmalle$factor.order2.s3.hdf -tf=$tf -tp=.1  -dtMax=$dtMax -nu=.1 -ad2=0 -ts=$ts -density=5 -radius=.25 -dropName=drop -channelName=channel -bcOption=walls -gravity=-1. -rampGravity=$rampGravity -project=0 -numberOfCorrections=2 -omega=.1 -addedMass=0  -useProvidedAcceleration=0 -addedDamping=0 -addedDampingCoeff=1. -addedDampingProjectVelocity=0  -scaleAddedDampingWithDt=0 -useTP=0 -debug=3  -solver=best -psolver=best -rtolp=$rtolp -atolp=$atolp -rtol=$rtol -atol=$atol -freqFullUpdate=1 -show=fallingDropG$factor.show -go=go >! fallingDropG$factor.out";

    # -- ramped pressure inflow 
    $cmd = "$cginsCmd -noplot cylDrop -g=cylGridSmallFixede$factor.order2.s3.hdf -tf=$tf -tp=.1 -dtMax=$dtMax -nu=.1 -ad2=0 -ts=$ts -density=5 -radius=.25 -move=$move -dropName=drop -channelName=channel -bcOption=rampedPressure -inflowPressure=1 -gravity=0. -rampGravity=0 -project=0 -numberOfCorrections=2 -omega=.1 -addedMass=0  -useProvidedAcceleration=0 -addedDamping=0 -addedDampingCoeff=1. -addedDampingProjectVelocity=0  -scaleAddedDampingWithDt=0 -useTP=0 -debug=3  -solver=best -psolver=best -rtolp=$rtolp -atolp=$atolp -rtol=$rtol -atol=$atol -freqFullUpdate=1 -show=fallingDropG$factor.show -go=go >! fallingDropG$factor.out";

  }
  elsif( $option eq "shearDisk" )
  {
  }
  else
  {
   printf("Unknown option=[$option]\n");
   exit;
  }
  # printf("pi=$pi\n");

  printf("--------------- option=$option factor=$factor -------------------\n");
  printf(">> run [$cmd]\n");
  $startTime=time();

  $returnValue = system("csh -f -c 'nohup $cmd'");    

  $cpuTime=time()-$startTime;
  printf("...returnValue=%i [$returnValue], cpu=%g (s)\n",$returnValue,$cpuTime);

  if( $returnValue ne 0 )
  {
    printf(" *****  ERROR running this case, returnValue=$returnValue **********\n");
  }
}


exit;
# ===============================================================================================================


