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
// $Id: IntVect.C,v 1.3 2008/12/03 17:54:46 chand Exp $
//

#include <stdlib.h>

#include <BL_Assert.H>
#include <BoxLib.H>
#include <Misc.H>
#include <IntVect.H>
#include <IndexType.H>
#include <Utility.H>

using namespace std;

const IntVect&
IntVect::TheUnitVector ()
{
    static const IntVect Unit(D_DECL(1,1,1));
    return Unit;
}

const IntVect&
IntVect::TheZeroVector ()
{
    static const IntVect Zero(D_DECL(0,0,0));
    return Zero;
}

const IntVect&
IntVect::TheNodeVector ()
{
    static const IntVect Node(D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE));
    return Node;
}

const IntVect&
IntVect::TheCellVector ()
{
    static const IntVect Cell(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));
    return Cell;
}

//
// Returns IntVect which is the componentwise integer projection
// of IntVect p1 by IntVect p2.
//

ostream&
operator<< (ostream&       os,
            const IntVect& p)
{
    os << D_TERM( '(' << p[0] , <<
                  ',' << p[1] , <<
                  ',' << p[2])  << ')';
    if (os.fail())
        BoxLib::Error("operator<<(ostream&,IntVect&) failed");
    return os;
}

istream&
operator>> (istream& is,
            IntVect& p)
{
  // *wdh* is >> ws;
    char c;
    is >> c;
    is.putback(c);
    if (c == '(')
    {
        D_EXPR(is.ignore(BL_IGNORE_MAX, '(') >> p[0],
               is.ignore(BL_IGNORE_MAX, ',') >> p[1],
               is.ignore(BL_IGNORE_MAX, ',') >> p[2]);
        is.ignore(BL_IGNORE_MAX, ')');
    }
    else if (c == '<')
    {
        D_EXPR(is.ignore(BL_IGNORE_MAX, '<') >> p[0],
               is.ignore(BL_IGNORE_MAX, ',') >> p[1],
               is.ignore(BL_IGNORE_MAX, ',') >> p[2]);
        is.ignore(BL_IGNORE_MAX, '>');
    }
    else
        BoxLib::Error("operator>>(istream&,IntVect&): expected \'(\' or \'<\'");

    if (is.fail())
        BoxLib::Error("operator>>(ostream&,IntVect&) failed");

    return is;
}

void
IntVect::printOn (ostream& os) const
{
    os << "IntVect: " << *this << '\n';
}

void
IntVect::dumpOn (ostream& os) const
{
    os << "IntVect(" << BoxLib::version << ")= " << *this << '\n';
}
