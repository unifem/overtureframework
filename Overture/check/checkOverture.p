eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/local/bin/perl
# perl program to check the proper generation of grids.
# 

require "ctime.pl";

sub checkError
{
  if( $rt != 0 )
  { 
    print "ERROR return rt=$rt\n";
    $result="FAIL"; recordResult();
    printf("\n An ERROR occured. NOTE that some error messages may appear above (out of order)\n");
    print "You may want to check the appropriate log file.\n";
    print "Use the following commands to set the enviromental variables:\n";
    print "alias module2 \'set args = (\!*); source /usr/casc/overture/Overture/bin/module\'\n";
    print "module2 $moduleArgs $OvertureDistribution\n";
    if( $package eq "OverBlown" ){ print "Building: $distribution\n";}
    exit 1;
  }
}

sub recordResult 
# Save results in the html file for $test=$result
{
  local($found,$fileName,$platform);

  $fileName = $testResultsFile;

  $platform=$moduleArgs;
  $platform =~ s/\./_/g;
  $platform =~ tr/a-z/A-Z/;

  if( $debug eq "true" )
  { 
    print "recordResult: record result for platform=[$platform], test=[$test] result=[$result]\n";
   }

  open(FILE,"$fileName") || die print "unable to open $fileName\n";
  open(NEW,">$fileName.new") || die print "unable to open $fileName.new\n";

  $date = &ctime(time);
  chop($date);
  $testResult="$test=$result";

  $found=0;
  while( <FILE> )
  {
    # testStatus[SUN_S_CC][build]=PASS;
    # print $_;
    if( /^[ ]*testStatus\[$platform\]\[$test\]/ )
    {
      $found=1;
      # $_ =~ s/$test=.*/$test=$result    user=[$user] date=[$date]/;
      s/^[ ]*testStatus\[$platform\]\[$test\].*/testStatus\[$platform\]\[$test\]=$result;/;
    }
    s/^comment\[$platform\].*/comment\[$platform\]=\'$user \[$date\]\';/;

    print NEW $_;
  }
  close(FILE);
  close(NEW);
  if( $found==0 )
  {
    print "recordResult:ERROR: test=$test not found in file=$fileName\n";
    $rt=1; 
    exit 1;
  }
  else
  {
    if( $debug eq "true" ){ print "update $fileName\n"; }
    system("mv $fileName $fileName.old");
    system("mv $fileName.new $fileName");
  }
  unlink("$fileName.new");
}


sub mySigIntCatcher
# signal catcher for interupts
{
  print ">>>Signal Int caught. Cleaning up\n";
  if( $test ne "" )
  {
    $SIG{'INT'} = 'DEFAULT'; # reset to avoid recursion
    $result="UNKNOWN";
    recordResult();  
  }
  exit 1;
}


# -----main------------------------------------------------------------------------------------------

# assign a signal catcher -- this doesn't work???
# $SIG{'HUP'} = 'mySigIntCatcher';
# $SIG{'QUIT'} = 'mySigIntCatcher';
$SIG{'INT'} = "mySigIntCatcher";
# $SIG{'KILL'} = 'mySigIntCatcher';
# $SIG{'ABRT'} = 'mySigIntCatcher';


# redirect STDERR to be the same as STDOUT
open(STDERR,">&STDOUT" ) || die print "Can't dup STDERR as STDOUT\n";

$package="Overture";
$build    = "false";   # build distribution from scratch if true
$makePackage = "true";
$runTests = "true";
$debug="false";
$sourceDistribution = "";
$noplot="";
$machine="";

$user = $ENV{"USER"};

# Here is the root directory where we build the various versions.

$checkdir = "/usr/casc_scratch/$user/Overture.checkout/Overture";
$OvertureCheckDirectory="/var/tmp/$user";

if( $OvertureCheckDirectory eq "" )
{
  $OvertureCheckDirectory = $HOME;
}

# ***** NOTE: we use the version of module in /usr/casc/overture/Overture/bin/module ******

# Here are the arguments to module
#    machine.precision.cxxCompiler[.cCompiler][.parallel]
#    machine = sun, dec, sgi, intel, ibm
#    precision = s (single) or d (double)
#    cxxCompiler = CC, cxx, gcc, kcc
#    cCompiler = cc, gcc
$moduleArgs = "dec.d.cxx"; 
# $moduleArgs = "dec.s.cxx"; 

# arguments for the Overture "configure" script
#    precision=single
#    
$configureArgs = "";
# $configureArgs = "precision=single";

foreach $arg ( @ARGV )
{
    if( $arg =~ "dir=(.*)" )
    {
      $OvertureCheckDirectory = $1;
      print "OvertureCheckDirectory=$OvertureCheckDirectory\n";
    }
    elsif( $arg =~ "package=(.*)" )
    {
      $package=$1;
      if( $package eq "OverBlown" )
      {
        $checkdir = "/usr/casc_scratch/$user/$package.checkout/$package";
      }
    }
    elsif( $arg =~ "module=(.*)" )
    {
      $moduleArgs=$1;
      print "moduleArgs=$moduleArgs\n";
    }
    elsif( $arg =~ "configure=(.*)" )
    {
      $configureArgs=$1;
      $configureArgs =~ s/\./ /g;
      print "configureArgs=$configureArgs\n";
    }
    elsif( $arg =~ "build=(.*)" )
    {
      $build=$1;
    }
    elsif( $arg =~ "make=(.*)" )
    {
      $makePackage=$1;
    }
    elsif( $arg =~ "runTests=(.*)" )
    {
      $runTests=$1;
      print "Setting runTests=$runTests\n";
    }
    elsif( $arg =~ "debug=(.*)" )
    {
      $debug=$1;
      print "Setting debug=$debug\n";
    }
    elsif( $arg =~ "sourceDistribution=(.*)" )
    {
      $sourceDistribution=$1;
      print "Setting sourceDistribution=$sourceDistribution\n";
    }
    elsif( $arg =~ "noplot" )
    {
      $noplot="noplot";
    }
    elsif( $arg =~ "machine=(.*)" )
    {
      $machine=$1;
      print "Setting machine=$machine\n";
    }
}

print "++++++> checkOverture. Setting machine=$machine\n";

if( $debug eq "true" || $#ARGV == 0 )
{
  printf("\n");
  printf("================================================================================\n");
  printf("checkOverture.p: This perl script will test the version Overture (or OverBlown) in the CVS repository\n");
  printf( "  It will compile Overture with various compilers and various options.\n");
  printf("  Usage: \n");
  printf("    checkOverture.p  [options] \n");
  printf(" options are: \n");
  printf("   dir=<check directory> : build and compile Overture in this directory \n");
  printf("   checkdir=<checked out version> : use this checked out out version of Overture \n");
  printf("   module=<args>   \n");
  printf("   configure=<args> (separate options by a '.')  \n");
  printf("   sourceDistribution=<name>   : use this empty source distribution (instead of the CVS repo) \n");
  printf("   build=true/false     : build the distribution \n");
  printf("   make=true/false      : make (compile) the distribution \n");
  printf("   runTests=true/false  : run tests   \n");
  printf("   package=[Overture/OverBlown] : specify the package to test, Overture is the default.  \n");
  printf("   machine=[name] : specify the name of the machine we are running on. \n");
  printf("   noplot     : do not plot during testing. \n");
  printf("   debug=true/false     \n");
  printf("==============================================================================\n\n");
}

$sourceTestResultsFile="/usr/casc/overture/Overture/testResults/buildTestResults.js";
$sourceDisplayTestResultsFile="/usr/casc/overture/Overture/testResults/displayBuildTestResults.html";

$fixTestResultsFile="false"; # sometimes we must create new test results files.
if( $package eq "Overture" )
{
  if( $sourceDistribution eq "" )
  {
    $testResultsFile="/usr/casc/overture/Overture/testResults/buildTestResults.js";
    $displayTestResultsFile="/usr/casc/overture/Overture/testResults/displayBuildTestResults.html";
  }
  else
  {
    # if we are not testing the repository, make a new html file to show results for this user
    $testResultsFile="/usr/casc/overture/Overture/testResults/buildTestResults.$user.js";
    $displayTestResultsFile="/usr/casc/overture/Overture/testResults/displayBuildTestResults.$user.html";
    $fixTestResultsFile="true";
  }
}
elsif( $package eq "OverBlown" )
{
  if( $sourceDistribution eq "" )
  {
    $testResultsFile="/usr/casc/overture/Overture/testResults/buildTestResults.$package.js";
    $displayTestResultsFile="/usr/casc/overture/Overture/testResults/displayBuildTestResults.$package.html";
  }
  else
  {
    # if we are not testing the repository, make a new html file to show results for this user
    $testResultsFile="/usr/casc/overture/Overture/testResults/buildTestResults.$package.$user.js";
    $displayTestResultsFile="/usr/casc/overture/Overture/testResults/displayBuildTestResults.$package.$user.html";
  }
  $fixTestResultsFile="true";

  # system("cp $sourceTestResultsFile $testResultsFile");
  # system("cp $sourceDisplayTestResultsFile $displayTestResultsFile");

  # to do: change the line var package= 'Overture'; in $displayTestResultsFile
}
else
{
  printf("testDistrubution:ERROR: unknown package=$package\n");
  exit(1);
}

if( $fixTestResultsFile eq "true" )
{
   # if we are not testing the repository, make a new html file to show results for this user
  if( -e $testResultsFile )
  {
    # if the test results file is already there but the master copy is newer then we make a new copy
    ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks)=stat($testResultsFile);
     # atime/mtime/ctime =time of last acces, modification or status change (in seconds since 1970
     # print("stat($testResultsFile): mode=[$mode] uid=[$uid] size=[$size] atime=[$atime] mtime=[$mtime] ctime=[$ctime]\n");
    ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtimes,$ctime,$blksize,$blocks)=stat($sourceTestResultsFile);
     # print("stat($sourceTestResultsFile): mode=[$mode] uid=[$uid] size=[$size] atime=[$atime] mtime=[$mtimes] ctime=[$ctime]\n");

     if( $mtimes > $mtime )
     {
       printf("***** The master file $sourceTestResultsFile is newer than $testResultsFile.\n");
       printf("  --> I am going to build a new copy.  The original file will be saved in $testResultsFile.old\n");
       system("cp $testResultsFile $testResultsFile.old");
       system("rm $testResultsFile");
     }
   }
   if( !( -e $testResultsFile) )
   {
     system("cp $sourceTestResultsFile $testResultsFile");

     # make a new copy of the html file, we need to change which .js file it includes
     open(FILE,"$sourceDisplayTestResultsFile") || die print "unable to open $sourceDisplayTestResultsFile\n";
     open(NEW,">$displayTestResultsFile") || die print "unable to open $displayTestResultsFile\n";
   
     while( <FILE> )
     { 
       s/buildTestResults\.js/$testResultsFile/; 
       s/subTitle=.*/subTitle='Testing from $sourceDistribution';/;
       s/var packageName=.*/var packageName='$package';/;
       print NEW $_; 
     }
     close(FILE);
     close(NEW);
   }
}
print ">>> **** Results will appear in: $displayTestResultsFile ******\n";


# Here is the name of the distribution where we will compile the package
$distribution="$OvertureCheckDirectory/$package.$moduleArgs";
# module always needs to know where Overture is:
$OvertureDistribution=$distribution;
$OvertureDistribution=~ s/$package/Overture/g;  # this assumes a certain naming convention
printf("***** OvertureDistribution=$OvertureDistribution\n");

if( $package eq "Overture" )
{
  $test="build";         $result="UNKNOWN"; recordResult(); 
  $test="buildGrids";    $result="UNKNOWN"; recordResult();
  $test="gridFunctions"; $result="UNKNOWN"; recordResult();
  # AP: new targets
  $test="configure";     $result="UNKNOWN"; recordResult();
  $test="buildRap";      $result="UNKNOWN"; recordResult();
  $test="testRap";       $result="UNKNOWN"; recordResult();
}
if( $package eq "OverBlown" )
{
   $test="configure";      $result="UNKNOWN"; recordResult();
   $test="buildOverBlown"; $result="UNKNOWN"; recordResult();  
}

# define the enviroment for when we run a "system" command
if( $moduleArgs =~ /^sun/ )
{
  # change path to find CC 4.2
  #   $setEnvironment = "set path = \(\".\" \"/opt/SUNWspro/SC4.2/bin\" \$path\); set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";
  # For the 5.2 compiler
  $setEnvironment = "set path = \(\".\" \"/opt/ws6/SUNWspro/bin\" \$path\); set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";
}
elsif( $moduleArgs =~ /^intel/ )
{
  # change path to find pgf77
#  $setEnvironment = "set path = \(\".\" \"/usr/apps/pgi/3.3/linux86/bin\" \$path\); set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";
# use gcc 3.2.2
#  $setEnvironment = "set path = \(\".\" \"/usr/apps/pgi/3.3/linux86/bin\" \"/usr/apps/gcc/3.2.2/bin\" \"/usr/local/pgi/linux86/bin\" \$path\); set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";

  # for tux:
  if( $machine =~ /^tux/ ){
    print "*** checkOverture.p : set path for tux ***\n";
    $setEnvironment = "set path = \(\".\" \"/usr/apps/pgi/5.1/linux86/5.1/bin\" \"/usr/apps/gcc/3.4.3/bin\" \"/usr/local/pgi/5.1/linux86/5.1/bin\" \$path\); set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module";

  }
  else{
    # for ilx
    print "*** checkOverture.p : set path for ilx ****\n";
    $setEnvironment = "set path = \(\".\"  \"/usr/local/bin\" \$path\); set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module; ";
  }
  # set the license server:
  if( $machine =~ /^tux/ ){
    $setEnvironment = $setEnvironment ."; source /usr/apps/pgi/default/setup.csh;";
  }
  else
  {
    $setEnvironment = $setEnvironment ."; source /usr/local/pgi5/linux86/5.1/bin/startpgi.csh; ";
  }
}
elsif( $moduleArgs =~ /^sgi/ )
{
  # change path to pick up the CC compiler
  $setEnvironment = "set path = \(\"/usr/local/totalview/5.0.0-1/bin\" \"/usr/local/MIPSpro/bin\" \"/usr/local/MIPSpro/7.3.1.2/usr/bin\" \$path\); which CC; CC -version; echo \"path=\$path\"; set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";
}
elsif( $moduleArgs =~ /^dec/ )
{
  # perl is found in /usr/local/bin
  $setEnvironment = "set path = \(\".\" \"/usr/local/bin\" \$path\); echo \"path=\$path\"; set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";
}
else
{
  $setEnvironment = "set path = \(\".\" \$path\); echo \"path=\$path\"; set args = \(\"$moduleArgs\" \"$OvertureDistribution\"\); source /usr/casc/overture/Overture/bin/module;";
}


# ******** as the first test we configure the distribution *************
$test="configure";  $result="COMPUTING"; recordResult();

if( $sourceDistribution eq "" ) 
{
  if( $build eq "true" )
  {
    # build a distribution from the version checked out in $checkdir

    print ">>>rm -rf $distribution\n";
    $rt = system("rm -rf $distribution"); checkError();

    print ">>>cd checkdir:  $checkdir...\n";
    $rt = chdir("$checkdir");  $rt=$rt-1; checkError();

    $logFile = "> build.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>build a distribution: build$package $distribution $logFile\n";
    $rt = system("csh -f -c '$setEnvironment; echo \"hi\"; ./build$package $distribution $logFile'"); checkError();
  }
}
else
{
  # build a distribution from a supplied source distribution
  if( $build eq "true" )
  {
    print ">>>rm -rf $distribution\n";
    $rt = system("rm -rf $distribution"); checkError();

    print ">>>copy the source distribution in directory $sourceDistribution to $distribution\n";
    $rt = system("cp -rf $sourceDistribution $distribution"); checkError();
  }
}


print ">>>cd distribution: $distribution...\n";
$rt = chdir("$distribution");  $rt=$rt-1; checkError();

if( $build eq "true" )
{
  $logFile = "> configure.log";   if( $debug eq "true"){  $logFile=""; }
  print ">>>configure with: $setEnvironment perl configure $configureArgs $logFile\n";

  $rt = system("csh -f -c '$setEnvironment; perl configure $configureArgs $logFile'");  checkError();
  # printf("After configure: rt=$rt\n");

}
print ">>>Configuration successful!\n";
$result="PASS";
recordResult();

$make = "make";
if( ($moduleArgs =~ /^intel/) )
{
  $make = "make -j4"; # make with more processors on some machines.
  # $make = "make"; # make with more processors on some machines.
}
if( ($moduleArgs =~ /^dec/ ) )
{
  $make = "gmake -j2"; # make with more processors on some machines.
}
if( ($moduleArgs =~ /^sun/ ) )
{
  $make = "gmake -j2"; # make with more processors on some machines.
}

if( $makePackage eq "true" )
{
  if( $package eq "Overture" )
  {
    $logFile = ">&make.rapsodi.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>/usr/casc/overture/Overture/bin/module $moduleArgs $distribution\n";
    print ">>>$make rapsodi $logFile\n";
    print ">>>setEnvironment: $setEnvironment\n";

    # AP: Start by making the rapsodi library and rap
    $test="buildRap";  $result="COMPUTING"; recordResult();

    $rt = system("csh -f -c '$setEnvironment $make rapsodi $logFile '");  checkError();

    print ">>>Rapsodi build successful! (rt=$rt)\n";

    $result="PASS";
    recordResult();

    # Now make Overture
    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>$make $logFile\n";
    $test="build";  $result="COMPUTING"; recordResult();
    $rt = system("csh -f -c '$setEnvironment $make $logFile '");  checkError();
  
    # now make the primer examples
    print ">>>cd $distribution/primer...\n";
    $rt = chdir("$distribution/primer");  $rt=$rt-1; checkError();
    $rt = system("csh -f -c '$setEnvironment $make $logFile '");  checkError();

    # now make examples in the tests directory
    print ">>>cd $distribution/tests...\n";
    $rt = chdir("$distribution/tests");  $rt=$rt-1; checkError();
    $rt = system("csh -f -c '$setEnvironment $make $logFile '");  checkError();
  }
  if( $package eq "OverBlown" )
  {
    # Make OverBlown
    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>$make $logFile\n";
    $test="buildOverBlown";  $result="COMPUTING"; recordResult();
    $rt = system("csh -f -c '$setEnvironment $make $logFile '");  checkError();
  }
}
print ">>>Build successful!\n";
$result="PASS";
recordResult();

if( $package eq "Overture" )
{
  if( $runTests eq "true" )
  {
    # *************  build the grids in the sampleGrids directory *********************
    $test="buildGrids"; $result="COMPUTING"; recordResult();

    print ">>>cd $distribution/sampleGrids...\n";
    $rt = chdir("$distribution/sampleGrids");  $rt=$rt-1; checkError();

    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    $grid = ""; # = "cic"; 
    print ">>>make the sample grids: generate.p $grid $logFile\n";
    $rt = system("csh -f -c '$setEnvironment perl generate.p $grid $logFile'");  checkError();
    print ">>>make sampleGrids successful!\n";

    # --- test 2 in the sampleGrids directory: build grids from CAD
    printf("Run $distribution/sampleGrids/gridsFromCad.p\n");
    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    $rt = system("csh -f -c '$setEnvironment perl gridsFromCad.p $logFile'");  checkError();

    print ">>>gridsFromCad.p successful!\n";


    $result="PASS"; recordResult();


    # *************  Test grid functions in the tests directory *********************
    $test="gridFunctions"; $result="COMPUTING"; recordResult();
    print "***Now test operators and grid functions...\n";
    print ">>>cd $distribution/tests...\n";
    $rt = chdir("$distribution/tests");  $rt=$rt-1; checkError();

    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>run checkop.p $logFile\n";
    $rt = system("csh -f -c '$setEnvironment perl checkop.p $logFile'");  checkError();

    print ">>>tests of operators and grid functions successful!\n";
    $result="PASS"; recordResult();

    # *************  run rap tests in the sampleMappings directory *********************
    $test="testRap"; $result="COMPUTING"; recordResult();

    print ">>>cd $distribution/sampleMappings...\n";
    $rt = chdir("$distribution/sampleMappings");  $rt=$rt-1; checkError();

    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>run the rap tests: check.p and heal.p $grid $logFile\n";
# 1st set of tests
    $rt = system("csh -f -c '$setEnvironment perl check.p $noplot $logFile'");  checkError();
    if( $rt == 0 ) { print "rap test check.p successful\n"; }
# 2nd set of tests
    $rt = system("csh -f -c '$setEnvironment perl heal.p $noplot $logFile'");  checkError();

    print ">>>rap tests successful!\n";
    $result="PASS"; recordResult();

  }
  else
  {
    $test="buildGrids";    $result="NA"; recordResult();
    $test="gridFunctions"; $result="NA"; recordResult();
    $test="testRap";       $result="NA"; recordResult();
  }
}

if( $package eq "OverBlown" )
{
  if( $runTests eq "true" )
  {
    # *************  Test OverBlown *********************
    $test="testOverBlown"; $result="COMPUTING"; recordResult();

    print ">>>cd $distribution/check...\n";
    $rt = chdir("$distribution/check");  $rt=$rt-1; checkError();

    $logFile = ">&make.log";   if( $debug eq "true"){  $logFile=""; }
    print ">>>run the OverBlown regression tests in the check directory\n";
    print ">>>check.p $logFile\n";
    $rt = system("csh -f -c '$setEnvironment perl check.p $logFile'");  checkError();

    print ">>>OverBlown regression tests successful!\n";
    $result="PASS"; recordResult();
  }
}

exit 0;

