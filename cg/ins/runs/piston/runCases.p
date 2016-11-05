eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#
# perl program to run different cases for checking added-damping stability
# 
# Examples:
#  runCases.p -option=shearBlock -tf=5 -rhos=.001

use Getopt::Long; use Getopt::Std;

$pi = 4.*atan2(1.,1.);

$numberOfParameters = @ARGV;
if ($numberOfParameters eq 0)
{
  
  printf("\n");
  printf("================================================================================\n");
  printf("This perl script will run different cases...\n");
  printf("  Usage: \n");
  printf("    runCases.p -option=[shearBlock|shearDisk|transDisk] -tf=<f> -rhos=<f> -projectVelocity=[0|1] ... \n");
  printf("               -instabilityErrorTol=<f> -beta=<f> \n");
  printf("  where \n");
  printf("    \n");
  printf("==============================================================================\n\n");
  exit;
  
}

$option="shearBlock";
$tf=1.; 
$rhos=.01; 
$projectVelocity=1;
$instabilityErrorTol=.1; 
$beta=-1;

foreach $arg ( @ARGV )
{
  if( $arg =~ /-option=(.*)/ )
  {
    $option = $1;
    printf("Setting option=[%s]\n",$option);
  }
  elsif( $arg =~ /-tf=(.*)/ )
  {
    $tf=$1; printf("Setting tf=[%g]\n",$tf);
  }
  elsif( $arg =~ /-rhos=(.*)/ )
  {
    $rhos=$1;  printf("Setting rhos=[%g]\n",$rhos);
  }
  elsif( $arg =~ /-beta=(.*)/ )
  {
    $beta=$1;  printf("Setting beta=[%g]\n",$beta);
  }
  elsif( $arg =~ /-instabilityErrorTol=(.*)/ )
  {
    $instabilityErrorTol=$1;  printf("Setting instabilityErrorTol=[%g]\n",$instabilityErrorTol);
  }
  elsif( $arg =~ /-projectVelocity=(.*)/ )
  {
    $projectVelocity=$1; printf("Setting projectVelocity=[%d]\n",$projectVelocity);
  }
}

$debug=0; # set to 1 for debug info 

printf("------------ Option=[$option] rhos=$rhos tf=$tf projectVelocity=$projectVelocity--------------\n");
printf("------------ instabilityErrorTol=[$instabilityErrorTol]                          --------------\n");
$CGBUILDPREFIX=$ENV{CGBUILDPREFIX};
$cginsCmd = "$CGBUILDPREFIX/ins/bin/cgins";  # command for cgins 

# Run with different values for the added damping coefficient


$n=0;
if( $beta ne -1 )
{
  # Run with this value of beta:
  $adc[$n]=$beta; $n++;

}
elsif( $option eq "shearBlock" )
{
  $adc[$n]=.2; $n++;
  $adc[$n]=.25; $n++; 
  $adc[$n]=.3; $n++; 
  $adc[$n]=.35; $n++; 
  $adc[$n]=.4; $n++; 
  if( $projectVelocity eq 0 ){ $adc[$n]=.54; $n++; } # check special island of stability
  $adc[$n]=1.; $n++;
  if( $projectVelocity eq 0 ){  $adc[$n]=1.5; $n++; } # check special island of stability
  $adc[$n]=2.; $n++;
  $adc[$n]=3.; $n++;
  $adc[$n]=4.; $n++;
  $adc[$n]=5.; $n++;
  $adc[$n]=5.5; $n++;
  $adc[$n]=6.; $n++;
  # $adc[$n]=10.; $n++;
}
elsif( $option eq "shearDisk" )
{
  $adc[$n]=.3; $n++;
  $adc[$n]=.5; $n++;
  if( $projectVelocity eq 0 ){ $adc[$n]=.54; $n++; } # special island of stability
  $adc[$n]=.6; $n++; 
  $adc[$n]=1.; $n++; 
  if( $projectVelocity eq 0 ){  $adc[$n]=1.3; $n++; } # check special island of stability
  if( $projectVelocity eq 0 ){  $adc[$n]=1.5; $n++; } # check special island of stability
  $adc[$n]=2.; $n++; 
  $adc[$n]=3.; $n++; 
  $adc[$n]=3.5; $n++; 
  $adc[$n]=4.; $n++; 
  $adc[$n]=5.; $n++; 
}
elsif( $option eq "transDisk" )
{
  $adc[$n]=.3; $n++;
  $adc[$n]=.5; $n++;
  if( $projectVelocity eq 0 ){ $adc[$n]=.54; $n++; } # special island of stability
  $adc[$n]=1.; $n++; 
  if( $projectVelocity eq 0 ){  $adc[$n]=1.5; $n++; } # check special island of stability
  $adc[$n]=2.; $n++; 
  $adc[$n]=3.; $n++; 
  $adc[$n]=4.; $n++; 
  $adc[$n]=5.; $n++; 
  $adc[$n]=6.; $n++; 
}
else
{
  printf("Unknown option=[$option]\n");
  exit;
}

$nc=2; $pv=$projectVelocity;
if( $projectVelocity eq 0 )
{
  $nc=1;   # do not project velocity
}

$name = "$option";
if( $pv eq 1 ){ $name .= "PV"; }else{ $name .= "NPV"; } 


for( $i=0; $i<@adc; $i++ )
{

  $ad = $adc[$i];

  $show="$name$ad.show"; 
  $outFile="$name$ad.out"; 
  $rigidBodyCheckFile="$name$ad.check"; 


  if( $option eq "shearBlock" )
  {
    $dt=.05; $nu=.1; $dy=.025; $mb=$rhos; $L=1; 
    $cmd = "$cginsCmd -noplot slider -g=shearBlockGrid4.order2 -ts=im -dtMax=.05 -bodyDensity=$rhos -tf=$tf -tp=.05 -option=shearBlock -amp=1 -relaxRigidBody=0 -numberOfCorrections=$nc -addedDampingProjectVelocity=$pv -omega=.2 -rtolc=1.e-3 -psolver=yale -addedMass=1 -addedDamping=1 -scaleAddedDampingWithDt=1 -inertia=1e100 -nu=.1 -useProvidedAcceleration=1 -gravity=0. -addedDampingCoeff=$ad -exitOnInstability=1 -instabilityErrorTol=$instabilityErrorTol -rigidBodyCheckFile=$rigidBodyCheckFile -show=$show -go=go >! $outFile";
  }
  elsif( $option eq "shearDisk" )
  {
     $dt=.025; $nu=.1; $dy=.0334; $mb=$rhos*$pi;  $L=2.*$pi; 

     $cmd = "$cginsCmd -noplot movingDiskInDisk.cmd -g=annulusGrid2.order2 -dropName=annulus -tf=$tf -tp=.05 -nu=.1 -ts=im -density=$rhos -radius=1. -gravity=0. -amp=1. -bodyForce=none -option=rotatingDisk -dtMax=.025 -project=0 -numberOfCorrections=$nc -addedDampingProjectVelocity=$pv -omega=.2 -addedMass=1 -addedDamping=1 -scaleAddedDampingWithDt=1 -useProvidedAcceleration=1 -useTP=0 -debug=3 -solver=best -rtol=1.e-10 -atol=1.e-12 -psolver=best -rtolp=1.e-10 -atolp=1.e-12 -ad2=0 -addedDampingCoeff=$ad -exitOnInstability=1 -instabilityErrorTol=$instabilityErrorTol -rigidBodyCheckFile=$rigidBodyCheckFile -show=$show -go=go >! $outFile";
  }
  elsif( $option eq "transDisk" )
  {
     $dt=.025; $nu=.1; $dy=.0334; $mb=$rhos*$pi;   $L=2.*$pi; 

     $cmd = "$cginsCmd -noplot movingDiskInDisk.cmd -g=diskInDiskGridHalfe4.order2.hdf -tf=$tf -tp=.1 -nu=.1 -ts=im -density=$rhos -radius=1. -gravity=0. -amp=1.e-7 -bodyForce=none -option=translatingDisk -dtMax=.025 -project=0 -numberOfCorrections=$nc -addedDampingProjectVelocity=$pv -omega=.2 -addedMass=1 -addedDamping=1 -scaleAddedDampingWithDt=1 -useProvidedAcceleration=1 -useTP=0 -debug=3 -solver=best -rtol=1.e-10 -atol=1.e-15  -psolver=best -rtolp=1.e-10 -atolp=1.e-15  -ad2=0 -addedDampingCoeff=$ad -exitOnInstability=1 -instabilityErrorTol=$instabilityErrorTol -rigidBodyCheckFile=$rigidBodyCheckFile -show=$show -go=go >! $outFile";

  }
  else
  {
   printf("Unknown option=[$option]\n");
   exit;
  }
  # printf("pi=$pi\n");

  $dnu=sqrt(.5*$nu*$dt);
  $delta=$dy/$dnu; 
  $mBar=$mb/($L*$dnu);

  printf("--------------- addedDampingCoefficient=$ad -------------------\n");
  printf(">> run [$cmd]\n");
  $startTime=time();

  $returnValue = system("csh -f -c 'nohup $cmd'");    

  $cpuTime=time()-$startTime;
  printf("...returnValue=%i [$returnValue], cpu=%g (s)\n",$returnValue,$cpuTime);

  $stability[$i]=$returnValue;

  if( $returnValue ne 0 )
  {
    printf(" *****  addedDampingCoefficient=$ad appears UNSTABLE **********\n");
  }
}
printf("------------ SUMMARY Option=[$option] rhos=$rhos tf=$tf projectVelocity=$projectVelocity--------------\n");
printf("------------    dy=%12.4e,  dnu=sqrt(.5*nu*dt)=%12.4e,  delta=dy/dnu=%12.4e,  mBar=%12.4e --------------\n",
           $dy,$dnu,$delta,$mBar);
$is=0; $iu=0; # counts stable and unstable modes
for( $i=0; $i<@stability; $i++ )
{
  $ad = $adc[$i];
  if( $stability[$i] eq 0 )
  {
    $stab[$is]=$ad; $is++;
    printf(" addedDampingCoefficient=%5.2f : stable.\n",$ad);
  }
  else
  {
    $unStab[$iu]=$ad; $iu++;
    printf(" addedDampingCoefficient=%5.2f : UNSTABLE ******\n",$ad);
  }
}


printf("%%------------ SUMMARY Option=[$option] rhos=$rhos tf=$tf projectVelocity=$projectVelocity--------------\n");
printf("%%------------    dy=%12.4e,  dnu=sqrt(.5*nu*dt)=%12.4e,  delta=dy/dnu=%12.4e,  mBar=%12.4e --------------\n",
           $dy,$dnu,$delta,$mBar);
printf("%%------------ instabilityErrorTol=%9.2e                                               ------------------\n",
        $instabilityErrorTol);

printf(" $name" . "Delta=%12.6e;\n",$delta);
printf(" $name" . "Mbar=%12.6e;\n",$mBar);

printf(" $name" ."Stab=["); for( $i=0; $i<@stab; $i++ ){ printf("%8.3f",$stab[$i]); if( $i <@stab-1 ){ printf(","); }} printf("];\n");


printf(" $name" . "Unstab=["); for( $i=0; $i<@unStab; $i++ ){ printf("%8.3f",$unStab[$i]); if( $i <@unStab-1 ){ printf(","); }} printf("];\n");



exit;
# ===============================================================================================================


