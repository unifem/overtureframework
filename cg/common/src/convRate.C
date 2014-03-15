#include "Overture.h"

// compute convergence rates for OverBlown and create files insTables.tex from insResults.tex etc.
//
// usage:
//    cr [fileName]
//     fileName = one of ins.results or asf.results or cns.results

int 
getLineFromFile( FILE *file, char s[], int lim);

int
convergenceRate(const RealArray & h, 
                const RealArray & e,
                RealArray & sigma)
// =========================================================================
//  Make a least squares fit to the convergence rate.
// =========================================================================
{
  const int n = h.getLength(0);
  const int m = e.getLength(1);
  
  Range N(0,n-1), M(0,m-1);

  RealArray hh(N), ee(N,M);
  
  hh(N)=log(h);
  ee(N,M)=log(e);
  
  // .............least squares fit to exponent of convergence

  for( int j=0; j<m; j++ )
  {
    real sumH=sum(hh);
    real sumE=sum(ee(N,j));
    real sumHE=sum(hh*ee(N,j));
    real sumH2=sum(hh*hh);
    
    sigma(j) = (sumHE*n-sumH*sumE)/(n*sumH2-sumH*sumH);
    real e0 = (sumH2*sumE-sumH*sumHE)/(n*sumH2-sumH*sumH);
  }

  return 0;
}



int
main(int argc, char *argv[])
{
  printf("Running convRate...\n");
  printf("Usage: convRate fileName [-output=outFile]\n");
  
  aString outputFileName;
  aString line;
  // look for file names
  aString *fileName = new aString [max(1,argc-1)];
  fileName[0]="ins.results"; // default name
  
  int numberOfFiles=0;
  for( int i=1; i<argc; i++ )
  {
    line=argv[i];
    if( line(0,7)=="-output=" )
      outputFileName=line(8,line.length()-1);
    else
    {
      numberOfFiles++;
      fileName[i-1]=argv[i];
    }
  }
  numberOfFiles=max(1,numberOfFiles);
  
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  const int maxNumberOfGridResolutions=10;
  const int maximumNumberOfComponents=100;
  RealArray h(maxNumberOfGridResolutions), e(maxNumberOfGridResolutions,maximumNumberOfComponents),
            sigma(maximumNumberOfComponents);
  h=1.;
  e=1.;

  if( outputFileName=="" )
  {
    if( fileName[0](0,2)=="ins" )
      outputFileName="insTables.tex";
    else if( fileName[0](0,2)=="asf" )
      outputFileName="asfTables.tex";
    else if( fileName[0](0,2)=="cns" )
      outputFileName="cnsTables.tex";
    else
      outputFileName="newTables.tex";
  }
  
  FILE *file, *outputFile;
  outputFile= fopen(outputFileName,"w" );
  cout << "convRate : output written to " << outputFileName << endl;
  
  for( int subFile=0; subFile<numberOfFiles; subFile++ )
  {
    printf("convRate : open file %s\n",(const char*)  fileName[subFile]);
    file = fopen((const char*)fileName[subFile],"r" );
    if( !file )
      cout << "convRate: ERROR:unable to open file [" << fileName << "]\n";

    bool done=FALSE;
    char buff[500], buff2[40], header[500];
    aString line,caption;
    bool tableIsOn=FALSE;
    int grid=0;
    int gridLines=1,numberOfComponents=1;
    int numberOfComponentsFromHeader=-1;
    while( !done )
    {
      int numberOfCharsRead=getLineFromFile(file,buff,sizeof(buff));
      done=numberOfCharsRead==0;
    
      line=buff;
      // skip leading blanks:
      int i=0;
      while( i<line.length() && line[i]==' ' ) i++;
      if( i==line.length() ) continue;
      line=line(i,line.length()-1);
      
      if( line[0]=='%' )
      { // echo a comment
	fprintf(outputFile,"%s\n",(const char*)line);
      }
      else if( tableIsOn )
      {
	if( line(0,7)=="\\caption" )
	{
	  tableIsOn=FALSE;
	  Range N(0,grid-1), M(0,numberOfComponents-1);

          // *****************************************
	  // ***** compute the convergence rates *****
          // *****************************************
	  convergenceRate( h(N),e(N,M),sigma);

          printf("Convergence rates: ");
	  fprintf(outputFile,"    rate             &       ");
	  for( int j=0; j<numberOfComponents; j++ )
	  {
	    fprintf(outputFile,"&  $%4.2f$       &      ",sigma(j));
	    printf("c%i=%4.2f ",j,sigma(j));
	  }
          printf("\n");
	  
	  fprintf(outputFile," \\\\ \\hline\n");

	  fprintf(outputFile,"\\end{tabular}\n");
	  // fprintf(outputFile,"\\hfill\n");

	  fprintf(outputFile,buff); fprintf(outputFile,"\n");   // caption

	  fprintf(outputFile,"\\end{center}\n");
	  fprintf(outputFile,"\\end{table}\n");

	}
	else
	{
	  // extract errors
	  real e1,e2,e3,e4;
	  numberOfComponents=sScanF(buff,"%s & %i & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e & %e",
                                    buff2,&gridLines,
				    &e(grid,0),&e(grid,1),&e(grid,2),&e(grid,3),&e(grid,4),&e(grid,5),&e(grid,6),&e(grid,7),
                                    &e(grid,8),&e(grid,9),&e(grid,10),&e(grid,11),&e(grid,12),&e(grid,13),&e(grid,14),
                                    &e(grid,15));
	  numberOfComponents-=2;
	  if( numberOfComponents>maximumNumberOfComponents )
	  {
	    printF("convRate:ERROR: numberOfComponents=%i > maximumNumberOfComponents=%i\n",
		   numberOfComponents,maximumNumberOfComponents);
	    OV_ABORT("error");
	  }
	  else if( numberOfComponents != numberOfComponentsFromHeader )
	  {
	    printF("convRate:ERROR: numberOfComponents=%i <> numberOfComponentsFromHeader=%i\n",
		   numberOfComponents,numberOfComponentsFromHeader);
            printF(" header=[%s]\n",buff);
	    OV_ABORT("error");
	  }
	  
	  h(grid)=1./gridLines;
	  if( false )
	    printf(" grid=%i, numberOfComponents=%i, gridLines=%i, e1=%e, e2=%e\n",grid,numberOfComponents,gridLines,
		   e(grid,0),e(grid,1));

	  fprintf(outputFile,"%20s & %5i ",buff2,gridLines);  // grid-name and 'N'
	  fprintf(stdout    ,"%20s & %5i ",buff2,gridLines);  // grid-name and 'N'
	  for( int j=0; j<numberOfComponents; j++ )
	  {
	    int exp = int( log10(e(grid,j))-.999999999999999);
	    real frac = e(grid,j)/pow(10.,exp);
	    // fprintf(outputFile,"& ~$%2.1f\\times10^{%3i}$~ ",frac,exp);
	    fprintf(outputFile,"& \\num{%2.1f}{%i} ",frac,exp);
	    fprintf(stdout    ,"& %8.2e ",e(grid,j));

            // compute the ratio of the errors:
            //    e(coarse)/e(fine)  
	    if( grid==0 )
	    {
              fprintf(outputFile,"&      ");  // no factor for first entry
              fprintf(stdout    ,"&      ");  // no factor for first entry
	    }
	    else
	    {
              real factor = e(grid-1,j)/max( REAL_MIN*1000., e(grid,j));
	      if( fabs(factor)<1.e4 )
	      {
		// fprintf(outputFile,"&~$%5.1f$~ ",factor);
		fprintf(outputFile,"&%5.1f ",factor);
		fprintf(stdout    ,"&%5.1f ",factor);
	      }
	      else
	      {
		// fprintf(outputFile,"& ~$%8.2e$~ ",factor);  // could do better here
		fprintf(outputFile,"& %8.2e ",factor);  // could do better here
		fprintf(stdout    ,"& %8.2e ",factor);  // could do better here
	      }
	    }
	  }
//    	  fprintf(outputFile,"&  $%7.1e$  ",e(grid,j));
	  fprintf(outputFile," \\\\ \\hline\n");
	  fprintf(stdout,"\n");
	  grid++;
	}
      }
      else if( line(0,3)=="grid" )
      {
	// this marks the start of a table
	fprintf(outputFile,"\\begin{table}[hbt]\\tableFont %% you should set \\tableFont to \\footnotesize or other size\n");
	fprintf(outputFile,"%% \\newcommand{\\num}[2]{#1e{#2}} %% use this command to set the format of numbers in the table.\n");
	fprintf(outputFile,"\\begin{center}\n");
	// fprintf(outputFile,"\\hfill\n");
	if( false )
	{
	  numberOfComponents=sscanf(buff,"%s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s & %s",
				    buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2,buff2);
	  numberOfComponents-=2;
	}
	else
	{
	  // count number of components from the header
          numberOfComponents=0;
          for( int i=0; buff[i]!=0; i++ )
	  {
            if( buff[i]=='&' ) numberOfComponents++;
	  }
          numberOfComponents-=1;
	}

        numberOfComponentsFromHeader=numberOfComponents;

        printf("convRate: header=[%s]\n",buff);
        printf("convRate: numberOfComponents=%i (from header)\n",numberOfComponents);
	
	fprintf(outputFile,"\\begin{tabular}{|l|c|");
	for( int j=0; j<numberOfComponents; j++ )
	  fprintf(outputFile,"c|c|");
	fprintf(outputFile,"} \\hline \n");

        // Here is the header line for the table, of the form: 
        //     "grid & N & u & v ..."

        // --- Add additional columns in the header for the ratio of errors --- *wdh* 090903
        int j=0,numAmpersands=0;
	for( int i=0; buff[i]!=0; i++ )
	{
          header[j]=buff[i]; j++;
	  if( buff[i]=='&' )
	  {
	    numAmpersands++;
	    if( numAmpersands>2 )
	    { // add a column: 
              header[j]=' '; j++; header[j]='r'; j++; header[j]=' '; j++;
              header[j]='&'; j++;
	    }
	  }
	}
	header[j]='&'; j++;
	header[j]=' '; j++; header[j]='r'; j++; header[j]=' '; j++;
        header[j]=0;
        // printf("*** convRate: old : header=[%s] \n"
        //        "              new : header=[%s] \n",buff,header);
        printf("                %s\n",header);
	
	// fprintf(outputFile,buff);  fprintf(outputFile,"\\\\ \\hline \n");
	fprintf(outputFile,header);  fprintf(outputFile,"\\\\ \\hline \n");
	tableIsOn=true;

	grid=0;
      }
      else 
      {
	fprintf(outputFile,buff); fprintf(outputFile,"\n");
      }
    
      if( false )
        cout << line << endl;
    }

  }
  
/* ----

  
  int n=3;
  int m=2;
  
  h(0)=1.;   e(0,0)=1.;     e(0,1)=1.;
  h(1)=.5;   e(1,0)=.25;    e(1,1)=.5;
  h(2)=.25;  e(2,0)=.0625;  e(2,1)=.25;


  printf("    h       error \n");
  int i;
  for( i=0; i<n; i++ )
  {
    printf(" %10.4e  ",h(i));
    for( int j=0; j<m; j++ )
      printf("  %12.4e ",e(i,j));
    printf("\n");
  }
  

  Range N(0,n-1), M(0,m-1);
  convergenceRate( h(N),e(N,M),sigma);
  
  for( int j=0; j<m; j++ )
  {
    printf("component=%i, slope=%e\n",j,sigma(j));
  }
---- */

  cout << "\n ****Output written to " << outputFileName << " *****\n";

  return 0;
}
