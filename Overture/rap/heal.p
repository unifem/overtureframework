eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl program to check the proper generation of grids.
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will run rap and heal a few different geometries\n");
printf("It will check to see if the healing commands seem to work.\n");
printf("  Usage: \n");
printf("    heal.p [<geoName>] [noplot][check=<checkFileDirectory>][special=<dir>}  (or `perl heal.p') \n");
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
# Unfortunately, we are only allowed to distribute the ship-5415-fix
@cmdFiles=("$specialDir/plate-fix",
           "$specialDir/electrode-fix",
	   "ship-5415-fix",
           "$specialDir/cat-fix",
           "$specialDir/volvo-fix");

$numberOfErrors = 0;
$numberOfBroken = 0;

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
    system("rm -f rap.log");
    $returnValue = system "$rap $noplot abortOnEnd $cmd > rap.out";
    if( $returnValue == 0 )
    {
	system("rm -f lastline.log");
	system("tail -1 rap.log > lastline.log");
	open(FILE,"lastline.log");
	$line = <FILE>;
	$line =~ /(\w+)\W+(\w+)\W+(\w+)/;
	close(FILE);
	if( $3 == 0 )
	{
	    printf("      ...$cmd could be read in and healed\n");
	}
	else
	{
	    printf("      ...$3 broken surfaces\n");
	    $numberOfErrors++;
	    $numberOfBroken+= $3;
	}
    }
    else
    {
	printf(" *** There was an error while executing rap $cmd ****\n");
	$numberOfErrors++;
    }
  }
}

if ( $numberOfErrors == 0 )
{
  printf("==================================================\n");
  printf("============ Test apparently successful ==========\n");
  print  "============ with $numberOfBroken broken surfaces     ==========\n";
  printf("==================================================\n");

  exit 0;
}
else
{
  printf("**************************************************\n");
  print  "************ There were $numberOfErrors failing tests **********\n";
  print  "************ and $numberOfBroken broken surfaces      **********\n";
  printf("**************************************************\n");

  exit $numberOfErrors;
}


exit


