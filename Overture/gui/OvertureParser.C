#include "OvertureParser.h"

// On the dec: these next includes seem to mess with bool -- causes problems if we include Overture stuff
#ifdef OV_USE_PERL
extern "C"
{
#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>
}
#endif

int OvertureParser::debug=0;

OvertureParser::
OvertureParser(int argc, char **argv) : parserPointer(0) // kkc 031230 added ctor initializer to prevent seg fault on linux (gcc 3.3.2) machines in destructor (?!)
//===============================================================================================
//  Class for parsing commands using perl
//  Commands containing a semi-colon are processed by perl; otherwise any perl variables (beginning with a '$')
//  are replaced.
//===============================================================================================
{
#ifdef OV_USE_PERL
  PerlInterpreter *const& my_perl_c = (PerlInterpreter *)parserPointer;

  PerlInterpreter *& my_perl = (PerlInterpreter *&)my_perl_c;
  my_perl=NULL;

  //kkc 051207  char *embedding[] = { "", "-e", "0" };
  char **embedding=0;
  int npa = 3;
  embedding = new char*[npa+argc];
  embedding[0] = (char *)"";
  // execute one line at a time starting with the argument that follows -e
  embedding[1] = (char *)"-e";
  // Load some usefull modules, maybe we should have an Overture module...
  char *overture = getenv("Overture");
  aString cmd("use Getopt::Long; use Getopt::Std;");
  if ( overture ) 
    { // add $Overture/include to the list of directories perl searches for modules
      cmd = aString("use Getopt::Long; use Getopt::Std; Getopt::Long::Configure(\"pass_through\"); use lib \"")+aString(overture) +aString("/include\"; use OvertureUtility;");
    }

  embedding[2] = (char *)cmd.c_str();

  // tack on the given argument list, probably should strip out things like noplot/nopause... etc ?
  for ( int i=npa; i<(npa+argc);i++ )
    embedding[i] = argv[i-npa];

  // perldoc.perl.org suggest that PERL_SYS_INIT3() should be invoked before the first interpreter 
  // is created and PERL_SYS_TERM() invoked after the last interpreter is freed.
  // The NULL argument forces PERL_SYS_INIT3() to take the current environment:
  PERL_SYS_INIT3(&argc,&argv,NULL); // 2013/11/06 - fix from Andrew Glassby for MAC

  my_perl = perl_alloc();
  perl_construct( my_perl );

  perl_parse((PerlInterpreter *)my_perl, NULL, npa+argc, embedding, NULL);
  perl_run(my_perl);

  parserPointer = my_perl;  // *wdh* 2012/06/17 -- this was not being set and thus my_perl was not being destroyed
  delete [] embedding;
#endif
}

OvertureParser::
~OvertureParser()
{
#ifdef OV_USE_PERL
  PerlInterpreter *const& my_perl_c = (PerlInterpreter *)parserPointer;

  PerlInterpreter *& my_perl = (PerlInterpreter *&)my_perl_c;
  if( my_perl!=NULL )
  {
    perl_destruct(my_perl);
    perl_free(my_perl);
  }

  PERL_SYS_TERM(); // 2013/11/06 - fix from Andrew Glassby
#endif
}

int OvertureParser::
parse(aString & answer )
// ===============================================================================
// /Description:
//   Parse a string with perl.
//   If the string contains a semi-colon, ';', then treat the string as perl
//   commands to process. Otherwise evaluate the string to replace any perl variables.
//   
// /answer (input/output): On input a string to parse. On output the string with perl variables
//    replaced IF the input string contained no semi-colons. If the input string had semi-colons 
//    then answer remains unchanged on output.
// /Return value: 0: answer was not changed or was evaluated replacing any perl variables with their values.
//                1: answer contained a semi-colon and was processed by perl, answer remains unchanged. 
//                2: answer contained a newline indicating multiple commands
// ===============================================================================
{
  int returnValue=0;

#ifdef OV_USE_PERL  

  const char * canswer=answer.c_str();
      
  if( strchr(canswer,';')!=NULL )
  {
    // answer has a semi-colon -- just evaluate
    // if( debug>0 ) printf("OvertureParser::Sending the answer to be parsed by perl...\n");

    eval_pv(canswer, TRUE);
//     if( SvTRUE(ERRSV) )
//     { 
//       printf(" OvertureParser::parse: I am going to purposely abort so that you can get a "
//              " traceback from a debugger\n");      
//      ::abort();
//     }
    
    returnValue=1;
  }
  else if( strchr(canswer,'$')!=NULL )
  {
    // NOTE: $overtureParserline should be unique so as to not over-write any user variables
    aString line = "$overtureParserline = \"" + answer + "\";";
    eval_pv((const char*)line.c_str(), TRUE);
    STRLEN numChars;
    const char *result=SvPV(get_sv("overtureParserline", FALSE), numChars);
    if( debug>0 && strlen(result)<160 ) printf("OvertureParser::result = [%s]\n",result);
    
    if( strchr(result,'\n')!=NULL )
    {
      // answer contains a new line
      returnValue=2;
    }
    answer=result;
  }
#endif
  return returnValue;
}

