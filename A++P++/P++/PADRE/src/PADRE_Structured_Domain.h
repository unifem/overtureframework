// file:  PADRE_Structured_Domain.h

     template <int PADRE_Dimension>
     class PADRE_Structured_Domain
        {
          public:
            ~PADRE_Structured_Domain ();
             PADRE_Structured_Domain ();
             PADRE_Structured_Domain ( const PADRE_Structured_Domain & X );
             PADRE_Structured_Domain & operator= ( const PADRE_Structured_Domain & X );

         // The purpose of the ordering it to hide the indirection of the access of the 
         // attributes in the collection.
            int BaseOffset  [PADRE_Dimension];
            int Base        [PADRE_Dimension];
            int Bound       [PADRE_Dimension];
            int Stride      [PADRE_Dimension];

        };


