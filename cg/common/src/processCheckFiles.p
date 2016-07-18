eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#
# perl program to process check files and compute convergence tables
# 

use Getopt::Long; use Getopt::Std;

sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }

$tol=1.e-30; # avoid division by zero errors

$numberOfParameters = @ARGV;
if ($numberOfParameters eq 0)
{
  
  printf("\n");
  printf("================================================================================\n");
  printf("This perl script will process check files and compute convergence tables        \n");
  printf("  (Currenty this file processes rigid-body results)                             \n");
  printf("  Usage: \n");
  printf("    processCheckFiles.p  -file=<master-check-file> -c=<list of components> ...  \n");
  printf("      -useGridNames=[0|1] -table=[0|1]                                          \n");
  printf("  where \n");
  printf("  -file:  master-check-file = the name of a master checkfile created by conv.p that \n");
  printf("      conatins check file resuts from a sequence of runs on increasing resolution\n");
  printf("  -c : comma separated list of components to output in tables, e.g. -c=0,2 or -c=3,4,6 \n");
  printf("  -useGridNames : 1=use grid names in table, 0=use grid-spacing\n"); 
  printf("  -table : 1=include \\begin{table} in LaTeX table\n"); 
  printf("==============================================================================\n\n");
  exit;
  
}

$file=""; 
$useGridNames=1; 
$table=1; 
foreach $arg ( @ARGV )
{
  if( $arg =~ /-file=(.*)/ )
  {
    $file = $1;
    printf("Using file [%s]\n",$file);
  }
  elsif( $arg =~ /-c=(.*)/ )
  {
    # $components = $1;
    # printf("components=[$components]\n");
    @cv = split(',',$1);
    printf("Output components: ");
    for( $j=0; $j<@cv; $j++){ printf(" cv[$j]=$cv[$j]"); } printf("\n");
    
  }
  elsif( $arg =~ /-useGridNames=(.*)/ )
  {
    $useGridNames=$1;
    printf("useGridNames=$useGridNames : 1=use grid names in table, 0=use grid-spacing.\n");
  }
  elsif( $arg =~ /-table=(.*)/ )
  {
    $table=$1;
    printf("table=$table : 1=include \\begin{table} in LaTeX table.\n");
  }
}

if( $file eq "" )
{
  printf("ERROR: no file specified\n");
  exit; 
}

# @errorArray = (); # array of errors

# Here in the input file with commands to run
open(FILE,"$file") || die print "unable to open $file.\n";

$count=0; 
$numberOfComponents=0; 
$caption=""; 
while( <FILE> )
{

  $line=$_; chop($line);
  # printf("line=[%s]\n",$line);

  # First commented caption in file defines details of the entire run: 
  if( $line =~ /^% \\caption/ ){
    $caption=$line;
    $caption =~ s/^% //; 
    printf("Setting caption=[$caption]\n");
  }

  if( $line =~ /^>>start.*grid=(\w*).*res=(\d+)/ )
  {
    # -------------- New Resolution Found -------------

    $gridName=$1;
    $res=$2;
    $domain=-1; # there can be results from different domains 

    printf("-----New resolution found: grid=[$gridName], res=[$res]\n");

    $grid[$count]=$gridName;
    $resolution[$count]=$res;

    while( <FILE> )
    {
      # ------ process this resolution ----
      $line=$_; chop($line);
      # printf("line=[%s]\n",$line);

      if( $line =~ /^\% names:(.*)/ )
      {
        $domain++; 
        printf("...new domain found, domain=$domain...\n");

        @components =  split(' ',$1); 
        for( $i=0; $i<$#components+1; $i++ ){ $componentNames[$domain][$i]=$components[$i]; }

        # @componentNames =  split(' ',$1); 

        printf("domain=$domain: ");

        $numberOfComponents[$domain]=$#components+1;
        for( $i=0; $i< $numberOfComponents[$domain]; $i++ )
        {
          printf(" name[$i]=$componentNames[$domain][$i], ");
        }
        printf("\n");
      }


      if(  $line =~ /^\\caption/ ){ next; } # skip later lines beginning with \caption 
      if(  $line =~ /^[ \t]*$/ ){ next; } # skip blank lines

      if(  $line =~ /^\%/ ){ next; } # skip comments 
      if( $line =~ /^<<end/ ){ last; } # finish

      # ---- This line must have results ----

      # NOTE: --- for now we keep reading results until the last set ----

      @tokens =  split(' ',$line); 


      $time[$count]=$tokens[0];
      $nc =$tokens[1];
      printf("data: t=%8.2e, ",$time[$count]);
      if( $nc ne $numberOfComponents[$domain] )
      {
        printf("ERROR: number of components=$nc from this resolution does not match numberOfComponents $numberOfComponents[$domain]\n");
        exit;
      }

      for( $i=0; $i<$numberOfComponents[$domain]; $i++ )
      {
        $err[$domain][$i][$count]=$tokens[3+3*$i]; # error in component $i
        printf(" err[$domain][$i][$count]=$err[$domain][$i][$count] ");
      }    
      printf("\n"); # end "data"

    }

    # Check that the times match 
    if( $count>0 && ($time[$count] ne $time[$count-1]) )
    {
      printf("ERROR: time=$time[$count] doe not match previous resolution time=$time[$domain][$count-1] \n");
      exit;
    }

    $count++; 
  }

}

$nres = $count;
close(FILE);

$numberOfDomains=$domain+1;

$domain=0; 
$totalNumberOfComponents=0; 
@domainNumber = ();  # holds domain number correspo=nding to each component
@componentNumberInDomain = ();  

for( $domain=0; $domain<$numberOfDomains; $domain++ )
{
  printf("\n --- SUMMARY domain=$domain, t=%9.3e---\n",$time[0]);  

  for( $i=0; $i<$numberOfComponents[$domain]; $i++ )
  {
    $domainNumber[$totalNumberOfComponents]=$domain;
    $componentNumberInDomain[$totalNumberOfComponents]=$i;
    $totalNumberOfComponents= $totalNumberOfComponents+1;

    printf(" name[$i]=$componentNames[$domain][$i] err=");
    for( $j=0; $j<$count; $j++)
    {
      printf("%9.2e, ",$err[$domain][$i][$j]);
    }
    printf("\n");
  }
}


# -------------------------------------------------------
# ----------------- OUTPUT A LATEX TABLE ----------------
# -------------------------------------------------------

$numberOfComponentsToOutput = $totalNumberOfComponents;
if( @cv > 0 )
{ # user has requested only outputing some components
  $numberOfComponentsToOutput=@cv; 
}
else
{
  for( $i=0; $i<$totalNumberOfComponents; $i++ ){ $cv[$i]=$i; } # fill in default components into @cv
}




$date= localtime();   # here is the date

# Name the LaTeX output file 
$latexFile = $file;
$latexFile =~ s/.check$//;
$latexFile .= ".ConvTable.tex"; 

# Output table to screen and to a file 
printf("-----------------------------------LateX Table ------------------------------------------------------\n");
for( $io=0; $io<2; $io++ )
{

  # $lfile = STDOUT; 
  if( $io eq 0 ){ open($lfile,'>&',\*STDOUT); }else{ open($lfile,">$latexFile") || die print "unable to open $latexFile.\n"; }

  print $lfile "% Table generated by processCheckFiles.p, $date\n";
  if( $table ne "1" ){ print $lfile "%"; } # comment out next line
  print $lfile "\\begin{table}[hbt]\\tableFont % you should set \\tableFont to \\footnotesize or other size\n";
  print $lfile "% \\newcommand{\\convTitle}{Title goes here}% define the multicolumn title.\n";
  print $lfile "% \\newcommand{\\strutt}{\\rule{0pt}{9pt}}% strutt to make table column height bigger.\n";
  if( $useGridNames eq "0" ){
    print $lfile "% GridNames: ";
    for( $j=0; $j<$count; $j++){ print $lfile "$grid[$j], "; }
    print $lfile "\n";
  }
  print $lfile "% \\newcommand{\\num}[2]{#1e{#2}}% use this command to set the format of numbers in the table.\n";
  for( $ii=0; $ii<$numberOfComponentsToOutput; $ii++ )
  {
    $i=$cv[$ii]; # component
    $ic=$componentNumberInDomain[$i]; 
    $domain = $domainNumber[$i]; 
    $cn = $componentNames[$domain][$ic]; 
    $cn =~ tr/1234567890/ABCDEFBGHI/; # translate numbers to letters
    print $lfile "% \\newcommand{\\err$cn}{\$E_j^{$cn}\$}% defines column header - note [1234567890]->[ABCDEFGHI]\n"; 

  }
  if( $table ne "1" ){ print $lfile "%"; } # comment out next line
  print $lfile "\\begin{center}\n";
  # 
  if( $useGridNames eq "1" ){ print $lfile "\\begin{tabular}{|l|c|"; }else{  print $lfile "\\begin{tabular}{|c|"; }
  for( $i=0; $i<$numberOfComponentsToOutput; $i++ ){ print $lfile "c|c|"; } print $lfile "} \\hline \n";
  # title =bar
  if( $useGridNames eq "1" ){ $numCols = $numberOfComponentsToOutput*2 + 2; }
                      else{$numCols = $numberOfComponentsToOutput*2 + 1; } 
  print $lfile "  \\multicolumn{$numCols}{|c|}{\\convTitle} \\\\ \\hline \n"; 
  #
  if( $useGridNames eq "1" ){ 
    print $lfile "    grid      &  N   ";}
  else{
    print $lfile "\\strutt\$h_j\$";}

  for( $ii=0; $ii<$numberOfComponentsToOutput; $ii++ )
  {
    $i=$cv[$ii]; # component
    $ic=$componentNumberInDomain[$i]; 
    $domain = $domainNumber[$i]; 
    $cn = $componentNames[$domain][$ic]; 
    $cn =~ tr/1234567890/ABCDEFBGHI/; # translate numbers to letters
    print $lfile "&     \\err$cn     &  r   ";  
    # print $lfile "&       $componentNames[$domain][$ic]       &  r   ";  
  }
  if( $useGridNames eq "1" ){ 
    print $lfile " \\\\ \\hline \n";}
  else{
    print $lfile " \\\\[3pt] \\hline \n";
  }
  #
  for( $j=0; $j<$count; $j++)
  {
    if( $useGridNames eq "1" ){ 
      print $lfile "  $grid[$j] &   $resolution[$j] "; }
    else{
      $numPoints = 10*$resolution[$j]; # inverse grid-spacing -- do this for now 
      print $lfile "  1/$numPoints  "; }

    for( $ii=0; $ii<$numberOfComponentsToOutput; $ii++ )
    {
      $i=$cv[$ii]; # component
      $ic=$componentNumberInDomain[$i]; 
      $domain = $domainNumber[$i]; 

      if( $j eq 0 ){ $ratio=""; }else{ $ratio=$err[$domain][$ic][$j-1]/max($err[$domain][$ic][$j],$tol); }
      $num = sprintf("%8.1e",$err[$domain][$ic][$j]);
      # printf(" &  %8.1e & %3.1f ",$num,$ratio);
      $num =~ s/[ ]*(.*)e(.)[0]*(.*)/\\num{\1}{\2\3}/; # convert number to \num{frac}{exponent}

      if( $j eq 0 ){ printf $lfile " & %s &     ",$num; }
      else{ 
        if( $ratio < 999 ){
           printf $lfile " & %s & %3.1f ",$num,$ratio; }
        elsif( $ratio < 99999 ){
           printf $lfile " & %s & %3.0f ",$num,$ratio; }
        else{
           printf $lfile " & %s &%5.0e",$num,$ratio; }
        
      }
    }
    print $lfile "\\\\ \\hline\n"; 
  }
  # -- Print average convergence rate from a least square fit to the log of the errors
  if( $useGridNames eq "1" ){ 
    print $lfile "  rate        &      "; }
  else{
    print $lfile "  rate   "; }
  for( $ii=0; $ii<$numberOfComponentsToOutput; $ii++ )
  {
    $i=$cv[$ii]; 
    $domain = $domainNumber[$i]; 
    $ic=$componentNumberInDomain[$i]; 

    #  Error:   e_j = c h_j^p 
    # Solve the least squares problem for
    #     log(e_j) = p log( h_j) + c  j=0,1,2,...
    # 
    #   [ log(h_1)  1 ] [ p ] = [ log(e_1) ]
    #   [ log(h_2)  1 ] [ c ] = [ log(e_2) ]
    #   [ log(h_3)  1 ]       = [ log(e_3) ]
    #   [ log(h_4)  1 ]       = [ log(e_4) ]
    #       ...                    ...
    $a11=0; $a12=0; $a22=0; $b1=0; $b2=0; 
    for( $j=0; $j<$count; $j++)
    {
      $h = 1/($resolution[$j]);  $logh = log($h);  
      $logErr = log(max($err[$domain][$ic][$j],$tol)); 
  
      # Normal equations: 
      $a11 = $a11 + $logh**2; 
      $a12 = $a12 + $logh;
      $a22 = $a22+1; 
      $b1 = $b1 + $logh*$logErr;
      $b2 = $b2 + $logErr;
    }
    $a21=$a12;
    $det = $a11*$a22-$a12*$a21; 
    $p = ($a22*$b1 -$a12*$b2)/$det; 
  
    printf $lfile "&    %4.2f       &      ",$p;
  }
  print $lfile "\\\\ \\hline\n"; 
  #    rate             &       &  $0.90$       &      &  $1.13$       &      &  $1.31$       &      &  $1.13$       &      &  $1.82$       &       \\ \hline
  
  print $lfile "\\end{tabular}\n"; 
  if( $table ne "1" ){ print $lfile "%"; } # comment out next line
  # print $lfile "\\caption{\\captionFont Results from $file, $date}\n";
  print $lfile "$caption\n";
  if( $table ne "1" ){ print $lfile "%"; } # comment out next line
  print $lfile "\\end{center}\n";
  if( $table ne "1" ){ print $lfile "%"; } # comment out next line
  print $lfile "\\end{table}\n"; 
  
  close($lfile);
  if( $io eq 1 ){ printf("Latex table saved to file [$latexFile]\n"); }

} # end for $io
