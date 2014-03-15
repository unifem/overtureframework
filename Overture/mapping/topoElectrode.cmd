* 
read iges file
  /home/henshaw/Overture/mapping/electrode3.igs
continue
choose some
    0 -1
* *   choose a list
* 0 1 2 3 4 5 6 7 8 9 
* 10 11 12 13 14 15 16 17 18 19 
* 20 21 22 23 24 25 26 27 28 29 
* 30 31 32 33 34 35 36 37 38 39 
* 41 42 43 44 45 
*   done
* ****NOTE: remove surface 40 : very thin ****
delete sub-surfaces 40
* surface 52 is not needed for the interior computation -- this removes a 
* place where three surfaces join as a non-manifold geometry.
delete sub-surfaces 52
  mappingName
    electrode
pause
    determine topology
      merge tolerance .15

      deltaS 5.
*
      build edge curves
      merge edge curves
      triangulate

      exit
    exit
  builder 
    create surface grid... 
     y-r:0 70
     x+r:0 20





*   this one does not get enough points on one end
   14 14 

   12 12 

    0 -1


    12 12


    52 52

   42 42


   0 -1

    52 52


    0 -1

    43 43


   0 -1
  delete sub-surfaces 40
* trouble with surface 40 -- very thin? 
*    40 45 
* choose all
  mappingName
    electrode
    determine topology
      merge tolerance .15
      deltaS 1.


    exit
  unstructured
    build topology
      electrode



