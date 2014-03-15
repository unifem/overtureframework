eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/local/bin/perl
# perl program to check the proper generation of grids.
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will run ogen and create many different grids\n");
printf("It will check to see if the grids seem to be correctly generated.\n");
printf("  Usage: \n");
printf("    generate.p [options] \n");
printf(" Options \n");
printf("   <gridName> : the name of a single grid to check. By default check all grids.   \n");
printf("   check=<checkFileDirectory> : directory in which to look for the check files, default=. \n");
printf("   ogen=<name> : specify where the ogen executable is.   \n");
printf("   cmdFileDirectory=<dir> : directory where to find the command files.   \n");
printf("   -replace (replace check files with those currently generated)   \n");
printf("   -replaceAll (replace .dp and .sp check files with those currently generated)   \n");
printf("   -np=<num> : use this many processors when running in parallel  \n");
printf("==============================================================================\n\n");

# $fileName          = $ARGV[0];
$checkFileDirectory = ".";
$grid="all";
$replace="";
$ogen = "ogen";
$cmdFileDirectory=".";
$np=1; 

# check to see if we are comparing double or single precision

$Overture = $ENV{"Overture"};

$fileName = "$Overture/configure.options";
open(FILE,"$fileName") || die print "unable to open $fileName\n";
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

print "*** machine=$machine, precision=$precision, parallel=$parallel\n";

if( $precision eq "single" )
{
  $checkFileDirectory = "./check.sp";
}
else
{
  $checkFileDirectory = "./check.dp";
}
$parallel = $parallel eq "parallel" ? 1 : 0;

# print "ARGV = @ARGV\n";

foreach $arg ( @ARGV )
{
  if( $arg =~ /check=.*/ )
  {
    $checkFileDirectory = $arg;
    $checkFileDirectory =~ s/check=//;
  }
  elsif( $arg eq "-replace" || $arg eq "replace" )
  {
    $replace="replace";
  }
  elsif( $arg eq "-replaceAll" || $arg eq "replaceAll" )
  {
    $replace="replaceAll";
  }
  elsif( $arg =~ /ogen=(.*)/ )
  {
    $ogen=$1;
  }
  elsif( $arg =~ /cmdFileDirectory=(.*)/ )
  {
    $cmdFileDirectory=$1;
  }
  elsif( $arg =~ /-np=(.*)/ )
  {
    $np=$1;
  }
  else
  {
    $grid=$arg;
  }
}
# printf(" grid = $grid \n");
# printf(" checkFileDirectory = $checkFileDirectory \n");



# Here are the command files that we will run to check the grids. (if they exist in this directory)
if( $parallel == 0 )
{
  @cmdFiles=(
           "square5.cmd",
           "square5CC.cmd",
           "square10.cmd",
           "square20.cmd",
           "square40.cmd",
           "channelShort.cmd",
           "sis.cmd",
           "cic.cmd", 
           "cic2.cmd", 
           "cicCC.cmd",
           "cic.4.cmd",
           "cicmg.cmd",
           "cicAdd.cmd",
           "cilc.cmd",
           "qcic.cmd",
           "valve.cmd", 
           "valveCC.cmd",
           "oneValve.cmd",
           "obstacle.cmd",     # test fourth order
           "inletOutlet.cmd", 
           "inletOutletCC.cmd",
           "edgeRefinement.cmd",
           "naca0012.cmd",
           "naca.hype.cmd",
           "mismatch.cmd",
           "mismatchAnnulus.cmd",
           "end.cmd",
           "filletThree.cmd",
           "mastSail2d.cmd",
           "stir.cmd",
           "stirSplit.cmd",
           "hgrid.cmd",
           "cgrid.cmd",
           "twoBump.cmd",
           "innerOuterTest.cmd",
           "sinfoil.cmd",
           "twoSquaresInterface.cmd -factor=1 -name=\"twoSquaresInterface1.hdf\"",
           "box5.cmd",
           "box10.cmd",
           "box20.cmd",
           "box40.cmd",
           "bib.cmd",
           "sib.cmd",
           "twoBoxesInterface.cmd",
           "sibCC.cmd",
           "pipes.cmd",
           "pipesCC.cmd",
           "ellipsoid.cmd",
           "revolve.cmd",
           "revolveCC.cmd",
           "valve3d.cmd",
# ***           "valve3dCC.cmd",
           "sphereInATube.cmd",
           "tse.cmd",
           "valvePort.cmd",
           "filletTwoCyl.cmd",   
           "joinTwoCyl.cmd",
           "sub.cmd",
           "building3.cmd",
# hybrid grid tests
           "valve.hyb.cmd",
           "inletOutlet.hyb.cmd",
           "obstacle.hyb.cmd",
           "mastSail2d.hyb.cmd",
           "twoBump.hyb.cmd",
           "sib.hyb.cmd"
                 );

}
else
{
  # Tests for parallel 
  @cmdFiles=(
           "square5.cmd",
           "square5CC.cmd",
           "square10.cmd",
           "square20.cmd",
           "square40.cmd",
           "channelShort.cmd",
           "sis.cmd",
           "cic.cmd", 
           "cic2.cmd", 
           "cicCC.cmd",
           "cic.4.cmd",
#p           "cicmg.cmd",
           "cicAdd.cmd",
           "cilc.cmd",
           "qcic.cmd",
           "valve.cmd", 
           "valveCC.cmd",
           "oneValve.cmd",
#p            "obstacle.cmd",     # test fourth order
#p            "inletOutlet.cmd", 
#p            "inletOutletCC.cmd",
#p            "edgeRefinement.cmd",
#p            "naca0012.cmd",
#p            "naca.hype.cmd",
#p            "mismatch.cmd",
#p            "mismatchAnnulus.cmd",
#p            "end.cmd",
#p            "filletThree.cmd",
#p            "mastSail2d.cmd",
           "stir.cmd",
#p            "stirSplit.cmd",
#p            "hgrid.cmd",
#p            "cgrid.cmd",
           "twoBump.cmd",
#p            "innerOuterTest.cmd",
#p            "sinfoil.cmd",
#p            "twoSquaresInterface.cmd -factor=1 -name=\"twoSquaresInterface1.hdf\"",
           "box5.cmd",
           "box10.cmd",
           "box20.cmd",
           "box40.cmd",
           "bib.cmd",
           "sib.cmd",
           "twoBoxesInterface.cmd",
           "sibCC.cmd",
#p            "pipes.cmd",
#p            "pipesCC.cmd",
#p           "ellipsoid.cmd",
           "revolve.cmd",
#p            "revolveCC.cmd",
#p            "valve3d.cmd",
           "sphereInATube.cmd",
#p            "tse.cmd",
#p            "valvePort.cmd",
#p            "filletTwoCyl.cmd",   
#p            "joinTwoCyl.cmd",
#p            "sub.cmd",
#p            "building3.cmd",
#p # hybrid grid tests
#p            "valve.hyb.cmd",
#p            "inletOutlet.hyb.cmd",
#p            "obstacle.hyb.cmd",
#p            "mastSail2d.hyb.cmd",
#p            "twoBump.hyb.cmd",
#p            "sib.hyb.cmd"
                 );
}

$numberOfErrors = 0;

# look for ogen either in the current directory or ../bin/ogen
if( -e $ogen )
{
}
elsif( -e "../bin/ogen" )
{
  $ogen = "../bin/ogen";
}
else
{
  printf("ERROR: unable to find ogen!\n");
  exit 1;
}
printf("Using the grid generator $ogen\n");
if( $parallel == 1 )
{
  $ogen = "mpirun -np $np $ogen"; 
}


if( $grid ne "all" )
{
  @cmdFiles= ($grid);
}

foreach $cmdCommand ( @cmdFiles )
{
  $cmdName = $cmdCommand;
  $cmdName =~ s/(.*)\.cmd.*/\1/;

  $cmd = "$cmdFileDirectory/$cmdCommand";
  # printf(" cmdCommand=[$cmdCommand], cmdName=[$cmdName]\n");

  $check = "$checkFileDirectory/$cmdName.check";
  if( -e "$checkFileDirectory/$cmdName.$machine.check" )
  {
    # There is a special check file for this machine
    $check="$checkFileDirectory/$cmdName.$machine.check";
  }

  if( -e "$cmdName.cmd" )
  {
    printf("checking $cmdCommand...\n");
    if( $parallel eq 1 ){ printf("$ogen noplot nopause abortOnEnd $cmd > ogen.out\n"); }
    $returnValue = system "$ogen noplot nopause abortOnEnd $cmd > ogen.out";
    if( $returnValue == 0 )
    {
      if( -e "$check" )
      {
	$diff = system "diff ogen.check $check ";
	if( $diff!=0 )
	{
	  printf("++++ The check file for $cmdCommand (top) is not the same as the old file $check (bottom)++++\n");
	  $numberOfErrors++;
	}
	else
	{
	  printf("      ...$cmd appears to be correct (compared to $check)\n");
	}
      }
      if( $replace eq "replaceAll" )
      {
	system "cp ogen.check ./check.sp/$cmdName.check";
	system "cp ogen.check ./check.dp/$cmdName.check";
        printf(" replacing ./check.sp/$cmdName.check and ./check.dp/$cmdName.check\n");
      }
      elsif( !( -e "$check") || ($replace eq "replace") )
      {
	system "mv ogen.check $check";
	printf("Creating file $check \n");
      }
    }
    else
    {
      printf(" *** There was an error generating $cmdCommand ****\n");
      $numberOfErrors++;
    }
  }
  else 
  {
    printf("Command file [$cmdName.cmd] not found\n");
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
  printf("********************************************************************\n");
  print  "********************* There were $numberOfErrors  ERRORS *************************\n";
  print  "**** NOTE: some errors may occur due to differences in         *****\n";
  print  "**** machine precision. If the numbers are similiar then there *****\n";
  print  "**** is probably no reason for concern. (try plotting the grid)*****\n";
  printf("********************************************************************\n");

  exit $numberOfErrors;
}


exit


