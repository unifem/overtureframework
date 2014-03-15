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
// $Id: MultiFab.C,v 1.5 2008/12/03 17:54:46 chand Exp $
//

//kkc 081124 #include <iostream.h>
#include <iostream>

//kkc 081124 #include <iomanip.h>
#include <iomanip>
using namespace std;

#include <BL_Assert.H>
#include <Misc.H>
#include <MultiFab.H>

#if defined(BL_ARCH_IEEE)
#ifdef BL_USE_DOUBLE
    const Real InFINITY=1.0e100;
#elif  BL_USE_FLOAT
    const Real InFINITY=1.0e35;
#endif
#elif defined(BL_ARCH_CRAY)
    const Real InFINITY = 1.0e100;
#endif

MultiFab::MultiFab ()
    : FabArray<Real,FArrayBox>()
{}

MultiFab::MultiFab (const BoxArray& bxs,
                    int             ncomp,
                    int             ngrow,
                    FabAlloc        alloc)
    : FabArray<Real,FArrayBox>(bxs,ncomp,ngrow,alloc)
{}

MultiFab::MultiFab (const BoxAssoc& ba,
                    int             ncomp,
                    int             ngrow,
                    FabAlloc        alloc)
    : FabArray<Real,FArrayBox>(ba,ncomp,ngrow,alloc)
{}

MultiFab::MultiFab (istream& is)
    : FabArray<Real,FArrayBox>()
{
    readFrom(is);
}

MultiFab::~MultiFab()
{}

ostream &
operator<< (ostream&        os,
            const MultiFab& mf)
{
    os << "(MultiFab "
       << mf.length() <<  ' '
       << mf.nGrow()  << '\n';
    for(int i = 0; i < mf.length(); ++i)
        os << mf[i] << '\n';
    os << ")" << flush;

    if (os.fail())
        BoxLib::Error("operator<<(ostream&,MultiFab&) failed");

    return os;
}

ostream &
MultiFab::writeOn (ostream& os) const
{
    boxAssert(bs.ready());

    os << n_comp << '\n';
    os << n_grow << '\n';
    bs.writeOn(os);
    if (bxskel == 0)
        os << 0 <<  '\n';
    else
        os << 1 << " " << bxskel->cacheWidth() << '\n';
    for (int i = 0; i < length(); ++i)
        fs[i].writeOn(os);
    //
    // No need to check 'os' here as it'll be done in FArrayBox::writeOn().
    //
    return os;
}

istream &
MultiFab::readFrom (istream& is)
{
    boxAssert(!bs.ready());

    is >> n_comp;
    while (is.get() != '\n')
        ;
    is >> n_grow;
    while (is.get() != '\n')
        ;
    bs.define(is);
    int has_ba;
    is >> has_ba;
    if (has_ba)
    {
        int cw;
        is >> cw;
        bxskel = new BoxAssoc(bs,cw);
        if (bxskel == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
    }
    while (is.get() != '\n')
        ;
    int nbox = bs.length();
    fs.resize(nbox);
    for (int i = 0; i < nbox; i++)
    {
        FArrayBox* tmp = new FArrayBox;
        if (tmp == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        tmp->readFrom(is);
        fs.set(i,tmp);
    }

    if (is.fail())
        BoxLib::Error("MultiFab::readFrom(istream&) failed");

    return is;
}

void
MultiFab::probe (ostream& os,
                 IntVect& pt)
{
    Real  dat[20];
    int prec = os.precision(14);
    for (int j = 0; j < length(); ++j)
    {
        if (bs[j].contains(pt))
        {
            FArrayBox& fab = fs[j];
            int nv = fab.nComp();

            boxAssert(nv <= 20);

            fab.getVal(dat,pt);
            os << "point " << pt << " in box " << bs[j]
               << " data = ";
            for (int i = 0; i < nv; i++)
                os << "  " << setw(20) << dat[i];
            os << endl;
        }
    }
    os.precision(prec);

    if (os.fail())
        BoxLib::Error("MultiFab::probe(ostream&,IntVect&) failed");
}

Real
MultiFab::min (int comp,
               int nghost) const
{
    boxAssert(nghost >= 0 && nghost <= n_grow);

    Real mn = InFINITY;
    for (int i = 0; i < length(); ++i)
    {
        const FArrayBox& fab = fs[i];
        Box b(grow(bs[i],nghost));
        Real mg = fab.min(b,comp);
        if (i == 0)
            mn = mg;
        mn = Min(mn,mg);
    }
    return mn;
}

Real
MultiFab::min (const Box& region,
               int        comp,
               int        nghost) const
{
    boxAssert(nghost >= 0 && nghost <= n_grow);

    Real mn = InFINITY;
    int first = true;
    for (int i = 0; i < length(); ++i)
    {
        Box b(grow(bs[i],nghost));
        b &= region;
        if (b.ok())
        {
            const FArrayBox& fab = fs[i];
            Real mg = fab.min(b,comp);
            if (first)
            {
                mn = mg;
                first = false;
            }
            mn = Min(mn,mg);
        }
    }
    return mn;
}

Real
MultiFab::max (int comp,
               int nghost) const
{
    boxAssert(nghost >= 0 && nghost <= n_grow);

    Real mn = -InFINITY;
    for (int i = 0; i < length(); ++i)
    {
        const FArrayBox& fab = fs[i];
        Box b(grow(bs[i],nghost));
        Real mg = fab.max(b,comp);
        if (i == 0)
            mn = mg;
        mn = Max(mn,mg);
    }
    return mn;
}

Real
MultiFab::max (const Box& region,
               int        comp,
               int        nghost) const
{
    boxAssert(nghost >= 0 && nghost <= n_grow);

    Real mn = -InFINITY;
    int first = true;
    for (int i = 0; i < length(); ++i)
    {
        Box b(grow(bs[i],nghost));
        b &= region;
        if (b.ok())
        {
            const FArrayBox& fab = fs[i];
            Real mg = fab.max(b,comp);
            if (first)
            {
                mn = mg;
                first = false;
            }
            mn = Max(mn,mg);
        }
    }
    return mn;
}

void
MultiFab::minus (const MultiFab& mf,
                 int             strt_comp,
                 int             num_comp,
                 int             nghost)
{
    boxAssert(bs == mf.bs);
    boxAssert(strt_comp >= 0);
#ifndef NDEBUG
    int lst_comp = strt_comp + num_comp - 1;
#endif
    boxAssert(lst_comp < n_comp && lst_comp < mf.n_comp);
    boxAssert(nghost <= n_grow && nghost <= mf.n_grow);

    for (int i = 0; i < length(); ++i)
    {
        Box bx(bs[i]);
        bx.grow(nghost);
        fs[i].minus(mf.fs[i], bx, strt_comp, strt_comp, num_comp);
    }
}

void
MultiFab::plus (Real val,
                int  comp,
                int  num_comp,
                int  nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        FArrayBox& fab = fs[i];
        Box b(grow(bs[i],nghost));
        fab.plus(val,b,comp,num_comp);
    }
}

void
MultiFab::plus (Real       val,
                const Box& region,
                int        comp,
                int        num_comp,
                int        nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        Box b(grow(bs[i],nghost));
        b &= region;
        if (b.ok())
        {
            FArrayBox& fab = fs[i];
            fab.plus(val,b,comp,num_comp);
        }
    }
}

void
MultiFab::plus (const MultiFab& mf,
                int             strt_comp,
                int             num_comp,
                int             nghost)
{
    boxAssert(bs == mf.bs);
    boxAssert(strt_comp >= 0);
#ifndef NDEBUG
    int lst_comp = strt_comp + num_comp - 1;
#endif
    boxAssert(lst_comp < n_comp && lst_comp < mf.n_comp);
    boxAssert(nghost <= n_grow && nghost <= mf.n_grow);

    for (int i = 0; i < length(); ++i)
    {
        Box bx(bs[i]);
        bx.grow(nghost);
        fs[i].plus(mf.fs[i],bx,strt_comp,strt_comp,num_comp);
    }
}

void
MultiFab::mult (Real val,
                int  comp,
                int  num_comp,
                int  nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        FArrayBox& fab = fs[i];
        Box b(grow(bs[i],nghost));
        fab.mult(val,b,comp,num_comp);
    }
}

void
MultiFab::mult (Real       val,
                const Box& region,
                int        comp,
                int        num_comp,
                int        nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        Box b(grow(bs[i],nghost));
        b &= region;
        if (b.ok())
        {
            FArrayBox& fab = fs[i];
            fab.mult(val,b,comp,num_comp);
        }
    }
}

void
MultiFab::invert (Real numerator,
                  int  comp,
                  int  num_comp,
                  int  nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        FArrayBox& fab = fs[i];
        Box b(grow(bs[i],nghost));
        fab.invert(numerator,b,comp,num_comp);
    }
}

void
MultiFab::invert (Real       numerator,
                  const Box& region,
                  int        comp,
                  int        num_comp,
                  int        nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        Box b(grow(bs[i],nghost));
        b &= region;
        if (b.ok())
        {
            FArrayBox& fab = fs[i];
            fab.invert(numerator,b,comp,num_comp);
        }
    }
}

void
MultiFab::negate (int comp,
                  int num_comp,
                  int nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        FArrayBox& fab = fs[i];
        Box b(grow(bs[i],nghost));
        fab.negate(b,comp,num_comp);
    }
}

void
MultiFab::negate (const Box& region,
                  int        comp,
                  int        num_comp,
                  int        nghost)
{
    boxAssert(nghost >= 0 && nghost <= n_grow);
    boxAssert(comp+num_comp <= n_comp);

    for (int i = 0; i < length(); ++i)
    {
        Box b(grow(bs[i],nghost));
        b &= region;
        if (b.ok())
        {
            FArrayBox& fab = fs[i];
            fab.negate(b,comp,num_comp);
        }
    }
}

void
linInterp (FArrayBox&      dest,
           const Box&      subbox,
           const MultiFab& f1,
           const MultiFab& f2,
           Real            t1,
           Real            t2,
           Real            t,
           bool            extrap)
{
    const Real teps = (t2-t1)/1000.0;

    boxAssert(t>t1-teps && (extrap || t < t2+teps));

    if (t < t1+teps)
        f1.copy(dest,subbox);
    else if (t > t2-teps && t < t2+teps)
        f2.copy(dest,subbox);
    else
    {
        const int       nv  = dest.nComp();
        const BoxArray& bs2 = f2.boxArray();

        boxAssert(f1.boxArray() == bs2);
        boxAssert(nv == f1.n_comp && nv == f2.n_comp);

        const int dc = 0;
        const int sc = 0;

        for (int j = 0; j < bs2.length(); j++)
        {
            //
            // Test all distributed objects.
            //
            if (bs2[j].intersects(subbox))
            {
                //
                // Restrict copy to domain of validity of source.
                //
                Box destbox(bs2[j]);
                destbox &= subbox;

                const FArrayBox& f1fab = f1[j];
                const FArrayBox& f2fab = f2[j];
                dest.linInterp(f1fab,destbox,sc,
                               f2fab,destbox,sc,
                               t1,t2,t,destbox,dc,nv);
            }
        }
    }
}

void
linInterp (FArrayBox&      dest,
           const Box&      subbox,
           const MultiFab& f1,
           const MultiFab& f2,
           Real            t1,
           Real            t2,
           Real            t,
           int             src_comp,
           int             dest_comp,
           int             num_comp,
           bool            extrap)
{
    const Real teps = (t2-t1)/1000.0;

    boxAssert(t>t1-teps && (extrap || t < t2+teps));

    if (t < t1+teps)
        f1.copy(dest,subbox,src_comp,dest_comp,num_comp);
    else if (t > t2-teps && t < t2+teps)
        f2.copy(dest,subbox,src_comp,dest_comp,num_comp);
    else
    {
        const BoxArray& bs2 = f2.boxArray();

        boxAssert(f1.boxArray() == bs2);
        boxAssert(f1.n_comp == f2.n_comp);
        boxAssert(src_comp + num_comp <= f1.n_comp);
        boxAssert(dest_comp + num_comp <= dest.nComp());

        const int dc = dest_comp;
        const int sc = src_comp;

        for (int j = 0; j < bs2.length(); j++)
        {
            //
            // Test all distributed objects.
            //
            if (bs2[j].intersects(subbox))
            {
                //
                // Restrict copy to domain of validity of source.
                //
                Box destbox(bs2[j]);
                destbox &= subbox;

                const FArrayBox& f1fab = f1[j];
                const FArrayBox& f2fab = f2[j];
                dest.linInterp(f1fab,destbox,sc,
                               f2fab,destbox,sc,
                               t1,t2,t,destbox,dc,num_comp);
            }
        }
    }
}

void
linInterp (FArrayBox&      dest,
           const Box&      subbox,
           const BoxAssoc& ba,
           int             ba_index,
           const MultiFab& f1,
           const MultiFab& f2,
           Real            t1,
           Real            t2,
           Real            t,
           bool            extrap)
{
    const Real teps = (t2-t1)/1000.0;

    boxAssert(t>t1-teps && (extrap || t < t2+teps));

    if (t < t1+teps)
        f1.copy(dest,subbox,ba,ba_index);
    else if (t > t2-teps && t < t2+teps)
        f2.copy(dest,subbox,ba,ba_index);
    else
    {
        boxAssert(f1.boxArray() == f2.boxArray());
        boxAssert(ba.boxArray() == f2.boxArray());

        const int nv = dest.nComp();

        boxAssert(nv == f1.n_comp && nv == f2.n_comp);

        const int dc = 0;
        const int sc = 0;

        for (int j = 0; j < ba.nborMax(ba_index); j++)
        {
            const int nbor = ba.nborIndex(ba_index,j);
            //
            // Restrict copy to domain of validity of nbor.
            //
            Box destbox(ba[nbor]);
            destbox &= subbox;
            const FArrayBox& f1fab = f1[nbor];
            const FArrayBox& f2fab = f2[nbor];
            dest.linInterp(f1fab,destbox,sc,
                           f2fab,destbox,sc,
                           t1,t2,t,destbox,dc,nv);
        }
    }
}

void
linInterp (FArrayBox&      dest,
           const Box&      subbox,
           const BoxAssoc& ba,
           int             ba_index,
           const MultiFab& f1,
           const MultiFab& f2,
           Real            t1,
           Real            t2,
           Real            t,
           int             src_comp,
           int             dest_comp,
           int             num_comp,
           bool            extrap)
{
    const Real teps = (t2-t1)/1000.0;

    boxAssert(t > t1-teps && (extrap || (t < t2+teps)));

    if (t < t1+teps)
         f1.copy(dest,subbox,ba,ba_index,src_comp,dest_comp,num_comp);
     else if (t > t2-teps && t < t2 + teps)
         f2.copy(dest,subbox,ba,ba_index,src_comp,dest_comp,num_comp);
     else
    {
        boxAssert(f1.boxArray() == f2.boxArray());
        boxAssert(f1.n_comp == f2.n_comp);
        boxAssert(src_comp + num_comp <= f1.n_comp);
        boxAssert(dest_comp + num_comp <= dest.nComp());

        const int dc = dest_comp;
        const int sc = src_comp;

        for (int j=0; j<ba.nborMax(ba_index); j++)
        {
            const int nbor = ba.nborIndex(ba_index,j);
            //
            // Restrict copy to domain of validity of nbor.
            //
            Box destbox(ba[nbor]);
            destbox &= subbox;
            const FArrayBox& f1fab = f1[nbor];
            const FArrayBox& f2fab = f2[nbor];
            dest.linInterp(f1fab,destbox,sc,
                           f2fab,destbox,sc,
                           t1,t2,t,destbox,dc,num_comp);
        }
    }
}

