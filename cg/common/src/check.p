eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl program to run regression tests
# 


# $fileName          = $ARGV[0];
$checkFileDirectory = ".";
$testName="all";
$testsFile="tests"; # location of tests
$replace="";
$verbose=1; 
$solver=""; 
$mpirunCommand="mpirun -np %NP";  # for running in parallel

foreach $arg ( @ARGV )
{
  if( ($arg =~ /-check=.*/) || ($arg =~ /check=.*/) )
  {
    $checkFileDirectory = $arg;
    $checkFileDirectory =~ s/check=//;
  }
  elsif( $arg =~ /-solver=(.*)/ )
  {
    $solver=$1;
  }
  elsif( $arg =~ /-test=(.*)/ )
  {
    $testName=$1;
  }
  elsif( ($arg eq "-replace") || ($arg eq "replace") )
  {
    $replace="replace";
  }
  elsif( $arg =~ /-verbose=(.*)/ )
  {
    $verbose=$1;
  }
  elsif( $arg =~ /-testsFile=(.*)/ )
  {
    $testsFile=$1;
  }
  else
  {
    $testName=$arg;
  }
}
if( $verbose ){
printf("\n");
printf("================================================================================\n");
printf("This perl script will run regression tests \n");
printf("      (The lists of tests is in the file named 'tests') \n");
printf("  Usage: \n");
printf("    check.p -test=testName -replace -check=<checkFileDirectory> -solver=name -verbose=<num> -testsFile=<s>\n");
printf(" -test=testName : name of a particular test to run (by default do all). \n");
printf(" -replace : replace check files with the new ones computed. \n");
printf(" -verbose : verbose=0 : print very little, verbose=1 print more. \n");
printf(" -testsFile : file that holds list of tests. \n");
printf("==============================================================================\n\n");
}

# printf(" grid = $grid \n");
# printf(" checkFileDirectory = $checkFileDirectory \n");

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
# print "*** precision=$precision, parallel=$parallel ****\n";

$tol=1.e-5; # error tolerance for comparing files -- this could vary with each test

# Read the file $testsFile (default "tests") to get a list of the command files that we will run. 
#   (if the command files exist in the check directory)
#  Parallel tests are normally defined in the file "parallel_tests.pm"
#
$program = "none";
if( $parallel eq "" || !( -e "parallel_tests.pm") )
{
  # serial regressions tests
  # $fileName = "tests";
  $fileName = $testsFile;
  open(FILE,"$fileName") || die print "unable to open $fileName\n";
  $n=-2;
  while( <FILE> )
  {
    if( /^\#/ ){ next; }  # skip comments
    $line = $_;
    chop($line);
    $line =~ s/[ ]*\#.*//;    # remove trailing blanks and comments
    if( $n == -2 )
    { 
       $program = $line; 
       if( $verbose ){ print " program=$program\n";}
    }   
    elsif( $n==-1 )
    { 
       $checkFile = $line; 
       if( $verbose ){ print " checkFile=$checkFile\n"; }
    }   
    else
    {
      # $line =~ s/ //g;
      if( $line ne "" )
      {
        $cmdFiles[$n]=$line;


        # print "check.p: cmdFile[$n] = $line,  testName=$testName\n";

        if( ( $testName ne "all" ) && $line =~ /^$testName/ )
        {
          print "***check.p: run this test: $line\n";
	  $testName=$line;
          last;
        }
      }
    }
    $n++;
  }
  close(FILE);

}
else
{
    # the following will find and import the variables in parallel_tests.pm (cmdFiles and numProcs)
  print "check.p: Looking in file `parallel_tests.pm for tests...\n";
  eval("use parallel_tests");
}

$numberOfErrors = 0;

# look for executable
# $program = "../bin/cgins";
$CGBUILDPREFIX = $ENV{"CGBUILDPREFIX"};
if( $program =~ /CGBUILDPREFIX/ )
{
  if( $CGBUILDPREFIX ne "" )
  {
    $program =~ s/CGBUILDPREFIX/$CGBUILDPREFIX/;
  }
  else
  {
    $program =~ s/CGBUILDPREFIX/..\/../; 
  }
  if( $verbose ){ printf(" Using program = $program\n"); }
}

if( !( -e "$program") )
{
  printf("ERROR: unable to find program=$program. Maybe you need to make it.\n");
  exit 1;
}
$checkCheckFiles = "../../common/bin/checkCheckFiles";
if( !( -e $checkCheckFiles ) )
{
  $checkCheckFiles = "../bin/checkCheckFiles";
  if( !( -e $checkCheckFiles ) )
  {
      $checkCheckFiles = "$CGBUILDPREFIX/common/bin/checkCheckFiles";
      if ( !( -e $checkCheckFiles ) )
      {
	  print "Making checkCheckFiles\n";
	  $rt = system "make checkCheckFiles";
	  if( $rt != 0 )
	  {
	      print "Error making checkCheckFiles \n";
	      exit;
	  }
	  $checkCheckFiles = "./checkCheckFiles";
	  if( !( -e $checkCheckFiles ) )
	  {
	      printf("ERROR: unable to find or make the program checkCheckFiles. Maybe you need to make it.\n");
	      exit 1;
	  }
      }
  }
}

if( $verbose ){ printf("Using the executable:  $program\n"); }

if( $testName ne "all" )
{
  @cmdFiles= ($testName);
}

$m=-1;
foreach $cmdCommand ( @cmdFiles )
{
  # Commands can be of two forms:
  #    commandFileName
  #    checkFileName.check commandFileName.cmd [arg1] [arg2] ...
  # The second form can be use to pass arguments to a command file
  # 
  if( $cmdCommand eq "exit" ){ last; }  # An exit in the script was found

  $m++;

  # In parallel we name the check files with the number of processors used: 'file.np2.check'
  $np=$numProc[$m];
  if( $np eq "" ){ $np=1; }
  $checkFileSuffix="";
  if( $parallel ne "" ){ $checkFileSuffix=".np$np"; }

  $checkFileName="$cmdCommand$checkFileSuffix.check";   # default name for the check file 
  $runTimeOutput="$cmdCommand$checkFileSuffix";     # default name for the run-time output is "$runTimeOutput.out"
  if( $cmdCommand =~ /\.check/ )
  {
    $checkFileName=$cmdCommand;
    $checkFileName =~ s/(.*)\.check.*/\1$checkFileSuffix.check/;   # check file name comes first (if it is there)
    $cmdCommand =~ s/(.*)\.check[ ]*//;            # remove the check file name
    $runTimeOutput=$checkFileName;
    $runTimeOutput=~ s/\.check[ ]*//;
  }
  $cmdName = $cmdCommand;
  if( $cmdName =~ /\.cmd/ )
  {
    $cmdName =~ s/(.*)\.cmd.*/\1/;   # command file name 
  }
  $checkFilePrefix=$checkFileName;
  $checkFilePrefix =~ s/(.*)\.check.*/\1/;   # check file name without ".check"

  # printf(" cmdName=[$cmdName], checkFileName=[$checkFileName], command=[$cmdCommand], \n");

  $cmd = $cmdCommand;              # full command line with arguments

  if( $checkFileDirectory ne "" )
  {
    $cmd = "$checkFileDirectory/$cmd";
  }
  
  # some tests only work in double precision
  if( $cmdName =~ /\.dp$/ )
  {
    if( $precision eq "single" )
    {
      next;
    }
    $cmdName =~ s/\.dp$//;
    $cmd =~ s/($cmdName)\.dp$/\1/;
  }

  if( -e "$cmdName.cmd" )
  {
    # printf("checking $cmdName.cmd ...(output saved to file $cmdName.out)\n");

    $moduleArgs = "g";
    $OvertureDistribution="/home/henshaw/Overture.g";

#    $setEnvironment = "set path = \(\".\" \"/usr/apps/pgi/3.3/linux86/bin\" \$path\); echo \"path=\$path\"; set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /home/henshaw/bin/module;";
    $setEnvironment = "echo \"path=\$path\"; echo \"\$Overture\"; printenv LD_LIBRARY_PATH; set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /home/henshaw/bin/module;";

    if( $parallel eq "" )
    {
      if( $verbose ){ printf("running:  $program noplot $cmd >! $runTimeOutput.out\n"); }
      $returnValue = system("csh -f -c '$program noplot $cmd >! $runTimeOutput.out'");
    }
    else
    {
      # replace %NP in the $mpirunCommand with the actual number of processors $np
      $mpirunCommandNP = $mpirunCommand;
      $mpirunCommandNP =~ s/\%NP/$np/;
      if( $verbose ){ printf("running: $mpirunCommandNP $program noplot $cmd >&! $runTimeOutput.out\n"); }
      $returnValue = system("csh -f -c '$mpirunCommandNP $program noplot $cmd>&! $runTimeOutput.out'");
    }

    if( $returnValue == 0 )
    {
      if( (-e $checkFileName ) && ($replace eq "")  )
      {
        $pipeOutput = $verbose ? "" : ">! junk"; 

        $diff = system "$checkCheckFiles $checkFile $checkFileName -tol=$tol $pipeOutput";

	if( $diff!=0 )
	{
	  if( $verbose ){ printf("++++ The check files do not agree +++++\n"); }
	  $numberOfErrors++;
	}
	else
	{
	  if( $verbose ){ printf("      ... \"$checkFilePrefix\" appears to be correct\n"); }
	}
      }
      else
      {
 	system "cp $checkFile $checkFileName";
	if( $verbose ){ printf("      ...creating file $checkFileName \n"); }
      }
    }
    else
    {
      if( $verbose ){ printf(" *** There was an error running $program and generating $checkFilePrefix ****\n"); }
      $numberOfErrors++;
    }
  }
  else
  {
    printf("check.p: ERROR: The command file $cmdName.cmd not found!\n");
  }
}

if ( $numberOfErrors == 0 )
{
  printf("========================================================\n");
  printf("============ $solver: Tests apparently successful =========\n");
  printf("========================================================\n");
  exit 0;
}
else
{
  printf("********************************************************\n");
  print  "************ $solver: There were $numberOfErrors  ERRORS ****************\n";
  printf("********************************************************\n");

  exit 1;
}


