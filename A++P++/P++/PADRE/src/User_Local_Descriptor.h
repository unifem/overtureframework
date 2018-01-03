  // We need a collection type and instantiation to use PADRE
     class User_Local_Descriptor
        {
          public:
          // Nested Class
             class Domain
                {
                  public:
                     int Base;
                     int Bound;

                     Domain (int Size) : Base(0), Bound(Size-1) {};
                };

          // Member Data
             float *Temperature;

          // Member Functions
             User_Local_Descriptor (int Size)
                {
                  Temperature = new float [Size];
                  Base  = 0;
                  Bound = 99;
                }
        };

  // Data allocated by user in this constructor call
     User_Local_Descriptor::Domain Global_Domain_0 (100);

  // Distribution using the order imposed by implicit position attributes onto processors 10-22
  // This distribution is independent of the size of the data
     PADRE_Distribution User_Distribution (Range(10,22));

  // Build a PADRE_Descriptor using the users instance of the "User_Local_Descriptor" object and
  // a "PADRE_Distribution" object to specify how the data is partitioned
     PADRE_Descriptor<User_Local_Descriptor> User_Descriptor_0 (Global_Domain_0,User_Distribution);

  // In the rest of a users application member functions of the variable "User_Descriptor" 
  // are called to generate communication schedules between different PADRE_Descriptors.

  // Build another descriptor object
     User_Local_Descriptor::Domain Global_Domain_1 (100);
     PADRE_Distribution User_Distribution (Range(10,22));
     PADRE_Descriptor<User_Local_Descriptor> User_Descriptor_1 (Global_Domain_1,User_Distribution);

  // Build a communication Schedule
     PADRE_CommunicationSchedule Schedule_0_to_1 = User_Descriptor_0.CommunicationSchedule (User_Descriptor_1);
