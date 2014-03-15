#include <Box.H>
#include <BoxList.H>

int main( int argc, char **argv)
{
  IntVect baseVect(D_DECL(0, 0, 0));
  IntVect boundVect(D_DECL(10, 10, 0));
  IndexType iType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
  Box innerBox( baseVect, boundVect, iType); // create a node centered box
  
  for( int i = 0; i < 2; i++) // grow each direction by 2
    innerBox.convert(i, IndexType::CELL);   // convert to cell centered in correct dimensions

  Box outerBox = innerBox;
  
  for(  i = 0; i < 2; i++) // grow each direction by 2
    outerBox.grow(i, 2);

  cout << innerBox << endl;
  cout << outerBox << endl;
  BoxList ghostBoxes;
  
  ghostBoxes = boxDiff(outerBox, innerBox );

  cout << ghostBoxes << endl;
}
