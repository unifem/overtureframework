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
// $Id: BoxDomain.C,v 1.3 2008/12/03 17:54:46 chand Exp $
//

#include <BoxDomain.H>

BoxDomain::BoxDomain ()
    : BoxList(IndexType::TheCellType())
{}

BoxDomain::BoxDomain (IndexType _ctype)
    : BoxList(_ctype)
{}

BoxDomain::BoxDomain (const BoxDomain& rhs)
    : BoxList(rhs)
{}

BoxDomain&
BoxDomain::operator= (const BoxDomain& rhs)
{
    BoxList::operator=(rhs);
    return *this;
}

BoxDomain::~BoxDomain ()
{}

void
BoxDomain::add (const Box& b)
{
    boxAssert(b.ixType() == ixType());

    List<Box> check;
    check.append(b);
    for (ListIterator<Box> bli(lbox); bli; ++bli)
    {
        List<Box> tmp;
        for (ListIterator<Box> ci(check); ci; )
        {
            if (ci().intersects(bli()))
            {
                //
                // Remove c from the check list, compute the
                // part of it that is outside bln and collect
                // those boxes in the tmp list.
                //
                BoxList tmpbl(boxDiff(ci(), bli()));
                tmp.catenate(tmpbl.listBox());
                check.remove(ci);
            }
            else
                ++ci;
        }
        check.catenate(tmp);
    }
    //
    // At this point, the only thing left in the check list
    // are boxes that nowhere intersect boxes in the domain.
    //
    lbox.catenate(check);
    boxAssert(ok());
}

void
BoxDomain::add (const BoxList& bl)
{
    for (BoxListIterator bli(bl); bli; ++bli)
        add(*bli);
}

BoxDomain&
BoxDomain::rmBox (const Box& b)
{
    boxAssert(b.ixType() == ixType());

    List<Box> tmp;

    for (ListIterator<Box> bli(lbox); bli; )
    {
        if (bli().intersects(b))
        {
            BoxList tmpbl(boxDiff(bli(),b));
            tmp.catenate(tmpbl.listBox());
            lbox.remove(bli);
        }
        else
            ++bli;
    }
    lbox.catenate(tmp);
    return *this;
}

bool
BoxDomain::ok () const
{
    //
    // First check to see if boxes are valid.
    //
    bool status = BoxList::ok();
    if (status)
    {
        //
        // Now check to see that boxes are disjoint.
        //
        for (BoxListIterator bli(*this); bli; ++bli)
        {
            BoxListIterator blii(bli); ++blii;
            while (blii)
            {
                if (bli().intersects(blii()))
                {
                    std::cout << "Invalid DOMAIN, boxes overlap" << std::endl;
                    std::cout << "b1 = " << bli() << std::endl;
                    std::cout << "b2 = " << blii() << std::endl;
                    status = false;
                }
                ++blii;
            }
        }
    }
    return status;
}

BoxDomain&
BoxDomain::accrete (int sz)
{
    BoxList bl(*this);
    bl.accrete(sz);
    clear();
    add(bl);
    return *this;
}

BoxDomain&
BoxDomain::coarsen (int ratio)
{
    BoxList bl(*this);
    bl.coarsen(ratio);
    clear();
    add(bl);
    return *this;
}

std::ostream&
operator<< (std::ostream&         os,
            const BoxDomain& bd)
{
    os << "(BoxDomain " << BoxList(bd) << ")" << std::flush;
    if (os.fail())
        BoxLib::Error("operator<<(ostream&,BoxDomain&) failed");
    return os;
}
