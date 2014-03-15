#include "IgesReader.h"

#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char *igesUnits[11] =
{
"Inches",
"Millimeters",
"Feet",
"Miles",
"Meters",
"Kilometers",
"Mils",
"Microns",
"Centimeters",
"Microinches"
};


IgesReader::
IgesReader()
// ===============================================================================
/// \details 
///     This Class can be used to read items from an IGES file. Use the MappingsFromCAD (in readMappings.C) class
///  to actually build Mappings from items found in the IGES file.
///  
///  There are two main sections in the IGES file. The directory entry section (DE) has a 2 line
///  description for each entry. The parameter data (PD) section has the actual parameter data for
///  the item (such as spline coefficients, pointers to trimming curves, etc.)
/// 
///  The {\tt readIgesFile} member function will read through the IGES file 
///  and build a list of 'items' of interest. These are items that we know what to
///  do with. This list of items is saved in the {\tt entityInfo} array
/// 
///  \begin{verbatim}
///      entityInfo(0,item)=entype      // entity type (from enum IgesEntries)
///      entityInfo(1,item)=seqnum;     // sequence number of the directory line for this item
///      entityInfo(2,item)=parameter;  // pointer to the parameter data.
///      entityInfo(3,item)=trans_mtr;  // transformation matrix (depends on entype)
///    or                  =form;       // save form info (depends on entype)
///      entityInfo(visiblePosition,item)=visible;    // =0:visible, =1 :blanked
///  \end{verbatim}
/// 
///  The "sequence number" of an item is the relative line number from the start of the DE section to
///  the first DE line for the item.
/// 
///  The actual building of a objects such as a {\tt rationalBSplineSurface} is currently NOT done
///  by this class. The {\tt NurbsMapping} for example knows how to build a {\tt rationalBSplineSurface}
///  by reading info accessed through this class.
/// 
// ===============================================================================
{
  infoLevel=1;	       // bit flag
  scale=0.;
  tolerance=0;
  units=0;
  entityCount=0;
  entityInfo.redim(5,100);
  entityInfo=-1;
  fp=NULL;
}

IgesReader::
~IgesReader()
{
}

int IgesReader::
numberOfEntities()
// =================================================================================
/// \details 
///    Return the number of items (of interest) saved in the {\tt entityInfo} array.
// =================================================================================
{
  return entityCount;
}


long IgesReader::
fieldNumber(const char *buff, int field)
// =================================================================================
/// \details 
///     Internal routine to return a specified field from the directory entry line stored in buff.
///  For example some selected fields are:
///   <ul>
///     <li>[field=1] : entity type number
///     <li>[field=2] : parameter data
///     <li>[field=9] : status number
///     <li>[field=10] : sequence number (current line number in the DE section)
///   </ul>
// =================================================================================
{
  char answer[9]; 

  int index = (field-1)*8;
  int i,j; 
  for( i=index,j=0; j<8; i++,j++ )
  {
    answer[j] = buff[i];      
    if (answer[j] == 'D' || answer[j] == 'd')
      answer[j]=' ';
  }
  answer[8]=0;
  return(atoi(answer));
} 


int IgesReader::
getSequenceNumber(const char *buff)
// =================================================================================
/// \details 
///     Internal routine to return seq number from Parameter line
// =================================================================================
{
  char answer[8];
  for(int j=0; j<8; j++)
    answer[j] = buff[64+j];      

  return(atoi(answer));
} 

int  IgesReader::
sequenceToItem( const int & sequence )  
// =================================================================================
/// \details 
///      Return the item in the entityInfo array for a given DE sequence number, i.e. return
///  the value 'item' where {\tt entityInfo(1,item)==sequence}
// =================================================================================
{
  int item=-1;
  const int dim=entityInfo.getLength(0);
  const int num=entityInfo.getLength(1);
  int *pe=&entityInfo(1,0);
#define ENTITY_INFO(i) pe[dim*i]

  for( int i=0; i<num; i++ )
  {
//    if( entityInfo(1,i)==sequence )
    if( ENTITY_INFO(i)==sequence )
    {
      item=i;
      break;
    }
  }
  return item;
}
#undef ENTITY_INFO

aString IgesReader::
entityName(const int & entity )
// =================================================================================
/// \details 
///     Return the name of the entity.
/// \param entity (input) : from the IgesEntries enum.
// =================================================================================
{
  switch (entity)
  {
  case nullEntry:
    return "null entry"; 
  case circularArc:
    return "circular arc"; 
  case compositeCurve:
    return "compositeCurve"; 
  case conicArc:
    return "conicArc"; 
  case copiusData:
    return "copiusData"; 
  case plane:
    return "plane"; 
  case line:
    return "line"; 
  case parametricSplineCurve:
    return "parametricSplineCurve"; 
  case parametricSplineSurface:
    return "parametricSplineSurface"; 
  case point:
    return "point"; 
  case ruledSurface:
    return "ruledSurface"; 
  case surfaceOfRevolution:
    return "surfaceOfRevolution"; 
  case tabulatedCylinder:
    return "tabulatedCylinder"; 
  case direction:
    return "direction"; 
  case transformationMatrix:
    return "transformationMatrix"; 
  case flash:
    return "flash"; 
  case rationalBSplineCurve:
    return "rationalBSplineCurve"; 
  case rationalBSplineSurface:
    return "rationalBSplineSurface"; 
  case offsetCurve:
    return "offsetCurve"; 
  case connectPoint:
    return "connectPoint"; 
  case node:
    return "node"; 
  case finiteElement:
    return "finiteElement"; 
  case nodalDisplacementAndRotation:
    return "nodalDisplacementAndRotation"; 
  case offsetSurface:
    return "offsetSurface"; 
  case boundary:
    return "boundary"; 
  case curveOnAParametricSurface:
    return "curveOnAParametricSurface"; 
  case boundedSurface:
    return "boundedSurface"; 
  case trimmedSurface:
    return "trimmedSurface"; 
  case nodalResults:
    return "nodalResults"; 
  case elementResults:
    return "elementResults"; 
  case block:
    return "block"; 
  case rightAngularWedge:
    return "rightAngularWedge"; 
  case rightCircularCylinder:
    return "rightCircularCylinder"; 
  case rightCircularConeFrustrum:
    return "rightCircularConeFrustrum"; 
  case sphere:
    return "sphere"; 
  case torus:
    return "torus"; 
  case solidOfRevolution:
    return "solidOfRevolution"; 
  case solidOfLinearExtrusion:
    return "solidOfLinearExtrusion"; 
  case ellipsoid:
    return "ellipsoid"; 
  case booleanTree:
    return "booleanTree"; 
  case selectedComponent:
    return "selectedComponent"; 
  case solidAssembly:
    return "solidAssembly"; 
  case manifoldSolidB_RepObject:
    return "manifoldSolidB_RepObject"; 
  case planeSurface:
    return "planeSurface"; 
  case rightCircularCylindricalSurface:
    return "rightCircularCylindricalSurface"; 
  case rightCircularConicalSurface:
    return "rightCircularConicalSurface"; 
  case sphericalSurface:
    return "sphericalSurface"; 
  case toroidalSurface:
    return "toroidalSurface"; 
  case angularDimension:
    return "angularDimension"; 
  case curveDimension:
    return "curveDimension"; 
  case diameterDimension:
    return "diameterDimension"; 
  case flagNote:
    return "flagNote"; 
  case generalLabel:
    return "generalLabel"; 
  case generalNote:
    return "generalNote"; 
  case newGeneralNote:
    return "newGeneralNote"; 
  case leader:
    return "leader"; 
  case linearDimension:
    return "linearDimension"; 
  case ordinateDimension:
    return "ordinateDimension"; 
  case pointDimension:
    return "pointDimension"; 
  case radiusDimension:
    return "radiusDimension"; 
  case generalSymbol:
    return "generalSymbol"; 
  case sectionedArea:
    return "sectionedArea"; 
  case associativityDefinition:
    return "associativityDefinition"; 
  case lineFontDefinition:
    return "lineFontDefinition"; 
  case subfigureDefinition:
    return "subfigureDefinition"; 
  case textFontDefinition:
    return "textFontDefinition"; 
  case textDisplayTemplate:
    return "textDisplayTemplate"; 
  case colorDefinition:
    return "colorDefinition"; 
  case unitsData:
    return "unitsData"; 
  case networkSubfigureDefinition:
    return "networkSubfigureDefinition"; 
  case attributeTableDefinition:
    return "attributeTableDefinition"; 
  case associativeInstance:
    return "associativeInstance"; 
  case drawing:
    return "drawing"; 
  case property:
    return "property"; 
  case singularSubfigureInstance:
    return "singularSubfigureInstance"; 
  case view:
    return "view"; 
  case rectangularArraySubfigureInstance:
    return "rectangularArraySubfigureInstance"; 
  case circularArraySubfigureInstance:
    return "circularArraySubfigureInstance"; 
  case externalReference:
    return "externalReference"; 
  case nodalLoad_Constraint:
    return "nodalLoad_Constraint"; 
  case networkSubfigureInstance:
    return "networkSubfigureInstance"; 
  case attributeTableInstance:
    return "attributeTableInstance"; 
  case solidInstance:
    return "solidInstance"; 
  case vertex:
    return "vertex"; 
  case edge:
    return "edge"; 
  case loop:
    return "loop"; 
  case face:
    return "face"; 
  case discreteData:
    return "discreteData"; 
  case parametricSurface:
    return "parametricSurface"; 
  default:
    return "unknown entry";
  }
}


int IgesReader::
readIgesFile(const char *fileName)
// ===================================================================================================
/// \details 
///     Read an IGES file. Build the {\tt entityInfo} array of interesting entities.
///  \begin{verbatim}
///      entityInfo(0,item)=entype      // entity type (from enum IgesEntries)
///      entityInfo(1,item)=seqnum;     // sequence number of the directory line for this item
///      entityInfo(2,item)=parameter;  // pointer to the parameter data.
///      entityInfo(3,item)=trans_mtr;  // transformation matrix (depends on entype)
///    or                  =form;       // save form info (depends on entype)
///      entityInfo(visiblePosition,item)=visible;    // =0:visible, =1 :blanked
/// \return  0 for success, 1 for error (either the filename was empty or the file 
///   could not be opened)
///  \end{verbatim}
// ===================================================================================================
{
  if( fileName == NULL )
  {
    printf("IgesReader::readIgesFile:ERROR: file name is NULL");
    return 1;
  }

  if( ( fp=fopen(fileName,"r") ) == NULL )
  {
    return 1;
  }

  printf("\n------ Reading IGES file %s -----\n",fileName);

  int numRead=processFile();
  if( numRead<=0 )
  {
    printf("IgesReader::readIgesFile:Error return from IgesReader::processFile\n");
    return 1;
  }    
  //fclose(fp);

  if( infoLevel & 1)
  {
    printf("scale = %g units = %s tolerance = %g bound = %g\n",
	   scale,igesUnits[units-1],tolerance,bound);
  }

  return 0;
}


int IgesReader:: 
processFile()
// ========================================================================================
/// \param Access: protected. Normally a user should use the {\tt readIgesFile} function.
/// \details 
///     Process the IGES file. 
/// 
///  Build the {\tt entityInfo} array of interesting items:
///  \begin{verbatim}
///      entityInfo(0,item)=entype      // entity type (from enum IgesEntries)
///      entityInfo(1,item)=seqnum;     // sequence number of the directory line for this item
///      entityInfo(2,item)=parameter;  // pointer to the parameter data.
///      entityInfo(3,item)=trans_mtr;  // transformation matrix (depends on entype)
///    or                  =form;       // save form info (depends on entype)
///      entityInfo(visiblePosition,item)=visible;    // =0:visible, =1 :blanked
///  \end{verbatim}
/// 
/// \return  a positive value denotes the number of entities read (?)
///      A return value of zero means an error.
// =================================================================================
{
    char buf[recordBufferSize+2], temp[3], *sp, *Global;
    int  i=0,j,ii;
    int ch, c_shift = 0;
    char string[100];
    int  global = 1;
    int count =-1;
    
    int numberOfRationalBSplineSurfaces=0;
    int numberOfTrimmedSurfaces=0;
    int numberOfBoundedSurfaces=0;
    int numberOfSurfaceOfRevolutions=0;
    int numberOfTabulatedCylinders=0;
    int numberOfParametricSplineSurfaces=0;

    int ddata = 0;
    int Unit = 1;
    double Scale = 1.0;
    double Tolerance = 0.;
    double Bound = 1000000.;
    int pos;

    temp[2] = '\0';

    recordDelimiter = ';';
    fieldDelimiter = ',';

    sp = fgets(buf, recordBufferSize, fp);  // read a line, at most recordBufferSize chars, end in newline

    if ((ch = fgetc(fp)) != '\n')   // next char, use fgetc instead of getc for linux!
    {
      if( ch == '\r' )
      {
	// There is a ^M in this file which will cause problems
        printf("\n****ERROR: This file contains carriage returns ^M instead of newlines. \n"
               "      You should remove the ^M characters using dos2unix for example\n\n");
        return 0;
      }
      ungetc(ch,fp);
    }
    else
    {
       if ((ch = fgetc(fp)) != '\n')
       {
          ungetc(ch,fp);
          c_shift = 1;
       }
       else
          c_shift = 2;
    }
    if( Mapping::debug & 2 )
      printf("c_shift=%i \n",c_shift);

    while (sp)
    {
      // printf("line=[%s]\n",sp);
      
      if (c_shift == 1) buf[recordBufferSize-1] = '\0';
      if (c_shift == 2) buf[recordBufferSize-1] = '\0';
      if (c_shift == 2) buf[recordBufferSize] = '\0';
      switch (buf[keyWordPosition]) 
      {
      case 'S':
	if( Mapping::debug & 2 )
	  printf("key = S (header)\n");
	sp = fgets(buf, recordBufferSize+c_shift, fp);
	break;
      case 'G':
        // read and save global header info
        if( Mapping::debug & 2 )
	  printf("key = G (global header info) \n");
	Global = new char[10*recordBufferSize];
	i=0;
	while (buf[keyWordPosition] == 'G')
        {
	  for(pos=keyWordPosition-1;pos>=0;pos--)
	  {
	    if (buf[pos] == fieldDelimiter) break;    // look for delimter ","
	    if (buf[pos] == recordDelimiter) break;   // look for delimiter ";"
	    if (buf[pos] != ' ')
	    {
	      pos = keyWordPosition-1;
	      break;
	    }
	  }
	  for(j=0;j<=pos;j++,i++)
	    Global[i] = buf[j]; 
	  sp = fgets(buf, recordBufferSize+c_shift, fp);
	}
	Global[i] = '\0';
	pos = i;
	if ( global == 1 ) 
	{
	  if (Global[0] == ',' )
	  {
	    fieldDelimiter = ',' ; global++;
	    if ( Global[1] == ',' )
	    {
	      recordDelimiter = ';' ; i=2; global++;
	    }
	    else
	    {
	      recordDelimiter = Global[3] ; i=5; global++;
	    }
	  }
	  else
	  {
	    fieldDelimiter=Global[2] ; global++;
	    if (Global[4] != fieldDelimiter)
	    {
	      recordDelimiter=Global[6] ; i = 8; global++;
	    }
	    else{
	      recordDelimiter=';'; i = 5; global++;
	    }
	  }
	}
	while(i<pos && Global[i] != ' ')
	{
	  ii=i;
	  while(Global[i] != fieldDelimiter && Global[i] != recordDelimiter && i<pos &&
		Global[i] != ' ')
	  {
	    if (Global[i] == 'H')
	    {
	      temp[0] = Global[ii];
	      if (ii+1 == i)
		temp[1] = '\0';
	      else
		temp[1] = Global[ii+1];
	      ii=i+1;
	      i += atoi(temp)+1;
	      break;
	    }
	    i++;
	  }
	  for (j=ii;j<i;j++) 
	  {
	    string[j-ii] = Global[j];
	  }
	  string[j-ii] = '\0';
	  if ( Mapping::debug & 2 && infoLevel & 1 ) 
            printf("%d: %s\n",global,string);
	  switch(global)
          {
	  case 13: /*Scale factor*/
	    if (isdigit(string[0]))
	    {
	      Scale = atof(string);
	    }
	    break;
	  case 14: /*Unit flag*/
	    if (isdigit(string[0]))
	    {
	      if( Mapping::debug & 2 )
                printf(" Unit: string=%s \n",string);
	      
	      Unit = atoi(string);
	      if (Unit == 3) Unit = 1;
	      if (Unit > 3) Unit--;
              assert( Unit>=0 && Unit<11 );
	    }
	    break;
	  case 19: /*Tolerance*/
	    if (isdigit(string[0]))
	    {
	      for(j=0;j<strlen(string);j++)
	      {
		if (string[j] == 'D' || string[j] == 'd')
		  string[j] = 'E';
	      }
	      Tolerance = atof(string);
	    }
	    break;
	  case 20: /*Bound*/
	    if (isdigit(string[0]))
	    {
	      Bound = atof(string);
	    }
	    break;
	  }
	  global++,i++;
	}
	delete [] Global;
	break ;

      case 'D':
        // directory

	entype    = fieldNumber(buf,1); 
	parameter = fieldNumber(buf,2);
	level     = fieldNumber(buf,5);
	trans_mtr = fieldNumber(buf,7);
	status    = fieldNumber(buf,9);
	seqnum    = fieldNumber(buf,10);

	ddata++;
	sp = fgets(buf, recordBufferSize+c_shift, fp);

	Color     = fieldNumber(buf,3);
	form      = fieldNumber(buf,5);
	// savep     = ftell(dfp);
	visible     = status/1000000L;
	subordinate = status/10000L-visible*100L;
	entityUse  = status/100L-visible*10000L-subordinate*100L;
	hierarchy   = status-100L*(status/100L);
	process = (int)entityUse;
	// if (process == 5) process = 0;

        if( Mapping::debug & 8 )
	{
          if( process!=0 && process!=5 )
	    printf(" *** ");
          printf("entity=%i seqnum=%6i level=%i trnmat=%i status=%8.8i %s %s %-s %s form=%i",
                 (int)entype,(int)seqnum,(int)level,
                 (int)trans_mtr,
                 (int)status,
                 subordinate==0 ? "independent" : "dependent  ",
                 visible==0 ? "visible" : "blanked",
                 entityUse==0 ? "geometry" : entityUse==1 ? "annotate" : entityUse==5 ? "2Dprmtrc" : "other   ",
                 visible==1 && entityUse==0 ? "*" : " ",(int)form);
	  printf(" name=%s \n",(const char*)entityName(entype));
	}

        if( entype==rationalBSplineSurface ) numberOfRationalBSplineSurfaces++;
	if( entype==trimmedSurface ) numberOfTrimmedSurfaces++;
	if( entype==boundedSurface ) numberOfBoundedSurfaces++;
        if( entype==surfaceOfRevolution ) numberOfSurfaceOfRevolutions++;
	if( entype==tabulatedCylinder ) numberOfTabulatedCylinders++;
	if( entype==parametricSplineSurface) numberOfParametricSplineSurfaces++;
	

        // Here are the entities that we choose to deal with

        if( entype==rationalBSplineSurface      || entype==trimmedSurface || entype==tabulatedCylinder ||
	    entype==surfaceOfRevolution         || entype==boundedSurface || entype==parametricSplineSurface ||
            entype==curveOnAParametricSurface   || entype==compositeCurve || entype==copiusData ||
            entype==rationalBSplineCurve        || entype==line           || entype==circularArc ||
            entype==transformationMatrix        || entype==finiteElement  || entype==node ||
	    entype==boundary                    || entype==parametricSplineCurve || entype==conicArc ||
	    entype==singularSubfigureInstance   || entype==subfigureDefinition   || 
            entype==manifoldSolidB_RepObject    || 
            entype==planeSurface                || entype==rightCircularCylindricalSurface ||
            entype==rightCircularConicalSurface || entype==sphericalSurface || entype==toroidalSurface ||
            entype==vertex || entype==edge || entype==loop || entype==face || entype==shellEntity || 
            entype==point  || entype==direction ) 
// AP added parametricSplineCurve (entity type 112)
// KKC added singular subfigure instance; entity type 408; 040213
// KKC added subfigureDefinition ; entity type 308; 040213
// wdh added entities for the manifoldSolidB_RepObject
	{
          count++;
	  if( entityInfo.getLength(1)<=count )
	    entityInfo.resize(entityInfo.getLength(0),entityInfo.getLength(1)+100);

          entityInfo(0,count)=entype;
	  
	  entityInfo(1,count)=seqnum;
	  entityInfo(2,count)=parameter;
          if( entype!=transformationMatrix && trans_mtr!=0 )
            entityInfo(3,count)=trans_mtr;  // transformation matrix
          else
            entityInfo(3,count)=form;  // save form 
          
	  // kkc added subordinate==2 check for logically subordinate but visible entities
          entityInfo(visiblePosition,count)=(visible==0) + 2*( (subordinate==0) || 
							       (subordinate==2)) ;  // 
	  // kkc 040213 the following line was commented out because dependentPosition==visiblePosition
	  //	  	  entityInfo(dependentPosition,count) = 2*( (subordinate==0) || 
	  //	  						    (subordinate==2));//subordinate; // kkc added this to fill in dependency info
	  //	  cout<<entityName(entype)<<"  subordinate = "<<subordinate<<"  entInfo "<<entityInfo(dependentPosition,count)<<endl;
	}
        else if( Mapping::debug & 4 )
	{
	  printf("INFO:Skipped entity=%i seqnum=%6i level=%i trnmat=%i status=%8.8i %s %s %-s %s form=%i",
                 (int)entype,(int)seqnum,(int)level,
                 (int)trans_mtr,
                 (int)status,
                 subordinate==0 ? "independent" : "dependent  ",
                 visible==0 ? "visible" : "blanked",
                 entityUse==0 ? "geometry" : entityUse==1 ? "annotate" : entityUse==5 ? "2Dprmtrc" : "other   ",
                 visible==1 && entityUse==0 ? "*" : " ",(int)form);
	  printf(" name=%s \n",(const char*)entityName(entype));
          
	}
	
	// max_seqnum = (int)fieldNumber(buf,10);

	// if (fwrite(buf,sizeof(char),recordBufferSize,dfp) != recordBufferSize) dstat++;
	ddata++;
	sp = fgets(buf, recordBufferSize+c_shift, fp);

	break;
      case 'P':
        // parameters (data)
	// if (fwrite(buf,sizeof(char),recordBufferSize,pfp) != recordBufferSize) pstat++;
        parameterPosition=ftell(fp)-recordBufferSize;
        if( Mapping::debug & 2 )
	  printf("Start of parameter info: parameterPosition=%i \n",(int)parameterPosition);
        sp=NULL;
	if( TRUE ) break;
/* ---
        seqnum  = getSequenceNumber(buf);
        if( seqnum==nurbSequenceNumber )
	{
	  printf("Nurb found: line=%s");
	}
	sp = fgets(buf, recordBufferSize+c_shift, fp);
	break;
----- */
      case 'T':
	printf("key = T (terminate?)\n");
	sp = 0;
	break;                    
      default:
	fprintf(stdout, "Cannot resolve IGES record:\n%s", buf);
	return 0;
      }
    }
    if( infoLevel & 1) 
      fprintf(stdout,"\n"); fflush(stdout);

    fclose(fp); 

    scale = Scale;
    units = Unit;
    tolerance = Tolerance;
    bound = Bound;
    entityCount=count+1;
    
    // entityInfo.display("entityInfo");
    
    printf(">>> IgesReader: There were %i rational b-spline surfaces found\n",numberOfRationalBSplineSurfaces);
    printf(">>> IgesReader: There were %i trimmed surfaces found\n",numberOfTrimmedSurfaces);
    printf(">>> IgesReader: There were %i bounded surfaces found\n",numberOfBoundedSurfaces);
    printf(">>> IgesReader: There were %i surfaces of revolution found\n",numberOfSurfaceOfRevolutions);
    printf(">>> IgesReader: There were %i tabulated cylinders found\n",numberOfTabulatedCylinders);
    printf(">>> IgesReader: There were %i parametric spline surfaces found\n",numberOfParametricSplineSurfaces);


    return(ddata/2);
}

int IgesReader::
getData(RealArray & data, int maximumNumberToRead)
// =================================================================================
/// \details 
///    Protected routine to read data from the IGES file from the current file position.
// =================================================================================
{
  char test[recordBufferSize],temp[recordBufferSize];
  int  count=0, endOfRecord=0;

  int i=0,j=0;
  while ( endOfRecord == 0 )
  {
    fread(test,sizeof(char),recordBufferSize,fp); j = 0;

    if (test[0] == recordDelimiter)
    {
      i = 0;
      while(test[i] == recordDelimiter && i<65)
      {
	data(count) = 0.;
	count++;
	if (count == maximumNumberToRead) return(0);
	i++;
      }
    }
    else
    {
      for( i=0; i<65; i++)
      {
	if ((test[i] != fieldDelimiter) && (test[i] != recordDelimiter ))
	{
	  if (test[i] == 'D' || test[i] == 'd' || test[i] == 'E') 
	    test[i] = 'e';
	  temp[j]=test[i];
	  j++;
	}
	else
	{
	  temp[j] = '\0'; j = 0;
	  data(count) = atof(temp);
	  temp[0] = '\0';
	  if (test[i] == recordDelimiter) endOfRecord = 1;
	  count++;
	  if (count == maximumNumberToRead) return(0);
	}
      }
    }
  }
  return (maximumNumberToRead-count);
}

int IgesReader::
readData(const int & item, RealArray & data, const int & numberToRead)
// ========================================================================================
/// \details 
///     Read data for an item. The actual data read will be pointed to by the parameter data pointer from the
///  directory entry section for "item" which has been stored in {\tt entityInfo(2,item)}
/// 
/// \param item (input): an item of interest (i.e. in the entityInfo list)
/// \param data (output): read "numberToRead" values.
/// \param numberToRead (input) : read this many values.
// =================================================================================
{
  fseek(fp,parameterPosition+(entityInfo(2,item)-1)*recordBufferSize,SEEK_SET);
  getData( data,numberToRead );
  return 0;
}

#ifdef USE_PPP
int IgesReader::
readData(const int & item, realArray & data, const int & numberToRead)
{
  // add this for now: this will not work in parallel
  const RealArray & dataLocal = data.getLocalArray();
  return readData(item,(RealArray &)dataLocal,numberToRead);
}
#endif

int IgesReader::
readParameterData(const int & parameterDataPointer, RealArray & data, const int & numberToRead)
// ========================================================================================
/// \details 
///     Read data from a given location in the parameter data (PD) section.
/// 
/// \param parameterDataPointer (input) : a relative offset from the start of the parameter data (PD)
///  section.
/// \param data (output): read "numberToRead" values.
/// \param numberToRead (input) : read this many values.
// =================================================================================
{
  fseek(fp,parameterPosition+(parameterDataPointer-1)*recordBufferSize,SEEK_SET);
  getData( data,numberToRead );
  return 0;
}


#ifdef USE_PPP
int IgesReader::
readParameterData(const int & parameterDataPointer, realArray & data, const int & numberToRead)
{
  // add this for now: this will not work in parallel
  const RealArray & dataLocal = data.getLocalArray();
  return readParameterData(parameterDataPointer, (RealArray &)dataLocal, numberToRead);
}
#endif
