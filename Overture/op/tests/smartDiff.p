eval 'exec perl -S $0 ${1+"$@"}'
if 0;
# perl program to perform a diff of check files.
#  usage: 
#         smartDiff  newFile oldFile
#
#  It assumed that each line in the check file begins with a unique "key" of the form
#         <<key>> results
#  The smart diff will look for lines with matching keys and compare them.
# 
#  Return values
#    0 = success
#   -1 = warning : there were keys that did not appear in both files but when a key appeared in both
#                  files the lines aggreed. Maybe a line was added or removed from the file.
#    1 = error   : some lines with the same key did not agree.

if( $#ARGV < 1 )
{
  print "usage: smartDiff newFile oldFile\n";
  exit(1);
}
$file1=$ARGV[0];
$file2=$ARGV[1];

printf(" compare files $file1 and $file2 \n");
  
open(FILE1,"$file1") || die "cannot open file $file1!" ;
open(FILE2,"$file2") || die "cannot open file $file2!" ;

$numberOfErrors=0;
$numberOfWarnings=0;

while( <FILE1> )
{
  $line1 = $_;
  $lineNumber1=$.;

  $line2= <FILE2>;
  if( $line1 eq $line2 ){ next; }

  # lines do not agree:
  # print "Do a careful check for\n$line1";

  $key1=$line1; chop($key1);
  $key1 =~ s/\<\<(.*)\>\>.*/\1/;  # look for a pattern ^<<.....>>
  open(FILE2A,"$file2") || die "cannot open file $file2!" ;

  # **** this is slow -- maybe put file2 into an assoc array ??
  while( <FILE2A> )
  {
    $line3=$_;
    $key2=$line3; chop($key2);
    $key2 =~ s/^\<\<(.*)\>\>.*/\1/;

    if( $key1 eq $key2 ){ $lineNumber2=$.; last; }
  }
  close(FILE2A);

  if( $key1 eq $key2 )
  {
    if( $line1 ne $line3 )
    {
      print "smartDiff:ERROR $numberOfErrors: keys match but not results, file1 $file1, line $lineNumber1 ";
      print " and file2 $file2, line $lineNumber2.\n   file1:$line1";     
      print "   file2:$line3";     
      $numberOfErrors++;
    }
  }
  else
  {
    print "smartDiff:WARNING $numberOfWarnings: no match for key=$key1 on line $lineNumber1 of file $file1.\n";
    $numberOfWarnings++;
  }     

}
close(FILE1);
close(FILE2);

if( $numberOfErrors==0 && $numberOfWarnings==0 )
{
  print "smartDiff: files $file1 and $file2 agree.\n";
  exit(0);
}
elsif( $numberOfErrors==0 )
{
  print "smartDiff:$numberOfWarnings warnings found, some keys did not appear in both files.\n";
  exit(-1);
}
else
{
  print "$numberOfErrors errors and $numberOfWarnings warnings found.\n";
  exit(1);
}


