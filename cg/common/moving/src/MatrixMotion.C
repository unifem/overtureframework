#include "MatrixMotion.h"
#include "PlotStuff.h"


// =============================================================================
/// \brief Constructor: this class knows how to rotate around a line in space 
///         or translate along a line
/// \details
///    Motions are of the form of a rotation and translation: 
///          x(t) = R(t) * x(0) + g(t) 
///   where R(t) is a 3x3 matrix and g(t) is a 3-vector.
// =============================================================================
MatrixMotion::
MatrixMotion()
{
  motionType=rotateAroundALine;
  
  preMotion=NULL;

  // rotate about the line through the point x0 and tangent v 
  x0[0]=0.; x0[1]=0.; x0[2]=0.; 

  // real v[3] ={0.,0.,1.};   // assumed normalized  v^T v = 1 
  v[0]=0.; v[1]=0.; v[2]=1.; 

  x0[0]=-1.; x0[1]=-1.; x0[2]=0.;
  v[0]=1.; v[1]=1.; v[2]=0.; 

  real vNorm = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
  v[0]/=vNorm;
  v[1]/=vNorm;
  v[2]/=vNorm;

  // default angle is 2*Pi*t : 
  timeFunction.setLinearFunction( 0.,2.*Pi );
}

// =============================================================================
/// \brief Copy constructor.
// =============================================================================
MatrixMotion::
MatrixMotion( const MatrixMotion & mm, const CopyType ct /* = DEEP */ )
{
  *this=mm;
}

// =============================================================================
/// \brief Destructor.
// =============================================================================
MatrixMotion::
~MatrixMotion()
{
  if( preMotion!=NULL && preMotion->decrementReferenceCount()==0 ) 
  {
    delete preMotion; preMotion=NULL;
  }
}

// =============================================================================
/// \brief Equals operator. Set this object to be equal to another.
// =============================================================================
MatrixMotion & MatrixMotion::
operator =( const MatrixMotion & mm )
{
  motionType=mm.motionType;
  for( int i=0; i<3; i++ )
  {
    x0[i]=mm.x0[i];
    v[i]=mm.v[i];
  }

  timeFunction=mm.timeFunction; 

//   if( preMotion!=NULL && preMotion->decrementReferenceCount()==0 )
//   {
//     delete preMotion; preMotion=NULL;
//   }
  if( mm.preMotion!=NULL )
  {
    if( preMotion==NULL )
    {
      preMotion =new MatrixMotion;  preMotion->incrementReferenceCount();
    }
    // deep copy: 
    *preMotion=*mm.preMotion;
  }
  else if( preMotion!=NULL && preMotion->decrementReferenceCount()==0 )
  {
    delete preMotion; preMotion=NULL;
  }
  
  return *this;
}

// =================================================================================
/// \brief Write information about the deforming body
// =================================================================================
void MatrixMotion::
writeParameterSummary( FILE *file /* =stdout */ )
{
  fPrintF(file,"------------------- MatrixMotion body -----------------------\n");
  fPrintF(file," motionType=%s\n",
	  (motionType==rotateAroundALine ? "rotate around a line" : "translate along a line"));
  fPrintF(file,"Point on the line   = (%g,%g,%g)",x0[0],x0[1],x0[2]);
  fPrintF(file,"Tangent to the line = (%g,%g,%g)",v[0],v[1],v[2]);
  fPrintF(file,"--------------------------------------------------------------------\n");
  
}


// =============================================================================
/// \brief Set the type of the motion.
/// \param moition (input) : set the motion type to this value.
// =============================================================================
int MatrixMotion::
setMotionType( const MotionTypeEnum motion )
{
  motionType=motion;
  return 0;
}



// ==========================================================
/// \brief Define the line of rotation or line of translation 
///          from a point on the line and the tangent
/// \param x0 (input) : x0[0:2] is a point on the line
/// \param v (input) : v[0:2] is the tangent to the line
/// \param motion (input) : set the motion type, default=rotateAroundALine
// ==========================================================
int MatrixMotion::
setLine( const real *x0_, const real *v_, const MotionTypeEnum motion /*= rotateAroundALine */ )
{
  motionType=motion;
  
  for( int i=0; i<3; i++ )
  {
    x0[i]=x0_[i];
    v[i]=v_[i];
  }
  real vNorm = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
  if( vNorm<REAL_MIN*100. )
  {
    printF("MatrixMotion::setLineOfRotation:ERROR: the tangent vector is too small, norm=%8.2e\n"
           " I am not going to normalize the tangent\n",vNorm);
    vNorm=1.;
  }
  
  v[0]/=vNorm;
  v[1]/=vNorm;
  v[2]/=vNorm;

  return 0;
}

// ==============================================================================================
/// \brief Compose this MatrixMotion with another which is applied first.
/// \param motion (input) : apply this MatrixMotion before the current. (set to NULL for none)
// =============================================================================================
int MatrixMotion::
compose( MatrixMotion *motion )
{
  // *wdh* 110324 -- check for any existing premotion and also check for motion==NULL
  if( preMotion!=NULL && preMotion->decrementReferenceCount()==0 ) 
  {
    delete preMotion; preMotion=NULL;
  }
  if( motion!=NULL )
  {
    preMotion=motion;
    preMotion->incrementReferenceCount();
  }
  return 0;
}



// ------------------------------------------------------------
/// \brief : Determine the motion matrix at time t 
///
///    x(t) = R(t) * x(0) + g(t) 
/// 
/// \param r(0:2,0:3) : 3x4 matrix holding the rotation matrix and shift: 
/// 
///  R = r(0:2,0;3) : 3x3 rotation matrix
///  g = r(0:2,3)   : 3 vector "shift"
// ------------------------------------------------------------
int MatrixMotion::
getMotion( const real & t, RealArray & r )
{
  return getMotion( t,r,r,-1 );
}

// ==========================================================================
/// \brief Evaluate the motion matrix and an arbitrary time derivative
/// \param t (input) : time to evaluate the motion
/// \param r (output) : matrix
/// \param rp (output) : holds the "derivative" time derivative of r
/// \param derivative (input) : if derivative>0 then evaluate this time derivative 
/// \param computeComposed (input) : if true (default) then evaluate the composed motion, otherwise
///            just evaluate the un-composed mapping.
// ==========================================================================
int MatrixMotion::
getMotion( const real & t, RealArray & r, RealArray & rp, int derivative, bool computeComposed /* =true */ )
{
  
       
//  real theta = 2.*Pi*t;  // do this for now 
  real theta;
  timeFunction.eval(t,theta);
  
  if( motionType==rotateAroundALine )
  {
    
    real ct = cos(theta), st=sin(theta);
  
    // Form the rotation matrix: (for a derivation, see the notes in RevolutionMapping.C)
    //     R = v v^T + cos(theta) ( I -v v^T ) + sin(theta) ( v X )(  I -v v^T)
    //
    //   x = R *( x(0) - x0 ) + x0 
    //     = R * x(0)  + (I-R)*x0 

    r(0,0) = v[0]*v[0]*(1.-ct)+ct     ;  r(0,1) = v[0]*v[1]*(1.-ct)-st*v[2];  r(0,2) = v[0]*v[2]*(1.-ct)+st*v[1];
    r(1,0) = v[0]*v[1]*(1.-ct)+st*v[2];  r(1,1) = v[1]*v[1]*(1.-ct)+ct     ;  r(1,2) = v[1]*v[2]*(1.-ct)-st*v[0];
    r(2,0) = v[0]*v[2]*(1.-ct)-st*v[1];  r(2,1) = v[2]*v[1]*(1.-ct)+st*v[0];  r(2,2) = v[2]*v[2]*(1.-ct)+ct     ;
  

    r(0,3) = (1.-r(0,0))*x0[0]    -r(0,1) *x0[1]    -r(0,2) *x0[2];
    r(1,3) =    -r(1,0) *x0[0]+(1.-r(1,1))*x0[1]    -r(1,2) *x0[2];
    r(2,3) =    -r(2,0) *x0[0]    -r(2,1) *x0[1]+(1.-r(2,2))*x0[2];
  

    if( derivative>0 )
    {
      // --- Compute a time derivative ---

      // thetap = d(theta)/dt
      real thetap;
      timeFunction.evalDerivative(t,thetap,1);  

      // ctp = d^p(cos(theta))/dt^p , p=derivative 
      // stp = d^p(sin(theta))/dt^p 
      real ctp, stp;
      if( derivative==1 )
      {
	ctp=-thetap*st; stp=thetap*ct;
      }
      else if( derivative==2 ) 
      {
	// thetapp = d^2(theta)/dt^2 
	real thetapp;
	timeFunction.evalDerivative(t,thetapp,2); 
	ctp=-thetap*thetap*ct -thetapp*st;   stp=-thetap*thetap*st +thetapp*ct;
      }
      else
      {
	OV_ABORT("finish me");
      }
    

      rp(0,0)= v[0]*v[0]*(  -ctp)+ctp    ;  rp(0,1)= v[0]*v[1]*(  -ctp)-stp*v[2]; rp(0,2)= v[0]*v[2]*(  -ctp)+stp*v[1];
      rp(1,0)= v[0]*v[1]*(  -ctp)+stp*v[2]; rp(1,1)= v[1]*v[1]*(  -ctp)+ctp     ; rp(1,2)= v[1]*v[2]*(  -ctp)-stp*v[0];
      rp(2,0)= v[0]*v[2]*(  -ctp)-stp*v[1]; rp(2,1)= v[2]*v[1]*(  -ctp)+stp*v[0]; rp(2,2)= v[2]*v[2]*(  -ctp)+ctp     ;
  

      rp(0,3) = (  -rp(0,0))*x0[0]    -rp(0,1) *x0[1]    -rp(0,2) *x0[2];
      rp(1,3) =    -rp(1,0) *x0[0]+(  -rp(1,1))*x0[1]    -rp(1,2) *x0[2];
      rp(2,3) =    -rp(2,0) *x0[0]    -rp(2,1) *x0[1]+(  -rp(2,2))*x0[2];


    }
  }
  else if( motionType==translateAlongALine )
  {
    // --- translate along a line ---

    r(0,0) = 1.;  r(0,1) = 0.;  r(0,2) = 0.;
    r(1,0) = 0.;  r(1,1) = 1.;  r(1,2) = 0.;
    r(2,0) = 0.;  r(2,1) = 0.;  r(2,2) = 1.;
  

    r(0,3) = x0[0] + v[0]*theta;
    r(1,3) = x0[1] + v[1]*theta;
    r(2,3) = x0[2] + v[2]*theta;

    if( derivative>0 )
    {
      // --- Compute a time derivative ---
      real thetap;
      timeFunction.evalDerivative(t,thetap,derivative); 

      rp(0,0) = 0.;  rp(0,1) = 0.;  rp(0,2) = 0.;
      rp(1,0) = 0.;  rp(1,1) = 0.;  rp(1,2) = 0.;
      rp(2,0) = 0.;  rp(2,1) = 0.;  rp(2,2) = 0.;

      rp(0,3) = v[0]*thetap;
      rp(1,3) = v[1]*thetap;
      rp(2,3) = v[2]*thetap;

    }
    
  }
  else
  {
    printF("Unknown motionType=%i\n",(int)motionType);
    OV_ABORT("ERROR");
  }
  
  if( computeComposed && preMotion!=NULL )
  {
    // --- we compose the current motion with preMotion ---

    // Current:   x = R1*x0 + g1
    // premotion: x = R2*x(0) + g2

    // Composed: 
    //    x = R1*( R2*x(0) + g2 ) + g1(t)
    //      = R1*R2*x(0) + R1*g2+g1 

    Range I3=3, I4=4;
    RealArray r1(3,4), r1p(3,4);
    r1=r(I3,I4);
    if( derivative>0 ) 
      r1p=rp(I3,I4);

    RealArray r2(3,4), r2p(3,4);
    // note: r2p will hold derivative "derivative" (we may need to compute lower derivatives below)   
    preMotion->getMotion( t, r2, r2p, derivative );
    
    for( int i=0; i<3; i++)
    {
      for( int j=0; j<3; j++ )
      {
        r(i,j)=0.;
	for( int k=0; k<3; k++ )
	  r(i,j) += r1(i,k)*r2(k,j);

	r(i,3) += r1(i,j)*r2(j,3);   // g = R1*g2 + g1
      }
      
    }
    
    // Derivatives of the composed mapping:
    if( derivative<=0 )
    {
    }
    else if( derivative==1 )
    {
      // x = R1*R2*x(0) + R1*g2+g1 
      // x.t = (R1' *R2 + R1*R2')*x(0) + R1'*g2 + R1*g2' + g1'
      for( int i=0; i<3; i++)
      {
	for( int j=0; j<3; j++ )
	{
	  rp(i,j)=0.;
	  for( int k=0; k<3; k++ )
	    rp(i,j) += r1p(i,k)*r2(k,j) + r1(i,k)*r2p(k,j);

	  rp(i,3) += r1p(i,j)*r2(j,3) + r1(i,j)*r2p(j,3);   // R1'*g2 + R1*g2' + g1'
	}
      }
    }
    else if( derivative==2 )
    {
      // Second derivative: 
      // x.tt = (R1'' *R2 + 2*R1'*R2' + R1*R2'')*x(0) + R1''*g2 + 2*R1'*g2' + R1*g2'' + g1''

      // -- we need to compute the first derivative too:
      RealArray r1t(3,4);
      bool compose=false;
      getMotion( t, r1, r1t, 1, compose );  // we need the first derivative of the un-composed mapping

      RealArray r2t(3,4);
      preMotion->getMotion( t, r2, r2t, 1 );

      for( int i=0; i<3; i++)
      {
	for( int j=0; j<3; j++ )
	{
	  rp(i,j)=0.;
	  for( int k=0; k<3; k++ )
	    rp(i,j) += r1p(i,k)*r2(k,j) + 2.*r1t(i,k)*r2t(k,j) + r1(i,k)*r2p(k,j);

	  rp(i,3) += r1p(i,j)*r2(j,3) + 2.*r1t(i,j)*r2t(j,3) +r1(i,j)*r2p(j,3);   // R1'*g2 + R1*g2' + g1'
	}
      }
    }
    else
    {
      printF("MatrixMotion::getMotion: derivative=%i not implemented.\n",derivative);
      OV_ABORT("finish me");
    }


  }
  
  return 0;
}

// ===========================================================================
/// \brief Get from a data base file.
// ===========================================================================
int MatrixMotion::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"MatrixMotion");

  aString className;
  subDir.get( className,"className" ); 

  int temp;
  subDir.get(temp,"motionType");  motionType=(MotionTypeEnum)temp;

  subDir.get(x0,"x0",3);
  subDir.get(v,"v",3);
  timeFunction.get(subDir,"timeFunction");

  int preMotionExists;
  subDir.get(preMotionExists,"preMotionExists");
  if( preMotionExists )
  {
    if( preMotion==NULL )
    {
      preMotion = new MatrixMotion; preMotion->incrementReferenceCount();
    }
    preMotion->get(subDir,"preMotion");
  }
  else
  {
    if( preMotion!=NULL && preMotion->decrementReferenceCount()==0 )
    {
      delete preMotion; preMotion=NULL;
    }
  }
  
  delete &subDir;
  return 0;
}


// ===========================================================================
/// \brief Put to a data base file.
// ===========================================================================
int MatrixMotion::
put( GenericDataBase & dir, const aString & name) const
{
  GenericDataBase & subDir = *dir.virtualConstructor();   // create a derived data-base object
  dir.create(subDir,name,"MatrixMotion");                 // create a sub-directory 

  aString className="MatrixMotion";
  subDir.put( className,"className" );

  subDir.put((int)motionType,"motionType"); 

  subDir.put(x0,"x0",3);
  subDir.put(v,"v",3);
  timeFunction.put(subDir,"timeFunction");

  int preMotionExists=preMotion!=NULL;
  subDir.put(preMotionExists,"preMotionExists");
  if( preMotionExists )
    preMotion->put(subDir,"preMotion");

  delete &subDir;
  return 0;  
}


// ===============================================================================
/// \brief Interactively update MatrixMotion parameters:
// ===============================================================================
int MatrixMotion::
update(GenericGraphicsInterface & gi )
{

  GUIState dialog;
  bool buildDialog=true;
  if( buildDialog )
  {
    dialog.setWindowTitle("MatrixMotion");
    dialog.setExitCommand("exit", "exit");

    // option menus
    dialog.setOptionMenuColumns(1);

    aString opCommand1[] = {"rotate around a line",
			    "translate along a line",
			    ""};
    
    dialog.addOptionMenu( "type:", opCommand1, opCommand1, motionType); 


    aString cmds[] = {"edit time function",
                      "add composed motion",
                      "edit composed motion",
                      "show parameters",
		      ""};

    int numberOfPushButtons=4;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    const int numberOfTextStrings=7;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "point on line:";  sPrintF(textStrings[nt],"%g,%g,%g",x0[0],x0[1],x0[2]);  nt++; 
    textLabels[nt] = "tangent to line:";  sPrintF(textStrings[nt],"%g,%g,%g",v[0],v[1],v[2]);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    // dialog.buildPopup(menu);
    gi.pushGUI(dialog);
  }
  

  aString answer;

  gi.appendToTheDefaultPrompt("MatrixMotion>"); // set the default prompt
  int len=0;
  for( int it=0;; it++ )
  {
    gi.getAnswer(answer,"");
 

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="rotate around a line" )
    {
      motionType=rotateAroundALine;
    }
    else if( answer=="translate along a line" )
    {
      motionType=translateAlongALine;
    }
    else if( len=answer.matches("point on line:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&x0[0],&x0[1],&x0[2]);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("point on line:",sPrintF(answer,"%g,%g,%g",x0[0],x0[1],x0[2]));
    }
    else if( len=answer.matches("tangent to line:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&v[0],&v[1],&v[2]);
      // if( !gi.isGraphicsWindowOpen() )
      dialog.setTextLabel("tangent to line:",sPrintF(answer,"%g,%g,%g",v[0],v[1],v[2]));

      real vNorm = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
      if( vNorm<REAL_MIN*100. )
      {
	printF("MatrixMotion::setLineOfRotation:ERROR: the tangent vector is too small, norm=%8.2e\n"
	       " I am not going to normalize the tangent\n",vNorm);
	vNorm=1.;
      }
      v[0]/=vNorm;
      v[1]/=vNorm;
      v[2]/=vNorm;

    }
    else if( answer=="show parameters" )
    {
      printF("Motion type: %s\n",(motionType==rotateAroundALine ? "rotate around a line" :
                                  motionType==translateAlongALine ? "translate along a line" : "unknown"));
      printF("Point on the line   = (%g,%g,%g)",x0[0],x0[1],x0[2]);
      printF("Tangent to the line = (%g,%g,%g)",v[0],v[1],v[2]);
      if( preMotion!=NULL )
	printF("This MatrixMotion is composed with another one.\n");
      else
        printF("This MatrixMotion is not currently composed with any other.\n");
    }
    else if( answer=="edit time function" )
    {
      timeFunction.update(gi);
    }
    else if( answer=="edit composed motion" )
    {
      printF("MatrixMotion::INFO: The current MatrixMotion can be composed with another MatrixMotion\n"
             "   The current MatrixMotion is applied AFTER the `composed MatrixMotion'\n");
      if( preMotion!=NULL )
      {
	preMotion->update(gi);
      }
      else
      {
	printF("MatrixMotion::WARNING: there is no composed motion defined. "
               "You should choose `add composed motion'\n");
      }
    }
    else if( answer=="add composed motion" )
    {
      printF("MatrixMotion::INFO: The current MatrixMotion can be composed with another MatrixMotion\n"
             "   The current MatrixMotion is applied AFTER the `composed MatrixMotion'\n");
      if( preMotion==NULL )
      {
        preMotion = new MatrixMotion; preMotion->incrementReferenceCount();
	
	preMotion->update(gi);
      }
      else
      {
	printF("MatrixMotion::WARNING: there is already a composed motion defined. "
               "You should choose `edit composed motion' to make changes.\n");
      }
    }
    else 
    {
      printF("MatrixMotion::update: unknown response=%s",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }
  gi.unAppendTheDefaultPrompt();  // reset prompt
  if( buildDialog )
  {
    gi.popGUI(); // restore the previous GUI
  }
  return 0;
}

