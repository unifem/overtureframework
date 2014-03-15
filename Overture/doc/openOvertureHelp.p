#!/usr/local/bin/perl
# perl program to open a web page for help on a Overture subject.
#  
#  usage: 
#         openOvertureHelp.p documentPrefix label
#  where
#       documentPrefix is the prefix used for the document, such as GG for the grid generator documentation
#                      or PR for the primer documentation
#       label is the index entry as specified in the LaTeX document, such as "airfoil" or 
#             "boundary conditions!explicit application"
#
# NOTES: this script looks for the following environmental variables
#     Overture : location of the Overture library
#     OvertureWebPage : if defined, look for web pages here, otherwise look in 
#           http://www.llnl.gov/CASC/Overture/henshaw/documentation
@fileNames = @ARGV;

if( $ARGV[0] eq "" )
{
  printf("Usage: labelToURL.p documentPrefix label \n");
  printf(" Example: `labelToURL GG airfoil' will open the URL for the label=airfoil in the grid generator doc  \n");
  exit 1;
}

$documentPrefix= $ARGV[0];
$label = $ARGV[1];
for( $i=2; $i<=$#ARGV; $i++ )
{
  $label = $label . " ". $ARGV[$i];   
}

print "search for documentPrefix=[$documentPrefix], label [$label]\n";

$Overture = $ENV{"Overture"};

$OvertureWebPage = $ENV{"OvertureWebPage"};
if( $OvertureWebPage eq "" )
{
  $OvertureWebPage = "http://www.llnl.gov/CASC/Overture/henshaw/documentation";
}
print "openOvertureHelp: OvertureWebPage=$OvertureWebPage\n";

$help="$Overture/doc/help.index";
open(HELP,"$help") || die "cannot open the help index file $help!" ;

$notDone = 1;
while( !eof(HELP) && $notDone==1 )
{
  $line =  <HELP>;

  if( $line =~ /^$label, \\htmladdnormallink/ )
  {
    print "line found: $line";

    if( $line =~ /htmladdnormallink\{$documentPrefix:\d*\}\{(\S*)\}/ )  # \S = non white space
    {
      $link = $1;

      print "link = $link\n";
      $notDone=0;
    }
  }
}
close(HELP);

if( $notDone==0 )
{
  # $overtureURL = "http://www.llnl.gov/CASC/Overture/henshaw/documentation";
  $overtureURL = $OvertureWebPage;
  $link =~ s/^\.\./$overtureURL/;
  print "URL: $link\n";

  system( "netscape -remote 'openURL($link)'");
  exit 0;
}
else
{
   print "labelToURL:ERROR: unable to find documentPrefix=[$documentPrefix], label [$label]\n";
}

exit 1;


