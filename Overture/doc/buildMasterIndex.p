eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
#
# perl program to build a master index for Overture
#  usage: 
#         buildMasterIndex.p
# 

# -- first build the file masterIndex.idx that combines all the separate indicies
# into one

$debug = 0;

@fileNames = ("../mapping/mapping.idx",
              "../hype/hyperbolic.idx",
              "../gf/gf.idx",
              "../op/doc/op.idx",
              "../ogen/ogen.idx",
              "../grid/gridGuide.idx",
              "../grid/grid.idx",
              "/home/henshaw/res/OverBlown/doc/ob.idx",
              "/home/henshaw/res/OverBlown/doc/obRef.idx",
              "../otherStuff/otherStuff.idx",
              "../ogshow/GraphicsDoc.idx",
              "../ogshow/ogshow.idx",
              "../primer/primer.idx");

@names =  ("MP","HY","GF","OP","GG","GU","GR","OBU","OBR","OS","PS","SF","PR"); 

$output="masterIndex.idx";
open(OUTFILE,">$output") || die "cannot open file $output!" ;

$i=0;
foreach $fileName ( @fileNames )  # process all files
{
  $prefix = $names[$i];
  print "$fileName\n";
  
  open(FILE,"$fileName") || die "cannot open file $fileName!" ;
  
  
  while( <FILE> )
  {
    $line = $_;
    # the latex PREFIX macro will change the page numbering
    $line =~ s/\}\{/|PREFIX\{$prefix\}\}\{//
      
    # $line =~ s/\{([0-9]*)\}$/\{$prefix-\1\}/;
    # $line =~ s/\{([0-9]*)\}$/\{$prefix\1\}/;

    print OUTFILE $line;
    
  }
    $i++;
}

close(OUTFILE);
close(FILE);

printf("Built $output as a merger of all document .idx files. Now I will run `makeindex masterIndex.idx'\n");
system("makeindex masterIndex.idx");

# -- now convert the .ind file into a form appropriate for latex2html
# In each html directory like webPage/gfHTML/ we search the node#.html that holds the index
# information. This is the second last file (i.e. highest number # minus 1) This file indicates
# the html link for each index key-word. As we read through the masterIndex.ind file and find
# each key-word we then read the appropriate node###.html file to find the html link. These
# files are all in alphabetical order so we can read them sequentially.

%htmlIndexFile = ( 'MP'  => "node00.html", # these are filled in below to the correct nodes holding the index info
                   'HY'  => "node00.html", 
                   'GF'  => "node00.html", 
                   'OP'  => "node00.html", 
                   'GG'  => "node00.html", 
                   'GU'  => "node00.html", 
                   'GR'  => "node00.html", 
                   'OBU' => "node00.html", 
                   'OBR' => "node00.html", 
                   'OS'  => "node00.html", 
                   'PS'  => "node00.html", 
                   'SF'  => "node00.html", 
                   'PR'  => "node00.html"  
                 );


$webDir = "/home/henshaw/webPage/documentation";

%htmlIndexDir = ( 'MP'  => "$webDir/mappingHTML", 
		  'HY'  => "$webDir/hyperbolicHTML", 
		  'GF'  => "$webDir/gfHTML", 
		  'OP'  => "$webDir/opHTML", 
		  'GG'  => "$webDir/ogenHTML", 
		  'GU'  => "$webDir/gridGuideHTML", 
		  'GR'  => "$webDir/gridHTML", 
		  'OBU' => "$webDir/obHTML", 
		  'OBR' => "$webDir/obRefHTML", 
		  'OS'  => "$webDir/otherStuffHTML", 
		  'PS'  => "$webDir/GraphicsDocHTML", 
		  'SF'  => "$webDir/ogshowHTML", 
		  'PR'  => "$webDir/primerHTML"  
                );

%htmlRelativeIndexDir = ( 'MP'  => "../mappingHTML", 
			  'HY'  => "../hyperbolicHTML", 
			  'GF'  => "../gfHTML", 
			  'OP'  => "../opHTML", 
			  'GG'  => "../ogenHTML", 
			  'GU'  => "../gridGuideHTML", 
			  'GR'  => "../gridHTML", 
			  'OBU' => "../obHTML", 
			  'OBR' => "../obRefHTML", 
			  'OS'  => "../otherStuffHTML", 
			  'PS'  => "../GraphicsDocHTML", 
			  'SF'  => "../ogshowHTML", 
			  'PR'  => "../primerHTML"  
                );

%docWasChecked = ( 'MP'  => "0", 
                   'HY'  => "0", 
                   'GF'  => "0", 
                   'OP'  => "0", 
                   'GG'  => "0", 
                   'GU'  => "0", 
                   'GR'  => "0", 
                   'OBU' => "0", 
                   'OBR' => "0", 
                   'OS'  => "0", 
                   'PS'  => "0", 
                   'SF'  => "0", 
                   'PR'  => "0"  
                 );


# find the second highest numbered file of the form node#.html in each HTML directory
# this file holds the html link for each index key-word.
foreach $dir ( @names )  
{
  $i=1;
  while( -e "$htmlIndexDir{$dir}/node$i.html" )
  {
    $nodeFile="$htmlIndexDir{$dir}/node$i.html";
    # printf("$nodeFile exists\n");
    $i++;
  }
  $i=$i-2;
  $indexNode="$htmlIndexDir{$dir}/node$i.html";
  printf("index entries for $dir should be found in : $indexNode\n");
  $htmlIndexFile{$dir}=$indexNode;

  $indexFile=$dir."file";
  open($indexFile,$htmlIndexFile{$dir}) || die "cannot open file $htmlIndexDir{$dir}!" ;

}

$indFile = "masterIndex.ind";

open(FILE,$indFile) || die "cannot open file $indFile!" ;
$output="makeMasterIndex.tex";
open(OUTFILE,">$output") || die "cannot open file $output!" ;

$help="help.index";
open(HELP,">$help") || die "cannot open file $help!" ;

$numberOfEntries=0; # count the number of entries we find
$indent=0;  # indent more for \subitems's
while( <FILE> )
{
  $line = $_;
  chop($line);

  $line =~ s/begin\{theindex\}/begin\{description\}/;
  $line =~ s/end\{theindex\}/end\{description\}/;

  # when a \subitem first appears create a new level of \begin{description}
  if( $indent eq 0 && /\\subitem/ )
  {
    $indent=1;
    print OUTFILE "   \\begin\{description\}\n";
  }
  if( $indent eq 1 && /\\item/ )
  {
    $indent=0;
    print OUTFILE "   \\end\{description\}\n";
  }
  $line =~ s/\\subitem/\\item/;

  # append continuation lines
  while( $line =~ /,[ ]*$/ )
  { # line ends in a "," -- we must have a continuation line
    $l=<FILE>;
    chop($l);
    $line=$line.$l;
  }
  
  # replace each PREFIX entry with the appropriate HTML link
  foreach $doc (@names )
  {
    $docWasChecked{$doc}=0;
  }
  if( $line =~ /\\item.*\\PREFIX/ )
  {
    # line is of the form : \item artificial diffusion, \PREFIX{OBU}{8}, \PREFIX{OBU}{24}
    $item=$line;

    # printf("before: item=[$item] ");

    $item =~ s/\\PREFIX.*//;
    $item =~ s/\\item (.*),.*/\1/;
    $item =~ s/^[ ]*//;
    # there may be multiple PREFIX's for a given item
    
    # watch out for special symbols
    $item =~ s/\+/.*/g;     # change "+" to .*
    $item =~ s/\\\w*[ ]*/.*/g;  # change macros such as \TeX to .* since we don't know how they are expanded
    if( $debug>0 ){ printf("item=[$item] in ");}

    $l = $line;
    $oldDocument="";
    while( $l =~ /\\PREFIX/ )  # replace all prefix entries of the form \PREFIX{OBU}{25} ...
    {
      $document=$l;
      $document =~ s/.*?\\PREFIX\{(.*?)\}.*/\1/;  # note *? : match minimal
      if( $debug>0 ){ printf("document=[$document] "); }
      $l =~ s/\\PREFIX\{(.*?)\}//;

      # if( $document ne $oldDocument ) # if we have not already looked in this document for this entry
      if( $docWasChecked{$document}==0 ) # if we have not already looked in this document for this entry
      {
        if( $debug>0 ){ printf(" look..."); }
        $docWasChecked{$document}=1;

        $oldDocument=$document;
        # look for the html link info in the file $indexFile:
	$indexFile=$document."file";
	$found=0;
	while( $found==0 && ($ln = <$indexFile>) )
	{
	  chop($ln);
	  if( $ln =~ /$item/ )
	  {
	    $found=1;
	    if( $debug>0 ){ printf("found $ln in file $indexFile, "); }
            $numberOfEntries++;
            # The next line in the should be of the form of a reference
            # <DD><A HREF="node331.html#8846">TFIMapping: Transfinite-Interpolation</A>
            $ln = <$indexFile>;
	    chop($ln);
	    $ln =~ s/.*HREF="(.*?)">.*/$1/;
	    if( $debug>0 ){ printf("-> [$ln] "); }
	    

	  }
	}
        if( $found==0 )
	{
	  printf("\n\n ***FATAL ERROR, item=[$item] not found\n");
          printf(" *** the index entries should be in $indexFile: $htmlIndexFile{$document}\n");
          printf(" *** original line=$_\n");
          printf(" *** current line=$line\n");
	  exit;
	}
      }
      else
      {
        #if we are looking for the same reference but in a file we have already looked in then
        #we just read the next line in the file (since multiple references follow on separate lines
        # as in
        # <DT><STRONG>artificial diffusion</STRONG>
        # <DD><A HREF="node7.html#741">Incompressible flow around a</A>
        #   | <A HREF="node32.html#1345">Artificial Diffusion</A>
        $indexFile=$document."file";
	$ln = <$indexFile>;
	chop($ln);
	$ln =~ s/.*HREF="(.*?)">.*/$1/;
	if( $debug>0 ){ printf("(SAME: $document)-> [$ln] "); }
      }
      # print OUTFILE "\htmladdnormallink{$item}{file:$htmlIndexDir{$dir}$ln}"
      $line =~ s/\\PREFIX\{\w*\}\{(.*?)\}/\\htmladdnormallink\{$document:$1\}\{$htmlRelativeIndexDir{$document}\/$ln\}/;
    }
    if( $debug>0 ){ printf("\n"); }
  }  


  print OUTFILE $line,  "\n";
    
  $line =~ s/[ ]*\\item[ ]*//;
  if( $indent==1 )
  {
    $line = $mainLabel . "!" . $line;
  }
  else
  {
    $mainLabel = $line;
    $mainLabel =~ s/, \\htmladdnormallink.*//g;
    if( $debug>0 ){ print "mainLabel : [$mainLabel]\n"; }
  }
  if( $line =~ "\indexspace" || $line =~ /^[ ]*$/ )
  {
  }
  else
  {
    print HELP $line, "\n";
  }
}
printf("Processed $numberOfEntries entries.\n");
if( $indent eq 1 )
{
  print OUTFILE "   \\end\{description\}";
}

close(OUTFILE);
close(HELP);
close(FILE);



exit

