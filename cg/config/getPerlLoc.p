eval 'exec perl -S $0 ${1+"$@"}'
if 0;
# perl program to return the location of perl

use Config; 
$perlloc="$Config{archlib}/CORE";
printf("$perlloc");

exit 0;
