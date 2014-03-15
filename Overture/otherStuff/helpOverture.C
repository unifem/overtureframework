#include "GenericDataBase.h"
#include "aString.H"

int
helpOverture( const aString & documentPrefix, const aString & label )
// ====================================================================================
// /Description:
//    Open web page documentation at the page corresponding to a given document and a given index label.
//  The document prefixes are
// \begin{description}
// \item {\bf AP}: A++P++ Reference Manual.
// \item {\bf AQ}: A++ Quick Reference Card : {\ff A++P++/DOCS/Quick\_Reference\_Card.tex}
// \item {\bf ES}: Oges ``Equation Solver'' documentation \cite{OGES}.
// \item {\bf FV}: Finite volume operators \cite{FVOPERATORS}.
// \item {\bf GF}: Grid and grid function documentation\cite{GF}.
// \item {\bf GG}: Grid generation documentation, Ogen,  \cite{OGEN}. 
// \item {\bf GR}: Grid reference guide\cite{gridRef}.
// \item {\bf GU}: Grid user guide\cite{gridGuide}.
// \item {\bf HY}: Hyperbolic grid generator documentation \cite{HyperbolicGuide}.
// \item {\bf MP}: Mapping class documentation \cite{MAPPINGS}.
// \item {\bf OBR}: Reference guide OverBlown\cite{OverBlownReferenceGuide}.
// \item {\bf OBU}: User guide for the OverBlown Navier-Stokes flow solver \cite{OverBlownUserGuide}.
// \item {\bf OP}: Finite difference operators and boundary conditions\cite{OPERATORS}.
// \item {\bf OS}: The other stuff documentation\cite{OTHERSTUFF}.
// \item {\bf PR}: A primer for Overture\cite{PRIMER}.
// \item {\bf PS}: Interactive plotting\cite{PLOTSTUFF}.
// \item {\bf SH}: Show file documentation \cite{OGSHOW}.
// \end{description}
//
// /documentPrefix (input) : is the prefix used for the document, such as GG for the grid generator documentation
//                      or PR for the primer documentation
// /label (input) :  is the index entry as specified in the LaTeX document, such as "airfoil" or 
//             "boundary conditions!explicit application". If label="Overture" then the Overture home page will
//             be accesed. If label="master index" then the master index will be found.
//
// /Notes: This program looks for the perl script "openOvertureHelp.p" in the Overture/doc directory.
//       This program also looks for the following environmental variables
// \begin{verbatim}
//    Overture : location of the Overture library (actually openOvertureHelp.p looks for this)
//    OvertureWebPage : if defined, look for Overture documentation web pages here, otherwise look in 
//          http://www.llnl.gov/CASC/Overture/henshaw/documentation
// \end{verbatim}
//
// ====================================================================================
{
  int returnCode=0;
  aString overtureWebPage,command;
  overtureWebPage=getenv("OvertureWebPage");
  if( overtureWebPage=="" )
    overtureWebPage="http://www.llnl.gov/CASC/Overture/henshaw/documentation";

  cout << "helpOverture: overtureWebPage = " << overtureWebPage << endl;
  
  if( label == "Overture" )
  {
    command = "netscape -remote 'openURL(" + overtureWebPage + ")'";
    system( command );
    // system( "netscape -remote 'openURL(http://www.llnl.gov/CASC/Overture)'");
  }
  else if( label == "master index" )
  {
    command = "netscape -remote 'openURL(" + overtureWebPage + "/masterIndex/node2.html)'";
    system( command );
    // system( "netscape -remote 'openURL(http://www.llnl.gov/CASC/Overture/henshaw/documentation/masterIndex/node2.html)'");
  }
  else 
  {
    aString overture;
    
    // command = "perl $Overture/doc/openOvertureHelp.p " + documentPrefix + " " + label;
    overture = getenv("Overture");
    if( overture=="" )
    {
      printf("helpOverture:ERROR: unable to get the environmental variable `Overture'\n");
      return 1;
    }
    cout << "helpOverture: environmental variable Overture =" << overture << endl;
    command = "perl " + overture + "/doc/" + "openOvertureHelp.p " + documentPrefix + " " + label;
    returnCode = system(command);
    printf(" returnCode = %i\n",returnCode);
  }
  
  return returnCode;
}
