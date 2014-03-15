#! /bin/csh -f
#

echo "Copy the DataBase files into the Overture.g/DataBase directory..."

# here is where the library is
set Overture        = "/home/henshaw/Overture.g"
set OvertureDB      = "/home/henshaw/Overture.g/DataBase"
set OvertureInclude = "/home/henshaw/Overture.g/include"

cp {GenericDataBase,HDF_DataBase,DataBaseBuffer,ListOfHDF_DataBaseRCData}.C             $OvertureDB
cp {GenericDataBase,HDF_DataBase,DataBaseBuffer,ListOfHDF_DataBaseRCData}.h             $OvertureInclude

exit
