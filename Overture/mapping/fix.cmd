  *
  read iges file
  cat2.igs
    * front port
    *
*      165
    choose a list
      110
      115
      112
      done
pause
*
* 112 : has a mistake. We fix this by adjusting the trimming curve
* and the un-trimmed surface
*
    edit a Mapping
      112
      * remove the extraneous piece of the trimming curve and
      * shift the curve back by .25
      edit trimming curves
        reparameterize
          .192 .898
        shift
          -.25 0.
        change control points
        * 53   7.4054e-01 7.3210e-01
        * 54   7.5742e-01 7.5799e-01 
        * 55   7.7432e-01 7.8454e-01 
        * 56   7.9143e-01 8.1234e-01 
        * 57   8.0905e-01 8.4184e-01   
        * 58   8.2760e-01 8.7333e-01 
        * 59   8.4773e-01 9.0708e-01
        * 60 : 8.7029e-01 9.4328e-01
        * 61 : 8.8718e-01 9.6810e-01 1.
        * 62 : 8.9639e-01 9.8077e-01
        * 63 : 8.8547e-01 9.8077e-01  
        *      8.6488e-01 9.8077e-01
        *      8.3720e-01 
          56
           .78    .81234  1.   7.9143e-01 8.1234e-01 
          57   
           .80    .84184 1.     8.0905e-01 8.4184e-01 1.  
          58 
           .81    .87333  1.     8.2760e-01 8.7333e-01 
          59
           .825   .94     1.     8.4773e-01 9.0708e-01
          60
           .832   .965    1.     8.7029e-01 9.4328e-01
          61 
           .839   .978    1.     8.8718e-01 9.6810e-01 1.
          62
           .842   .98077 1.      8.9639e-01 9.8077e-01
          63 
           .839   .98077 1.      8.8547e-01 9.8077e-01 
          64 
           .838   .98077 1.
        done
        pause
        exit
      *
      * shift the parameterization of the surface to move the branch cut
      * Note that we cannot just rotate the surface since the parameterization
      * is not rotationally invariant.
      edit untrimmed surface
        periodicity
          2 0
        restrict
          .25 1.25 0. 1.
        exit
      pause
    exit
