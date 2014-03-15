#!/n/c3servet/henshaw/bin/perl

# perl program to create Templates for gridFunction's


# These files contain the "Template" code
@filenames=("ListOfReferenceCountedObjects.h","ListOfReferenceCountedObjects.C");

# Make these classes into Templates named ListOf<className>
@classNames=("floatGenericGridFunction","doubleGenericGridFunction","intGenericGridFunction",
             "floatMappedGridFunction","doubleMappedGridFunction","intMappedGridFunction",
             "floatCompositeGridFunction","doubleCompositeGridFunction","intCompositeGridFunction",
             "intArray","floatArray","doubleArray",
             "ListOfIntArray","ListOfFloatArray","ListOfDoubleArray",
             "GenericGrid","MappedGrid","GridCollection","CompositeGrid",
             "MappedGridPlus","CompositeGridPlus","ListOfMappedGridPlus",
             "MappingRC");

# Here are the include file names that we add to the top of the .h file
@includeFile=("floatGenericGridFunction.h","doubleGenericGridFunction.h","intGenericGridFunction.h",
              "floatMappedGridFunction.h","doubleMappedGridFunction.h","intMappedGridFunction.h",
              "floatCompositeGridFunction.h","doubleCompositeGridFunction.h","intCompositeGridFunction.h",
              "A++.h","A++.h","A++.h",
              "ListOfIntArray.h","ListOfFloatArray.h","ListOfDoubleArray.h",
              "GenericGrid.h","MappedGrid.h","GridCollection.h","CompositeGrid.h",
              "MappedGridPlus.h","CompositeGridPlus.h","ListOfMappedGridPlus.h",
              "MappingRC.h");

foreach $file ( @filenames )
{

  $i=-1;
  foreach $class ( @classNames )
  {
    $i++;
    open(FILE,"$file") || die "cannot open file $file!" ;

    $ext = $file; $ext = chop($ext);  # file extension, h or C

    $template = $file;
    chop($template);  chop($template); # template name

    $include = $includeFile[$i];            # Here is the include file

    $className="ListOf\u$class";
    print(" className = $className \n");
    
    $cgfile = "\u$class";   # upper case first letter
    $cgfile = "ListOf$cgfile.$ext";
    
    open(FILEC,">$cgfile");

    while( <FILE> )
    {
      $line = $_;   

      $line =~ s/LIST_OF_REFERENCE_COUNTED_OBJECTS_H/\U${className}_H/;
      $line =~ s/include "ReferenceCounting.h"/include "ReferenceCounting.h"\n#include "$include"/;
      
      $line =~ s/template<class T>//g;     # remove lines with this
      $line =~ s/template <class T>//g;     # remove lines with this
      $line =~ s/$template<T>/$className/g;
      $line =~ s/\bT\b/$class/g;
      $line =~ s/\b($template)\b/$className/g;

      print FILEC $line;
    }
    close(FILE);
   } 

}

