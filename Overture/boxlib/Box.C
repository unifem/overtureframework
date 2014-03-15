/*
** This software is copyright (C) by the Lawrence Berkeley National
** Laboratory.  Permission is granted to reproduce this software for
** non-commercial purposes provided that this notice is left intact.
**  
** It is acknowledged that the U.S. Government has rights to this
** software under Contract DE-AC03-765F00098 between the U.S.  Department
** of Energy and the University of California.
**  
** This software is provided as a professional and academic contribution
** for joint exchange. Thus it is experimental, is provided ``as is'',
** with no warranties of any kind whatsoever, no support, no promise of
** updates, or printed documentation. By using this software, you
** acknowledge that the Lawrence Berkeley National Laboratory and Regents
** of the University of California shall have no liability with respect
** to the infringement of other copyrights by any part of this software.
**  
** For further information about this software, contact:
** 
**         Dr. John Bell
**         Bldg. 50D, Rm. 129,
**         Lawrence Berkeley National Laboratory
**         Berkeley, CA, 94720
**         jbbell@lbl.gov
*/

//
// $Id: Box.C,v 1.6 2008/12/03 17:54:46 chand Exp $
//

#include <limits.h>

#include <BL_Assert.H>
#include <BoxLib.H>
#include <Misc.H>
#include <Box.H>
#include <Utility.H>

const Box&
Box::TheUnitBox ()
{
    static const Box Unit(IntVect::TheZeroVector(), IntVect::TheUnitVector());
    return Unit;
}

//
// Administrative functions.
//
Box::Box ()
    : smallend(IntVect::TheUnitVector()),
      bigend(IntVect::TheZeroVector()),
      btype()
{
    //
    // Mark as illegal Box.
    //
    D_EXPR(len[0] = -1, len[1] = -1, len[2] = -1);
}

Box::Box (const IntVect& small,
          const int*     vec_len)
    : smallend(small),
      bigend(D_DECL(small[0]+vec_len[0]-1,
                    small[1]+vec_len[1]-1,
                    small[2]+vec_len[2]-1))
{
    D_EXPR(len[0] = vec_len[0], len[1] = vec_len[1], len[2] = vec_len[2]);
}

Box::Box (const IntVect&   small,
          const IntVect&   big,
          const IndexType& t)
    : smallend(small),
      bigend(big),
      btype(t)
{
    computeBoxLen();
}

Box::Box (const IntVect& small,
          const IntVect& big)
    : smallend(small),
      bigend(big)
{
    computeBoxLen();
}

Box::Box (const IntVect& small,
          const IntVect& big,
          const IntVect& typ)
    : smallend(small),
      bigend(big),
      btype(typ)
{
    boxAssert(typ >= IntVect::TheZeroVector() && typ <= IntVect::TheUnitVector());
    computeBoxLen();
}

bool
Box::numPtsOK (long& N) const
{
    boxAssert(ok());

    N = len[0];

    for (int i = 1; i < BL_SPACEDIM; i++)
    {
        if (len[i] == 0)
        {
            N = 0;
            return true;
        }
        else if (N <= LONG_MAX/len[i])
        {
            N *= len[i];
        }
        else
        {
            //
            // The return value `N' will be undefined.
            //
            return false;
        }
    }

    return true;
}

long
Box::numPts () const
{
    long result;
    if (!numPtsOK(result))
        BoxLib::Error("Arithmetic overflow in Box::numPts()");
    return result;
}

bool
Box::volumeOK (long& N) const
{
    boxAssert(ok());

    N = len[0]-btype[0];

    for (int i = 1; i < BL_SPACEDIM; i++)
    {
        long diff = (len[i]-btype[i]);

        if (diff == 0)
        {
            N = 0;
            return true;
        }
        else if (N <= LONG_MAX/diff)
        {
            N *= diff;
        }
        else
        {
            //
            // The return value of N will be undefined.
            //
            return false;
        }
    }

    return true;
}

long
Box::volume () const
{
    long result;
    if (!volumeOK(result))
        BoxLib::Error("Arithmetic overflow in Box::volume()");
    return result;
}

Box&
Box::shiftHalf (int dir,
                int nzones)
{
    int nbit = (nzones<0 ? -nzones : nzones)%2;
    int nshift = nzones/2;
    unsigned int bit_dir = btype[dir];
    //
    // Toggle btyp bit if nzones is odd.
    //
    if (nbit)
        btype.flip(dir);
    if (nzones < 0)
        nshift -= (bit_dir ? nbit : 0);
    else
        nshift += (bit_dir ? 0 : nbit);
    smallend.shift(dir,nshift);
    bigend.shift(dir,nshift);
    return *this;
}

//
// Boolean functions.
//

bool
Box::intersects (const Box& b) const
{
 // cout << "Inside of Box::intersects (const Box& b)" << endl;

    boxAssert(sameType(b));
    IntVect low(smallend);
    IntVect hi(bigend);

 // cout << "In Box::intersects(Box): *this = " << *this << endl;
 // cout << "In Box::intersects(Box): (input) b = " << b << endl;

 // cout << "In Box::intersects(Box): low = " << low << endl;
 // cout << "In Box::intersects(Box): hi  = " << hi  << endl;

    low.max(b.smallend);
    hi.min(b.bigend);

 // cout << "In Box::intersects(Box): (after max) low = " << low << endl;
 // cout << "In Box::intersects(Box): (after min) hi  = " << hi  << endl;

    // *wdh* 010922 : undo this fix as suggested by Bobby Philip
#if 0
 // Modification by Dan Quinlan for NODE centered data!
    bool result = 0; // FALSE
 // Make sure that the box is all NODE centered or all CELL centered!
    boxAssert(btype[0] == btype[1]);
    boxAssert(btype[0] == btype[2]);
    if (btype[0])
       {
      //
      // NODE centered Box.
      //
      // cout << "NODE centered Box" << endl;
      // PROBLEM: In the case of a node centered box we need to
      // only consider the valid dimensions (for a 2D box
      // this would mean the first two dimensions).  We can't
      // interpret this from the SPACEDIM value which for OVETURE
      // applications is always set to three!
      // SOLUTION: We will detect the dimensionality based upon the
      // the size of the highest dimension.  In the case of
      // NODE centering we detect a 2D box by the size of the third
      // dimension being 1.  A 1D box is detected similarly.
         if ( (length(2) == 1) && (b.length(2) == 1) )
            {
           // boxes are at most less than 3D
              if ( (length(1) == 1) && (b.length(1) == 1) )
                 {
                // boxes are at most less than 1D
                   if ( (length(0) == 1) && (b.length(0) == 1) )
                      {
                     // boxes are 0D
                        result = 0;
                      }
                     else 
                      {
                     // boxes are 1D
                        low.setVal(1,hi[1]-1);
                        low.setVal(2,hi[2]-1);
                      }
                 }
                else 
                 {
                // boxes are 2D
                   low.setVal(2,hi[2]-1);
                 }
            }
           else 
            {
           // boxes are 3D (nothing to do)
            }

      // cout << "In Box::intersects(Box): (after resetting higher dimensions) low = " << low << endl;
      // cout << "In Box::intersects(Box): (after resetting higher dimensions) hi  = " << hi  << endl;
         result = low < hi;
       }
      else
       {
      //
      // CELL centered Box.
      //
      // cout << "CELL centered Box" << endl;
         result = low <= hi;
       }

 // cout << "result = " << ((result == 0) ? "FALSE" : "TRUE") << endl;

    return result;
#else
 // Original code
    return low <= hi;
#endif
}

//
// Intersection functions.
//

Box&
Box::operator&= (const Box& b)
{
    boxAssert(sameType(b));
    smallend.max(b.smallend);
    bigend.min(b.bigend);
    computeBoxLen();
    return *this;
}

Box&
Box::surroundingNodes ()
{
    for (int i = 0; i < BL_SPACEDIM; ++i)
        if ((btype[i] == 0))
            bigend.shift(i,1);
    btype.setall();
    computeBoxLen();
    return *this;
}

Box&
Box::enclosedCells ()
{
    for (int i = 0 ; i < BL_SPACEDIM; ++i)
        if (btype[i])
            bigend.shift(i,-1);
    btype.clear();
    computeBoxLen();
    return *this;
}

//
// Next:step through the rectangle with unit increment.
//

void
Box::next (IntVect& p) const
{
    boxAssert(contains(p));

    p.shift(0,1);
#if BL_SPACEDIM==2
    if (!(p <= bigend))
    {
        p.setVal(0,smallend[0]);
        p.shift(1,1);
    }
#elif BL_SPACEDIM==3
    if (!(p <= bigend))
    {
        p.setVal(0,smallend[0]);
        p.shift(1,1);
        if (!(p <= bigend))
        {
            p.setVal(1,smallend[1]);
            p.shift(2,1);
        }
    }
#endif
}

//
// Scan point over region of object Box with a vector incrment
// point incrments by 0 direction portion of vector.  When end of
// Box is reached, an increment is made with the 1 direction portion
// of the vector, and the 0 direction scan is resumed.
// effectively, we are scanning a Box, whose length vector is the argument
// vector over the object Box.
// when scan is over, the argument point is over edge of object Box
// this is the signal that we can go no further.
//

void
Box::next (IntVect&   p,
           const int* shv) const
{
    boxAssert(contains(p));

#if   BL_SPACEDIM==1
    p.shift(0,shv[0]);
#elif BL_SPACEDIM==2
    p.shift(0,shv[0]);
    if (!(p <= bigend))
    {
        //
        // Reset 1 coord is on edge, and 2 coord is incremented.
        //
        p.setVal(0,smallend[0]);
        p.shift(1,shv[1]);
    }
#elif BL_SPACEDIM==3
    p.shift(0,shv[0]);
    if (!(p <= bigend))
    {
        //
        // Reset 1 coord is on edge, and 2 coord is incremented.
        //
        p.setVal(0,smallend[0]);
        p.shift(1,shv[1]);
        if(!(p <= bigend))
        {
            p.setVal(1,smallend[1]);
            p.shift(2,shv[2]);
        }
    }
#endif
}

Box&
Box::refine (int refinement_ratio)
{
    IntVect shft(IntVect::TheUnitVector());
    shft -= btype.ixType();
    smallend.scale(refinement_ratio);
    bigend += shft; // Bigend does more than just multiply.
    bigend.scale(refinement_ratio);
    bigend -= shft;
    computeBoxLen();
    return *this;
}

Box&
Box::refine (const IntVect& refinement_ratio)
{
    IntVect shft(IntVect::TheUnitVector());
    shft -= btype.ixType();
    smallend *= refinement_ratio;
    bigend += shft;
    bigend   *= refinement_ratio;
    bigend -= shft;
    computeBoxLen();
    return *this;
}

//
// Define a macro which will compute an object's length vector from
// the smallend and bigend.  Do several versions according to dimension
// requires you to be in a member functions.

int
Box::longside () const
{
    int maxlen = len[0];
    for (int i = 1; i < BL_SPACEDIM; i++)
        if (len[i] > maxlen)
            maxlen = len[i];
    return maxlen;
}

int
Box::longside (int& dir) const
{
    int maxlen = len[0];
    dir = 0;
    for (int i = 1; i < BL_SPACEDIM; i++)
    {
        if (len[i] > maxlen)
        {
            maxlen = len[i];
            dir = i;
        }
    }
    return maxlen;
}

int
Box::shortside () const
{
    int minlen = len[0];
    for (int i = 1; i < BL_SPACEDIM; i++)
        if(len[i] < minlen)
            minlen = len[i];
    return minlen;
}

int
Box::shortside (int& dir) const
{
    int minlen = len[0];
    dir = 0;
    for (int i = 1; i < BL_SPACEDIM; i++)
    {
        if(len[i] < minlen)
        {
            minlen = len[i];
            dir = i;
        }
    }
    return minlen;
}

//
// Modified Box is low end, returned Box is high end.
// If CELL: chop_pnt included in high end.
// If NODE: chop_pnt included in both Boxes.
//

Box
Box::chop (int dir,
           int chop_pnt)
{
    //
    // Define new high end Box including chop_pnt.
    //
    IntVect sm(smallend);
    IntVect bg(bigend);
    sm.setVal(dir,chop_pnt);
    if (btype[dir])
    {
        //
        // NODE centered Box.
        //
        boxAssert(chop_pnt > smallend[dir] && chop_pnt < bigend[dir]);
        //
        // Shrink original Box to just contain chop_pnt.
        //
        bigend.setVal(dir,chop_pnt);
    }
    else
    {
        //
        // CELL centered Box.
        //
        boxAssert(chop_pnt > smallend[dir] && chop_pnt <= bigend[dir]);
        //
        // Shrink origional Box to one below chop_pnt.
        //
        bigend.setVal(dir,chop_pnt-1);
    }
    computeBoxLen();
    return Box(sm,bg,btype);
}

//
// Shift by half increments.
//

Box&
Box::shiftHalf (const IntVect& nz)
{
   for (int dir = 0; dir < BL_SPACEDIM; dir++)
   {
       int nzones = nz[dir];
       int nbit = (nzones<0 ? -nzones : nzones)%2;
       int nshift = nzones/2;
       unsigned int bit_dir = btype[dir];
       //
       // Toggle btype bit if nzones is odd.
       //
       if (nbit)
           btype.flip(dir);
       if (nzones < 0)
           nshift -= (bit_dir ? nbit : 0);
       else
           nshift += (bit_dir ? 0 : nbit);
       smallend.shift(dir,nshift);
       bigend.shift(dir,nshift);
   }
   return *this;
}

Box&
Box::convert (int                  dir,
              IndexType::CellIndex typ)
{
   unsigned int bitval = btype[dir];
   int off = typ - bitval;
   bigend.shift(dir,off);
   if (off != 0)
      computeBoxLen();
   //
   // Set dir'th bit to typ.
   //
   btype.setType(dir,typ);
   return *this;
}

Box&
Box::convert (IndexType t)
{
   for (int dir = 0; dir < BL_SPACEDIM; dir++)
   {
      unsigned int typ = t[dir];
      unsigned int bitval = btype[dir];
      int off = typ - bitval;
      bigend.shift(dir,off);
      btype.setType(dir, (IndexType::CellIndex) typ);
   }
   computeBoxLen();
   return *this;
}

//
// Refinement functions.
//

Box
refine (const Box& b,
        int        refinement_ratio)
{
    IntVect small(b.smallend);
    small.scale(refinement_ratio);
    IntVect big(b.bigend);
    IntVect shft(IntVect::TheUnitVector());
    shft -= b.btype.ixType();
    big += shft;        // Large end is not just multiply.
    big.scale(refinement_ratio);
    big -= shft;
    return Box(small,big,b.btype);
}

Box
refine (const Box&     b,
        const IntVect& refinement_ratio)
{
    IntVect small(b.smallend);
    small *= refinement_ratio;
    IntVect big(b.bigend);
    IntVect shft(IntVect::TheUnitVector());
    shft -= b.btype.ixType();
    big += shft;
    big *= refinement_ratio;
    big -= shft;
    return Box(small,big,b.btype);
}

//
// Coarsening.
//

Box&
Box::coarsen (const IntVect& refinement_ratio)
{
    smallend.coarsen(refinement_ratio);

    if (btype.any())
    {
        IntVect off(IntVect::TheZeroVector());
        for (int dir = 0; dir < BL_SPACEDIM; dir++)
        {
            if (btype[dir])
            {
                int b = bigend[dir];
                int r = refinement_ratio[dir];
                if (b%r)
                    off.setVal(dir,1);
            }
        }
        bigend.coarsen(refinement_ratio);
        bigend += off;
    }
    else
        bigend.coarsen(refinement_ratio);

    computeBoxLen();
    return *this;
}

Box
coarsen (const Box& b,
         int  refinement_ratio)
{
    IntVect small(b.smallend);
    small.coarsen(refinement_ratio);
    IntVect big(b.bigend);

    if (b.btype.any())
    {
        IntVect off(IntVect::TheZeroVector());
        for (int dir = 0; dir < BL_SPACEDIM; dir++)
        {
            if (b.btype[dir])
                if (big[dir]%refinement_ratio)
                    off.setVal(dir,1);
        }
        big.coarsen(refinement_ratio);
        big += off;
    }
    else
        big.coarsen(refinement_ratio);

    return Box(small,big,b.btype);
}

Box&
Box::coarsen (int refinement_ratio)
{
    smallend.coarsen(refinement_ratio);
    if (btype.any())
    {
        IntVect off(IntVect::TheZeroVector());
        for (int dir = 0; dir < BL_SPACEDIM; dir++)
        {
            if (btype[dir])
                if (bigend[dir]%refinement_ratio)
                    off.setVal(dir,1);
        }
        bigend.coarsen(refinement_ratio);
        bigend += off;
    }
    else
        bigend.coarsen(refinement_ratio);

    computeBoxLen();
    return *this;
}

Box
coarsen (const Box&     b,
         const IntVect& refinement_ratio)
{
    IntVect small(b.smallend);
    small.coarsen(refinement_ratio);
    IntVect big(b.bigend);

    if (b.btype.any())
    {
        IntVect off(IntVect::TheZeroVector());
        for (int dir = 0; dir < BL_SPACEDIM; dir++)
        {
            if (b.btype[dir])
                if (big[dir]%refinement_ratio[dir])
                    off.setVal(dir,1);
        }
        big.coarsen(refinement_ratio);
        big += off;
    }
    else
        big.coarsen(refinement_ratio);

    return Box(small,big,b.btype);
}

//
// I/O functions.
//

std::ostream&
operator<< (std::ostream&   os,
            const Box& b)
{
    os << '('
       << b.smallend << ' '
       << b.bigend   << ' '
       << b.btype.ixType()
       << ')';

    if (os.fail())
        BoxLib::Error("operator<<(ostream&,Box&) failed");

    return os;
}

std::istream&
operator>> (std::istream& is,
            Box&     b)
{
  // *wdh* is >> ws;
    char c;
    is >> c;
    is.putback(c);
    if (c == '(')
    {
        is.ignore(BL_IGNORE_MAX, '(');
        is >> b.smallend ;
        is >> b.bigend ;
        IntVect v;
        is >> v;
        b.btype = v;
        boxAssert(b.btype.ok());
        is.ignore(BL_IGNORE_MAX,')');
        b.computeBoxLen();
    }
    else if (c == '<')
    {
        is >> b.smallend;
        is >> b.bigend;
        IntVect v;
        is >> v;
        b.btype = v;
        boxAssert(b.btype.ok());
        b.computeBoxLen();
    }
    else
        BoxLib::Error("operator>>(istream&,Box&): expected \'<\'");

    if (is.fail())
        BoxLib::Error("operator>>(istream&,Box&) failed");

    return is;
}

void
Box::dumpOn (std::ostream& strm) const
{
    strm << "Box ("
         << BoxLib::version
         << ")"
         << smallend
         << " to "
         << bigend
         << " type ["
         << btype.ixType()
         << "]"
         << std::endl;

    if (strm.fail())
        BoxLib::Error("Box::dumpOn(ostream&) failed");
}

//
// Give smallest Box containing both Boxes.
//

Box&
Box::minBox (const Box &b)
{
    boxAssert(b.ok() && ok());
    boxAssert(sameType(b));
    smallend.min(b.smallend);
    bigend.max(b.bigend);
    computeBoxLen();
    return *this;
}

Box
minBox (const Box& b,
        const Box& o)
{
    boxAssert(b.ok() && o.ok());
    boxAssert(o.sameType(b));
    IntVect small = b.smallend;
    IntVect big = b.bigend;
    small.min(o.smallend);
    big.max(o.bigend);
    return Box(small,big,b.btype);
}

//
// Intersection.
//

Box
Box::operator& (const Box& b) const
{
    boxAssert(sameType(b));
    IntVect low(smallend);
    IntVect hi(bigend);
    low.max(b.smallend);
    hi.min(b.bigend);
    return Box(low,hi,btype);
}

//
// Translation functions acting on relative vectors.
//

Box
surroundingNodes (const Box& b,
                  int        dir)
{
    boxAssert(!(b.btype[dir]));
    IntVect hi(b.bigend);
    hi.shift(dir,1);
    //
    // Set dir'th bit to 1 = IndexType::NODE.
    //
    IndexType typ(b.btype);
    typ.setType(dir,IndexType::NODE);
    return Box(b.smallend,hi,typ);
}

Box
surroundingNodes (const Box& b)
{
   IntVect hi(b.bigend);
   for (int i = 0; i < BL_SPACEDIM; ++i)
       if ((b.btype[i]) == 0)
           hi.shift(i,1);
   return Box(b.smallend,hi,IntVect::TheUnitVector());
}

Box
enclosedCells (const Box& b,
               int        dir)
{
    boxAssert(b.btype[dir]);
    IntVect hi(b.bigend);
    hi.shift(dir,-1);
    //
    // Set dir'th bit to 0 = IndexType::CELL.
    //
    IndexType typ(b.btype);
    typ.setType(dir,IndexType::CELL);
    return Box(b.smallend,hi,typ);
}

Box
enclosedCells (const Box& b)
{
   IntVect hi(b.bigend);
   for (int i = 0; i< BL_SPACEDIM; i++)
       if (b.btype[i])
           hi.shift(i,-1);
   return Box(b.smallend,hi,IntVect::TheZeroVector());
}

Box
bdryLo (const Box& b,
        int        dir,
        int        len)
{
    IntVect low(b.smallend);
    IntVect hi(b.bigend);
    int sm = low[dir];
    hi.setVal(dir,sm+len-1);
    //
    // set dir'th bit to 1 = IndexType::NODE.
    //
    IndexType typ(b.btype);
    typ.set(dir);
    return Box(low,hi,typ);
}

Box
bdryHi (const Box& b,
        int        dir,
        int        len)
{
    IntVect low(b.smallend);
    IntVect hi(b.bigend);
    unsigned int bitval = b.btype.ixType(dir);
    int bg              = hi[dir]  + 1 - bitval%2;
    low.setVal(dir,bg);
    hi.setVal(dir,bg+len-1);
    //
    // Set dir'th bit to 1 = IndexType::NODE.
    //
    IndexType typ(b.btype);
    typ.set(dir);
    return Box(low,hi,typ);
}

Box
bdryNode (const Box&         b,
          const Orientation& face,
          int                len)
{
    int dir = face.coordDir();
    IntVect low(b.smallend);
    IntVect hi(b.bigend);
    if (face.isLow())
    {
        int sm = low[dir];
        hi.setVal(dir,sm+len-1);
    }
    else
    {
        unsigned int bitval = b.btype.ixType(dir);
        int bg              = hi[dir]  + 1 - bitval%2;
        low.setVal(dir,bg);
        hi.setVal(dir,bg+len-1);
    }
    //
    // Set dir'th bit to 1 = IndexType::NODE.
    //
    IndexType typ(b.btype);
    typ.set(dir);
    return Box(low,hi,typ);
}

Box
adjCellLo (const Box& b,
           int        dir,
           int        len)
{
    boxAssert(len > 0);
    IntVect low(b.smallend);
    IntVect hi(b.bigend);
    int sm = low[dir];
    low.setVal(dir,sm - len);
    hi.setVal(dir,sm - 1);
    //
    // Set dir'th bit to 0 = IndexType::CELL.
    //
    IndexType typ(b.btype);
    typ.unset(dir);
    return Box(low,hi,typ);
}

Box
adjCellHi (const Box& b,
           int        dir,
           int        len)
{
    boxAssert(len > 0);
    IntVect low(b.smallend);
    IntVect hi(b.bigend);
    unsigned int bitval = b.btype.ixType(dir);
    int bg              = hi[dir]  + 1 - bitval%2;
    low.setVal(dir,bg);
    hi.setVal(dir,bg + len - 1);
    //
    // Set dir'th bit to 0 = IndexType::CELL.
    //
    IndexType typ(b.btype);
    typ.unset(dir);
    return Box(low,hi,typ);
}

Box
adjCell (const Box&         b,
         const Orientation& face,
         int                len)
{
    boxAssert(len > 0);
    IntVect low(b.smallend);
    IntVect hi(b.bigend);
    int dir = face.coordDir();
    if (face.isLow())
    {
        int sm = low[dir];
        low.setVal(dir,sm - len);
        hi.setVal(dir,sm - 1);
    }
    else
    {
        unsigned int bitval = b.btype.ixType(dir);
        int bg              = hi[dir]  + 1 - bitval%2;
        low.setVal(dir,bg);
        hi.setVal(dir,bg + len - 1);
    }
    //
    // Set dir'th bit to 0 = IndexType::CELL.
    //
    IndexType typ(b.btype);
    typ.unset(dir);
    return Box(low,hi,typ);
}
