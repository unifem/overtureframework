// file:  PADRE_Ordering.h

     template <class Dimension>
     class PADRE_Ordering
        {
          public:
            ~PADRE_Ordering ();
             PADRE_Ordering ();
             PADRE_Ordering ( const PADRE_Ordering & X );
             PADRE_Ordering & operator= ( const PADRE_Ordering & X );
       // This would force all attributes to have an internal code to avoid them
       // having the constraint of being the same type (and thus being in a single array)
          int AttributeIndexValues[Dimension];
        };
