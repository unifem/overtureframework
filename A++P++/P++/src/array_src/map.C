#define COMPILE_PPP

#include "A++.h"

// **********************************************************************
// This function is used to find out how the current processor (the one 
// where this code executes) is positioned relative to the array which 
// is partitioned on a collection of processors.
// **********************************************************************

int Array_Domain_Type::Get_Processor_Position ( int Axis ) const
{
   int Return_Position = -1;

   // The logic in this function could be made more efficent later but 
   // for now we just try to make it very clear.
   bool Is_A_Left_Partition   = isLeftPartition   (Axis);
   bool Is_A_Right_Partition  = isRightPartition  (Axis);
   bool Is_A_Middle_Partition = isMiddlePartition (Axis);
   bool Is_A_Non_Partition    = isNonPartition    (Axis);

   if (Is_A_Non_Partition)
   {
       Return_Position = NOT_PRESENT_ON_PROCESSOR;
   }
   else if (Is_A_Left_Partition && Is_A_Right_Partition)
   {
      Return_Position = SINGLE_PROCESSOR;
   }
   else
   {
      if (Is_A_Left_Partition)
      {
          Return_Position = LEFT_PROCESSOR;
      }
      else if (Is_A_Right_Partition)
      {
         Return_Position = RIGHT_PROCESSOR;
      }
      else if (Is_A_Middle_Partition)
      {
          Return_Position = MIDDLE_PROCESSOR;
      }
      else
      {
         printf ("ERROR: in Array_Descriptor_Type::");
	 printf ("Get_Processor_Position \n");
         APP_ABORT();
      }
   }

   return Return_Position;
}

