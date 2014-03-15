      ! This common block holds the pointer to the user defined EOS class
      ! We can set this pointer from C++ and then the fortran routines can
      ! pass it to the user defined EOS routines.
      double precision userEOSDataPointer  
      common /eosUserDefined/ userEOSDataPointer
