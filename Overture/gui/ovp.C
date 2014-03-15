#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "OvertureParser.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <queue>
OV_USINGNAMESPACE(std);
#else
#include <queue.h>
#endif

//===============================================================================================
//    Test routine for parsing perl from Overture
//
//===============================================================================================

void 
test( queue<aString> & stringCommands )
{
  aString *s=new aString("test");
  stringCommands.push(*s);
  delete s;
}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  GL_GraphicsInterface ps(false,"ovp");

  printf("Usage:ovp [file.cmd]]\n");
  aString commandFileName="";
  if( argc>1 )
  {
    commandFileName=argv[1];
  }

  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

/* ----
  queue<aString> stringCommands;
  
  stringCommands.push("hello");
  cout << " stringCommands.front()=" << stringCommands.front() <<endl;
  
  stringCommands.push(aString("world"));
  cout << " stringCommands.front()=" << stringCommands.front() <<endl;

  test( stringCommands );
  cout << " stringCommands.front()=" << stringCommands.front() <<endl;

  stringCommands.pop();
  cout << " stringCommands.front()=" << stringCommands.front() <<endl;

  aString line;

  line="hello\nworld";
  int i;
  for( i=0; i<line.length(); i++ )
  {
    char c =line[i];
    printf(" line[%i]=[%c]\n",i,c);
    if( c=='\n' )
    {
      printf(" character %i is a newline\n",i);
    }
  }
  
  ---- */

  GraphicsParameters psp;               // create an object that is used to pass parameters
    
  char buff[160];  // buffer for sprintf
  aString answer,answer2;
  aString menu[] = { "!ovp",
		    "exit",
                    "" };

  OvertureParser::debug=3;
  
  OvertureParser parser;

  for(;;)
  {
    ps.getMenuItem(menu,answer,"Enter a string to parse");
    cout << "answer(in)=[" << answer << "]\n";
  
    if( answer=="exit" )
    {
      break;
    }
    else
    {
      parser.parse(answer);

      cout << "answer(out)=[" << answer << "]\n";

    } // end for(;;)
    
  }

  Overture::finish();          
  return 0;
}
