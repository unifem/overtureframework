eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
#
# perl program to extract documentation
#  usage: 
#         extract.p fileName1 [fileName2] [ fileName3] ...
# 
# See the file test.C for examples of how to imbed documentation in your 
# source file

@fileNames = @ARGV;
# $fileName          = $ARGV[0];

foreach $fileName ( @fileNames )  # process all files
{
  
  
  $debug = 0;   # set to 1 for debug info
  
  if( $debug==1 ) { printf("  fileName = $fileName \n");  }     
  
  open(FILE,"$fileName") || die "cannot open file $fileName!" ;
  
  
  while( <FILE> )
  {
    $line = $_;
    if( $debug==1 ) { printf(" line:  $line ");} 
    if( /\/\/\\begin/ )  # look for //\begin
    {
      $output = $line;
      $output =~ s/.*\\begin{(.*)}{.*/\1/;
      if( $debug==1 ) { printf(" output = $output \n");} 
      open(OUTFILE,"$output") || die "cannot open file $output!" ;
  
      $section = $line;
      $section =~ s/.*{(.*)}{(.*)}.*/\2/;
      if( $debug==1 ){ printf("---- section = $section \n");} 
      
      print OUTFILE "$section \n";
  
  
      #  Create a distinct \newlength name that we can use for indentation
      $argIndent = $output; chop($argIndent);
      $argIndent =~ s/(\W*)(\w*).*/\2/;
      $argIndent =~ s/\_//g;
      $argIndent =~ s/[0-9]/WDH/g; # convert numbers to letters since TeX will otherwise complain
      $argIndent = "${argIndent}ArgIndent";  # Here is a distinct name to use for indentation

      if( !( $output =~ /^>>/) )
      {
	print OUTFILE "\\newlength\{\\${argIndent}\}\n";
      }

      #  ****Grab the function declaration****
      $notDone = 1;
      while( $notDone && $_ )
      {
        $_ = <FILE>;
        if( $debug==1 ) { printf(" line:  $_ ");  }
        if( !/\/\// && $notDone == 3 )
	{
          next;  # skip lines in the initialization list
	}
        if( /^\/\/\\no function header\:/ )  # look for a line with //\no function header:
	{
	  last;
	}

        # first look for a function return type on a separate line
        $returnType = "";
        if( $notDone=="1" && ! /\(/  && !/^[ ]*class \b/ )
        {
          while( ! /\(/ ) # the return type includes all lines before a line with "("
          {
	   $new = $_;
           chop($new);
           $new =~ s/([\w<>]*):://g;   # remove ClassName::
           $new =~ s/\~/\$\\sim\$/g; # change ~ to $\sim$
           if( $returnType ne "" )
           {
             if( $new ne "" )
             {
               # print "new = [$new]\n";
     	       $returnType = $returnType . "\\\\" .  $new;
	     }
	   }
           else 
           {
             $returnType = $new;
           }
  	   $_ = <FILE>;
          }    
          if( $debug==1 ) { printf(" ***returnType=$returnType \n");} 
        }       
        if( /^[ ]*\bclass \b/ )  # special case for class declaration
        {
          $functionName = $_;
          chop($functionName);
          $notDone=0;
          print OUTFILE "\\begin{flushleft} \\textbf\{%\n";
          print OUTFILE "$functionName\n";
          $_ = <FILE>;  # skip next line assuming it to be a separator line like // =======
        }
        # extract the function name
        if( $notDone=="1" && /\(/ )
        {
  	  $functionName = $_;
          if( $functionName =~ /.*operator.*\(\)/ )  # special case for function name operator()
          {
            $functionName =~ s/(operator[ ]*\(\))[ ]*\(.*/\1/;     # first look for operator.*()(
	  }
	  else
          {    
    	    $functionName =~ s/([\w ]*)\(.*/\1/;     # look for wwwww(...  
	  }
 	  if( $debug==1 ) { printf(" ***functionName(1) = $functionName \n");} 

          $functionName =~ s/([\w<>]*)::/ /g;   # remove ClassName::
          $functionName =~ s/\_/\\\_/g; # change _ to \_
          $functionName =~ s/\</\$<\$/g; # < --> $<$
          $functionName =~ s/\>/\$>\$/g; # <
          $_ =~ s/([\w<>]*)::/ /;   # remove ClassName::
          $_ =~ s/\</\$<\$/g; # < --> $<$
          $_ =~ s/\>/\$>\$/g; # <
          $_ =~ s/\~/\$\\sim\$/g; # change ~ to $\sim$
    	  chop($functionName);
  	  if( $debug==1 ) { printf(" ***functionName = $functionName \n");} 
  
          print OUTFILE "\\begin{flushleft} \\textbf\{%\n";
  
  	  $argIndent = $output; chop($argIndent);
  	  $argIndent =~ s/(\W*)(\w*).*/\2/;
  	  $argIndent =~ s/\_//g;
          $argIndent =~ s/[0-9]/WDH/g; # convert numbers to letters since TeX will otherwise complain
  	  $argIndent = "${argIndent}ArgIndent";  # Here is a distinct name to use for indentation
  	  if( $debug==1 ){ printf(" argIndent = [$argIndent] \n");}
  
          if( $returnType )
  	  {
            $returnType =~ s/&/\\&/g;   # change & to \&
            $returnType =~ s/\_/\\\_/g; # change _ to \_
            $returnType =~ s/\</\$<\$/g; # < --> $<$
            $returnType =~ s/\>/\$>\$/g; # <
            #print OUTFILE "\\settowidth\{\\${argIndent}\}\{$returnType  $functionName(\}\%\n";

     	    print OUTFILE $returnType, " \\\\ \n";
            print OUTFILE "\\settowidth\{\\${argIndent}\}\{$functionName(\}\%\n";
  	  }
  	  else
  	  {
            print OUTFILE "\\settowidth\{\\${argIndent}\}\{$functionName(\}\% \n";
  	  }
          $notDone=2;	 
        }
  
        # *** now print function declaration lines ****
        if( $notDone==2 && ! /^\/\// )  # lines with no comments
        {
          $line = $_;
          $line =~ s/&/\\&/g;   # change & to \&
          # add default arguments that appear as comments like:
          #            real x, // = 5.
          #            real y  // = 6.
          #            int i /* = 2 */
          #            real a /* = 1. */, real b /* = 2. */,
  	  $line =~ s/,[ ]*\/\/(.*)/\1,/;   
  	  $line =~ s/[ ]*\/\/(.*)/\1/;       
  	  $line =~ s/,[ ]*\/\*(.*) \*\//\1,/g;
  	  $line =~ s/\/\*(.*) \*\/[ ]*/\1/g;
  
  	  $line =~ s/\_/\\\_/g;   # change _ to \_
          $line =~ s/^[ \t]*//;   # remove leading blanks or tabs
  	  chop($line);
          # check for the initialization list
	  if( /\:/ && !/\:\:/ )
	  {
            $line =~ s/\:.*//;  # remove stuff after a :
            $notDone=3;
          }	        
          if( $line =~ /^[ ]*$/ )
	  {
            next;   # skip a blank line
	  }
          if( ! ($line =~ /\(/) )  # if not first line, indent
  	  {
  	    print OUTFILE "\\hspace\{\\${argIndent}\}";
  	  }
          if( $debug==1 ){ printf("line = $line\n") ;}

  	  print OUTFILE $line;
          if( !($line =~ /\)[\w ]*$/) )   # add \\ to end of the line unless this is the last line
  	  {                               # last line ends line "...)$"   or "...) const $"
  	    print OUTFILE "\\\\ \n";
  	  }
  	  else
  	  {
	    print OUTFILE "\n";
    	  }
        }
        else
        {
  	  $notDone=0;
          last;       # exit loop
        }
      }
      if( $notDone == 0 )
      {
        print OUTFILE "\}\\end\{flushleft\}\n";
      }    
      # --- Now look for key-words
      $notDone = 1;
      $first = 1;
      while( $notDone && $_ )
      {
        $_ = <FILE>;
        if( $debug==1 ) { printf(" line:  $_ ");} 
        if( !/\/\// )  # skip lines that are not comments, these could be the initialization list
	{
          next;
	}
        if( /\/\/\\end/ )  # look for //\end
        {
          if( $debug==1 ) { printf(" *** end found *** ");} 
          $notDone=0;
  	  last;
        }
  
        if( /\/\/[ \t]*\/\w+/ )  # look for //[space or tab].../(no space)
        {
          $keyword = $_;
          $keyword =~ s/\/\/[ \t]*\/([^:]*):.*/\1/;
          $rest = $_;
          $rest =~ s/\/\/[ \t]*\/([^:]*):(.*)/\2/;
          chop($keyword);
  	  if( $debug==1 ) { printf("+++ keyword = $keyword \n");} 
          if( $first )
	  {
            print OUTFILE "\\begin{description}\n";
            $first = 0;
	  }
          # print OUTFILE "\\item\[$keyword :\]~\\newline $rest";
          print OUTFILE "\\item\[{\\bf $keyword:}\] $rest";
        }
        else
        {
          $l = $_;
  	  $l =~ s/^\/\///;
          print OUTFILE $l;      
        }
      }
      if( $first == 0 )
      {
        print OUTFILE "\\end{description}\n";
      }
      if( $debug==1 ) { printf(" *** close the output file \n");} 
    }
    close(OUTFILE);
  }
}

close(FILE);
