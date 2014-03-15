#include "GL_GraphicsInterface.h"
#include "Annulus.h"
#include "Sphere.h"

#include <GL/gl.h>
#include <GL/glu.h>


void selectObject(const real & x=-1., const real & y=-1.);
void getCursor( real & x, real & y );

int 
main(int argc, char *argv[])
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    

  aString commandFileName="";
  bool plotOption=TRUE;
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" )
        plotOption=FALSE;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `pk [noplot][file.cmd]' \n"
            "          noplot:   run without graphics \n" 
            "          file.cmd: read this command file \n";



  GL_GraphicsInterface ps;
  GraphicsParameters params; 
  ps.appendToTheDefaultPrompt("pk>"); // set the default prompt
    
  // By default start saving the command file called "ogen.cmd"
  aString logFile="pk.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);


  SphereMapping map1(.3,.7,-.25,-.25,-.25);  
  map1.setDomainDimension(2);
  map1.setName(Mapping::mappingName,"sphere1");

  SphereMapping map2(.3,.7,.25,.25,.25); 
  map2.setDomainDimension(2);
  map2.setName(Mapping::mappingName,"sphere2");
  
  SphereMapping map3(.3,.7,-.25,.25,.25);
  map3.setDomainDimension(2);
  map3.setName(Mapping::mappingName,"sphere3");
  
  SphereMapping map4(.3,.7,.25,-.25,.25);
  map4.setDomainDimension(2);
  map4.setName(Mapping::mappingName,"sphere4");
  
  SphereMapping map5(.3,.7,.25,.25,-.25);
  map5.setDomainDimension(2);
  map5.setName(Mapping::mappingName,"sphere5");
  
  SphereMapping map6(.3,.7,-.25,-.25,.25);
  map6.setDomainDimension(2);
  map6.setName(Mapping::mappingName,"sphere6");
  
  SphereMapping map7(.3,.7,-.25,.25,-.25);
  map7.setDomainDimension(2);
  map7.setName(Mapping::mappingName,"sphere7");
  
  SphereMapping map8(.3,.7,.25,-.25,1.5);
  map8.setDomainDimension(2);
  map8.setName(Mapping::mappingName,"sphere8");
  

  RealArray bounds(2,3);
  bounds(Start,0)=-1.0;
  bounds(End  ,0)= 1.0;
  bounds(Start,1)=-1.0;
  bounds(End  ,1)= 1.0;
  bounds(Start,2)=-1.0;
  bounds(End  ,2)= 2.0;
  
  aString answer,answer2;
  aString menu[] = { "select objects",
                    "pick points",
                    "transform points",
		    "erase",
                    "redraw",
		    "exit",
                    "" };

  IntegerArray selection;
  RealArray pickRegion(2,2), xRegion(2,2);

  for( int it=0;; it++)
  {
    if( it==0 )
      answer="redraw";
    else
      ps.getMenuItem(menu,answer);

    if( answer=="select objects" )
    {
      //  int numberSelected=ps.select(selection,"select objects with mouse");
      for(;;) 
      {
	const aString menu[]=
	{
	  "done",
          ""
	};
        int numberSelected=ps.getMenuItem(menu,answer,"pk: select objects with mouse",selection);
        cout << "pk: answer= " << answer << endl;
        if( answer=="done" )
	{
	  break;
	}
	else
	{
	  printf("pk: numberSelected=%i \n",numberSelected);
          const int numberOfMaps=8;
	  Mapping *mapList[numberOfMaps] = { &map1,&map2,&map3,&map4,&map5,&map6,&map7,&map8}; //
          int chosen=-1, ichosen=0;
	  for( int i=0; i<=selection.getBound(0); i++ )
	  {
            for( int j=0; j<numberOfMaps; j++ )
	    {
  	      if( selection(i,0)==mapList[j]->getGlobalID() )
	      {
	        printf("map%i selected, z1=%i, z2=%i \n",j,selection(i,1),selection(i,2));
                if( chosen<0 || selection(i,1)<selection(ichosen,1) )
		{
                  chosen=j;
                  ichosen=i;
		}
	      }
	    }
	  }
          if( chosen<0 )
	    printf("no mapping selected\n");
	  else
	  {
            printf(" Mapping %i was chosen \n",chosen);
	  }
	}
      }
    }
    else if( answer=="select and pick objects" )
    {
      //  int numberSelected=ps.select(selection,"select objects with mouse");
      for(;;) 
      {
	const aString menu[]=
	{
	  "done",
          ""
	};



        ps.redraw(TRUE);
	
        int numberSelected=ps.getMenuItem(menu,answer,"pk: select objects with mouse",selection,pickRegion);
        cout << "pk: answer= " << answer << endl;
        if( answer=="done" )
	{
	  break;
	}
	else
	{

          ps.normalizedToWorldCoordinates( pickRegion,xRegion );
	  printf("pk: region selected = (%9.3e,%9.3e)X(%9.3e,%9.3e) world=(%9.3e,%9.3e)X(%9.3e,%9.3e) \n",
             pickRegion(0,0),pickRegion(1,0),pickRegion(0,1),pickRegion(1,1),
             xRegion(0,0),xRegion(1,0),xRegion(0,1),xRegion(1,1) );

	  printf("pk: numberSelected=%i \n",numberSelected);
          const int numberOfMaps=8;
	  Mapping *mapList[numberOfMaps] = { &map1,&map2,&map3,&map4,&map5,&map6,&map7,&map8}; //
          int chosen=-1, ichosen=0;
	  for( int i=0; i<=selection.getBound(0); i++ )
	  {
            for( int j=0; j<numberOfMaps; j++ )
	    {
  	      if( selection(i,0)==mapList[j]->getGlobalID() )
	      {
	        printf("map%i selected, z1=%i, z2=%i \n",j,selection(i,1),selection(i,2));
                if( chosen<0 || selection(i,1)<selection(ichosen,1) )
		{
                  chosen=j;
                  ichosen=i;
		}
	      }
	    }
	  }
          if( chosen<0 )
	    printf("no mapping selected\n");
	  else
	  {
            printf(" Mapping %i was chosen \n",chosen);
	    
            if( TRUE )
	    {
	      RealArray r(1,3),x(1,3);
	      r(0,0)=.5*(pickRegion(0,0)+pickRegion(1,0));
	      r(0,1)=.5*(pickRegion(0,1)+pickRegion(1,1));
	      r(0,2)=selection(ichosen,1);
	      ps.pickToWorldCoordinates(r,x );
	      printf(" Point chosen: x=(%9.2e,%9.2e,%9.2e)\n",
		     x(0,0),x(0,1),x(0,2));
	    }
	    else
	    {

	      ps.redraw(TRUE);
	    
	      GLdouble model[16],project[16];
	      glGetDoublev(GL_MODELVIEW_MATRIX,model);
	      glGetDoublev(GL_PROJECTION_MATRIX,project);
	      GLint view[4];
	      glGetIntegerv( GL_VIEWPORT, view );

	      printf("pk     :modelview matrix: %5.2f,%5.2f,%5.2f,%5.2f \n"
		     "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		     "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		     "                          %5.2f,%5.2f,%5.2f,%5.2f \n",
		     model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],
		     model[9],model[10],model[11],model[12],model[13],model[14],model[15]);
	      printf("pk     :projection matrix %5.2f,%5.2f,%5.2f,%5.2f \n"
		     "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		     "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		     "                          %5.2f,%5.2f,%5.2f,%5.2f \n",
		     project[0],project[1],project[2],project[3],project[4],project[5],project[6],project[7],project[8],
		     project[9],project[10],project[11],project[12],project[13],project[14],project[15]);




	      GLdouble objx,objy,objz, winx,winy,winz;
	  


	      real z0=selection(ichosen,1);
	      real zBufferResolution=pow(2.,31.); // where does 31 come from ?
	      z0=z0/zBufferResolution;
	      // real near=-10., far=25.;
	      // z0=z0/(far-near)+near;
	    
	      printf(" ***** z0=%e \n",z0);

	      real x,y,z;
	      x=.5*(pickRegion(0,0)+pickRegion(1,0));
	      y=.5*(pickRegion(0,1)+pickRegion(1,1));
	      z=0.;
	  
	      winx=x*(view[2]-view[0])+view[0];
	      winy=y*(view[3]-view[1])+view[1];
	      winz=z0;
	  
	      GLdouble x1,y1,z1, x2,y2,z2;
	      gluUnProject( winx,winy,winz,model,project,view,&x1,&y1,&z1);

	      winz=1.0;
	      gluUnProject( winx,winy,winz,model,project,view,&x2,&y2,&z2);

	      real n1,n2,n3;
	      n1=x2-x1;
	      n2=y2-y1;
	      n3=z2-z1;
	  
	      // intersect the line x1 -> x2 with the chosen object.
	      printf(" win=(%9.2e,%9.2e,%9.2e) x1=(%9.2e,%9.2e,%9.2e) x2=(%9.2e,%9.2e,%9.2e)\n",
		     winx,winy,winz,x1,y1,z1,x2,y2,z2);
	    
	      Mapping & map = *mapList[chosen];
	      RealArray r(1,3),x0(1,3),xp(1,3);
	      r=0.;
	      x0(0,0)=x1;
	      x0(0,1)=y1;
	      x0(0,2)=z1;
	      map.inverseMap(x0,r);
	      r=min(1.,max(0.,r));
	      map.map(r,xp);
	      printf(" x0=(%9.2e,%9.2e,%9.2e), r=(%9.2e,%9.2e,%9.2e), xp=(%9.2e,%9.2e,%9.2e)\n",
		     x0(0,0),x0(0,1),x0(0,2),r(0,0),r(0,1),r(0,2),xp(0,0),xp(0,1),xp(0,2));
	    
	      x1=xp(0,0);
	      y1=xp(0,1);
	      z1=xp(0,2);
	      gluProject( x1,y1,z1,model,project,view,&winx,&winy,&winz);
	      printf(" x=(%9.2e,%9.2e,%9.2e) -> win=(%9.2e,%9.2e,%9.2e)  winz/z0=%e z0/winz=%e\n",
		     x1,y1,z1,winx,winy,winz, winz/z0,z0/winz);
	    }
	    
	  }
	}
      }
    }
    else if( answer=="pick points" )
    {
      for(;;) 
      {
	const aString menu[]=
	{
	  "done",
          ""
	};
        int numberSelected=ps.getMenuItem(menu,answer,"select a point or region",pickRegion);
        if( answer=="done" )
	{
	  break;
	}
	else
	{
	  printf("pk: region selected = (%e,%e)X(%e,%e) \n",
             pickRegion(0,0),pickRegion(1,0),pickRegion(0,1),pickRegion(1,1));

          // ps.redraw(TRUE); // redraw to get view matrices correct

	  GLdouble model[16],project[16];
	  glGetDoublev(GL_MODELVIEW_MATRIX,model);
	  printf("pk     :modelview matrix: %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n",
		 model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],
		 model[9],model[10],model[11],model[12],model[13],model[14],model[15]);
	  glGetDoublev(GL_PROJECTION_MATRIX,project);
	  printf("pk     :projection matrix %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n",
		 project[0],project[1],project[2],project[3],project[4],project[5],project[6],project[7],project[8],
		 project[9],project[10],project[11],project[12],project[13],project[14],project[15]);

	  GLint view[4];
	  glGetIntegerv( GL_VIEWPORT, view );

	  GLdouble objx,objy,objz, winx,winy,winz;
	  
          real x,y,z;
	  x=.5*(pickRegion(0,0)+pickRegion(1,0));
	  y=.5*(pickRegion(0,1)+pickRegion(1,1));
	  z=0.;
	  
          winx=x*(view[2]-view[0])+view[0];
          winy=y*(view[3]-view[1])+view[1];
	  winz=.5;
	  
	  gluUnProject( winx,winy,winz,model,project,view,&objx,&objy,&objz);
          printf(" win=(%9.2e,%9.2e,%9.2e) obj=(%9.2e,%9.2e,%9.2e)\n",
		 winx,winy,winz,objx,objy,objz);



	}
      }
    }
    else if( answer=="transform points" )
    {
      for(;;) 
      {
	const aString menu[]=
	{
	  "done",
          ""
	};
        ps.inputString(answer,"Enter (x,y,z)");
        if( answer=="done" )
	{
	  break;
	}
	else
	{
          real x=0.,y=0.,z=0.;
          sScanF(answer,"%e %e %e",&x,&y,&z);
	  
          // ps.redraw(TRUE);
	  
	  // These don't work in compile mode!
	  GLdouble model[16],project[16];
	  glGetDoublev(GL_MODELVIEW_MATRIX,model);
	  printf("pk     :modelview matrix: %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n",
		 model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],
		 model[9],model[10],model[11],model[12],model[13],model[14],model[15]);
	  glGetDoublev(GL_PROJECTION_MATRIX,project);
	  printf("pk     :projection matrix %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n"
		 "                          %5.2f,%5.2f,%5.2f,%5.2f \n",
		 project[0],project[1],project[2],project[3],project[4],project[5],project[6],project[7],project[8],
		 project[9],project[10],project[11],project[12],project[13],project[14],project[15]);

	  GLint view[4];
	  glGetIntegerv( GL_VIEWPORT, view );

	  GLdouble objx,objy,objz, winx,winy,winz;
	  GLint returnCode;
          objx=x;
          objy=y;
          objz=z;
	  
	  gluProject( objx,objy,objz,model,project,view,&winx,&winy,&winz);
          printf(" x=(%9.2e,%9.2e,%9.2e) win=(%9.2e,%9.2e,%9.2e)\n",
		 x,y,z,winx,winy,winz);
	
	  gluUnProject( winx,winy,winz,model,project,view,&objx,&objy,&objz);
          printf(" x=(%9.2e,%9.2e,%9.2e) win=(%9.2e,%9.2e,%9.2e)\n",
		 objx,objy,objz,winx,winy,winz);

	}
      }
    }
    else if( answer=="redraw" )
    {
      ps.erase();
      
      params.set(GI_PLOT_BOUNDS,bounds); // initialize the plot bounds
      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      params.set(GI_MAPPING_COLOUR,"red");
      ps.plot(map1,params);
      params.set(GI_MAPPING_COLOUR,"blue");
      ps.plot(map2,params);
      params.set(GI_MAPPING_COLOUR,"orange");
      ps.plot(map3,params);
      params.set(GI_MAPPING_COLOUR,"green");
      ps.plot(map4,params);
      params.set(GI_MAPPING_COLOUR,"yellow");
      ps.plot(map5,params);
      params.set(GI_MAPPING_COLOUR,"pink");
      ps.plot(map6,params);
      params.set(GI_MAPPING_COLOUR,"cyan");
      ps.plot(map7,params);
      params.set(GI_MAPPING_COLOUR,"yellow");
      ps.plot(map8,params);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      cout << "unknown response = " << answer << endl;
      ps.stopReadingCommandFile();
      
    }
  }

  ps.unAppendTheDefaultPrompt();  // reset
  return 0;
}
