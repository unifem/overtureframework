// =============================================================================
//
// Mark hole points on the refinement grid that lie between two holes on the
// base grid
//
// RATIO: 2,4,general -- refinement ratio
// DIM: 2,3 -- number of dimensions
// =============================================================================
#beginMacro setMaskAtAlignedHoles(DIM,RATIO)
I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();

I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();

for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
{
  if( MASKB(i1b,i2b,i3b)==0 )
  {
    #If #RATIO == "2"
      // *wdh* 070513 -- fixed so that only points between two adjacent maskb==0 points are marked 0 
      if( i1b<I1bBound ) 
      {
	if( MASKB(i1b+1,i2b,i3b)==0 ) MASK(i1r+1,i2r,i3r)=0;
      }
      else
      {
	if( MASKB(i1b-1,i2b,i3b)==0 ) MASK(i1r-1,i2r,i3r)=0;
      }
      if( i2b<I2bBound ) 
      {
	if( MASKB(i1b,i2b+1,i3b)==0 ) MASK(i1r,i2r+1,i3r)=0;
      }
      else
      {
	if( MASKB(i1b,i2b-1,i3b)==0 ) MASK(i1r,i2r-1,i3r)=0;
      }
      #If #DIM == "3"
      if( i3b<I3bBound ) 
      {
	if( MASKB(i1b,i2b,i3b+1)==0 ) MASK(i1r,i2r,i3r+1)=0;
      }
      else
      {
	if( MASKB(i1b,i2b,i3b-1)==0 ) MASK(i1r,i2r,i3r-1)=0;
      }
      #End
    #Elif #RATIO == "4"
      if( i1b<I1bBound )
      {
        if( MASKB(i1b+1,i2b,i3b)==0 )
	{
	  MASK(i1r+1,i2r,i3r)=0;
	  MASK(i1r+2,i2r,i3r)=0;
	  MASK(i1r+3,i2r,i3r)=0;
	}
      }
      else
      {
        if( MASKB(i1b-1,i2b,i3b)==0 )
	{
	  MASK(i1r-1,i2r,i3r)=0;
	  MASK(i1r-2,i2r,i3r)=0;
	  MASK(i1r-3,i2r,i3r)=0;
	}
      }
      if( i2b<I2bBound )
      {
        if( MASKB(i1b,i2b+1,i3b)==0 )
	{
	  MASK(i1r,i2r+1,i3r)=0;
	  MASK(i1r,i2r+2,i3r)=0;
	  MASK(i1r,i2r+3,i3r)=0;
	}
      }
      else
      {
        if( MASKB(i1b,i2b-1,i3b)==0 )
	{
	  MASK(i1r,i2r-1,i3r)=0;
	  MASK(i1r,i2r-2,i3r)=0;
	  MASK(i1r,i2r-3,i3r)=0;
	}
      }
      #If #DIM == "3"
      if( i3b<I3bBound )
      {
        if( MASKB(i1b,i2b,i3b+1)==0 )
	{
	  MASK(i1r,i2r,i3r+1)=0;
	  MASK(i1r,i2r,i3r+2)=0;
	  MASK(i1r,i2r,i3r+3)=0;
	}
      }
      else 
      {
        if( MASKB(i1b,i2b,i3b-1)==0 ) 
	{
	  MASK(i1r,i2r,i3r-1)=0;
	  MASK(i1r,i2r,i3r-2)=0;
	  MASK(i1r,i2r,i3r-3)=0;
	}
      }
      #End
    #Elif #RATIO == "general"
      if( i1b<I1bBound )
      {
        if( MASKB(i1b+1,i2b,i3b)==0 )
          for( r=1; r<rf[0]; r++ ) 
	    MASK(i1r+r,i2r,i3r)=0;
      }
      else 
      {
        if( MASKB(i1b-1,i2b,i3b)==0 )
          for( r=1; r<rf[0]; r++ ) 
	    MASK(i1r-r,i2r,i3r)=0;
      }
      if( i2b<I2bBound )
      {
        if( MASKB(i1b,i2b+1,i3b)==0 )
          for( r=1; r<rf[1]; r++ )
	    MASK(i1r,i2r+r,i3r)=0;
      }
      else 
      {
        if( MASKB(i1b,i2b-1,i3b)==0 )
          for( r=1; r<rf[1]; r++ )
	    MASK(i1r,i2r-r,i3r)=0;
      }
      #If #DIM == "3"
      if( i3b<I3bBound )
      {
        if( MASKB(i1b,i2b,i3b+1)==0 )
          for( r=1; r<rf[2]; r++ )
	    MASK(i1r,i2r,i3r+r)=0;
      }
      else 
      {
        if( MASKB(i1b,i2b,i3b-1)==0 ) 
	  for( r=1; r<rf[2]; r++ )
	    MASK(i1r,i2r,i3r-r)=0;
      }
      #End
    #Else
      stop 7373
    #End
  }
}
#endMacro
