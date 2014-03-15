eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/local/bin/perl
# perl program 
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will build some grids from CAD \n");
printf("It will check to see if the grids seem to be correctly generated.\n");
printf("  Usage: \n");
printf("    gridsFromCad.p [options] \n");
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

if( $precision eq "single" )
{
  $checkFileDirectory = "./check.sp";
}
else
{
  $checkFileDirectory = "./check.dp";
}


# print "ARGV = @ARGV\n";

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

$numberOfErrors = 0;

# look for ogen either in the current directory or $Overture/bin
if( -e $ogen )
{
}
elsif( -e "$Overture/bin/ogen" )
{
  $ogen = "$Overture/bin/ogen";
}
else
{
  printf("ERROR: unable to find ogen!\n");
  exit 1;
}
printf("Using the grid generator $ogen\n");
if( -e $rap )
{
}
elsif( -e "$Overture/bin/rap" )
{
  $rap = "$Overture/bin/rap";
}
else
{
  printf("ERROR: unable to find rap!\n");
  exit 1;
}
printf("Using rap version: $rap\n");
if( -e $mbuilder )
{
}
elsif( -e "$Overture/bin/mbuilder" )
{
  $mbuilder = "$Overture/bin/mbuilder";
}
else
{
  printf("ERROR: unable to find mbuilder!\n");
  exit 1;
}
printf("Using mbuilder version: $mbuilder\n");

#          1. ../bin/rap asmoNoWheels.cmd
#          2. ../bin/mbuilder asmoBody.cmd
#          3. ../bin/mbuilder asmoFrontWheel.cmd
#          4. ../bin/mbuilder asmoBackWheel.cmd
#          5. ../bin/ogen asmo.cmd

$logFile="gridsFromCad.out";

@commands = ( "$rap noplot nopause abortOnEnd asmoNoWheels.cmd > $logFile",
              "$mbuilder noplot nopause abortOnEnd asmoBody.cmd > $logFile",
              "$mbuilder noplot nopause abortOnEnd asmoFrontWheel.cmd > $logFile",
              "$mbuilder noplot nopause abortOnEnd asmoBackWheel.cmd > $logFile",
	      "$ogen noplot nopause abortOnEnd $Overture/sampleGrids/asmo.cmd > $logFile" );


foreach $cmd (@commands) 
{

  printf("Running: $cmd...\n");
  $returnValue = system "$cmd";
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

# Build the slac grid if the igs file exists
# if( -e "/usr/casc/overture/Overture/sampleMappings/pinvertedcoupler2.igs" )
# {
#   @commands = ("$mbuilder noplot nopause abortOnEnd createSlac2.cmd > $logFile",
#  	       "$ogen noplot nopause abortOnEnd $Overture/sampleGrids/slac2.cmd > $logFile" );
# 
# 
#   foreach $cmd (@commands) 
#   {
#     printf("Running: $cmd...\n");
#     $returnValue = system "$cmd";
#     if( $returnValue != 0 )
#     {
#       printf("ERROR running $cmd. Check the logfile $logFile\n");
#       exit(1);
#     }
#     else
#     {
#       printf("...success\n");
#     }
#   }
# 
# }


if ( $numberOfErrors == 0 )
{
  printf("=========================================================\n");
  printf("==== gridsFromCad.p : Tests apparently successful =======\n");
  printf("=========================================================\n");

  exit 0;
}
else
{
  printf("********************************************************\n");
  print  "***** gridsFromCad.p : There were $numberOfErrors ERRORS ******\n";
  printf("********************************************************\n");

  exit $numberOfErrors;
}


exit


