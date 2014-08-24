//
//  Ply Polygonal file reader
//

//..sketch: reproduce
//   void OPINGRID(char filename[], char mtype[], int & iunit, int & rdim, int & ddim, int & nnode, int & nelem, int & nemax, const int len_filename, const int len_mtype);
 
//   void CLINGRID(int &iunit);

//   void RDINGRID(int & iunit, char mtype[], int & rdim, int & ddim, int & nnode, int & nelem, int &nemax, real &xyz, int &elements, int &tags, const int len_mtype);

//   void WRINGRID(char filename[], int & iunit, int & rdim, int & ddim, int & nnode, int & nelem, int & nemax, real & xyz, int & elements, int & tags, const int len_filename);

#include <stdio.h>
//kkc 081124 #include <iostream.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "plyFileInterface.h"

using namespace std;

extern "C" {
#include "ply.h"
// sample geometry for debugging "writePlyFile"
//#include "ply_debug_data.h"
}

// PLY datatypes, for the reader
typedef struct PlyVertex {
  float x,y,z;             /* the usual 3-space position of a vertex */
} PlyVertex;

typedef struct PlyFace {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} PlyFace;

PlyFileInterface::
PlyFileInterface()
{
  fileType = ASCII_PLY_FILE;
}

PlyFileInterface::
~PlyFileInterface()
{
  //..use default destructor for now.
}


int PlyFileInterface:: 
openFile(const aString &fileName, 
	 PlyFileReadWriteFlag readWrite /* = READ_PLY_FILE*/ )
{
  //Cast to 'const char*' to get pointer. 
  //.. Cast away 'const' at call to ply_open...
  const char *fname =  (const char*)fileName;

  switch (readWrite)
    {
    case READ_PLY_FILE:
      {
	/* open a PLY file for reading */
	int _fileType;
	ply = ply_open_for_reading((char *)fname, &nelems, &elist, &_fileType, &version);

	fileType = PlyFileType( _fileType ); 
	
	/* print what we found out about the file */
	cout << "  Version:   "<<  version << endl;
	cout << "  File type: ";
	if (fileType == ASCII_PLY_FILE) cout << "ASCII" <<endl;
	else if (fileType == BE_BINARY_PLY_FILE) cout << "BINARY_BE" <<endl;
	else if (fileType == LE_BINARY_PLY_FILE) cout << "BINARY_LE" <<endl;
	else cout << "UNKNOWN"<<endl;

 	for (int i = 0; i < nelems; i++) 
 	{
 	  /* get the description of the first element */
 	  char *elem_name = elist[i];
 	  plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);
 	  /* print the name of the element, for debugging */
 	  if ( (equal_strings ("vertex", elem_name))
 	       ||(equal_strings ("face", elem_name)) ) 
 	    {
 	      printf ("   %i items of type <%s> \n",  num_elems, elem_name);
 	    } else {
 	      printf ("   ERROR: Unknown item type <%s>, cannot proceed! \n",
 		      elem_name);
 	    }
 	}
      };
      break;
    case WRITE_PLY_FILE:
      {
	cout << "ERROR plyFileInterface::openFile "
	     << "-- WRITING not supported!\n";
	ply = NULL;
	//if ( fileType == ASCII_PLY_FILE ) {
	//    ply = ply_open_for_writing(fname, 2, elem_names, 
	//			       PLY_ASCII, &version);
	//} else if (fileType == BE_BINARY_PLY_FILE ) {
	//  ply = ply_open_for_writing(fname, 2, elem_names, 
	//			     BE_BINARY_PLY, &version);
	//}
      };
      break;
    };
  return ( NULL!= ply );
}

void PlyFileInterface::
closeFile()
{
  /* close the PLY file */
  ply_close (ply);

}

void  PlyFileInterface::
readFile(intArray &elems, intArray &tags, realArray &xyz,
	 int &nnode00, int &nelem00, int &ddim00, int &rdim00)
{
  int i,j,k;

  for (i = 0; i < nelems; i++) {
    /* get the description of the first element */
    char *elem_name = elist[i];
    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

    /* print the name of the element, for debugging */
    printf ("reading %i PlyElements of type <%s> \n",  num_elems, elem_name);

    /* if we're on vertex elements, read them in */
    if (equal_strings ("vertex", elem_name)) {

      nnode = num_elems; // number of vertices
      rdim  = 3;         // HARD WIRED TO THREE-D   ***FIX THIS?
      ddim  = 2;         // DOMAIN is always 2D?    ***FIX THIS?
      nnode00=nnode; rdim00=rdim; ddim00=ddim;

      xyz.redim(nnode, rdim);
      xyz = 0.;

      /* create a vertex list to hold all the vertices */
      //vlist = (PlyVertex **) malloc (sizeof (PlyVertex *) * num_elems);
      //vlist =  (PlyVertex **) new (PlyVertex *)[num_elems];
      //vlist =  new PlyVertexPointer[num_elems];

      /* set up for getting vertex elements */
      //PlyProperty vert_props[3];
      /* list of property information for a vertex */
      PlyProperty vert_props[] = {
	{(char*)"x", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,x), 0, 0, 0, 0},
	{(char*)"y", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,y), 0, 0, 0, 0},
	{(char*)"z", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex,z), 0, 0, 0, 0},
      };
      ply_get_property (ply, elem_name, &vert_props[0]);
      ply_get_property (ply, elem_name, &vert_props[1]);
      ply_get_property (ply, elem_name, &vert_props[2]);

      /* grab all the vertex elements */
      PlyVertex vlist; // = new PlyVertex;
      const int xaxis=0, yaxis=1, zaxis=2;
      for (j = 0; j < num_elems; j++) {

        /* grab and element from the file */
	ply_get_element (ply, (void *) &vlist);
	xyz(j, xaxis) = vlist.x;
	xyz(j, yaxis) = vlist.y;
	xyz(j, zaxis) = vlist.z;

        /* print out vertex x,y,z for debugging */
        if (num_elems <100)
        {
	  printf ("vertex: %g %g %g\n", 
		  vlist.x, vlist.y, vlist.z);
	}
      }
    }

    /* if we're on face elements, read them in */
    if (equal_strings ("face", elem_name)) {

      nelem = num_elems; // number of 'faces'=triangles/quads
      nemax = 4;         // HARDWIRED, this isn't good  *******FIX THIS
      elems.redim(nelem, nemax);
      tags.redim(nelem);
      elems = -1;
      tags = 0;
      
      /* list of property information for a vertex */
      PlyProperty face_props[] = { 
	//{"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(PlyFace,intensity), 0, 0, 0, 0},
	{(char*)"vertex_indices", PLY_INT, PLY_INT, offsetof(PlyFace,verts),
	 1, PLY_UCHAR, PLY_UCHAR, offsetof(PlyFace,nverts)},
      };

      ply_get_property (ply, elem_name, &face_props[0]);
      //ply_get_property (ply, elem_name, &face_props[1]);

      /* grab all the face elements */
      PlyFace flist; // = new PlyFace; assert( flist != NULL);
      for (j = 0; j < num_elems; j++) {

        /* grab and element from the file */
        //flist[j] = (PlyFace *) malloc (sizeof (PlyFace));
        ply_get_element (ply, (void *) &flist);
	if ( flist.nverts  >  nemax ) {
	  printf("ERROR  plyFileInterface::readFile: ");
	  printf("face %i has %i nodes, more than nemax=%i.\n",
		 j, flist.nverts,nemax);
	} else {
	  for (k = 0; k< flist.nverts; k++ ) {
	    elems(j,k) = flist.verts[k];
	  }
	}
	    
        /* print out face info, for debugging */
	if (num_elems< 100)
	{
	  printf ("face: %d, list = ", flist.intensity);
	  for (k = 0; k < flist.nverts; k++)
	    printf ("%d ", flist.verts[k]);
	  printf ("\n");
	}
      }
    }
    
    /* print out the properties we got, for debugging */
    for (j = 0; j < nprops; j++)
      printf ("property %s\n", plist[j]->name);
  }

  /* grab and print out the comments in the file */
  comments = ply_get_comments (ply, &num_comments);
  for (i = 0; i < num_comments; i++)
    printf ("comment = '%s'\n", comments[i]);

  /* grab and print out the object information */
  obj_info = ply_get_obj_info (ply, &num_obj_info);
  for (i = 0; i < num_obj_info; i++)
    printf ("obj_info = '%s'\n", obj_info[i]);

}
