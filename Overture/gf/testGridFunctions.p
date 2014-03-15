eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl regression tests for grid functions.
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will run some regression tests on grid functions and operators.\n\n");
printf("  Usage: \n");
printf("    testGridFunctions.p [<application>]  (or `perl testGridFunctions.p') \n");
printf("  Notes: \n");
printf(" Applications:                                                                \n");
printf("     tgf             : tests grids and grid functions.                        \n");
printf("     cellFace        : test cell/face centred grid functions.           \n");
printf("     tz              : tests twilight-zone functions.                         \n");
printf("     testInterpolant : tests the overlapping grid interpolation.    \n");
printf("==============================================================================\n\n");


$numberOfErrors = 0;

# Here are the command files that we will run to check the grids. (if they exist in this directory)
@cmdFiles=("tgf",
           "cellFace",
           "tz",
           "testInterpolant",
          );

$argc = @ARGV;
if( $argc > 0 )
{
  @cmdFiles=@ARGV;
}


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
  $precision = "sp";
}
else
{
  $precision = "dp";
}


$rt=0;
$numberOfErrors=0;

foreach $cmd (@cmdFiles)
{

  if( ! -e $cmd )
  {
    print "Making $cmd\n";
    $rt = system "make $cmd";
    if( $rt != 0 )
    {
      print "Error making $cmd \n";
      exit;
    }
  }

  print "running $cmd...\n";
  if( -e "$cmd.check.new" )
  {
    system "rm $cmd.check.new";
  }

  if( $cmd eq "tderivatives" ) # this one should have robust checking for sp and dp
  {
    # printf("$cmd > $cmd.out\n");
    $rt = system "./$cmd > $cmd.out";
    if( -e "$cmd.$precision.check" )
    {
      $diff = system "diff $cmd.$precision.check.new $cmd.$precision.check";
      if( $diff!=0 )
      {
        printf("++++ The new check file for $cmd (top) differs from the old file $cmd.$precision.check (bottom)++++\n");
        $numberOfErrors++;
      }
      else
      {
        printf("      ...$cmd appears to be correct\n");
      }
    }
    else
    {
      print "creating the check file $cmd.$precision.check. No checks were done for this case\n";
      system "mv $cmd.$precision.check.new $cmd.$precision.check";

    }
  }
  else
  {
    # Some tests do timings. We turn these off so we can compare output files
    # from one run to the next.
    if( $cmd eq "tbc" || $cmd eq "tbcc" || $cmd eq "testInterpolant" || $cmd eq "tcm3" || $cmd eq "tcm4" )
    {
      $rt = system "./$cmd -noTiming > $cmd.check.new";
    }
    else
    {
      $rt = system "./$cmd > $cmd.check.new";
    }
    print "               ...done\n";

    if( -e "$cmd.check" )
    {
      $diff = system "diff $cmd.check.new $cmd.check";
      if( $diff!=0 )
      {
	printf("++++ The check file for $cmd (top) is not the same as the old file $cmd.check (bottom)++++\n");
	$numberOfErrors++;
      }
      else
      {
	printf("      ...$cmd appears to be correct\n");
      }
    }
    else
    {
      print "creating the check file $cmd.check. No checks were done for this case\n";
      system "mv $cmd.check.new $cmd.check";
    }
  }
}

if ( $numberOfErrors == 0 )
{
  printf("==================================================\n");
  printf("============ Test apparently successful ==========\n");
  printf("==================================================\n");
}
else
{
  printf("**************************************************\n");
  print  "************ There were $numberOfErrors  ERRORS ****************\n";
  printf("**************************************************\n");
}

exit $numberOfErrors;


