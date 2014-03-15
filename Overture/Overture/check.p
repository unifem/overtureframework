eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#
# perl program to run the Overture regression tests
# 

sub checkError
{
  if( $rt != 0 )
  { 
    print " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";
    print "ERROR return rt=$rt\n";
    $result="FAIL"; 
    printf("\n An ERROR occured for the test=$test\n");
    print " You may want to check the appropriate log file=$logFile.\n";
    print " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";

    $numFailed++;
  }
}

# -----main------------------------------------------------------------------------------------------

# redirect STDERR to be the same as STDOUT
open(STDERR,">&STDOUT" ) || die print "Can't dup STDERR as STDOUT\n";

$Overture = $ENV{"Overture"};

$fileName = "$Overture/configure.options";
open(FILE,"$fileName") || die print "unable to open $fileName. Check your Overture env variable.\n";
$precision="single";
$machine="unknown";
$parallel="";
while( <FILE> )
{
  if( /^double=Compiled in double precision/ ){ $precision="double";}
  if( /^machine=(.*)$/ ){ $machine=$1;}
  if( /^parallel=(.*)$/ ){ $parallel=$1;}
}
close(FILE);

print "*** check.p : machine=$machine, precision=$precision, parallel=$parallel\n";


$user = $ENV{"USER"};
$debug = "true";
$noplot="noplot";

# The different tests can be turned off or on here
$gridTests = "true";
$gridFromCad ="true";
$operatorTests ="true";
$rapTests = "true";

printf("\n");
printf("====================================================================================\n");
printf("  check.p : This perl script will run the Overture regression tests \n");
printf(" \n");
printf("    Usage: check.p [debug=true/false][grids=false][cadGrids=false][op=false][rap=false] \n");
printf("        debug=true/false : see more detailed results printed to the screen if true. \n");
printf("        grids=false    : turn off the test for grid generation.\n");
printf("        cadGrids=false : turn off the test for grid generation from CAD.\n");
printf("        op=false       : turn off the test for operators.\n");
printf("        rap=false      : turn off the test for rapsodi (CAD fixup tests)\n");
printf(" \n");
printf("==================================================================================\n\n");

foreach $arg ( @ARGV )
{
  if( $arg =~ "debug=(.*)" )
  {
    $debug=$1;
    print "Setting debug=$debug\n";
  }
  elsif( $arg =~ "plot" )
  {
    $noplot="";
  }
  elsif( $arg =~ "grids=false" )
  {
    $gridTests="false";
  }
  elsif( $arg =~ "cadGrids=false" )
  {
    $gridFromCad="false";
  }
  elsif( $arg =~ "op=false" )
  {
    $operatorTests="false";
  }
  elsif( $arg =~ "rap=false" )
  {
    $rapTests="false";
  }
}


$distribution = `pwd`;
chop($distribution);
$numTests=0;
$numFailed=0;

if( $gridTests eq "true" )
{
  printf("   ************************************************************************************************\n");
  printf("   *** Test: build grids : build a collection of overlapping grids in the sampleGrids directory ***\n");
  printf("   ************************************************************************************************\n");
  $test="buildGrids"; $logFile="";
  
  print ">>>cd $distribution/sampleGrids...\n";
  $rt = chdir("$distribution/sampleGrids");  $rt=$rt-1; checkError();
  
  $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
  $grid = ""; # = "cic"; 
  print ">>>make the sample grids: generate.p $grid $logFile\n";
  $rt = system("csh -f -c '$setEnvironment perl generate.p $grid $logFile'");  checkError();
  if( $rt eq 0 ) { print ">>>make sampleGrids successful!\n"; }
  $numTests++;
}

if( $gridFromCad eq "true" ) 
{
 if( $parallel ne "parallel" )
 {
  printf("   ************************************************************************************************\n");
  printf("   *** Test: grids from cad : build grids from CAD geometries (in the sampleGrids directory)    ***\n");
  printf("   ************************************************************************************************\n");
  $test="grids from cad";  $logFile="";
  
  print ">>>cd $distribution/sampleGrids...\n";
  $rt = chdir("$distribution/sampleGrids");  $rt=$rt-1; checkError();
  printf("Run $distribution/sampleGrids/gridsFromCad.p\n");
  $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
  $rt = system("csh -f -c '$setEnvironment perl gridsFromCad.p $logFile'");  checkError();
  
  if( $rt eq 0 ) { print ">>>gridsFromCad.p successful!\n"; }
  $numTests++;
 }
 else
 {
   print "\n ****** skipping grids from cad checks in parallel  ******\n\n";
 }
}

if( $operatorTests eq "true" )
{
  # *************  Test grid functions in the tests directory *********************
  printf("   ************************************************************************************************\n");
  printf("   *** Test: test operators and grid functions (in the tests directory)                         ***\n  ");
  printf("   ************************************************************************************************\n");
  $test="gridFunctions";  $logFile="";
  
  print ">>>cd $distribution/tests...\n";
  $rt = chdir("$distribution/tests");  $rt=$rt-1; checkError();
  
  $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
  print ">>>run checkop.p $logFile\n";
  $rt = system("csh -f -c '$setEnvironment perl checkop.p $logFile'");  checkError();
  
  if( $rt eq 0 ) { print ">>>tests of operators and grid functions successful!\n"; }
  $numTests++;
}

if( $rapTests eq "true" )
{
 if( $parallel ne "parallel" )
 {  
  # *************  run rap tests in the sampleMappings directory *********************
  printf("   ************************************************************************************************\n");
  printf("   *** Run the rapsodi tests (CAD fixup etc.) (in the sampleMappings directory)                 ***\n");
  printf("   ************************************************************************************************\n");
  $test="testRap";  $logFile="";
  
  
  print ">>>cd $distribution/sampleMappings...\n";
  $rt = chdir("$distribution/sampleMappings");  $rt=$rt-1; checkError();
  
  $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
  print ">>>run the rap tests: check.p and heal.p $grid $logFile\n";
  # 1st set of tests
  $rt = system("csh -f -c '$setEnvironment perl check.p $noplot $logFile'");  checkError();
  if( $rt == 0 ) { print "rap test check.p successful\n"; }
  # 2nd set of tests
  $rt = system("csh -f -c '$setEnvironment perl heal.p $noplot $logFile'");  checkError();
  
  if( $rt eq 0 ) { print ">>>rap tests successful!\n"; }
  $numTests++;
 }
 else
 {
   print "\n ****** skipping rapsodi checks in parallel ****** \n\n";
 }
}


printf("   ************************************************************************************************\n");
if( $numFailed == 0 )
{
  printf("   ****************** SUCCESS -- all $numTests tests were passed ***************************************** \n");
}
else
{
  $numPassed=$numTests-$numFailed;
  printf("   ****************** Tests: passed = $numPassed, failed=$numFailed ***************************************** \n");
}
printf("   ************************************************************************************************\n");

exit 0;

