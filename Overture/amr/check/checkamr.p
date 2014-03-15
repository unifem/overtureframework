eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
#
#    *****************************************************
#    *********** check program for amrh ******************
#    ****** Compute errors for grids with AMR    *********
#    *****************************************************
# 
# Parallel Examples:
#          checkamr.p -np=1 square
#          checkamr.p -N=1 -n=1 square
#          checkamr.p -N=2 -n=8 sib

sub getResults # ($s)
# ========================================================================================
#   parse results from a file
# ========================================================================================
{

  local($i,$check,$line);

  $i = $_[0];
  $check = $_[1]; 
  open(FILE,"$check") || die "cannot open file $check!" ;

  $line = <FILE>;
  $line = <FILE>;
  $line =~ /\s*(\S*)\s*(\S*)\s*(\S*)\s*(\S*)\s*(\S*)\s*(\S*)/;  # extract time error gridPoints cpu
  $time[$i]=$1;
  $error[$i]=$2;
  $gridPoints[$i]=$3;
  $cpu[$i]=$4;
  $et[$i]=$5;  # errorThreshold
  $nb[$i]=$6;  # number of buffer zones
  print  "$check: time=$time[$i] error=$error[$i] gridPoints=$gridPoints[$i] cpu=$cpu[$i] ",
          "errorThreshold$et[$i] nb=$nb[$i]\n";

  printf CHECKFILE "<<%s>>  error=%8.1e\n",$check,$error[$i];

  close(FILE);
}


printf("\n");
printf("================================================================================\n");
printf("This perl script will run some tests for AMR.\n\n");
printf("  Usage: \n");
printf("    checkamr.p [-np=<num>] [-N=<num> -n=<num>] [testCase] ...  (or `perl checkamr.p') \n");
printf("  Notes: \n");
printf("   By default run all tests cases.                                            \n");
printf(" -np=<num> : run in parallel with this many processors        \n");
printf(" -N=<num> -n=<num> : run in parallel with this many nodes and processors       \n");
printf(" testCase= sissGT     : a series of different grids (GT=Grid Test)            \n");
printf("         = sissCT     : a series of convergence tests (CT=Convergence Test)   \n");
printf("         = siss2CT    : convergence tests on a finer grid.                    \n");
printf("         = ciceGT     : circle in a channel.                  \n");
printf("         = square     : square.                        \n");
printf("         = box        : box.                           \n");
printf("         = rbib       : rotated box in a box.                 \n");
printf("         = sis        : square in a square (*new*).           \n");
printf("         = sib        : sphere in a box.                      \n");
printf("         = sibCT      : sphere in a box (convergence test).         \n");
printf("                                                                     \n");
printf("==============================================================================\n\n");

$numberOfErrors=0;
@allTestCases=( "square",
                 "sissCT", 
                 "sissCT", 
#                 "siss2CT", 
                 "ciceGT", 
                  );

if( $#ARGV >=0 )
{
#  $testCase  = $ARGV[0];
  @allTestCases=@ARGV;
}

$np=0;   # number of processors (0=serial run)
$nodes=0; # for srun 

foreach $arg ( @ARGV )
{
  if( $arg =~ /-np=(.*)/ )
  {
    $np= $1;
  }
  elsif( $arg =~ /-N=(.*)/ )
  {
    $nodes = $1;
    $np = $1; # default value 
  }
  elsif( $arg =~ /-n=(.*)/ )
  {
    $np = $1;
  }
  else
  {
    @allTestCases=( $arg )
  }
}


# *** here is the executable ***
$amrh = "../amrh"; 
$texFileSuffix="table.tex";
$labelComment="";
$ofs=".out"; # output file suffix
if( $np > 0 )
{ # this must be a parallel run
  $amrh = "mpirun -np $np ../amrh"; 
  if( $nodes > 0 )
  {
    $amrh = "srun -N $nodes -n $np -ppdebug ../amrh"; 
  }
  $texFileSuffix="np$np.table.tex";
  $labelComment=", $np processors";
  $ofs=".np$np.out";
}


foreach $testCase (@allTestCases)  # ------------------------------------
{


$checkFile="$testCase.check.new";
open(CHECKFILE,">$checkFile") ||  die "cannot open file $checkFile!" ;

if( $testCase eq "sissGT" )
{

  @cases = ("check.cmd -noplot -grid=siss2.hdf -tf=1. -rr=2 -rl=1 -nb=2 -cf=siss2.212.check",
	    "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=2 -rl=2 -nb=2 -cf=siss.222.check",
	    "check.cmd -noplot -grid=siss3.hdf -tf=1. -rr=2 -rl=1 -nb=2 -cf=siss3.212.check",
	    "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=2 -rl=3 -nb=2 -cf=siss.232.check",
	    "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=4 -rl=2 -nb=2 -cf=siss.422.check",
	    "check.cmd -noplot -grid=siss4.hdf -tf=1. -rr=2 -rl=1 -nb=2 -cf=siss4.212.check",
	    "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=2 -rl=4 -nb=2 -cf=siss.242.check",
	    "check.cmd -noplot -grid=siss2.hdf -tf=1. -rr=2 -rl=3 -nb=2 -cf=siss2.232.check",
	    "check.cmd -noplot -grid=siss2.hdf -tf=1. -rr=4 -rl=2 -nb=2 -cf=siss2.422.check"
             );


  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }


  getResults( 0,"siss2.212.check" );
  getResults( 1,"siss.222.check" );

  getResults( 2,"siss3.212.check" );
  getResults( 3,"siss.232.check" );
  getResults( 4,"siss.422.check" );

  getResults( 5,"siss4.212.check" );
  getResults( 6,"siss.242.check" );
  getResults( 7,"siss2.232.check" );
  getResults( 8,"siss2.422.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40\\times 40\$  & \$20\\times20+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40\\times40\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|}                   \\hline \n",
  "       &  \$80\\times 80\$   & \$20\\times20+ (r=2)^2\$ & \$20\\times20+ (r=4)^1\$  \\\\   \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  &  \$$gridPoints[3]\$      &  \$ $gridPoints[4]\$      \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80\\times80\$}  \\\\   \n",
  " \\end{tabular}  \\\\  \n",
  " \\vspace{.25\\baselineskip}  \n",
  " \\begin{tabular}{|c|c|c|c|c|}                   \\hline  \n",
  "   &\$160\\times 160\$ & \$20\\times20+ (r=2)^3\$ & \$40\\times40+ (r=2)^2\$ & \$40\\times40+ (r=4)^1\$  \\\\  \n",
  "  \\hline  \n",
  " error       & \$$error[5]\$     & \$$error[6]\$     & \$$error[7]\$  & \$$error[8]\$  \\\\   \n",
  " grid points & \$$gridPoints[5]\$   & \$ $gridPoints[6]\$  &  \$$gridPoints[7]\$ & \$$gridPoints[8]\$   \\\\  \n",
  " time (s)    &  \$$cpu[5]\$          &   \$ $cpu[6]\$       &    \$$cpu[7]\$     &  \$$cpu[8]\$      \\\\  \n",
  "  \\hline  \n",
  " \\multicolumn{5}{c}{Effective resolution \$160\\times160\$}  \\\\    \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=1.\$ for a pulse crossing a square inside a square$labelComment (sissGT.$texFileSuffix).}  \n",
  " \\label{tab:amrh.siss}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix sissGT.$texFileSuffix)\n");
}
elsif( $testCase eq "sissCT" )
{
  # ******************** Compare results with different tolerences and buffer cells ***************

  @cases = ("check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=4 -rl=2 -nb=2 -cf=siss.422.01.check -et=.01",
            "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=4 -rl=2 -nb=2 -cf=siss.422.05.check -et=.05",
            "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=4 -rl=2 -nb=1 -cf=siss.421.01.check -et=.01", 
            "check.cmd -noplot -grid=siss.hdf  -tf=1. -rr=4 -rl=2 -nb=1 -cf=siss.421.05.check -et=.05" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 1,"siss.422.01.check" );
  getResults( 2,"siss.422.05.check" );
  getResults( 3,"siss.421.01.check" );
  getResults( 4,"siss.421.05.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|c|c|}                   \\hline \n",
  " &\$20\\times20+(r=4)^1\$&\$20\\times20+(r=4)^1\$ &\$20\\times20+(r=4)^1\$&\$20\\times20+(r=4)^1\$ \\\\ \\hline \n",
  " error       & \$$error[1]\$     & \$$error[2]\$     & \$$error[3]\$  & \$$error[4]\$  \\\\  \n",
  " grid points & \$$gridPoints[1]\$& \$$gridPoints[2]\$& \$$gridPoints[3]\$  &  \$ $gridPoints[4]\$      \\\\ \n",
  " time (s)    &   \$$cpu[1]\$     &    \$$cpu[2]\$     &   \$$cpu[3]\$      &  \$$cpu[4]\$   \\\\ \n",
  "\$\\epsilon\$&   \$$et[1]\$      &   \$$et[2]\$      &   \$$et[3]\$        &  \$$et[4]\$   \\\\ \n",
  " buffer      &   \$$nb[1]\$      &   \$$nb[2]\$      &   \$$nb[3]\$        &  \$$nb[4]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{5}{c}{Effective resolution \$80\\times80\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Results for different values of the error tolerance and number of buffer zones. 
              Computed errors at \$t=1.\$ for a pulse crossing a square inside a square$labelComment (sissCT.$texFileSuffix).}  \n",
  " \\label{tab:amrh.sissCT}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix sissCT.$texFileSuffix)\n");
}
elsif( $testCase eq "siss2CT" )
{
  # ******************** Compare results with different tolerences and buffer cells ***************

  print(" testcase: $testCase\n");

  @cases = ("check.cmd -noplot -grid=siss2.hdf  -tf=1. -rr=4 -rl=2 -nb=2 -cf=siss2.422.01.check -et=.01",
            "check.cmd -noplot -grid=siss2.hdf  -tf=1. -rr=4 -rl=2 -nb=2 -cf=siss2.422.05.check -et=.05",
            "check.cmd -noplot -grid=siss2.hdf  -tf=1. -rr=4 -rl=2 -nb=1 -cf=siss2.421.01.check -et=.01", 
            "check.cmd -noplot -grid=siss2.hdf  -tf=1. -rr=4 -rl=2 -nb=1 -cf=siss2.421.005.check -et=.005" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 1,"siss2.422.01.check" );
  getResults( 2,"siss2.422.05.check" );
  getResults( 3,"siss2.421.01.check" );
  getResults( 4,"siss2.421.005.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|c|c|}                   \\hline \n",
  " &\$40\\times40+(r=4)^1\$&\$40\\times40+4^1\$ &\$40\\times40+4^1\$&\$40\\times40+4^1\$ \\\\ \\hline \n",
  " error       & \$$error[1]\$     & \$$error[2]\$     & \$$error[3]\$  & \$$error[4]\$  \\\\  \n",
  " grid points & \$$gridPoints[1]\$& \$$gridPoints[2]\$& \$$gridPoints[3]\$  &  \$ $gridPoints[4]\$      \\\\ \n",
  " time (s)    &   \$$cpu[1]\$     &    \$$cpu[2]\$     &   \$$cpu[3]\$      &  \$$cpu[4]\$   \\\\ \n",
  "\$\\epsilon\$&   \$$et[1]\$      &   \$$et[2]\$      &   \$$et[3]\$        &  \$$et[4]\$   \\\\ \n",
  " buffer      &   \$$nb[1]\$      &   \$$nb[2]\$      &   \$$nb[3]\$        &  \$$nb[4]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{5}{c}{Effective resolution \$160\\times160\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Results for different values of the error tolerance and number of buffer zones. 
              Computed errors at \$t=1.\$ for a pulse crossing a square inside a square$labelComment (siss2CT.$texFileSuffix).}  \n",
  " \\label{tab:amrh.siss2CT}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix siss2CT.$texFileSuffix)\n");
}
elsif( $testCase eq "sis" )  # ************** NEW version of sis **************
{
  print(" testcase: $testCase\n");

  $tf = 1.; $et=.01; $nb=1; 
  @cases = ("sis.check.cmd -noplot -grid=sise2.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=sis40.211.01.check -et=$et",
            "sis.check.cmd -noplot -grid=sise1.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sis20.221.01.check -et=$et",
            "sis.check.cmd -noplot -grid=sise4.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=sis80.211.01.check -et=$et", 
            "sis.check.cmd -noplot -grid=sise1.order2 -tf=$tf -rr=2 -rl=3 -nb=$nb -cf=sis20.231.01.check -et=$et",
            "sis.check.cmd -noplot -grid=sise1.order2 -tf=$tf -rr=4 -rl=2 -nb=$nb -cf=sis20.421.01.check -et=$et",
            "sis.check.cmd -noplot -grid=sise2.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sis40.221.01.check -et=$et" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"sis40.211.01.check" );
  getResults( 1,"sis20.221.01.check" );
  getResults( 2,"sis80.211.01.check" );
  getResults( 3,"sis20.231.01.check" );
  getResults( 4,"sis20.421.01.check" );
  getResults( 5,"sis40.221.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40\\times 40\$  & \$20\\times20+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40\\times40\\times40\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|c|}                   \\hline \n",
  "  & \$80\\times 80\$ & \$20\\times20+(r=2)^2\$ & \$20\\times20+(r=4)^1\$ & \$40\\times40+(r=2)^1\$ \\\\  \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  & \$$error[5]\$ \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  & \$$gridPoints[3]\$   & \$ $gridPoints[4]\$  & \$ $gridPoints[5]\$   \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$  &  \$$cpu[5]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80\\times80\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=$tf\$ for a pulse crossing sis$labelComment (sis.$texFileSuffix).}  \n",
  " \\label{tab:amrh.sis}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix sis.$texFileSuffix)\n");
}
elsif( $testCase eq "cic" )  # ************** NEW version of cic **************
{
  print(" testcase: $testCase\n");

  $tf = 1.75; $et=.01; $nb=1; 
  @cases = ("cic.check.cmd -noplot -grid=cice4.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=cic40.211.01.check -et=$et",
            "cic.check.cmd -noplot -grid=cice2.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=cic20.221.01.check -et=$et",
            "cic.check.cmd -noplot -grid=cice8.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=cic80.211.01.check -et=$et", 
            "cic.check.cmd -noplot -grid=cice2.order2 -tf=$tf -rr=2 -rl=3 -nb=$nb -cf=cic20.231.01.check -et=$et",
            "cic.check.cmd -noplot -grid=cice2.order2 -tf=$tf -rr=4 -rl=2 -nb=$nb -cf=cic20.421.01.check -et=$et",
            "cic.check.cmd -noplot -grid=cice4.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=cic40.221.01.check -et=$et" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"cic40.211.01.check" );
  getResults( 1,"cic20.221.01.check" );
  getResults( 2,"cic80.211.01.check" );
  getResults( 3,"cic20.231.01.check" );
  getResults( 4,"cic20.421.01.check" );
  getResults( 5,"cic40.221.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40\\times 40\$  & \$20\\times20+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40\\times40\\times40\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|c|}                   \\hline \n",
  "  & \$80\\times 80\$ & \$20\\times20+(r=2)^2\$ & \$20\\times20+(r=4)^1\$ & \$40\\times40+(r=2)^1\$ \\\\  \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  & \$$error[5]\$ \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  & \$$gridPoints[3]\$   & \$ $gridPoints[4]\$  & \$ $gridPoints[5]\$   \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$  &  \$$cpu[5]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80\\times80\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=$tf\$ for a pulse crossing cic$labelComment (file sis.$texFileSuffix).}  \n",
  " \\label{tab:amrh.sis}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix sis.$texFileSuffix)\n");
}
elsif( $testCase eq "ciceGT" )
{
  print(" testcase: $testCase\n");

  @cases = ("cice.check.cmd -noplot -grid=cice2.hdf  -tf=1. -rr=2 -rl=1 -nb=1 -cf=cice2.211.01.check -et=.01",
            "cice.check.cmd -noplot -grid=cice.hdf   -tf=1. -rr=2 -rl=2 -nb=1 -cf=cice.221.01.check  -et=.01",
            "cice.check.cmd -noplot -grid=cice3.hdf  -tf=1. -rr=2 -rl=1 -nb=1 -cf=cice3.211.01.check -et=.01", 
            "cice.check.cmd -noplot -grid=cice.hdf   -tf=1. -rr=2 -rl=3 -nb=1 -cf=cice.231.01.check  -et=.01",
            "cice.check.cmd -noplot -grid=cice.hdf   -tf=1. -rr=4 -rl=2 -nb=1 -cf=cice.421.01.check  -et=.01" );

# @cases = ("cice.check.cmd -grid=cice.hdf   -tf=1. -rr=2 -rl=3 -nb=1 -cf=cice.231.01.check  -et=.01");

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
  }

  getResults( 0,"cice2.211.01.check" );
  getResults( 1,"cice.221.01.check" );
  getResults( 2,"cice3.211.01.check" );
  getResults( 3,"cice.231.01.check" );
  getResults( 4,"cice.421.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$62\\times 62\$  & \$31\\times31+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$62\\times62\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|}                   \\hline \n",
  "       &  \$124\\times 124\$ & \$31\\times31+ (r=2)^2\$ & \$31\\times31+ (r=4)^1\$  \\\\   \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  &  \$$gridPoints[3]\$      &  \$ $gridPoints[4]\$      \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$124\\times124\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=1.\$ for a pulse crossing a circle inside a square$labelComment (ciceGT.$texFileSuffix).}  \n",
  " \\label{tab:amrh.ciceGT}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix ciceGT.$texFileSuffix)\n");
}
# -------------------------------------------------------------------------------------------------
elsif( $testCase eq "square" )
{
  print(" testcase: $testCase\n");

  $tf=.5;
  @cases = ("sq.check.cmd -noplot -grid=square40.hdf -tf=$tf -rr=2 -rl=1 -nb=1 -cf=sq40.211.01.check -et=.01",
            "sq.check.cmd -noplot -grid=square20.hdf -tf=$tf -rr=2 -rl=2 -nb=1 -cf=sq20.221.01.check -et=.01",
            "sq.check.cmd -noplot -grid=square80.hdf -tf=$tf -rr=2 -rl=1 -nb=1 -cf=sq80.211.01.check -et=.01", 
            "sq.check.cmd -noplot -grid=square20.hdf -tf=$tf -rr=2 -rl=3 -nb=1 -cf=sq20.231.01.check -et=.01",
            "sq.check.cmd -noplot -grid=square20.hdf -tf=$tf -rr=4 -rl=2 -nb=1 -cf=sq20.421.01.check -et=.01" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"sq40.211.01.check" );
  getResults( 1,"sq20.221.01.check" );
  getResults( 2,"sq80.211.01.check" );
  getResults( 3,"sq20.231.01.check" );
  getResults( 4,"sq20.421.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40\\times 40\$  & \$20\\times20+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40\\times40\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|}                   \\hline \n",
  "       &  \$80\\times 80\$ & \$20\\times20+ (r=2)^2\$ & \$20\\times20+ (r=4)^1\$  \\\\   \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  &  \$$gridPoints[3]\$      &  \$ $gridPoints[4]\$      \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80\\times80\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=$tf\$ for a pulse crossing square$labelComment (square.$texFileSuffix).}  \n",
  " \\label{tab:amrh.square}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix square.$texFileSuffix)\n");
}
elsif( $testCase eq "box" )
{
  print(" testcase: $testCase\n");
  $tf=.05; 
  @cases = ("box.check.cmd -noplot -grid=box40.hdf -tf=$tf -rr=2 -rl=1 -nb=1 -cf=box40.211.01.check -et=.01",
            "box.check.cmd -noplot -grid=box20.hdf -tf=$tf -rr=2 -rl=2 -nb=1 -cf=box20.221.01.check -et=.01",
            "box.check.cmd -noplot -grid=box80.hdf -tf=$tf -rr=2 -rl=1 -nb=1 -cf=box80.211.01.check -et=.01", 
            "box.check.cmd -noplot -grid=box20.hdf -tf=$tf -rr=2 -rl=3 -nb=1 -cf=box20.231.01.check -et=.01",
            "box.check.cmd -noplot -grid=box20.hdf -tf=$tf -rr=4 -rl=2 -nb=1 -cf=box20.421.01.check -et=.01" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"box40.211.01.check" );
  getResults( 1,"box20.221.01.check" );
  getResults( 2,"box80.211.01.check" );
  getResults( 3,"box20.231.01.check" );
  getResults( 4,"box20.421.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40^3\$  & \$20^3+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40^3\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|}                   \\hline \n",
  "       &  \$80^3\$ & \$20^3+ (r=2)^2\$ & \$20^3+ (r=4)^1\$  \\\\   \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  &  \$$gridPoints[3]\$      &  \$ $gridPoints[4]\$      \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80^3\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=.5\$ for a pulse crossing box$labelComment (box.$texFileSuffix).}  \n",
  " \\label{tab:amrh.box}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix box.$texFileSuffix)\n");
}
elsif( $testCase eq "rbib" )
{
  print(" testcase: $testCase\n");

  # *NOTE* important to take enough steps or else errors on coarse grids rbibe1 are larger 
  $tf = .5; $et=.01; $nb=1; 
  @cases = ("rbib.check.cmd -noplot -grid=rbibe2.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=rbib40.211.01.check -et=$et",
            "rbib.check.cmd -noplot -grid=rbibe1.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=rbib20.221.01.check -et=$et",
            "rbib.check.cmd -noplot -grid=rbibe4.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=rbib80.211.01.check -et=$et", 
            "rbib.check.cmd -noplot -grid=rbibe1.order2 -tf=$tf -rr=2 -rl=3 -nb=$nb -cf=rbib20.231.01.check -et=$et",
            "rbib.check.cmd -noplot -grid=rbibe1.order2 -tf=$tf -rr=4 -rl=2 -nb=$nb -cf=rbib20.421.01.check -et=$et",
            "rbib.check.cmd -noplot -grid=rbibe2.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=rbib40.221.01.check -et=$et" );

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"rbib40.211.01.check" );
  getResults( 1,"rbib20.221.01.check" );
  getResults( 2,"rbib80.211.01.check" );
  getResults( 3,"rbib20.231.01.check" );
  getResults( 4,"rbib20.421.01.check" );
  getResults( 5,"rbib40.221.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40^3\$  & \$20^3+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40^3\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|c|}                   \\hline \n",
  "  & \$80^3\$ & \$20^3+(r=2)^2\$ & \$20^3+(r=4)^1\$ & \$40^3+(r=2)^1\$ \\\\  \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  & \$$error[5]\$ \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  & \$$gridPoints[3]\$   & \$ $gridPoints[4]\$  & \$ $gridPoints[5]\$   \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$  &  \$$cpu[5]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80^3\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=$tf\$ for a pulse crossing a grid for a rotated-box-in-a-box$labelComment (rbib.$texFileSuffix).}  \n",
  " \\label{tab:amrh.rbib}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix rbib.$texFileSuffix)\n");
}
elsif( $testCase eq "sib" )
{
  print(" testcase: $testCase\n");

  $tf =.25; $et=.01; $nb=1; # $tf=.25; 
  @cases = ("sib.check.cmd -noplot -grid=sibFixede2.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=sib40.211.01.check -et=$et",
            "sib.check.cmd -noplot -grid=sibFixede1.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sib20.221.01.check -et=$et",
            "sib.check.cmd -noplot -grid=sibFixede4.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=sib80.211.01.check -et=$et", 
            "sib.check.cmd -noplot -grid=sibFixede1.order2 -tf=$tf -rr=2 -rl=3 -nb=$nb -cf=sib20.231.01.check -et=$et",
            "sib.check.cmd -noplot -grid=sibFixede1.order2 -tf=$tf -rr=4 -rl=2 -nb=$nb -cf=sib20.421.01.check -et=$et",
            "sib.check.cmd -noplot -grid=sibFixede2.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sib40.221.01.check -et=$et" );

#   $tf =.25; $et=.01; $nb=1; 
#   @cases = ("sib.check.cmd -noplot -grid=sibe4.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=sib40.211.01.check -et=$et",
#             "sib.check.cmd -noplot -grid=sibe2.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sib20.221.01.check -et=$et",
#             "sib.check.cmd -noplot -grid=sibe8.order2 -tf=$tf -rr=2 -rl=1 -nb=$nb -cf=sib80.211.01.check -et=$et", 
#             "sib.check.cmd -noplot -grid=sibe2.order2 -tf=$tf -rr=2 -rl=3 -nb=$nb -cf=sib20.231.01.check -et=$et",
#             "sib.check.cmd -noplot -grid=sibe2.order2 -tf=$tf -rr=4 -rl=2 -nb=$nb -cf=sib20.421.01.check -et=$et",
#             "sib.check.cmd -noplot -grid=sibe4.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sib40.221.01.check -et=$et" );y

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"sib40.211.01.check" );
  getResults( 1,"sib20.221.01.check" );
  getResults( 2,"sib80.211.01.check" );
  getResults( 3,"sib20.231.01.check" );
  getResults( 4,"sib20.421.01.check" );
  getResults( 5,"sib40.221.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|}  \\hline \n",
  "            & \$40^3\$  & \$20^3+ (r=2)\^1\$  \\\\   \\hline \n",
  " error      & \$$error[0]\$     & \$$error[1]\$  \\\\  \n",
  "grid points & \$$gridPoints[0]\$   &   \$$gridPoints[1]\$     \\\\ \n",
  "time (s)    & \$$cpu[0]\$   &  \$$cpu[1]\$      \\\\ \n",
  " \\hline \n",
  " \\multicolumn{3}{c}{Effective resolution \$40\\times40\\times40\$} \\\\ \n",
  " \\end{tabular}  \\\\ \n",
  "\\vspace{.25\\baselineskip}\n",
  "\\begin{tabular}{|c|c|c|c|c|}                   \\hline \n",
  "  & \$80^3\$ & \$20^3+(r=2)^2\$ & \$20^3+(r=4)^1\$ & \$40^3+(r=2)^1\$ \\\\  \\hline \n",
  " error       & \$$error[2]\$      & \$$error[3]\$  & \$$error[4]\$  & \$$error[5]\$ \\\\  \n",
  " grid points &  \$$gridPoints[2]\$  & \$$gridPoints[3]\$   & \$ $gridPoints[4]\$  & \$ $gridPoints[5]\$   \\\\ \n",
  " time (s)    &    \$$cpu[2]\$        &   \$$cpu[3]\$           &  \$$cpu[4]\$  &  \$$cpu[5]\$   \\\\ \n",
  "  \\hline \n",
  " \\multicolumn{4}{c}{Effective resolution \$80^3\$}  \\\\   \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=$tf\$ for a pulse crossing a grid for a sphere-in-a-box$labelComment (sib.$texFileSuffix).}  \n",
  " \\label{tab:amrh.sib}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix sib.$texFileSuffix)\n");
}
elsif( $testCase eq "sibCT" )
{
  print(" testcase: $testCase (converegence test)\n");

  $tf =.25; $et=.01; $nb=1; # $tf=.25; 
  @cases = ("sib.check.cmd -noplot -grid=sibFixede1.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sibCT20.221.01.check -et=$et",
            "sib.check.cmd -noplot -grid=sibFixede2.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sibCT40.221.01.check -et=$et",
            "sib.check.cmd -noplot -grid=sibFixede4.order2 -tf=$tf -rr=2 -rl=2 -nb=$nb -cf=sibCT80.221.01.check -et=$et");

  $i=0;
  foreach $case (@cases)
  {
    # ****** run amrh ********
    print "Run $amrh $case > $testCase$i$ofs\n";
    system("$amrh $case > $testCase$i$ofs");
    $i++;
  }

  getResults( 0,"sibCT20.221.01.check" );
  getResults( 1,"sibCT40.221.01.check" );
  getResults( 2,"sibCT80.221.01.check" );

  $file="amrh.$texFileSuffix";
  open(FILE,">$file") || die "cannot open file $file!" ;

  print FILE "\\begin{table}[hbt]\n",
  "\\footnotesize\n",
  "\\begin{center}\n",
  "\\begin{tabular}{|c|c|c|c|}                   \\hline \n",
  "  & \$20^3+(r=2)^1\$ & \$40^3+(r=2)^1\$ & \$80^3+(r=2)^1\$  \\\\  \\hline \n",
  " error       & \$$error[0]\$       & \$$error[1]\$       & \$$error[2]\$  \\\\  \n",
  " grid points &  \$$gridPoints[0]\$ & \$$gridPoints[1]\$  & \$ $gridPoints[2]\$   \\\\ \n",
  " time (s)    &  \$$cpu[0]\$        &   \$$cpu[1]\$       & \$$cpu[2]\$    \\\\ \n",
  "  \\hline \n",
  " \\end{tabular}  \n",
  " \\end{center}  \n",
  " \\caption{Computed errors at \$t=$tf\$ for a pulse crossing a grid for a sphere-in-a-box$labelComment (convergence test) (sibCT.$texFileSuffix).}  \n",
  " \\label{tab:amrh.sibCT}  \n",
  " \\end{table}  \n";

  printf("results saved in amrh.$texFileSuffix (cp amrh.$texFileSuffix sibCT.$texFileSuffix)\n");
}

else
{
  printf("Unknown testCase=[$testCase]\n");
}

print "**** Check results written to $checkFile with $testCase.check using smartDiff...\n ***";
close(CHECKFILE);

$rt =  system("../../op/tests/smartDiff.p $checkFile $testCase.check");
if( $rt == 0 )
{
  print "Test $testCase apparently successful\n";
}
else
{
  print "***Test: $testCase ERRORS FOUND****\n";
  $numberOfErrors++;
}

} # end foreach testCase

if( $rt == 0 )
{
  print "Tests apparently successful\n";
  exit(0);
}
else
{
  print "***ERRORS FOUND****\n";
  exit(1);
}

exit;

