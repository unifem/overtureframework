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
  printf("   runCases.p -option=[cylDrop|offsetDrop] -tf=<f> -rhos=<f> -numResolutions=<i> -rgd=[var|fixed] ...\n");
  printf("              -bcOption=[rampedPressure] -amp=[0|1] \n");
  printf(" Options:\n");
  printf("     -rgd : fixed=fixed-with boundary grids, var=variable-width boundary grids  \n");
  printf("    \n");
  printf("==============================================================================\n\n");
  exit;
  
}

$option="cylDrop";
$tf=1.; 
$rhob=5.; 
$numResolutions=2; 
$move=1; 
$dt0=-1; 
$ts="im"; 
$rgd="var";
$bcOption="rampedPressure"; 
$show="fallingDrop"; 
$amp=0; 
$rampGravity=0;
$gravity=0.; 
$inflowVelocity=0;
$d=.1; # parabolic inflow width
$nu=.1; 
$cp0=.1; $cpn=1.; # coefficients in pressure outflow BC
$inflowPressure=1.; 
$rtol0=1.e-6; $rtolp0=1.e-8; # tol's for coarse grid

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
  elsif( $arg =~ /-rhob=(.*)/ )
  {
    $rhob=$1;  printf("Setting rhob=[%g]\n",$rhob);
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
  elsif( $arg =~ /-rgd=(.*)/ )
  {
    $rgd=$1;  printf("Setting rgd=[$rgd]\n");
  }
  elsif( $arg =~ /-show=(.*)/ )
  {
    $show=$1;  printf("Setting show=[$show]\n");
  }
  elsif( $arg =~ /-bcOption=(.*)/ )
  {
    $bcOption=$1;  printf("Setting bcOption=[$bcOption]\n");
  }
  elsif( $arg =~ /-rampGravity=(.*)/ )
  {
    $rampGravity=$1;  printf("Setting rampGravity=[$rampGravity]\n");
  }
  elsif( $arg =~ /-gravity=(.*)/ )
  {
    $gravity=$1;  printf("Setting gravity=[$gravity]\n");
  }
  elsif( $arg =~ /-inflowVelocity=(.*)/ )
  {
    $inflowVelocity=$1;  printf("Setting inflowVelocity=[$inflowVelocity]\n");
  }
  elsif( $arg =~ /-ts=(.*)/ )
  {
    $ts=$1;  printf("Setting ts=[$ts]\n");
  }
  elsif( $arg =~ /-d=(.*)/ )
  {
    $d=$1;  printf("Setting d=[$d]\n");
  }
  elsif( $arg =~ /-amp=(.*)/ )
  {
    $amp=$1;  printf("Setting amp=[$amp]\n");
  }
  elsif( $arg =~ /-nu=(.*)/ )
  {
    $nu=$1;  printf("Setting nu=[$nu]\n");
  }
  elsif( $arg =~ /-cp0=(.*)/ )
  {
    $cp0=$1;  printf("Setting cp0=[$cp0]\n");
  }
  elsif( $arg =~ /-cpn=(.*)/ )
  {
    $cpn=$1;  printf("Setting cpn=[$cpn]\n");
  }
  elsif( $arg =~ /-rtol0=(.*)/ )
  {
    $rtol0=$1;  printf("Setting rtol0=[$rtol0]\n");
  }
  elsif( $arg =~ /-rtolp0=(.*)/ )
  {
    $rtolp0=$1;  printf("Setting rtolp0=[$rtolp0]\n");
  }
  elsif( $arg =~ /-inflowPressure=(.*)/ )
  {
    $inflowPressure=$1;  printf("Setting inflowPressure=[$inflowPressure]\n");
  }
}

$show .= "G"; 

$debug=0; # set to 1 for debug info 

$CGBUILDPREFIX=$ENV{CGBUILDPREFIX};
$cginsCmd = "$CGBUILDPREFIX/ins/bin/cgins";  # command for cgins 
$OVERTURE=$ENV{Overture};
$plotStuff = "$OVERTURE/bin/plotStuff";  # command for plotStuff


for( $i=1; $i <= $numResolutions; $i++ )
{

  $factor=2**$i; 

  $showFileName="$show$factor";

  if( $option eq "cylDrop" || $option eq "offsetDrop" )
  {
    if( $dt0 < 0.  ){ $dt0=.05; }
    #    $dtMax=.05/$factor; 
    $dtMax=$dt0/$factor; 
    $fact = $factor**2; 
    $rtolp=$rtolp0/$fact; $atolp=1.e-14; 
    $rtol =$rtol0/$fact;  $atol=1.e-14; 

    # $rampGravity=1;

    # -- ramped pressure inflow  
    if( $option eq "cylDrop" ){
      if( $rgd eq "var" ) { $baseGrid="cylGridSmalle"; }else{ $baseGrid="cylGridSmallFixede"; }
    }  
    else{
      if( $rgd eq "var" ) { $baseGrid="cylOffsetGride"; }else{ $baseGrid="cylOffsetGridFixede"; }
    }

    $cmd = "$cginsCmd -noplot cylDrop -g=$baseGrid$factor.order2.s3.hdf -tf=$tf -tp=.1 -dtMax=$dtMax -nu=$nu -ad2=0 -ts=$ts -density=$rhob -radius=.25 -move=$move -dropName=drop -channelName=channel -bcOption=$bcOption -d=$d -inflowPressure=$inflowPressure -inflowVelocity=$inflowVelocity -gravity=$gravity -rampGravity=$rampGravity -cp0=$cp0 -cpn=$cpn -project=0 -numberOfCorrections=2 -omega=.1 -addedMass=$amp -useProvidedAcceleration=$amp -addedDamping=$amp -addedDampingCoeff=1. -addedDampingProjectVelocity=$amp  -scaleAddedDampingWithDt=$amp -useTP=0 -debug=3  -solver=best -psolver=best -rtolp=$rtolp -atolp=$atolp -rtol=$rtol -atol=$atol -freqFullUpdate=1 -show=$showFileName.show -go=go >! $show$factor.out";

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

  printf("------------- option=$option factor=$factor bcOption=$bcOption move=$move rgd=$rgd amp=$amp rhob=$rhob gravity=$gravity ----------------\n");
  printf(">> run [$cmd]\n");
  $startTime=time();

  $returnValue = system("csh -f -c 'nohup $cmd'");    

  $cpuTime=time()-$startTime;
  printf("...returnValue=%i [$returnValue], cpu=%g (s)\n",$returnValue,$cpuTime);

  if( $returnValue ne 0 )
  {
    printf(" *****  ERROR running this case, returnValue=$returnValue **********\n");
  }
  else
  {
    # --- save matlab file with rigid body variables -----
    $cmd = "$plotStuff -noplot plotRigidBody.cmd -name=$showFileName >! junk.out ";
    printf(">> run [$cmd]\n");
    $returnValue = system("csh -f -c 'nohup $cmd'"); 
    if( $returnValue ne 0 )
    {
      printf(" *****  ERROR generating matlab file, returnValue=$returnValue **********\n");
    }
    else
    {
      printf("Matlab file with rigid body variables saved to file=[$showFileName.m].\n");
    }
  }



}


exit;
# ===============================================================================================================


