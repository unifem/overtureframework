#! /bin/csh -f
#

echo "Copy Ogshow files into the Overture.g/lib and include directory..."

# here is where the original files are:
set OvertureOgshow  = "/home/henshaw/Overture.g/Ogshow"
set OvertureInclude = "/home/henshaw/Overture.g/include"
set OvertureBin     = "/home/henshaw/Overture.g/bin"
set OvertureStatic  = "/home/henshaw/Overture.g/static"
set OvertureExamples = "/home/henshaw/Overture.g/examples"
set OvertureTests    = "/home/henshaw/Overture.g/tests"

# copy these into the static directory
cp {GL_GraphicsInterface.C,mogl.C,overlay.c}                                       $OvertureStatic
cp  xColours.C                                                                     $OvertureStatic


cp {Ogshow,ShowFileReader,NameList,stroke,ParallelUtility}.C                       $OvertureOgshow
cp {GenericGraphicsInterface,GraphicsParameters}.C                                 $OvertureOgshow
cp {plotMapping,label,plotAxes,xInterpolate}.C                                     $OvertureOgshow
cp {PlotStuff,PlotStuffParameters}.C $OvertureOgshow
cp {grid3d,grid,contour,colourTable,streamLines,streamLines3d}.C                   $OvertureOgshow
cp {contour3d.C,osRender.C,render.C,plotPoints.C}                                  $OvertureOgshow
cp cggi.f isosurf.f                                                                $OvertureOgshow

# copy include files into the include directory
cp {GenericGraphicsInterface,GL_GraphicsInterface,GraphicsParameters}.h            $OvertureInclude
cp {Ogshow,ShowFileReader,NameList,PlotStuff,PlotStuffParameters,mogl}.h           $OvertureInclude
cp {ParallelUtility}.{h}                                                           $OvertureInclude

# Here is the plotStuff program for displaying show files
cp {plotStuffDriver,plotStuff}.C                                                   $OvertureBin

cp {ps2ppm}.C                                                                      $OvertureBin

cp readShowFile.C $OvertureExamples

cp {paperplane.c,giMain.C}                                                $OvertureTests

