*
* a NACA0012 airfoil using a C-grid
*
create mappings
  read plot3d file
    /home/henshaw/nas/over1.6r/test/naca/naca0012x.fmt
*
  change a mapping
   naca0012x.fmt-grid0
    c-grid
      determine c-grid automatically    
    done
  mappingName
    naca0012
  exit
*
  exit this menu
  generate an overlapping grid
    naca0012
  done choosing mappings
  compute overlap
  pause
exit
*
save an overlapping grid
naca0012c.hdf
naca
exit

  
