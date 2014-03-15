eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl regression tests for grid functions and operators
# 

printf("\n");
printf("================================================================================\n");
printf("This perl script will run some regression tests on grid functions and operators.\n\n");
printf("  Usage: \n");
printf("    checkop.p [<application>]  (or `perl checkop.p') \n");
printf("  Notes: \n");
printf(" Applications:                                                                \n");
printf("     tderivatives    : tests derivatives in the operators.          \n");
printf("     tbcc            : tests boundary conditions for coefficient matrices.   \n");
printf("     tbc             : tests explicit boundary conditions.                    \n");
printf("     tcm3            : tests coefficient matrix solver on a CompositeGrid    \n");
# printf("     tgf             : tests grids and grid functions.                        \n");
# printf("     cellFace        : test cell/face centred grid functions.           \n");
# printf("     tz              : tests twilight-zone functions.                         \n");
# printf("     testInterpolant : tests the overlapping grid interpolation.    \n");
# printf("     tcm             : tests coefficient matrix solver on a MappedGrid.      \n");
# printf("     tcm2            : tests coefficient matrix solver (systems) on a MappedGrid.  \n");
printf("     tcm4            : tests coefficient matrix solver  (systems) on a CompositeGrid    \n");
printf("==============================================================================\n\n");


$numberOfErrors = 0;


# check to see if we are comparing double or single precision

$Overture = $ENV{"Overture"};
$fileName = "$Overture/configure.options";
open(FILE,"$fileName") || die print "unable to open $fileName\n";
$precision="single";
$parallel="";
while( <FILE> )
{
  if( /^double=Compiled in double precision/ ){ $precision="double";}
  if( /^parallel=parallel/ ){ $parallel="parallel";}
}
close(FILE);
print "*** precision=$precision, parallel=$parallel ****\n";

if( $precision eq "single" )
{
  $precision = "sp";
}
else
{
  $precision = "dp";
}


# Here are the command files that we will run to check the grids. (if they exist in this directory)
@cmdFiles=(
           "tderivatives",     # fixed to be robust, I hope.
           "tbcc",
           "tbc",
           "tcm3",
#           "tgf",
#             "cellFace",
#             "tz",
#             "testInterpolant",
#             "tcm",
#             "tcm2",
           "tcm4"
          );

if( $parallel eq "parallel" )
{
  # Here are the tests we do in parallel (so far)
  @cmdFiles=(
             "tderivatives"     # fixed to be robust, I hope.
             #  "tcm3"    # needs PETSc for parallel -- fix me 
            );
}

# over-ride default tests from the command line
$argc = @ARGV;
if( $argc > 0 )
{
  @cmdFiles=@ARGV;
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
  $flags="";
  if( $cmd eq "tcm3" || $cmd eq "tcm4" ){ $flags = "-check"; }

  printf("Running: ./$cmd $flags > $cmd.out\n");
  $rt = system "./$cmd $flags > $cmd.out";
  if( $rt != 0 )
  {
    printf(" ERROR running the test program! return code=$rt\n");
    $numberOfErrors++;
  }
  elsif( -e "$cmd.$precision.check" )
  {
    $diff = system "perl smartDiff.p $cmd.$precision.check.new $cmd.$precision.check";
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


