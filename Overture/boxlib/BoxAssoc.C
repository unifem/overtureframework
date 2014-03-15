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
// $Id: BoxAssoc.C,v 1.4 2008/12/03 17:54:46 chand Exp $
//

#include <BL_Assert.H>
#include <BoxAssoc.H>

const int GROW_SIZE = 10;

BoxAssoc::indexVect::indexVect (int len)
{
    nelem = len;
    vec   = new int[len];
    if (vec == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
}

BoxAssoc::indexVect::indexVect (const indexVect& iv,
                                int              len)
{
    nelem = len;
    vec = new int[nelem];
    if (vec == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
    for (int i = 0; i < len; i++)
        vec[i] = iv.vec[i];
}

void
BoxAssoc::indexVect::copy (const indexVect& iv,
                           int              len)
{
    delete [] vec;
    nelem = len;
    vec = new int[len];
    if (vec == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
    for (int i = 0; i < len; i++)
        vec[i] = iv.vec[i];
}

void
BoxAssoc::indexVect::resize (int newlen)
{
    if (newlen > nelem)
    {
        int* tmp = new int[newlen];
        if (tmp == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        for (int i = 0; i < nelem; i++)
            tmp[i] = vec[i];
        delete [] vec;
        vec = tmp;
        nelem = newlen;
    }
}

BoxAssoc::BoxAssoc ()
    : bs(0),
      current(0),
      first(0),
      bso(0)
{}

BoxAssoc::BoxAssoc (const BoxArray& bxs,
                    int             cachewidth)
    : bs(0),
      current(0),
      first(0),
      bso(0)
{
    define(bxs,cachewidth);
}

BoxAssoc::BoxAssoc (const BoxArray& bxs,
                    const BoxArray& bxso,
                    int             cachewidth)
    : bs(0),
      current(0),
      first(0),
      bso(0)
{
    define(bxs,bxso,cachewidth);
}

BoxAssoc::BoxAssoc (const BoxAssoc& ba)
    : bs(0),
      current(0),
      first(0),
      bso(0)
{
    copy(ba);
}

BoxAssoc::~BoxAssoc ()
{
    clear();
}

BoxAssoc&
BoxAssoc::operator= (const BoxAssoc& ba)
{
    if (this != &ba)
        copy(ba);
    return *this;
}

void
BoxAssoc::copy (const BoxAssoc& ba)
{
    clear();
    boxAssert(ba.ready());
    bs = new BoxArray(ba.boxArray());
    if (bs == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
    bso = (ba.bs == ba.bso) ? bs : new BoxArray(ba.otherBoxArray());
    if (bso == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
    int nbx = bs->length();
    first = current = 0;
    BARec *prev = 0;
    for (BARec *a=ba.first; a != 0; a = a->next)
    {
        BARec* b = new BARec;
        if (b == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        b->cwidth = a->cwidth;
        b->next = 0;
        b->lst = new indexVect[nbx];
        if (b->lst == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        for (int i = 0; i < nbx; i++)
            b->lst[i].copy(a->lst[i],a->lst[i].size());
        if (first == 0)
            first = b;
        else
            prev->next = b;
        prev = b;
        if (ba.current == a)
            current = b;
    }
}

void
BoxAssoc::clear ()
{
    while (first != 0)
    {
       BARec *r = first;
       first = r->next;
       delete [] r->lst;
       delete r;
    }
    current = 0;
    if (bs != bso)
        delete bso;
    delete bs;
}

std::ostream&
operator<< (std::ostream&        os,
            const BoxAssoc& b)
{
    os << "(BoxAssoc\n";
    if (!b.bs)
        os << "BoxAssoc has no BoxArray\n";
    else
    {
        os << "BoxAssoc BoxArray 1:" << '\n';
        int n_bx = b.length();
        for (int i = 0; i < n_bx; i++)
            os << "\t[" << i << "] = " << b.get(i) << '\n';
        if (b.bs != b.bso)
        {
            os << "BoxAssoc BoxArray 2:\n";
            for(int i = 0; i < b.otherLength(); ++i)
                os << "\t[" << i << "]" << b.getOther(i) << '\n';
        }
        else
            os << "BoxAssoc BoxArray 2: Same as BoxArray 1\n";
        BoxAssoc::BARec *r;
        for (r=b.first; r!=0; r = r->next)
        {
            os << "   Cache width = " << r->cwidth << ": \n";

            for (int i = 0; i < n_bx; i++)
            {
                const BoxAssoc::indexVect &iv = r->lst[i];

                for (int j = 0, len = iv.size(); j < len; j++ )
                {
                    int nbor = iv[j];
                    if (b.bs == b.bso && i == nbor)
                        continue;
                    Box tmp(b[i]);
                    tmp.grow(r->cwidth);
                    tmp &= b[nbor];
                    os << '\t' << i << " -->  " << nbor
                       << " : " << tmp << '\n';
                }
                os << '\n';
            }
        }
    }
    os << ")" << std::endl;

    if (os.fail())
        BoxLib::Error("operator<<(ostream&,BoxAssoc&) failed");

    return os;
}

bool
BoxAssoc::ready () const
{
    return  bs ? bs->ready() && (current != 0) && (first != 0) : 0;
}

BoxAssoc::BARec*
BoxAssoc::getCW (int cw) const
{
    BARec *r = first;
    while (r != 0 && (r->cwidth != cw))
        r = r->next;
    return r;
}

void
BoxAssoc::setCacheWidth (int cw)
{
    boxAssert(bs!=0);
    boxAssert(bso != 0);
    BARec *ptr = getCW(cw);
    if (ptr == 0)
    {
        ptr = new BARec;
        if (ptr == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        ptr->cwidth = cw;
        ptr->next = first;
        first = ptr;
        int len = bs->length();
        ptr->lst = new indexVect[len];
        if (ptr->lst == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        makeneighbors(ptr->lst,cw);
    }
    current = ptr;
}

void
BoxAssoc::makeneighbors (indexVect* lst,
                         int        cw)
{
    //
    // Make a temporary indexVect to hold neighbors as we scan over list.
    //
    int nused = 0;
    int cursize = GROW_SIZE;
    indexVect pi(GROW_SIZE);
    Array<Array<int> > aai(length());
    //
    // Scan over boxes.
    //
    for (int i=0; i<length(); i++)
    {
        Box tmp = get(i);
        tmp.grow(cw);
        //
        // Scan over boxes, including self.
        //
        for (int j=0; j< otherLength(); j++ )
        {
            if (tmp.intersects(getOther(j)))
            {
                //
                // Box inter = tmp & (*bso)[j]);
                //
                if (nused == cursize)
                {
                    cursize += GROW_SIZE;
                    pi.resize(cursize);
                }
                pi.vec[nused] = j;
                nused++;
            }
        }
        //
        // Now make a permanent indexVect and store it.
        //
        lst[i].copy(pi,nused);
        aai[i].resize(nused);
        aai[i] = Array<int>(pi.vec, nused);
        nused = 0;
    }
}
