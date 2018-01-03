// *****************************************************************
// BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT
// *****************************************************************
// BUG DISCRIPTION: A bug in the Sun C++ compiler was found which
// is particularly difficult to figure out. ALL template class member
// function not defined in the header file MUST be declared before any
// of the member function defined in the header file!!!!!  If this is not
// done then the compiler will not search the *.C file and will not instantiate
// the template function.  The result is that templated member function will not
// be found at link time of any application requiring the templated class's member
// function.
// *****************************************************************
// BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT
// *****************************************************************

// file: PADRE_Kelp_Descriptor.h

//========================================================================

#ifndef PADRE_Kelp_Descriptorh
#define PADRE_Kelp_Descriptorh

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class KELP_Descriptor
   {
  // This is analogous to the PADRE_Descriptor class (though without the pointer 
  //   to the data)
     public:
       // This should be a global domain which is translation independent
          UserLocalDomain *globalDomain;
       // This is a local domain not a global domain and it is not translation 
       // independent
          UserLocalDomain *localDomain;
          KELP_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
	     *representation;
          UserGlobalDomain *localDescriptor;

	  XARRAY<GRID<double> >* DXArrayPointer;
	  XARRAY<GRID<float> >*  FXArrayPointer;
	  XARRAY<GRID<int> >*    IXArrayPointer;

	  //void* DXArrayPointer;
	  //void* FXArrayPointer;
	  //void* IXArrayPointer;

         ~KELP_Descriptor ();

       // The only useful constructor!
          KELP_Descriptor 
             ( const UserLocalDomain *inputGlobalDomain,
               const UserLocalDomain *inputLocalDomain,
               const KELP_Representation<UserCollection, UserGlobalDomain, 
	       UserLocalDomain> *inputRepresentation, 
               const UserGlobalDomain *inputLocalDescriptor );

          void allocateData (double**);
          void allocateData (float**);
          void allocateData (int**);

          static void swapDistribution 
             ( const PADRE_Distribution<UserCollection, UserGlobalDomain, 
	       UserLocalDomain> & oldDistribution ,
               const PADRE_Distribution<UserCollection, UserGlobalDomain, 
	       UserLocalDomain> & newDistribution );

          static void freeMemoryInUse();
          void testConsistency ( const char *label = "" ) const;

          static void displayDefaultValues ( const char *Label = "" );
          void display ( const char *Label = "" ) const;

          int referenceCount;
          void incrementReferenceCount () const
             { ((KELP_Descriptor*) this)->referenceCount++; }
          void decrementReferenceCount () const
             { ((KELP_Descriptor*) this)->referenceCount--; }
          int getReferenceCount () const
             { return referenceCount; }
          static int getReferenceCountBase ()
             { return 1; }

          UserLocalDomain* getGlobalDomainPointer()
             { 
               PADRE_ASSERT (globalDomain != NULL);
               return globalDomain; 
             }

       // This function sets up the KELP specific data in the representation 
       // specific to the size data represented by the PADRE_Descriptor's 
       // globalDomain.  If this data has been previously setup then the KELP 
       // specific data is used directly (in this case for the representation 
       // of a second distributed data object)

       // This is the only way I can see to build a descriptor
          void InitializeLocalDescriptor ();
          void InitializeLocalDescriptor 
	     ( UserGlobalDomain & inputLocalDescriptor, 
               UserLocalDomain & inputLocalDomain );

       // ... Kelp must allocate it's own data ...

          friend ostream & operator<< (ostream & os, const KELP_Descriptor & X)
             {
               os << "{KELP_Descriptor:  "
                  << ", more information as yet omitted"
                  << "}" << endl;
               return os;
             }

       // I wonder if these member functions should not be 
       // better placed into the KELP_Representation object?
       //  static SCHED *BuildCommunicationScheduleRegularSectionTransfer
       //      ( const UserGlobalDomain & LhsDomain,
       //        const UserGlobalDomain & RhsDomain );
          static MOTIONPLAN * BuildCommunicationScheduleRegularSectionTransfer
             ( const FLOORPLAN& sendFloorPlan,
               const REGION& sendView,
               const FLOORPLAN& receiveFloorPlan,
               const REGION& receiveView);

          static void transferData 
             ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const double *sourceDataPointer,
               const double *destinationDataPointer );


          static void transferData 
             ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const float *sourceDataPointer,
               const float *destinationDataPointer );

          static void transferData 
             ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const int *sourceDataPointer,
               const int *destinationDataPointer );

          FLOORPLAN* makeFloorPlan 
	     ( const UserGlobalDomain & Domain);

          REGION* makeRegionView ( const UserGlobalDomain & Domain);

#if 0
          static void transferData 
             ( const XArray4<Grid4<double>>& sendArray,
               XArray4<Grid4<double>>& receiveArray);

          static void transferData 
             ( const XArray4<Grid4<float>>& sendArray,
               XArray4<Grid4<float>>& receiveArray);

          static void transferData 
             ( const XArray4<Grid4<int>>& sendArray,
               XArray4<Grid4<int>>& receiveArray);

          static void transferData 
             ( const KELP_Descriptor& sendDescriptor,
               KELP_Descriptor& receiveDescriptor);
#endif

       // These can only be implemented at this level since they require the
       // isNonPartition() member function and this can only be implemented 
       // using the localDescriptor which is only available within this object.
          int isLeftPartition   (int i) const;
          int isMiddlePartition (int i) const;
          int isRightPartition  (int i) const;
          int isNonPartition    (int i) const;

          void updateGhostBoundaries ( double *dataPointer );
          void updateGhostBoundaries ( float  *dataPointer );
          void updateGhostBoundaries ( int    *dataPointer );

          SCHED* BuildCommunicationScheduleUpdateAllGhostBoundaries ();


     private:
          KELP_Descriptor ();
          KELP_Descriptor ( const KELP_Descriptor & X );
          KELP_Descriptor & operator= ( const KELP_Descriptor & X );

   };

//========================================================================

template <class tGRID> void kelpTransferData
  (const XARRAY<tGRID>& sendArray, XARRAY<tGRID>&receiveArray,
   double dummyVar);

template <class tGRID> void kelpTransferData
  (const XARRAY<tGRID>& sendArray, XARRAY<tGRID>&receiveArray,
   float dummyVar);

template <class tGRID> void kelpTransferData
  (const XARRAY<tGRID>& sendArray, XARRAY<tGRID>&receiveArray,
   int dummyVar);


#endif //PADRE_Kelp_Descriptorh





