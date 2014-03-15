eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl

# perl program to de-Templify a Template Class
#  usage: 
#         dt.p  templateFileName className templatedClassName includeFile

$template          = $ARGV[0];
$className         = $ARGV[1];
$templateClassName = $ARGV[2];
$includeFile       = $ARGV[3];

if( $template eq "" )
{
  printf("Usage: dt templateFileName className templatedClassName includeFile \n");
  printf("For example templateFileName= ListOfReferenceCountedObjects \n");
  printf("            className= floatMappedGridFunction \n");
  printf("            templatedClassName= ListOfFloatMappedGridFunction \n");
  printf("            includeFile= floatMappedGridFunction.h \n");
  exit;
}

#printf(" template=$template, className=$className, templateClassName=$templateClassName,".
#       " includeFile =$includeFile \n");

printf("  className=$className, templateClassName=$templateClassName \n");

# These files contain the "Template" code
@filenames=("$template.h","$template.C");

# Make these classes into Templates named ListOf<className>
@classNames=("$className");

# First look for some info about the Template
$file = "$template.h";
open(FILE,"$file") || die "cannot open file $file!" ;

$templateH = <FILE>;
$templateH =~ s/#ifndef //;    # The first line must be of the form #ifndef FILE_H     
chop($templateH);

# printf("template define string is <$templateH> \n");

$T = "T";
while( <FILE> )
{
  if( /.*<class .*/)
  {
    $T = $_;
    $T =~ s/.*<class (.+)>.*/\1/;
    last;
  }
}
chop($T);
# printf("Template generic symbol = <$T>\n");

while( <FILE> )
{
  # printf(" looking for templateName: $_ \n");
  if( /.*class .*/ )
  {
    $templateName = $_;
    $templateName =~ s/.*class (\w*).*/\1/;
    last;
  }
}
chop($templateName);
# printf("templateName = <$templateName> \n");


close(FILE);



foreach $file ( @filenames )
{

  $includeFileAdded=0;
  foreach $class ( @classNames )
  {
    open(FILE,"$file") || die "cannot open file $file!" ;

    $ext = $file; $ext = chop($ext);  # file extension, h or C

    $cgfile = "\u$class";   # upper case first letter
    $cgfile = "$templateClassName.$ext";
    
    open(FILEC,">$cgfile");

    while( <FILE> )
    {
      $line = $_;   


      if( $includeFileAdded == 0 && $ext eq "h" )
      { # add a forward reference to the .h file
        $line2 = $line;
	chop($line2);
	if( $line2 =~ /"[ ]*"/ || $line2 eq "" )
	{
          if( $includeFile eq "A++.h" )
	  { # special case for A++
            $line = "#include \"$includeFile\"\n";
	  }
          elsif( $className eq "float" || $className eq "int" || $className eq "double" || $className eq "long" )
          {}  # nothing needed in this case
	  else
	  {
            $line = "class $className;\n";
	  }
          $includeFileAdded=1;
        }
      }
      if( $includeFileAdded == 0 && $ext eq "C" )
      { # add the include line to the .C file
        $line2 = $line;
	chop($line2);
	if( $line2 =~ /"[ ]*"/ || $line2 eq "" )
	{
          $line = "#include \"$includeFile\"\n";
          $includeFileAdded=1;
        }
      }

      $line =~ s/$templateH/\U${templateClassName}_H/;
      
      $line =~ s/template<class $T>//g;     # remove lines with this
      $line =~ s/template <class $T>//g;     # remove lines with this
      $line =~ s/$templateName<$T>/$templateClassName/g;
      $line =~ s/\b$T\b/$class/g;
      $line =~ s/\b($templateName)\b/$templateClassName/g;
      $line =~ s/\b($template)\b/$templateClassName/g;

      print FILEC $line;
    }
    close(FILE);
   } 

}
