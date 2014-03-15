#!/n/c3servet/henshaw/bin/perl

# usage:
#         c  filename

# These files contain the "Template" code

$fileName   = $ARGV[0];

@filenames=("$fileName");


foreach $file ( @filenames )
{

  open(FILE,"$file") || die "cannot open file $file!" ;
  open(FILE2,">$file.N") || die "cannot open file $file!" ;

  while( <FILE> )
  {
    $line = $_;   

    $line =~ s/\b(myOldName)\b/myNewName/g;

    print FILE2 $line;
  }
  close(FILE);
  close(FILE2);
} 

