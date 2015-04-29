eval 'exec perl -S $0 ${1+"$@"}'
if 0;
#!/usr/bin/perl
# perl program to run flexible channel cases
#  Usage:
#      runCase.p
# 

use Time::HiRes;

# require "ctime.pl"; 
# 
# $output="/home/henshw/mybackup.log"; 
# open(OUTFILE,">>$output") || die "cannot open file $output!" ;
# 
# $date=&ctime(time);  chop($date);  # here is the date

$firstCase=0; 
$numCases=3;

# run finest case only:
# $firstCase=3; $numCases=4;

# Case "a" - follow Fernandez but bigger nu 
# t=.015 is too late
$tf=.0075; $tp=.0025; $nu=1.; $E0=.75e6; $tMax=.005; $beamScale=10.; $psolver="yale"; 
$U0=1.; # for non-dimensionalizing
@case = ( "fc2a", "fc4a", "fc8a", "fc16a", "fc32a" );

# -- case "b" more flexible beam:
if( 1==0 )
{
  $firstCase=0; $numCases=3;
  # run finest case only:
  $firstCase=3; $numCases=4;

  $tf=.020; $tp=.005; $nu=1.; $E0=.25e6; $tMax=.0075; $beamScale=5.; 
  $psolver="best"; 
  @case = ( "fc2b", "fc4b", "fc8b", "fc16b", "fc32b" );
}

# -- case "c" cleaner case -- non-dimensional , no time-damping
if( 1==1 )
{
  # $firstCase=0; $numCases=4;
  # run finest case only:
  # $firstCase=4; $numCases=5;

  $tf=1.5; $tp=.5; $nu=1.; $E0=.25e6; $tMax=.0075; $beamScale=5.; 
  $U0=100.; # for non-dimensionalizing

  $psolver="best"; 
  @case = ( "fc2c", "fc4c", "fc8c", "fc16c", "fc32c" );

}

@grid = ( "flexibleChannelGridFixede2.order2.hdf", "flexibleChannelGridFixede4.order2.hdf", "flexibleChannelGridFixede8.order2.hdf", "flexibleChannelGridFixede16.order2.hdf", "flexibleChannelGridFixede32.order2.hdf" );
@nelem  = ( 120, 240, 480, 960, 1920 ); 

$cgins = "/home/henshw/cg.g/ins/bin/cgins"; 

for( $i=$firstCase; $i<$numCases; $i++ )
{

  printf("run case $i: $grid[$i], output=$case[$i].out\n");
  $start = Time::HiRes::gettimeofday();

#  $returnValue =  system("csh -f -c '/home/henshw/cg.g/ins/bin/cgins -noplot flexibleChannel -g=$grid[$i] -numElem=$nelem[$i] -cfl=.9 -cfls=.25 -slowStartSteps=100 -slowStartCFL=.25 -nu=$nu -tf=$tf -tp=$tp -psolver=$psolver -addedMass=1 -beamPlotScaleFactor=$beamScale -pMax=2.e4 -E0=$E0 -tMax=$tMax  -smoothInterfaceVelocity=1 -nis=2 -projectNormalComponent=1 -show=$case[$i].show -go=go >! $case[$i].out'");

  $returnValue =  system("csh -f -c '/home/henshw/cg.g/ins/bin/cgins -noplot flexibleChannel -g=$grid[$i] -numElem=$nelem[$i] -cfl=.9 -ad2=1 -nu=$nu -tf=$tf -tp=$tp -addedMass=1 -ampProjectVelocity=1 -projectNormalComponent=1 -beamPlotScaleFactor=$beamScale -pMax=2.e4 -tMax=$tMax -E0=$E0 -U0=$U0 -cfls=5. -Kt=0. -Kxxt=0.  -show=$case[$i].show -go=go >! $case[$i].out'");

  $end = Time::HiRes::gettimeofday();
  printf("CPU = %.2f\n", $end - $start);


}

