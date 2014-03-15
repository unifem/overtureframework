    if( !sameDimensions )
      if( newSize!=oldSize )
      {
	cout << "updateToMatchGrid: new size! " << endl;
        resize(R[0],R[1],R[2],R[3]);   // this breaks the reference
      }
      else
      {
	cout << "updateToMatchGrid: same size, different shape! " << endl;
        reshape(R[0],R[1],R[2],R[3]);  // this does not break a reference
      }
    else
    {
      cout << "updateToMatchGrid: same Dimensions! nothing changed " << endl;
    }
