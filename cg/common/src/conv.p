eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#
# perl program to run cg convergence tests
# 
# -conv= conv directory (and directory for output). .. So you can run the script outside the conv dir, e.g. for parallel
# 
#require "ctime.pl"; 

use Getopt::Long; use Getopt::Std;

$numberOfParameters = @ARGV;
if ($numberOfParameters eq 0)
{
  
  printf("\n");
  printf("================================================================================\n");
  printf("This perl script will run cg regression tests\n");
  printf("  Usage: \n");
  printf("    conv.p  -conv=<convDir> file \n");
  printf("  where \n");
  printf("    file = the name of a conv file that contains a list of cases to run \n");
  printf("    -conv= conv directory (and directory for output)\n");
  printf("==============================================================================\n\n");
  exit;
  
}

$convFile="";
$convDir =".";  # conv directory (and where we save results)
foreach $arg ( @ARGV )
{
  if( $arg =~ /-conv=(.*)/ )
  {
    $convDir = $1;
    printf("Using conv directory [%s]\n",$convDir);
  }
  else
  {
    $convFile=$arg;
    printf("conv.p: file=%s\n",$convFile);
    last;
  }
}

$debug=0; # set to 1 for debug info 
$caseName="junk";
$numberOfComponents=-1;
$numberOfDomains=1;      # cgmp can have multiple domains
@numberOfComponentsPerDomain = ();  # -- finish me ---
$baseGridSpacing=1.;     # grid spacing = $baseGridSpacing/$res 
$title="";
$caption="";
$count=0;  # counts the number of different grids that we check
$check=""; # name of the check file (e.g. "ins.check")
$cmd="";
$alwaysCompute=0; # if 1 always compute errors, otherwise look for computed results in a saved check file.
$outputFile=""; # name of final TeX file with errors and convergence rates
$closeFile="false";
#Longfei: ctime.pl is not supported in perl 5. use localtime() to get the date string
#$date=&ctime(time);  chop($date);  # here is the date
$date=localtime();  # new way to get $date

@ignoreComponent = (); # specify any components to ignore by setting ignoreComponent[$component]=1; 

# $errm[$count][$component] : save errors for each component here -- for output to a matlab file
# @resm[$count]             : resolution (h)

printf("Using the conv file [$convDir/$convFile]\n");


# The results file is passed to the convRate function to compute convergence rates
$results="$convDir/results"; 
open(RESULTS,">$results") || die print "unable to open $results\n";

# Here in the input file with commands to run
open(FILE,"$convDir/$convFile") || die print "unable to open $convDir/$convFile\n";


while( <FILE> )
{
  # concatenate continuation lines: 
  while( /\\$/ )
  {
    # printf("***** continuation line found\n");
    $cc = <FILE>;
    $cc =~ s/^[ ]*//; # remove leading blanks
    chop($_); chop($_);
    $_ = $_ . $cc;
  }

  if( /^\#/ ){ next; }  # skip comments
  if( /^exit/ ){ last; }  # exit

  $line=$_;

  # printf("conv.p: Input: line=[$line]\n");

  chop($line);

  $line =~ s/\#.*//;   # remove comments on the end of lines

  # eval the command in the file. 
  if( $line =~ /;/ )
  {  
    if( $debug > 0 ){ printf(" send to the perl parser: line=[$line]\n"); }
    eval($line);
  }

  # if $cmd has been set then assume it is a command to run a cg solver
  if( $cmd ne "" )
  {
    if( $count == 0 )
    {
      printf(" numberOfComponents=$numberOfComponents\n");
      printf(" title=[$title]\n");
      printf(" caption=[$caption]\n");

      print RESULTS "$title\n";
    }
    if( $debug > 0 ){ printf(" grid=[$grid], res=[$res]\n"); }

    $resm[$count]=$res;  # save $res

    #$cmd=$1;
    #eval($cmd);
    # printf(" cmd=[$cmd]\n");

    $count++;

    # $checkFileName="$caseName$count.check"; 
    $checkFileName="$convDir/$caseName.$grid.check"; 

    # ---If the check file is already there we use the values in it 
    if( $alwaysCompute eq 0 && (-e $checkFileName) )
    {
      printf("INFO: check file already exists, will use values from : $checkFileName\n");
      printf("INFO: set option \$alwaysCompute=1 to overide this. Current: $alwaysCompute\n");
      system("cp $checkFileName $convDir/$check");
    }
    else
    {
      printf(">> run [$cmd >! $convDir/$caseName.$grid.Case$count.out]\n");
      $startTime=time();

      $returnValue = system("csh -f -c 'nohup $cmd >! $convDir/$caseName.$grid.Case$count.out'");    

      $cpuTime=time()-$startTime;
      printf("...returnValue=%g, cpu=%g (s)\n",$returnValue,$cpuTime);

      if( $returnValue ne 0 )
      {
        printf("conv.p:ERROR return $returnValue from running the code. I am going to abort\n");
        exit 1;
      }

      printf("Saving check file as $checkFileName\n");
      system("cp $convDir/$check $checkFileName");
    }

    # NOTE: the cg check file is assumed to be in the current directory: 
    open(CHECKFILE,"$check") || die "cannot open file $check!" ;

    # read the last line in the check file: (for multi-domain problems we need the last "numberOfDomains" lines)
    while( <CHECKFILE> )
    {
      for( $j=$numberOfDomains-1; $j>0; $j-- ){ $checkLine[$j]=$checkLine[$j-1]; }
      $checkLine[0]=$_; chop($checkLine[0]);
    }
    close(CHECKFILE);
    
    # concatenate results from the different domains (chop off start of subsequent lines: these start with time ...)
    $line = $checkLine[$numberOfDomains-1]; 
    for( $j=$numberOfDomains-2; $j>=0; $j-- )
    { 
      $extra = substr($checkLine[$j],12);
      printf(" concatenate: add [$extra]\n");
      $line = $line . $extra;
    } 

    print "----------------------------------------------------------------------------------------------- \n";
    print "              t    nc c=0   err     max   c=1    err    max    c=2    err     max    c=3   err    max\n";
    $line1=$line;
    $line1 =~ s/  / /g;
    print " Results: $line1\n";
    print "---------------------------------------------------------------------------------------------- \n";


    @token =  split(' ',$line); 
    foreach $t (@token)
    {
      # print "token=$t\n";
    }
    $time=$token[0];
    

    print RESULTS "$grid \&  $res ";
    for( $j=0; $j<$numberOfComponents; $j++ )
    {
      if( $ignoreComponent[$j] ne 1 )
      {
        $error = $token[3+($j)*3];
        print RESULTS "\&  $error ";
        $errm[$count-1][$j] = $error;
      }
    }
    print RESULTS "\n";

    $cmd="";
  }

  
  if( $closeFile eq "true" )
  {
     # compute convergence rates and save the TeX file. 

     print RESULTS $caption, "\n";
     close(RESULTS);

     $CGBUILDPREFIX=$ENV{CGBUILDPREFIX};
     if( $CGBUILDPREFIX ne "" )
     {
       $cr = "$ENV{CGBUILDPREFIX}/common/bin/convRate";
     }
     else
     {
       $cr = "../../common/bin/convRate"; 
     }
     print "Now run `$cr $results -output=$convDir/$outputFile' to compute the convergence rates\n";
     $rt = system("$cr $results -output=$convDir/$outputFile");
     
     # initialize parameters for the next case
     system("mv $results $results.$caseName");
     print "Saving the results file as $results.$caseName\n";
     $closeFile="false";
     open(RESULTS,">$results") || die print "unable to open $results\n";

    # save results to a matlab file for plotting
    $mFile = $caseName;
    $mFile =~ s/\./_/g;   # matlab does not like "." in the name to change to _
    $mFile .= ".m";
    open(MFILE,">$convDir/$mFile") || die print "unable to open $convDir/$mFile\n";
    print MFILE "%\n% $caption\n%\n"; 
    print MFILE "hh = [";
    for( $ii=0; $ii<$count; $ii++ )
    {
      $hh[$ii] = $baseGridSpacing/$resm[$ii];
      print MFILE "$hh[$ii] ";
    }
    print MFILE "];\n";
    for( $j=0; $j<$numberOfComponents; $j++ )
    {
      if( $ignoreComponent[$j] ne 1 )
      {
        
        $error = $token[3+($j)*3];
        # $componentName[$j];
        if( $componentName[$j] eq "" ){ $componentName[$j] = "u$j"; }
        print MFILE "$componentName[$j] = [";
        for( $ii=0; $ii<$count; $ii++ )
        {
          print MFILE "$errm[$ii][$j] ";
        }
        print MFILE "];\n";
      }
    }
    print MFILE "% loglog(";
    @markers = ( "r-+" , "g-+" , "b-+" , "c-+", "r-o" , "g-o" , "b-o" , "c-o", "r-x" , "g-x" , "b-x" , "c-x" );
    $numMarkers=@markers;
    for( $j=0; $j<$numberOfComponents; $j++ )
    {
      if( $ignoreComponent[$j] ne 1 )
      {
        print MFILE "hh,$componentName[$j],'$markers[$j % $numMarkers]'";
        if( $j < $numberOfComponents-1 ){ print MFILE ","; }
      }
    }
    print MFILE ");\n";
    print MFILE "% legend(";
    for( $j=0; $j<$numberOfComponents; $j++ )
    {
      if( $ignoreComponent[$j] ne 1 )
      {
        print MFILE "'$componentName[$j]',";
      }
    }
    print MFILE "'Location','NorthWest');\n";
    print MFILE "%title('$caseName');\n";
    print MFILE "%ylabel('Errors');\n";
    print MFILE "%xlabel('h');\n";
    print MFILE "%set(gca,'XLim',[$hh[$count-1],$hh[0]]);\n";
    close( MFILE );
    print "Matlab results written to file [$convDir/$mFile].\n";

    $count=0;
  }

}


close(FILE);


exit;
# ===============================================================================================================


