#include "Overture.h"
#include "PlotStuff.h"
#include "NurbsMapping.h"
#include "IgesReader.h"

int 
main()
{
  char fileName[] = "test.igs";

  
  IgesReader iges;

  iges.readIgesFile(fileName);

  FILE *fp = iges.fp;
  intArray & entityInfo = iges.entityInfo;
  long & parameterPosition= iges.parameterPosition;

  fclose(fp);
  fp = fopen(fileName,"rb");
  char buf[RECBUF+2];
  buf[RECBUF]=0;
  buf[RECBUF+1]=0;

  for( int i=0; i<entityInfo.getLength(1); i++ )
  {
    if( entityInfo(0,i)>0 )
    {
      printf("entityType(%i)=%i, sequenceNumber=%i, lineNumber=%i \n",i,entityInfo(0,i),entityInfo(1,i),
	     entityInfo(2,i));
      fseek(fp,parameterPosition+(entityInfo(2,i)-1)*RECBUF,SEEK_SET);
      fread(buf,sizeof(char),RECBUF,fp);
      printf("line = %s",buf);


    }
  }
  
  PlotStuff ps(TRUE,"iges reader");
  NurbsMapping nurbs;
  int item=0;

  PlotStuffParameters psp;               // create an object that is used to pass parameters
    
  aString answer,line;
  char buff[80];
  aString menu[] = { "plot an item",
                    "erase",
		    "exit",
                    "" };
  for(;;)
  {
    ps.getMenuItem(menu,answer);
    if( answer=="plot an item" )
    {
      ps.inputString(line,sPrintF(buff,"Enter the item to plot"));
      if( line!="" ) sScanF(line,"%i",&item);
      nurbs.readFromIgesFile(iges,item);
      PlotIt::plot(ps,nurbs);
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="exit" )
    {
      break;
    }
  }


/* ---
  // read in data for a nurbs:
  i=0;
  doubleArray data(10);
  
  iges.readData(i,data,10);
  
// pdata[0] : entity number 
// pdata[1] : upper index of first sum 
// pdata[2] : upper index of second sum 
// pdata[3] : degree of first set of basis functions 
// pdata[4] : degree of second set of basis functions 
// pdata[5] : 1 = Closed in first parametric variable direction
//              0 = Not Closed 
// pdata[6] : 1 = Closed in second parametric variable direction
//              0 = Not Closed 
// pdata[7] : 0 = Rational
//              1 = Polynomial 
// pdata[8] : 0 = Nonperiodic in first parametric variable direction
//              1 = Periodic in first parametric variable direction 
// pdata[9] : 0 = Nonperiodic in second parametric variable direction
//              1 = Periodic in second parametric variable direction 

   int k1 = (int) data(1) ;
   int k2 = (int) data(2) ;
   int m1 = (int) data(3) ;
   int m2 = (int) data(4) ;

   printf(" k1=%i, k2=%i, m1=%i, m2=%i \n",k1,k2,m1,m2);

   int n1 = 1 + k1 - m1    ; 
   int n2 = 1 + k2 - m2    ;
   int a  = n1+2*m1        ;  // k1+m1+1
   int b  = n2+2*m2        ;  // k2+m2+1
   int c  = (1+k1)*(1+k2)  ;


// allocate a proper memory for the NURBS surface   

   int maxData = 19+a+b+4*c;
   data.redim(maxData);
   
   int resid = iges.readData(i,data,maxData); // resid should be zero -- we read all the data

   if (resid == 0)
   {
     if ((int)data(maxData-3) == 0)
     {
       if ((int)data(maxData-2) == 1)
       {
	 printf("looking for the name...\n");
         //  name = ins_406((int)pdata[max_data-1]);
       }
     }
   }

   int m = k1; // surf->cp_res.k;
   int n = k2; // surf->cp_res.l;
   int k = m1; // surf->order.k;
   int l = m2; // surf->order.l;

   realArray knotU(a+1), knotV(b+1), cPoint(k1+1,k2+1,4);
   
   int j,ii,jj;
   for (i=10,ii=0;i < 11+a;i++,ii++)
       knotU(ii) = data(i);

   knotU.display("Here is knotU");
   

   for (j=11+a,jj=0;j < 12+a+b;j++,jj++)
       knotV(jj) = data(j);

   knotV.display("Here is knotV");

   ii = 0;
   jj = 0;
   j = 12+a+b ;
   for (i=j;i < (j+c);i++)
   {
     cPoint(ii,jj,0) = data(i+c+2*(i-j));
     cPoint(ii,jj,1) = data(i+c+2*(i-j)+1);
     cPoint(ii,jj,2) = data(i+c+2*(i-j)+2);
     cPoint(ii,jj,3) = data(i);

     if(ii == m)
     {
        ii=0;
        jj++;
     }
     else
        ii++;
   }

----- */

   // cPoint.display("Here is cPoint");

/* ---
   surf->closed_in_u = (int)pdata[5];
   surf->closed_in_v = (int)pdata[6];
   surf->periodic_in_u = (int)pdata[8];
   surf->periodic_in_v = (int)pdata[9];

   if (resid <= 1)
   {
      surf->u_min = pdata[12+a+b+4*c];
      surf->u_max = pdata[13+a+b+4*c];
      if (surf->u_min >= surf->u_max)
      {
         surf->u_min = 999.;
         surf->u_max = -999.;
      }
      surf->v_min = pdata[14+a+b+4*c];
      surf->v_max = pdata[15+a+b+4*c];
      if (surf->v_min >= surf->v_max)
      {
         surf->v_min = 999.;
         surf->v_max = -999.;
      }
      if (surf->u_min < surf->knotU[surf->order.k-1])
      {
         printf(">>>> umin out of knot domain\n");
         surf->u_min = surf->knotU[surf->order.k-1];
      }
      if (surf->u_max > surf->knotU[surf->cp_res.k+1])
      {
         printf(">>>> umax out of knot domain\n");
         surf->u_max = surf->knotU[surf->cp_res.k+1];
      }
      if (surf->v_min < surf->knotV[surf->order.l-1])
      {
         printf(">>>> vmin out of knot domain\n");
         surf->v_min = surf->knotV[surf->order.l-1];
      }
      if (surf->v_max > surf->knotV[surf->cp_res.l+1])
      {
         printf(">>>> vmax out of knot domain\n");
         surf->v_max = surf->knotV[surf->cp_res.l+1];
      }
   }
   else
   {
      surf->u_min = surf->knotU[surf->order.k-1];
      surf->u_max = surf->knotU[surf->cp_res.k+1];
      surf->v_min = surf->knotV[surf->order.l-1];
      surf->v_max = surf->knotV[surf->cp_res.l+1];
   }

   if (trans_mtr) transform_surface_cps(surf);
   if (Color != 0) ins_314();

--- */

  return 0;
}
