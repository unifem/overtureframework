// file:  PADRE_Incomplete_Domain.h

     template <class PADRE_Ordering> 
     class PADRE_Incomplete_Domain
        : public PADRE_Complete_Domain<1,PADRE_Ordering>
        {
          public:
            ~PADRE_Incomplete_Domain();
             PADRE_Incomplete_Domain();
             PADRE_Incomplete_Domain( const PADRE_Incomplete_Domain & X );
             PADRE_Incomplete_Domain & operator= ( const PADRE_Incomplete_Domain & X );
        };

