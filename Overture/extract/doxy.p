eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
#
# perl program to convert "extra.p" comments to doxygen
#  usage: 
#         doxy.p fileName1 [fileName2] [ fileName3] ...
# 

@fileNames = @ARGV;
# $fileName          = $ARGV[0];

$debug = 0;   # set to 1 for debug info

foreach $fileName ( @fileNames )  # process all files
{
  
  $backupFile="$fileName.doxy";
  printf("doxy.p: process file [$fileName] (backup=[$backupFile]).\n");

  # We only save a backupFile if it doesn't already exist. 
  # In this way
  if( !(-e $backupFile) )
  {
    system("cp $fileName $backupFile");
    printf("doxy.p: saving backup file [$backupFile]\n");
  }
  else
  {
     printf("doxy.p: backup file [$backupFile] already exists.\n");
  }
  
  
  if( $debug==1 ) { printf("  fileName = $fileName \n");  }     
  
  open(FILE,"$fileName") || die "cannot open file $fileName!" ;
  
  open(OUTFILE,">doxyJunk.C") || die "cannot open output file junk.X!" ;
  
  while( <FILE> )
  {
    $line = $_;

    ## print OUTFILE $line;


    if( $debug==1 ) { printf(" line:  $line ");} 

    # --- look for //\begin ----
    # THIS IS THE START OF AN EXTRACT.P comment 
    if( /\/\/\\begin/ )  
    {
      $notDone = 1;
      $first = 1;
      $beginLine = $line;

      while( $notDone && $_ )
      {
        $_ = <FILE>;

        if( $debug==1 ) { printf(" line:  $_ ");} 

        if( /^[ \t]*{/ )
        {
          # line begins with a "{"
          printf("************************************************************************************\n");
          printf("doxy.p:ERROR: line found starting with '{' before \\end found for extract.p header.\n");
          printf("   File=[$fileName], near line $.\n");
          printf("   You are likely missing an \\end to match : \n $beginLine");
          printf("   You should fix this file since otherwise doxy.p could remove valid code\n");
          printf("************************************************************************************\n");
          exit 100;
        }

        if( !/^[ \t]*\/\// )  # skip lines that are not comments -- these should be the function declaration
	{
          print OUTFILE $_;
          next;
	}
        if( /\/\/\\end/ )  # look for //\end
        {
          if( $debug==1 ) { printf(" *** end found *** ");} 
          $notDone=0;
  	  last;
        }
  
        # --- Look for extract.p KEYWORDS 
        # look for //[space or tab].../(no space)
        if( /\/\/[ \t]*\/\w+/ )  
        {
          $keyword = $_;
          $keyword =~ s/\/\/[ \t]*\/([^:]*):.*/\1/;
          $rest = $_;
          $rest =~ s/\/\/[ \t]*\/([^:]*):(.*)/\2/;

          # replace \_ by _
          $keyword =~ s/\\_/_/g;
          $rest =~ s/\\_/_/g;

          chop($keyword);
  	  if( $debug==1 ) { printf("+++ keyword = $keyword \n");} 
          if( $first )
	  {
            ### print OUTFILE "\\begin{description}\n";
            $first = 0;
	  }
          # print OUTFILE "\\item\[$keyword :\]~\\newline $rest";
          ### print OUTFILE "\\item\[{\\bf $keyword:}\] $rest";

         if( $keyword eq "Purpose" )
         {
           print OUTFILE "/// \\brief $rest";
         }
	 elsif( $keyword eq "Description" )
	 {
           print OUTFILE "/// \\details $rest";
	 }
	 elsif( $keyword eq "Return"  || $keyword eq "Return values" || $keyword eq "Return value" )
	 {
           # // /Return values: 0=sucees.
           print OUTFILE "/// \\return $rest";
	 }
	 else
         { # assume this is a parameter:
           # print OUTFILE "/// \\param $keyword$rest";

           $line = $_;
           $line =~ s/\/\/[ \t]*\///;
           # replace \_ by _
           $line =~ s/\\_/_/g;
           print OUTFILE "/// \\param $line";
         }
        }
        else
        {
          # $l = $_;
  	  # $l =~ s/^\/\///;
          # print OUTFILE $l;      
          if( /\/\/[ ]*[=]+$/ )
          {
            # line of the form:
            #  // ========================
	    print OUTFILE $_;
          }
          else
          {
            # this must be a random comment
            $line = $_;
            $line =~ s/^\/\///;


            # Look for \begin{description} or \begin{itemize} or \begin{enumerate}
            $line =~ s/\\begin{description}/<ul>/; # begin unordered list
            $line =~ s/\\end{description}/<\/ul>/; # end unordered list

            $line =~ s/\\begin{itemize}/<ul>/; # begin unordered list
            $line =~ s/\\end{itemize}/<\/ul>/; # end unordered list

            $line =~ s/\\begin{enumerate}/<ol>/; # begin ordered list
            $line =~ s/\\end{enumerate}/<\/ul>/; # end ordered list

            $line =~ s/\\item\[([\w]*)\]/<li> <B>\1<\/B>/; # description item of the form \item[label] ...
            $line =~ s/\\item/<li>/; 

            print OUTFILE "/// $line";      
          }
        }
      }
      if( $first == 0 )
      {
        ### print OUTFILE "\\end{description}\n";
      }
      if( $debug==1 ) { printf(" *** close the output file \n");} 
    } # end \begin
    else
    {
      print OUTFILE $line; 
    }
  }
  close(OUTFILE);

  #  now copy over the original file
  system("mv doxyJunk.C $fileName");

}



close(FILE);
