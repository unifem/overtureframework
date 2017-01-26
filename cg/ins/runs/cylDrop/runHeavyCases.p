eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#
# perl program to run different cases 
# 
# Examples:
#  runHeavyCases.p -option=HeavyDrop -rgd=fixed -icase=1 -numResolutions=2

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

$icase=1;
$option="HeavyDrop";
$tf=25; 
$tplot=1;
$rhob=2;
$numResolutions=2;
$move=1; 
$dt0=-1; 
$ts="im";
$rgd="var";
$bcOption="inflowGivenPressure"; 
$show="HeavyDrop"; 
$amp=0;
$rampGravity=0;
$gravity=-0.7994900950; 
$nu=0.02856232168; #mu=0.1
$inflowPressure=0.; 
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
  elsif( $arg =~ /-icase=(.*)/ )
  {
    $icase=$1; printf("Setting icase=[%g]\n",$icase);
  } 
}

if( $icase eq 2) #mu=0.2
{
	$gravity=-3.197960380;
	$nu=0.1142492867;
	$tf=17;
	$tplot=1.7;
}
elsif( $icase eq 3) #mu=0.5
{
	$gravity=-19.98725238;
	$nu=0.7140580420;
	$tf=14;
	$tplot=1.4;
}
$show .="icase$icase";

$show .= "G"; 

$debug=0; # set to 1 for debug info 

$CGBUILDPREFIX=$ENV{CGBUILDPREFIX};
$cginsCmd = "$CGBUILDPREFIX/ins/bin/cgins";  # command for cgins 
$OVERTURE=$ENV{Overture};
$plotStuff = "$OVERTURE/bin/plotStuff";  # command for plotStuff


for( $i=0; $i <= $numResolutions; $i++ )
{

  $factor=2**$i; 

  $showFileName="$show$factor";

  if( $option eq "cylDrop" || $option eq "HeavyDrop" )
  {
    if( $dt0 < 0.  ){ $dt0=.1; }
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
      if( $rgd eq "var" ) { $baseGrid="cylGridHeavyNonDime"; }else{ $baseGrid="cylGridHeavyNonDimFixede"; }
    }

    $cmd = "$cginsCmd -noplot cylDrop -g=$baseGrid$factor.order2.s3.hdf -tf=$tf -tp=$tplot -dtMax=$dtMax -nu=$nu -ad2=0 -ts=$ts -density=$rhob -radius=0.5 -move=$move -dropName=drop -channelName=channel -bcOption=$bcOption -inflowPressure=$inflowPressure -gravity=$gravity -rampGravity=$rampGravity -project=0 -numberOfCorrections=2 -omega=.1 -addedMass=$amp -useProvidedAcceleration=$amp -addedDamping=$amp -addedDampingCoeff=1. -addedDampingProjectVelocity=$amp  -scaleAddedDampingWithDt=$amp -useTP=0 -debug=3  -solver=best -psolver=best -rtolp=$rtolp -atolp=$atolp -rtol=$rtol -atol=$atol -freqFullUpdate=1 -show=$showFileName.show -go=go >! $show$factor.out";

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

}


exit;
# ===============================================================================================================


