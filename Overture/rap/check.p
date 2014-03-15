eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl program to check the proper generation of grids.
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will run rap and create many different geometries\n");
printf("It will check to see if the geometry seem to be correctly generated.\n");
printf("  Usage: \n");
printf("    check.p [<geoName>] [noplot] [check=<checkFileDirectory>][special=<dir>}  (or `perl check.p') \n");
printf(" <checkFileDirectory> = directory in which to look for the check files, default=. \n");
printf(" <geoName> = the name of a single geometry to check. By default check all geometries. \n");
printf(" noplot = do not plot results. \n");
printf(" special = location of special files that are not distributed. \n");
printf("==============================================================================\n\n");

# $fileName          = $ARGV[0];
$checkFileDirectory = ".";
$grid="all";
$replace="";
$noplot="";
$specialDir="/home/henshaw/iges";  # here are where files are that we can't distribute

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
  elsif( $arg eq "noplot" )
  {
    $noplot="noplot";
  }
  elsif( $arg =~ /special=.*/ )
  {
    $specialDir = $arg;
    $specialDir =~ s/special=//;
  }
  else
  {
    $grid=$arg;
  }
}
# printf(" grid = $grid \n");
# printf(" checkFileDirectory = $checkFileDirectory \n");

# Here are the command files that we will run to check the grids. (if they exist in this directory)
@cmdFiles=(#kkc this test fails in triangle on the dec (gps09) "nozzleTest",
           "$specialDir/truck-trailer",
	   "$specialDir/manifold",
	   "$specialDir/ring",
	   "$specialDir/plate",
           "$specialDir/electrode",
	   "ship-5415",
	   "ship-kcs",
	   "ship-kvlcc",
	   "ship-5-parts",
	   "asmo",
           "$specialDir/cat",
           "$specialDir/volvo");

$numberOfErrors = 0;

# look for rap either in the current directory or ../bin/rap
if( -e "../bin/rap" )
{
  $rap= "../bin/rap";
}
else
{
  $rap = "rap";
}
printf("Using the geometry generator $rap\n");

if( $grid ne "all" )
{
  @cmdFiles= ($grid);
}

foreach $cmd ( @cmdFiles )
{
  if( -e "$cmd.cmd" )
  {
    printf("checking $cmd...\n");
    $returnValue = system "$rap $noplot abortOnEnd $cmd > rap.out";
    if( $returnValue == 0 )
    {
	printf("      ...$cmd could be read in and displayed\n");
    }
    else
    {
	printf(" *** There was an error while reading or displaying $cmd ****\n");
	$numberOfErrors++;
    }
  }
}

if ( $numberOfErrors == 0 )
{
  printf("==================================================\n");
  printf("============ Test apparently successful ==========\n");
  printf("==================================================\n");

  exit 0;
}
else
{
  printf("**************************************************\n");
  print  "************ There were $numberOfErrors  ERRORS ****************\n";
  printf("**************************************************\n");

  exit $numberOfErrors;
}


exit


