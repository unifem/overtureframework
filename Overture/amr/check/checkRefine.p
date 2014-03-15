eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/local/bin/perl
# perl program to check the refine program for updating overlapping AMR grids
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will run the regression tests for updating overlapping AMR grids.\n");
printf("  Usage: \n");
printf("    checkRefine.p [options] \n");
printf(" Options \n");
printf("   <file.cmd> : the name of a single command file to check. By default check all.   \n");
printf("   check=<checkFileDirectory> : directory in which to look for the check files, default=. \n");
printf("   refine=<name> : specify where the refine executable is.   \n");
printf("   cmdFileDirectory=<dir> : directory where to find the command files.   \n");
printf("   replace (replace check files with those currently generated)   \n");
printf("   replaceAll (replace .dp and .sp check files with those currently generated)   \n");
printf("   np=<> : number of processors to use in parallel   \n");
printf("==============================================================================\n\n");

# $fileName          = $ARGV[0];
$checkFileDirectory = ".";
$grid="all";
$replace="";
$refine = "refine";  
$cmdFileDirectory=".";
$np=1;  # number of processors to use in parallel

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
  if( /^parallel=parallel/ ){ $parallel="parallel";}
}
close(FILE);

print "*** machine=[$machine], precision=[$precision], parallel=[$parallel]\n";

if( $precision eq "single" )
{
  $checkFileDirectory = "./check.sp";
}
else
{
  $checkFileDirectory = "./check.dp";
}
$mpirun ="mpirun";

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
  elsif( $arg eq "replaceAll" )
  {
    $replace="replaceAll";
  }
  elsif( $arg =~ /refine=(.*)/ )
  {
    $refine=$1;
  }
  elsif( $arg =~ /cmdFileDirectory=(.*)/ )
  {
    $cmdFileDirectory=$1;
  }
  elsif( $arg =~ /np=(.*)/ )
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
@cmdFiles=("cic.refine",
           "sis.refine",
           "cicp.refine",
           "bib.refine",
          # "rbib.refine",
                 );

$numberOfErrors = 0;

# look for refine either in the current directory or ../bin/refine
if( ! -e $refine )
{
  print "Making $refine\n";
  $rt = system "make $refine";
  if( $rt != 0 )
  {
    print "Error making $refine \n";
    exit;
  }
}

if( $grid ne "all" )
{
  @cmdFiles= ($grid);
}
$mpirunCommand ="";
foreach $cmdName ( @cmdFiles )
{
  $cmd = "$cmdFileDirectory/$cmdName";

  $check = "$checkFileDirectory/$cmdName.check";
  if( -e "$checkFileDirectory/$cmdName.$machine.check" )
  {
    # There is a special check file for this machine
    $check="$checkFileDirectory/$cmdName.$machine.check";
  }

  if( -e "$cmd.cmd" )
  {
    if( $parallel ne "" )
    {
      $mpirunCommand = "$mpirun -np $np ";
    }
    
    printf("checking $cmd.cmd...\n");
    $returnValue = system "$mpirunCommand $refine -noplot $cmd >\! refine.out";
    if( $returnValue == 0 )
    {
      if( -e "$check" )
      {
	$diff = system "diff refine.check $check ";
	if( $diff!=0 )
	{
	  printf("++++ The check file for $cmd (top) is not the same as the old file $check (bottom)++++\n");
	  $numberOfErrors++;
	}
	else
	{
	  printf("      ...$cmd appears to be correct (compared to $check)\n");
	}
      }
      if( $replace eq "replaceAll" )
      {
	system "cp refine.check ./check.sp/$cmdName.check";
	system "cp refine.check ./check.dp/$cmdName.check";
        printf(" replacing ./check.sp/$cmdName.check and ./check.dp/$cmdName.check\n");
      }
      elsif( !( -e "$check") || ($replace eq "replace") )
      {
	system "mv refine.check $check";
	printf("Creating file $check \n");
      }
    }
    else
    {
      printf(" *** There was an error generating $cmd ****\n");
      $numberOfErrors++;
    }
  }
  else 
  {
    printf("Command file [$cmd] not found\n");
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
  printf("********************************************************************\n");

  exit $numberOfErrors;
}


exit


