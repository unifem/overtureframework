#include "Checker.h"


// This class is used for writing a check file for regression testing

//! Build a class for writing a "check file" for regression testing
/*!
   Each line in the check file consists of a unique key and a result:

      <<key1>>  result1
      <<key2>>  result2
      <<key3>>  result3
      <<key4>>  result4

    The key is formed from a set of default labels plus the label passed to the printMessage function
      key = checkLabel[0]/checkLabel[1]/checkLabel[2]/checkLabel[3]/label
   
    The labels should be chosen to make the key unique so that the smartDiff.p perl script
    can perform a smart diff on two check files.
 */
Checker::
Checker( const aString & checkFileName )
{
  fileName=checkFileName;
  checkFile = fopen((const char*)checkFileName,"w" );      // for regression tests.

  if( REAL_EPSILON==DBL_EPSILON )
    errorCutOff = REAL_EPSILON*5000.;
  else
    errorCutOff = REAL_EPSILON*500.;
}

Checker::
~Checker()
{
  printF("\n >>>>>>>>>>> results saved in %s for regression testing <<<<<<\n",(const char*)fileName);
  fclose(checkFile);
}

//! Set the cutOff for errors (errors below this value are set to zero)
void Checker::
setCutOff( real cutOff )
{
  errorCutOff=cutOff;
}


//! Set the checkLabel[index]
/*!
    Each "key" consists of a sequence of labels. This function sets one of these labels.
 */
void 
Checker::
setLabel(const aString & label, const int index)
{
  assert( index>=0 && index<4 );
  checkFileLabel[index]=label;
}

  
//! print a message to the check file and to stdout.
//
void Checker::
printMessage(const aString & label, real error, real time, real timeInit )
{
  assert( checkFile!=NULL );

  aString markLargeError=" ";
  if( error>max(1.e-2,REAL_EPSILON*10000.) )
    markLargeError="  *****";
        
  aString check=checkFileLabel[0]+"/"+checkFileLabel[1]+"/"+checkFileLabel[2]+"/"+checkFileLabel[3]+"/"+label;

  //              123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 
  aString blanks="                                                                                          ";
  printF("%s %s: err = %8.2e, cpu=%7.1e(s)",(const char*)check,
	 (const char*)blanks(0,max(1,90-check.length())),error,time);
  if( timeInit>0. )
    printF(" (%7.1e init)",timeInit);
  printF(" %s\n",(const char*)markLargeError);
    
  // Do not save cpu in the check file. 
  // Truncate very small numbers to zero.
  real truncatedError = error < errorCutOff ? 0. : error;

  // printF(" *** error=%e errorCutOff=%e truncatedError=%e \n",error,errorCutOff,truncatedError);
  
  fPrintF(checkFile,"<<%s>> %s: err = %7.1e\n",(const char*)check,
	  (const char*)blanks(0,max(0,90-check.length())),truncatedError);

}
