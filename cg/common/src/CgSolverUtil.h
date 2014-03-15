class GenericGraphicsInterface;
class CompositeGrid;
class aString;

aString readOrBuildTheGrid(GenericGraphicsInterface &ps, CompositeGrid &m, bool loadBalance=false,
                        int numberOfParallelGhost=2, int maxWidthExtrapInterpNeighbours=3 );
