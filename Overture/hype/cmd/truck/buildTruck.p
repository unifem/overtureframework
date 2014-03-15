eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/local/bin/perl
# perl program 
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will build the grids for the truck \n");
printf("  Usage: \n");
printf("    buildTruck.p [options] \n");
printf(" Options \n");
printf("   no options yet   \n");
printf("==============================================================================\n\n");

# $fileName          = $ARGV[0];
$checkFileDirectory = ".";
$grid="all";
$replace="";
$ogen = "ogen";
$cmdFileDirectory=".";

# check to see if we are comparing double or single precision

$Overture = $ENV{"Overture"};

$fileName = "$Overture/configure.options";
open(FILE,"$fileName") || die print "unable to open $fileName\n";
$precision="single";
while( <FILE> )
{
  if( /^double=Compiled in double precision/ ){ $precision="double";}
}
close(FILE);

print "*** precision=$precision\n";

foreach $arg ( @ARGV )
{
  if( $arg =~ /check=.*/ )
  {
    $checkFileDirectory = $arg;
    $checkFileDirectory =~ s/check=//;
  }
  elsif( $arg eq "replace" )
  {
    $replace="replace";
  }
  else
  {
    $grid=$arg;
  }
}


$hype = "/home/henshaw/Overture/hype/hype";

$logFile="buildTruck.out";

@commands = ( "createTruckCabNoWheels",
              "createCabWithoutWheels",
              "cabTop",
              "hood",
              "front",
              "windshield",
              "body",
              "tender",
              "backTender",
              "backCabTopEdge",
              "backCabBottomEdge",
              "backCabMiddleEdge",
              "leftCabCorner",
              "rightCabCorner",
              "frontLeftWheel",
              "frontRightWheel",
              "rearWheels",
              "cabGrids" );

foreach $cmd (@commands) 
{

  printf("Running: $hype noplot nopause abortOnEnd $cmd.cmd > $logFile...\n");
  $returnValue = system "$hype noplot nopause abortOnEnd $cmd.cmd > $logFile";

# printf("Running: ../../hypeNew noplot nopause abortOnEnd $cmd.new.cmd > $logFile...\n");
# $returnValue = system "../../hypeNew noplot nopause abortOnEnd $cmd.new.cmd > $logFile";

#  system "cp $cmd.cmd $cmd.old.cmd";
#  system "mv $cmd.new.cmd $cmd.cmd";
#  $returnValue = system "$hype nopause abortOnEnd $cmd > $logFile";
  if( $returnValue != 0 )
  {
    printf("ERROR running $cmd. Check the logfile $logFile\n");
    exit(1);
  }
  else
  {
    printf("...success\n");
  }
}

if ( $numberOfErrors == 0 )
{
  printf("=========================================================\n");
  printf("==== buildTruck.p : apparently successful         =======\n");
  printf("=========================================================\n");

  exit 0;
}
else
{
  printf("********************************************************\n");
  print  "***** buildTruck.p : There were $numberOfErrors ERRORS ******\n";
  printf("********************************************************\n");

  exit $numberOfErrors;
}


exit
