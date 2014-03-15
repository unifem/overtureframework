#include "FaceInfo.h"

// default constructor usually only called before get()
CurveSegment::
CurveSegment()
{
  curveNumber = globalCount++;
  map = NULL;

  startingPoint = -1;
  endingPoint = -1;
  newStartPoint = -1;
  newEndPoint = -1;
  usage = 0; 
  numberOfGridPoints = -1;
  arcLength = -1;

  surfaceNumber = -1;
  surfaceLoop = NULL;
  subCurve = NULL;
}

// constructor
CurveSegment::
CurveSegment(NurbsMapping &newSegment, int sp, int surf, NurbsMapping *sl, NurbsMapping *sc)
{
  curveNumber = globalCount++;

  map = &newSegment;
  map->incrementReferenceCount();

  startingPoint = sp;
  endingPoint = -1;
  newStartPoint = -1;
  newEndPoint = -1;
// usage counts the number of EdgeInfo objects pointing to this CurveSegment. 
// Note that both curve and initialCurve counts, so a boundary curve will have usage == 2, 
// and an unused CurveSegment will have usage == 1.
  usage = 0; 
  numberOfGridPoints = -1;
  arcLength = -1;

  surfaceNumber = surf;
// sl and sc are reference counted!
  surfaceLoop = sl;
  subCurve = sc;
  if (sl) sl->incrementReferenceCount();
  if (sc) sc->incrementReferenceCount();
//    printf("new CurveSegment build out of ID:%i, ID:%i, ID:%i\n", map->getGlobalID(), 
//  	 surfaceLoop->getGlobalID(), subCurve->getGlobalID());
}

CurveSegment::
~CurveSegment()
{
//    printf("CurveSegment destructor called for curve %i, mapID:%i (rc=%i), loopID:%i (rc=%i), scID:%i (rc=%i)\n",
//  	 curveNumber, (map? map->getGlobalID(): 0), (map? map->getReferenceCount(): 0), 
//  	 (surfaceLoop? surfaceLoop->getGlobalID(): 0), (surfaceLoop? surfaceLoop->getReferenceCount(): 0), 
//  	 (subCurve? subCurve->getGlobalID(): 0), (subCurve? subCurve->getReferenceCount(): 0));
  
  int rcnt;
  if (map && (rcnt=map->decrementReferenceCount()) == 0)
    delete map;
//    cout << " Mapping::Destructor called, globalID=" << map->getGlobalID() << endl;
//    else
//      printf("Reference count for 3D curve was %i\n", rcnt);
  
  if (subCurve && subCurve->decrementReferenceCount() == 0)
    delete subCurve;
//    cout << " Mapping::Destructor called, globalID=" << subCurve->getGlobalID() << endl;
  if (surfaceLoop && surfaceLoop->decrementReferenceCount() == 0)
    delete surfaceLoop;
//    cout << " Mapping::Destructor called, globalID=" << surfaceLoop->getGlobalID() << endl;
  
}

int CurveSegment::
put(GenericDataBase & dir, const aString & name, CompositeSurface & cs)
{
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"CompositeTopology");             // create a sub-directory 

  subDir.put(startingPoint,"startingPoint");
  subDir.put(endingPoint,"endingPoint");
  subDir.put(newStartPoint,"newStartPoint");
  subDir.put(newEndPoint,"newEndPoint");
  subDir.put(usage,"usage");
  subDir.put(numberOfGridPoints,"numberOfGridPoints");
  subDir.put(arcLength,"arcLength");
  subDir.put(surfaceNumber,"surfaceNumber");
// curveNumber & globalCount are taken care of by the constructor!
// save map
  map->put(subDir, "map");
// only save subCurve if this CurveSegment doesn't belong to a trimmed mapping
  int isTrimmed= cs[surfaceNumber].getClassName() == "TrimmedMapping";
  subDir.put(isTrimmed,"isTrimmed");
  int loop, scNumber;
  if (isTrimmed)
  {
// loop through the trim curves to find the matching loop number
    TrimmedMapping &trim = (TrimmedMapping &) cs[surfaceNumber];
    for (loop=0; loop < trim.getNumberOfTrimCurves(); loop++)
    {
      if (surfaceLoop == (NurbsMapping *) trim.getTrimCurve(loop)) break;
    }
    assert(loop < trim.getNumberOfTrimCurves());
  }
  else
  {
    loop = 0; // loop number is always zero for untrimmed surfaces
  }
  subDir.put(loop,"loop"); // save the loop number

// now look for the matching sub curve number
  for (scNumber = 0; scNumber < surfaceLoop->numberOfSubCurves(); scNumber++)
  {
    if (subCurve == &surfaceLoop->subCurve(scNumber)) break;
  }
  if( scNumber >= surfaceLoop->numberOfSubCurves() )
  {
    printf("CurveSegment::put:ERROR: subCurve not found in surfaceLoop\n"
           "   surfaceLoop->numberOfSubCurves()=%i\n", surfaceLoop->numberOfSubCurves());
    for (scNumber = 0; scNumber < surfaceLoop->numberOfSubCurves(); scNumber++)
    {
      printf(" subCurve=%i ==? surfaceLoop->subCurve(%i) = %i \n",subCurve,scNumber,surfaceLoop->numberOfSubCurves());
    }
    Overture::abort("CurveSegment::put:ERROR");
    // assert(scNumber < surfaceLoop->numberOfSubCurves());
  }
  
  subDir.put(scNumber,"scNumber"); // save the sub curve number

// the surface loops are stored separately  
  delete &subDir;
  return 0;
}

int CurveSegment::
get(GenericDataBase & dir, const aString & name, CompositeSurface & cs, NurbsMapping **allSurfaceLoops)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"CompositeTopology");

  subDir.get(startingPoint,"startingPoint");
  subDir.get(endingPoint,"endingPoint");
  subDir.get(newStartPoint,"newStartPoint");
  subDir.get(newEndPoint,"newEndPoint");
  subDir.get(usage,"usage");
  subDir.get(numberOfGridPoints,"numberOfGridPoints");
  subDir.get(arcLength,"arcLength");
  subDir.get(surfaceNumber,"surfaceNumber");
// curveNumber & globalCount are taken care of by the constructor!
// save map
  map = new NurbsMapping;
  map->incrementReferenceCount();
  map->get(subDir, "map");
// only save subCurve if this CurveSegment doesn't belong to a trimmed mapping
  int isTrimmed= cs[surfaceNumber].getClassName() == "TrimmedMapping";
  subDir.get(isTrimmed,"isTrimmed");
  int loop, scNumber;
  subDir.get(loop,"loop");
  subDir.get(scNumber,"scNumber");
  if (isTrimmed)
  {
    TrimmedMapping &trim = (TrimmedMapping &) cs[surfaceNumber];
// copy the pointers from the trimmed surface
    surfaceLoop = (NurbsMapping *) trim.getTrimCurve(loop);
    surfaceLoop->incrementReferenceCount();
  }
  else
  {
    surfaceLoop = allSurfaceLoops[surfaceNumber];
    surfaceLoop->incrementReferenceCount();
  }
  subCurve = &surfaceLoop->subCurve(scNumber);
  subCurve->incrementReferenceCount();
  
  delete &subDir;
  return 0;
}


// default contructor should only be called before calling get()
EdgeInfo::
EdgeInfo()
{
  next = prev = NULL;
  slave = master = NULL;
  loopy = NULL;
  
  orientation = 0;
  loopNumber = 0;
  faceNumber = 0;
  edgeNumber = 0;
    
  curve = NULL;
  initialCurve = NULL;

  status = edgeCurveIsBoundary;

  dList = 0;

  startLastChangedBy = -1; // never been touched
  endLastChangedBy = -1; // never been touched

  prevNumber = nextNumber = slaveNumber = masterNumber = -1;

//  printf("EdgeInfo default CONSTRUCTOR called\n");
}

EdgeInfo::
EdgeInfo(CurveSegment *newCurve, int l, int f, int o, int e)
{
  next = prev = NULL;
  slave = master = NULL;
  loopy = NULL;
  
  orientation = o;
  loopNumber = l;
  faceNumber = f;
  edgeNumber = e;
    
  curve = newCurve;
  curve->usage++; // increment usage

  initialCurve = curve;
  initialCurve->usage++; // increment usage

  status = edgeCurveIsBoundary;

  dList = 0;

  startLastChangedBy = -1; // never been touched
  endLastChangedBy = -1; // never been touched

  prevNumber = nextNumber = slaveNumber = masterNumber = -1;

//    printf("EdgeInfo CONSTRUCTOR called edgeNumber=%i, curve->mapID=%i, curve->loopID=%i, curve->scID=%i\n",
//  	 edgeNumber, curve->getNURBS()->getGlobalID(), curve->surfaceLoop->getGlobalID(), 
//  	 curve->subCurve->getGlobalID());
    
}

EdgeInfo::
~EdgeInfo()
{
  bool sameCurve = curve==initialCurve;
//    printf("EdgeInfo destructor called edgeNumber=%i, status=%i, sameCurve=%i, curve->usage=%i, initialCurve->usage=%i\n",
//    	 edgeNumber, status, sameCurve, curve->usage, initialCurve->usage);
  if (sameCurve)
    assert(curve && curve->usage >= 2);
  else
  {
    assert(curve && curve->usage > 0);
    assert(initialCurve && initialCurve->usage > 0);
  }
  
  
  if (curve && --(curve->usage) == 0)
  {
    delete curve;
  }

  if (initialCurve && --(initialCurve->usage) == 0)
  {
    delete initialCurve;
  }

}

int EdgeInfo::
getStartPoint()
{
  if (orientation == 1)
  {
    if (curve->newStartPoint >= 0)
      return curve->newStartPoint;
    else
      return curve->startingPoint;
  }
  else
  {
    if (curve->newEndPoint >= 0)
      return curve->newEndPoint;
    else
      return curve->endingPoint;
  }
  
}

int EdgeInfo::
getEndPoint()
{
  if (orientation == 1)
  {
    if (curve->newEndPoint >= 0)
      return curve->newEndPoint;
    else
      return curve->endingPoint;
  }
  else
  {
    if (curve->newStartPoint >= 0)
      return curve->newStartPoint;
    else
      return curve->startingPoint;
  }
}

bool EdgeInfo::
setStartPoint(int np, realArray & endPoint, real mergeTolerance, int firstEdgeNumber, EdgeInfoArray &masterEdge,
	      EdgeInfoArray &unusedEdges)
{
  if (startLastChangedBy == firstEdgeNumber)
  {
    printf("Exiting setStartPoint for edge %i, since it already has been called from edge %i\n", 
	   edgeNumber, firstEdgeNumber);
    return true;
  }
  else
    startLastChangedBy = firstEdgeNumber;
  
  if (orientation == 1)
  {
    curve->newStartPoint = np;
//    printf("setStartPoint: Edge %i Setting newStartPoint %i\n", edgeNumber, np);
  }
  else
  {
    curve->newEndPoint = np;
//    printf("setStartPoint: Edge %i Setting newEndPoint %i\n", edgeNumber, np);
  }
// the edge should be removed from its loop if newStartPoint == newEndPoint, 
// but this is only safe for boundaryCurves
  int sp, ep;
  sp = (curve->newStartPoint >=0)? curve->newStartPoint : curve->startingPoint;
  ep = (curve->newEndPoint >=0)? curve->newEndPoint : curve->endingPoint;
  if (sp == ep)
  {
    if (status == EdgeInfo::edgeCurveIsBoundary)
    {
      printf("setStartPoint: REMOVING ZERO LENGTH BOUNDARY EDGE %i\n", edgeNumber);
      loopy->removeEdge(this); // remove this EdgeInfo object from the Loop
//      status = EdgeInfo::edgeCurveIsNotUsed;
      setUnused(unusedEdges);
// should also pop this edge on the unusedEdges stack

      printf("Changing masterEdge[sp]->edgeNumber from %i to %i\n", masterEdge.array[sp]->edgeNumber, next->edgeNumber);
// use next segment as the master instead
      masterEdge.array[sp] = next;
    }
    else
    {
      printf("setStartPoint: ERROR: ENCOUNTERED NON-BOUNDARY ZERO LENGTH EDGE %i, status: %i\n", 
	     edgeNumber, status);
      return false;
    }
  }
  else // adjust the end points of the edge
  {
    if (!adjustOneSegmentEndPoints(endPoint, mergeTolerance))
      return false;
  }
  
  
// what happens if the slave->prev or master->prev gets zero length?
  if (slave && slave->edgeNumber != firstEdgeNumber)
  {
// to avoid infinite recursion, we need to break the calling chain if firstEdgeNumber == edgeNumber
    if (orientation*slave->orientation > 0)
    {
      if (!slave->prev->setEndPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
    else
    {
      if (!slave->next->setStartPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
    
  }
  if (master && master->edgeNumber != firstEdgeNumber)
  {
    if (orientation*master->orientation > 0)
    {
      if (!master->prev->setEndPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
    else
    {
      if (!master->next->setStartPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
    
  }

  return true; 
}

bool EdgeInfo::
setEndPoint(int np, realArray & endPoint, real mergeTolerance, int firstEdgeNumber, EdgeInfoArray &masterEdge,
	    EdgeInfoArray &unusedEdges)
{
  if (endLastChangedBy == firstEdgeNumber)
  {
    printf("Exiting setEndPoint for edge %i, since it already has been called from edge %i\n", 
	   edgeNumber, firstEdgeNumber);
    return true;
  }
  else
    endLastChangedBy = firstEdgeNumber;

  if (orientation == 1)
  {
    curve->newEndPoint = np;
//    printf("setEndPoint: Edge %i Setting newEndPoint %i\n", edgeNumber, np);
  }
  else
  {
    curve->newStartPoint = np;
//    printf("setEndPoint: Edge %i Setting newStartPoint %i\n", edgeNumber, np);
  }
// the edge should be removed from its loop if the newStartPoint == newEndPoint
  int sp, ep;
  sp = (curve->newStartPoint >=0)? curve->newStartPoint : curve->startingPoint;
  ep = (curve->newEndPoint >=0)? curve->newEndPoint : curve->endingPoint;
  if( sp == ep )
  {
    if (status == EdgeInfo::edgeCurveIsBoundary)
    {
      printf("setEndPoint: REMOVING ZERO LENGTH BOUNDARY EDGE %i\n", edgeNumber);
      loopy->removeEdge(this); // remove this EdgeInfo object from the Loop
//      status = EdgeInfo::edgeCurveIsNotUsed;
      setUnused(unusedEdges);
// should also pop this edge on the unusedEdges stack

// also need to update masterEdge[sp], which should point to some neighboring edge that is defined
      printf("Changing masterEdge[sp]->edgeNumber from %i to %i\n", masterEdge.array[sp]->edgeNumber, next->edgeNumber);
// use next segment as the master instead
      masterEdge.array[sp] = next;
    }
    else
    {
      printf("setEndPoint: ERROR: ENCOUNTERED NON-BOUNDARY ZERO LENGTH EDGE %i, status: %i\n", 
	     edgeNumber, status);
      return false;
    }
  }
  else // adjust the curve
  {
    if (!adjustOneSegmentEndPoints(endPoint, mergeTolerance))
      return false;
  }
 
// what happens if the slave->prev or master->prev gets zero length?
  if (slave && slave->edgeNumber != firstEdgeNumber)
  {
    if (orientation*slave->orientation > 0)
    {
      if (!slave->next->setStartPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
    else
    {
      if (!slave->prev->setEndPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
  }
  if (master && master->edgeNumber != firstEdgeNumber)
  {
    if (orientation*master->orientation > 0)
    {
      if (!master->next->setStartPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
    else
    {
      if (!master->prev->setEndPoint(np, endPoint, mergeTolerance, firstEdgeNumber, masterEdge, unusedEdges))
	return false;
    }
  }
  return true; 
}

bool EdgeInfo::
adjustOneSegmentEndPoints(realArray & endPoint, real mergeTolerance)
{
  RealArray newLocation(3);
  real dist;
  int q;
  bool retCode=true;
  
  if( curve->newStartPoint >= 0 )
  {
//      printf("Changing starting point for edge %i, old point=%i, new point=%i\n", edgeNumber, 
//  	   curve->startingPoint, curve->newStartPoint);
    dist = 0.;
    if (curve->startingPoint >=0)
    {
// check the distance
      for (q=0; q<3; q++)
	dist += SQR(endPoint(curve->startingPoint,q) - endPoint(curve->newStartPoint,q));
      dist = sqrt(dist);
    }
    else
    {
      printf("Initial starting point invalid. Just moving the starting point to: (%e,%e,%e)\n",
	     endPoint(curve->newStartPoint,0), endPoint(curve->newStartPoint,1), 
	     endPoint(curve->newStartPoint,2));
    }

// make the actual change...
    if (dist <= 2*mergeTolerance)
    {
      for (q=0; q<3; q++)
	newLocation(q) = endPoint(curve->newStartPoint,q);
	  
      curve->getNURBS()->moveEndpoint(0, newLocation);
      curve->startingPoint = curve->newStartPoint;
      curve->newStartPoint = -1;
// update graphics
      eraseEdge();
    }
    else
    {
      printf("Attempting to move startpoint %i to %i by %e, which exceeds twice the merge tolerance %e\n", 
	     curve->startingPoint, curve->newStartPoint, dist, mergeTolerance);
	    
      printf("Initial startPoint %i:(%e,%e,%e), new %i:(%e,%e,%e)\n", 
	     curve->startingPoint, endPoint(curve->startingPoint,0), endPoint(curve->startingPoint,1), 
	     endPoint(curve->startingPoint,2),
	     curve->newStartPoint, endPoint(curve->newStartPoint,0), endPoint(curve->newStartPoint,1), 
	     endPoint(curve->newStartPoint,2));
      retCode = false;
    }
    
  } // end if newStartPoint >= 0
  
	  
  if( curve->newEndPoint >=0 )
  {
//      printf("Changing ending point for edge %i, old point=%i, new point=%i\n", edgeNumber, 
//  	   curve->endingPoint, curve->newEndPoint);
    dist = 0.;
    if (curve->endingPoint >=0)
    {
// check the distance
      for (q=0; q<3; q++)
	dist += SQR(endPoint(curve->endingPoint,q) - endPoint(curve->newEndPoint,q));
      dist = sqrt(dist);
    }
    else
    {
      printf("Initial ending point invalid. Just moving the ending point to: (%e,%e,%e)\n",
	     endPoint(curve->newEndPoint,0), endPoint(curve->newEndPoint,1), 
	     endPoint(curve->newEndPoint,2));
    }
	  
// make the actual change...
    if (dist <= 2*mergeTolerance)
    {
      for (q=0; q<3; q++)
	newLocation(q) = endPoint(curve->newEndPoint,q);
	  
      curve->getNURBS()->moveEndpoint(1, newLocation);
      curve->endingPoint = curve->newEndPoint;
      curve->newEndPoint = -1;

// update graphics
      eraseEdge();
    }
    else
    {
      printf("Attempting to move endpoint %i to %i by %e, which exceeds twice the merge tolerance %e\n", 
	     curve->endingPoint, curve->newEndPoint, dist, mergeTolerance);
	    
      printf("Initial endPoint %i:(%e,%e,%e), new %i:(%e,%e,%e)\n", 
	     curve->endingPoint, endPoint(curve->endingPoint,0), endPoint(curve->endingPoint,1), 
	     endPoint(curve->endingPoint,2),
	     curve->newEndPoint, endPoint(curve->newEndPoint,0), endPoint(curve->newEndPoint,1), 
	     endPoint(curve->newEndPoint,2));
      retCode = false;
    }
  } // end if newEndPoint >= 0
  return retCode;
} // end adjust one segment endpoint


void EdgeInfo::
eraseEdge()
{
  GenericGraphicsInterface * gi = Overture::getGraphicsInterface();
  
  if (dList>0)
    gi->deleteList(dList);
  dList = 0;

  if (slave)
  {
    if ( slave->dList>0 )
      gi->deleteList(slave->dList);
    slave->dList = 0;
  }

  if (master)
  {
    if ( master->dList>0 )
      gi->deleteList(master->dList);
    master->dList = 0;
  }
  
} // end eraseEdge

int EdgeInfo::
masterEdgeNumber()
{
  EdgeInfo *e=this;
// For non-manifold geometries, an edge could be the slave of a master that is the slave of another master
  while (e->master != NULL)
    e = e->master;
  return e->edgeNumber;
}

void EdgeInfo::
setUnused(EdgeInfoArray &unusedEdges)
{
  status = EdgeInfo::edgeCurveIsNotUsed;
  eraseEdge();
// the subcurve does no longer exist in loop tc so we no longer needs to refer to these curves
  if (curve->subCurve && curve->subCurve->decrementReferenceCount() == 0)
    delete curve->subCurve;
  curve->subCurve = NULL;

  if (curve->surfaceLoop && curve->surfaceLoop->decrementReferenceCount() == 0)
    delete curve->surfaceLoop;
  curve->surfaceLoop = NULL;

// push this onto the stack of unused edges
  unusedEdges.push(*this);
}

int EdgeInfo::
put(GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"CompositeTopology");             // create a sub-directory 

  subDir.put(next->edgeNumber,"nextNumber");
  subDir.put(prev->edgeNumber,"prevNumber");
  subDir.put((slave? slave->edgeNumber:-1),"slaveNumber");
  subDir.put((master? master->edgeNumber:-1),"masterNumber");
// we don't have to save loopy since we have the face number and the loop number
  subDir.put(orientation,"orientation");
  subDir.put(loopNumber,"loopNumber");
  subDir.put(faceNumber,"faceNumber");
  subDir.put(edgeNumber,"edgeNumber");
// makes no sense to save the display list number (dList)
  subDir.put(startLastChangedBy,"startLastChangedBy");
  subDir.put(endLastChangedBy,"endLastChangedBy");
  subDir.put((curve? curve->getCurveNumber(): -1), "curveNumber");
  subDir.put(initialCurve->getCurveNumber(), "initialCurveNumber");
  subDir.put((int &) status,"status");
    
  delete &subDir;
  return 0;
}

int EdgeInfo::
get(GenericDataBase & dir, const aString & name, CurveSegment * allCurveSegments[]) 
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"CompositeTopology");

// we must fill in the pointers after all EdgeInfo's have been read!
  subDir.get(nextNumber,"nextNumber");
  subDir.get(prevNumber,"prevNumber");
  subDir.get(slaveNumber,"slaveNumber");
  subDir.get(masterNumber,"masterNumber");
// we can restore loopy from the face number and the loop number
  subDir.get(orientation,"orientation");
  subDir.get(loopNumber,"loopNumber");
  subDir.get(faceNumber,"faceNumber");
  subDir.get(edgeNumber,"edgeNumber");
// makes no sense to read the display list number (dList)
  subDir.get(startLastChangedBy,"startLastChangedBy");
  subDir.get(endLastChangedBy,"endLastChangedBy");
  int curveNumber, initialCurveNumber;
  subDir.get(curveNumber, "curveNumber");
  subDir.get(initialCurveNumber, "initialCurveNumber");
// assign the curve and initialCurve pointers
  curve = (curveNumber != -1? allCurveSegments[curveNumber]:NULL);
  initialCurve = allCurveSegments[initialCurveNumber];
  
  subDir.get((int &) status,"status");
  
  delete &subDir;
  return 0;
}

void EdgeInfo::
assignPointers(EdgeInfo * allEdgeInfos[])
{
// all objects must have prev and next pointers
  assert(prevNumber>=0 && allEdgeInfos[prevNumber]);
  assert(nextNumber>=0 && allEdgeInfos[nextNumber]);
  prev = allEdgeInfos[prevNumber];
  next = allEdgeInfos[nextNumber];

  if (slaveNumber >= 0)
  {
    assert(allEdgeInfos[slaveNumber] != NULL);
    slave = allEdgeInfos[slaveNumber];
  }

  if (masterNumber >= 0)
  {
    assert(allEdgeInfos[masterNumber] != NULL);
    master = allEdgeInfos[masterNumber];
  }
}



EdgeInfoArray::
EdgeInfoArray()
{
  nMax = 0;
  array = NULL;
  sp = 0; // stack pointer
}

EdgeInfoArray::
~EdgeInfoArray()
{
// the EdgeInfo's are deleted elsewhere
//    for (int i=0; i<nMax; i++)
//      delete array[i];
  nMax = 0;
  delete [] array; // *wdh* 030825 added: [] 
}

void EdgeInfoArray::
resize(int size)
{
  if (size == nMax) 
    return;
  EdgeInfo **newArray = new EdgeInfo * [size];
  int i;
  if (size > nMax)
  {
    for (i=0; i<nMax; i++)
      newArray[i] = array[i];
    for (i=nMax; i<size; i++)
      newArray[i] = NULL;
  }
  else // size < nMax
  {
    for (i=0; i<size; i++)
      newArray[i] = array[i];
  }
  delete [] array;    // *wdh* 030825 added: [] 
  array = newArray;
  nMax=size;
}

void EdgeInfoArray::
push(EdgeInfo & e)
{
// automatically increase the size the array if we are running out of space...
  if (nMax <= sp)
    resize(nMax+100);
  
  array[sp++] = &e;
}

EdgeInfo* EdgeInfoArray::
pop()
{
  if (sp <= 0)
    return NULL;
  else
    return array[--sp];
}

int EdgeInfoArray::
put(GenericDataBase & dir, const aString & name) const
{
  aString buf;
  
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"CompositeTopology");             // create a sub-directory 

  subDir.put(nMax,"nMax");
  subDir.put(sp,"sp");
  for (int i=0; i<nMax; i++)
  {
    subDir.put((array[i]? array[i]->edgeNumber:-1), sPrintF(buf,"array-%i",i));
  }

  delete &subDir;
  return 0;
}

int EdgeInfoArray::
get(GenericDataBase & dir, const aString & name, EdgeInfo * allEdgeInfos[])
{
  aString buf;
  int edgeNumber, arraySize;
  
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.find(subDir,name,"CompositeTopology");             // create a sub-directory 

  subDir.get(arraySize,"nMax");
// allocate the array
  resize(arraySize); // sets nMax = arraySize
  subDir.get(sp,"sp");
  for (int i=0; i<nMax; i++)
  {
    subDir.get(edgeNumber, sPrintF(buf,"array-%i",i));
    array[i] = (edgeNumber==-1? NULL: allEdgeInfos[edgeNumber]);
  }

  delete &subDir;
  return 0;
}


Loop::
Loop()
{
  firstEdge = lastEdge = NULL;
  trimOrientation = 0; // should get copied from the trimming curve (or outer boundary for non-trimmed mappings)
}

Loop::
~Loop()
{
// delete all edges in this loop
//  printf("Loop destructor called, numberOfEdges:%i\n", numberOfEdges());
  int numberLeft = numberOfEdges();
  EdgeInfo *e=firstEdge, *nextVictim;
  while (numberLeft>0)
  {
    nextVictim = e->next;
    if (e->status == EdgeInfo::edgeCurveIsNotUsed)
      printf("WARNING: Attempting to delete unused edge #%i in the Loop destructor\n", e->edgeNumber);
    else
      delete e;
    e = nextVictim;
    numberLeft--;
  }
}

void Loop::
insertEdge(EdgeInfo * newEdge)
{
// add an edge to the end of the edge list
  if (lastEdge == NULL)
  {
    lastEdge  = newEdge;
    firstEdge = newEdge;
  }
  else
  {
    lastEdge->next = newEdge;
    newEdge->prev = lastEdge;
    lastEdge = newEdge;
  }
// make the list circular
  firstEdge->prev = lastEdge;
  lastEdge->next  = firstEdge;

// store loop pointer
  newEdge->loopy = this;
}

bool Loop::
addEdge(EdgeInfo * newEdge, EdgeInfo * loc)
{
// add newEdge after loc
  if (!newEdge || !loc) return false;
  
  int ne = numberOfEdges();
  int e;
  EdgeInfo *ei;
  
// check that loc really is in the loop
  for (e=0, ei = firstEdge; e<ne; e++, ei=ei->next)
    if (ei == loc) break;
  if (ei != loc) return false;
  
// now insert it!
  if (ei == lastEdge)
    lastEdge = newEdge;

  newEdge->next = ei->next;
  newEdge->prev = ei;
  ei->next->prev = newEdge;
  ei->next = newEdge;

// update the ending point information
// assign newEndPoint too, to make sure it will get updated
  newEdge->curve->endingPoint = newEdge->curve->newEndPoint = newEdge->next->getStartPoint();
  newEdge->curve->startingPoint = loc->curve->endingPoint;

// store loop pointer
  newEdge->loopy = this;

  return true;
}

bool Loop::
edgeInLoop(EdgeInfo * oldEdge)
{
// check if oldEdge is in the loop
  int e, ne = numberOfEdges();
  EdgeInfo *ei;

  for (e=0, ei = firstEdge; e<ne; e++, ei=ei->next)
    if (ei == oldEdge) break;
  if (ei != oldEdge) 
    return false;
  else
    return true;
}


bool Loop::
replaceEdge(EdgeInfo *newEdge, EdgeInfo *oldEdge)
{
  if (newEdge == NULL || oldEdge == NULL)
    return false;

  int ne = numberOfEdges();
  int e;
  EdgeInfo *ei;
  
// check that oldEdge really is in the loop
  for (e=0, ei = firstEdge; e<ne; e++, ei=ei->next)
    if (ei == oldEdge) break;
  if (ei != oldEdge) return false;

// now replace it
  newEdge->prev = oldEdge->prev;
  newEdge->next = oldEdge->next;
  newEdge->prev->next = newEdge;
  newEdge->next->prev = newEdge;
  if (firstEdge == oldEdge)
    firstEdge = newEdge;
  if (lastEdge == oldEdge)
    lastEdge = newEdge;

// store loop pointer
  newEdge->loopy = this;

  return true;
}


bool Loop::
deleteEdge(EdgeInfo * oldEdge)
{
  if (removeEdge( oldEdge ))
  {
    delete oldEdge;
    return true;
  }
  else
    return false;
  
}

// remove an edge from the list
bool Loop::
removeEdge(EdgeInfo * oldEdge)
{
  if (oldEdge == NULL || firstEdge == NULL)
    return false;
  
// make sure oldEdge is in this loop
  EdgeInfo *edge=firstEdge;
  int i;
  bool found = false;
  
  edge=firstEdge;
  do
  {
    if (oldEdge == edge)
    {
      found = true;
      break;
    }
    edge = edge->next;
  } while (edge != firstEdge);
  
  if (!found)
  {
    printf("Loop::removeEdge(): attempting to delete an edge that is not in the list!\n");
    return false;
  }

  EdgeInfo *oldPrev=NULL, *oldNext;
// check if oldEdge is the last element
  if (firstEdge == lastEdge)
  {
    firstEdge = lastEdge = NULL;
  }
  else
  {
    if (firstEdge == oldEdge)
      firstEdge = oldEdge->next;
    if (lastEdge == oldEdge)
      lastEdge = oldEdge->prev;
  }
  
// exclude oldEdge from the circular list
  oldPrev = oldEdge->prev;
  oldNext = oldEdge->next;
  if (oldPrev)
    oldPrev->next = oldNext;
  if (oldNext)
    oldNext->prev = oldPrev;

  oldEdge->loopy = NULL; // no longer part of this loop
  
  return true;
}

int Loop:: 
numberOfEdges()
{
  int n=0;
  if (firstEdge == NULL)
    return 0;
  else if (firstEdge == lastEdge)
    return 1;
  else
  {
    EdgeInfo *first, *e;
    n = 0; first = firstEdge; e = firstEdge;
    do
    {
      n++;
      e = e->next;
    } while (e != firstEdge);
  }

  return n;
}

void Loop::
assignEndPointNumbers()
{
  int i, nE=numberOfEdges();
  EdgeInfo *edge;
  for (i=0, edge = firstEdge; i<nE; i++, edge = edge->next)
  {
    edge->curve->endingPoint = edge->next->curve->startingPoint;
  }
}

int Loop::
put(GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"CompositeTopology");             // create a sub-directory 

  subDir.put(trimOrientation,"trimOrientation");
  subDir.put(firstEdge->edgeNumber,"firstEdgeNumber");
  subDir.put(lastEdge->edgeNumber,"lastEdgeNumber");

  delete &subDir;
  return 0;
}

int Loop::
get(GenericDataBase & dir, const aString & name, EdgeInfo * allEdgeInfos[])
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"CompositeTopology");

  subDir.get(trimOrientation,"trimOrientation");
  int firstEdgeNumber, lastEdgeNumber;
  subDir.get(firstEdgeNumber,"firstEdgeNumber");
  subDir.get(lastEdgeNumber,"lastEdgeNumber");
// assign pointers
  firstEdge = allEdgeInfos[firstEdgeNumber];
  lastEdge = allEdgeInfos[lastEdgeNumber];

// assign the loopy pointer in all edgeInfo objects in this loop
  EdgeInfo *e;
  int sc, ne = numberOfEdges();
  for (sc=0, e=firstEdge; sc<ne; sc++, e=e->next)
  {
    e->loopy = this;
  }
  
  delete &subDir;
  return 0;
}



FaceInfo::
FaceInfo()
{
  numberOfLoops=0;
  loop = NULL;
}

void FaceInfo::
allocateLoops(int nol)
{
// remove any existing edges
  if (loop)
    delete [] loop;
  
  if (nol>0)
  {
    numberOfLoops=nol;
    loop = new Loop[nol];
  }
  else
  {
    numberOfLoops=0;
    loop = NULL;
  }
   
}

FaceInfo::
~FaceInfo()
{
//  printf("FaceInfo destructor called\n");
  if (loop)
    delete [] loop;
}

int FaceInfo::
put(GenericDataBase & dir, const aString & name)
{
  aString buf;
  
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"CompositeTopology");             // create a sub-directory 

  subDir.put(numberOfLoops,"numberOfLoops");
  for (int i=0; i<numberOfLoops; i++)
    loop[i].put(subDir, sPrintF(buf,"Loop-%i", i));

  delete &subDir;
  return 0;
}

int FaceInfo::
get(GenericDataBase & dir, const aString & name, EdgeInfo * allEdgeInfos[])
{
  aString buf;

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"CompositeTopology");

  subDir.get(numberOfLoops,"numberOfLoops");
  allocateLoops(numberOfLoops);
  for (int i=0; i<numberOfLoops; i++)
    loop[i].get(subDir, sPrintF(buf,"Loop-%i", i), allEdgeInfos);

  delete &subDir;
  return 0;
}

