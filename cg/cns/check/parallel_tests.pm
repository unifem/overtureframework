   # Parallel regression tests:
@cmdFiles=(
 	     "squarej.cns",    # compressible, Jameson with TZ
 	     "squarej.cns", 
	     "cicej.cns",
	     "cicej.cns",
	     "squareg.cns",    # compressible, Godunov with TZ
	     "squareg.cns",
	     "cicej.cns",
	     "cicej.cns"  
	     ); 
  # specify the number of processors to use in each of the above cases 
@numProc=(
             "1",
             "2",
             "1",
             "2",
             "1",
             "2",
             "1",
             "2",
             "1",
             "2",
             "1",
             "2"
	    );
