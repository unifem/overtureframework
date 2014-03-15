*
  read iges file
    asmo.igs
    continue
  choose some
     0 -1
*  choose a list
*    26 93 
*    24 26 92 93 
*  done
*   double surface: 231==232, 159==160, 21==91 -- there may be more  
*    231 232 
*
*     0 1 2 3 11 12
* here is the front:
*     29 30 31 32 33 34 35 36 37 38 39 41 42 43 44 
*     45 46 47 48 49 50 51 52 56 57 58 59 60 61 62 
*     63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 
*     78 79 80 81 82 83 84 85 86 87 245 246 247 248 
* here is the back
*   8 9 10 21 22 23 24 25 26 27 
*   28 40 91 92 93 94 95 96 97 98 
* here is the rear wheel ** needs tol=.6
*   choose a list
*     5 17 
*     5 6 17 14 
*  4 5 6 7 13 14 15 16 17 18 
*  19 20 96 97 98 106 107 115 116 124 
*  125
*   done
*   choose some
*     0 20  0 2 2 0 10 30 0 -1  65 124 
*   choose all
*  here is the front wheel
*   choose a list
*    1 53 55
*    1 11 53 55
*   0 1 2 3 11 12 53 54 55 88 89 90 225 235 244
*   0 1 2 3 11 12 53 54 55 78 
*   83 84 85 86 87 88 89 90 220 221 
*   222 223 224 225 229 230 231 233 234 235 
*   239 240 241 242 243 244 248
*   done
    mappingName
      asmo
* remove duplicate surfaces
    delete sub-surfaces 232
    delete sub-surfaces 160
    delete sub-surfaces 91
*
*    x-r:0 190
    determine topology
* debug 3
      merge tolerance .65
      deltaS 10. 5.
      build edge curves
      merge edge curves
      triangulate


      deltaS 2.

      build edge curves
      debug 7
      merge edge curves



    exit
*
  unstructured
    build topology
     asmo
     x+r:0 160


