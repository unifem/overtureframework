#!/usr/bin/perl
# perl script to run fourBeamsInAChannel
# 
#

$resultsDir="/data1/Longfei/beamPaperResults/oneBeamTests/"; # location for results
$cleanResults=1; # delete all existing results
$runTheCommand=1; # runs the cmds or just print out the cmds
$verbose=0; 
# create symbolic links for this files in runDir
@files=("oneBeamInAChannel.cmd","oneBeamInALongerChannelGride1.order2.hdf", "oneBeamInALongerChannelGride2.order2.hdf","oneBeamInALongerChannelGride4.order2.hdf","oneBeamInALongerChannelGride8.order2.hdf","oneBeamInALongerChannelGride16.order2.hdf","oneBeamInALongerChannelGridFixede1.order2.hdf", "oneBeamInALongerChannelGridFixede2.order2.hdf","oneBeamInALongerChannelGridFixede4.order2.hdf","oneBeamInALongerChannelGridFixede8.order2.hdf","oneBeamInALongerChannelGridFixede16.order2.hdf");

@runDirs;
@runCmds;



$tf=20;
$gridNumber=16;
$rhob=0.1;
$E=2.;
$uIn=1.5;
$ff=1; #frequency to flush
$nSmooths=20*$gridNumber;
#FD1_bSmoother_NBNB_CFLs100
$runName="FD1_bSmoother_NBNB_CFLs100_G$gridNumber";
$nElem=21;#7*$gridNumber;
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd -noplot oneBeamInAChannel -uIn==$uIn -BM1=FD  -g=oneBeamInALongerChannelGridFixede$gridNumber.order2.hdf -tf=$tf  -tp=.01 -rhoBeam=$rhob -E=$E -numElem=$nElem -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1  -numberOfCorrections=1 -saveProbe=1 -cfls=100. -ps=newmark2Implicit -cs=newmarkCorrector -smoothInterfaceVelocity=1 -nis=$nSmooths -smoothBeam=1 -numberOfBeamSmooths=$nSmooths -rampInflow=1  -recomputeGVOnCorrection=0 -ff=$ff -show=$runName.show -go=go > $runName.out &";


#FD0_bSmoother_NBNB_CFLs100
$runName="FD0_bSmoother_NBNB_CFLs100_G$gridNumber";
$nElem=7*$gridNumber;
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd -noplot oneBeamInAChannel -uIn=$uIn -BM1=FD  -g=oneBeamInALongerChannelGridFixede$gridNumber.order2.hdf -tf=$tf  -tp=.01 -rhoBeam=$rhob -E=$E -numElem=$nElem -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=0  -numberOfCorrections=1 -saveProbe=1 -cfls=100. -ps=newmark2Implicit -cs=newmarkCorrector -smoothInterfaceVelocity=1 -nis=$nSmooths -smoothBeam=1 -numberOfBeamSmooths=$nSmooths -rampInflow=1  -recomputeGVOnCorrection=0 -ff=$ff -show=$runName.show -go=go > $runName.out &";



#FD1_nobSmoother_NBNB_CFLs100
$runName="FD1_nobSmoother_NBNB_CFLs100_G$gridNumber";
$nElem=7*$gridNumber;
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd -noplot oneBeamInAChannel -uIn=$uIn -BM1=FD  -g=oneBeamInALongerChannelGridFixede$gridNumber.order2.hdf -tf=$tf  -tp=.01 -rhoBeam=$rhob -E=$E -numElem=$nElem -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1  -numberOfCorrections=1 -saveProbe=1 -cfls=100. -ps=newmark2Implicit -cs=newmarkCorrector -smoothInterfaceVelocity=1 -nis=$nSmooths -smoothBeam=0  -rampInflow=1  -recomputeGVOnCorrection=0 -ff=$ff -show=$runName.show -go=go > $runName.out &";

#FEM_nobSmoother_NBNB_CFLs100
$runName="FEM_nobSmoother_NBNB_CFLs100_G$gridNumber";
$nElem=7*$gridNumber;
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd -noplot oneBeamInAChannel -uIn=$uIn -BM1=FEM  -g=oneBeamInALongerChannelGridFixede$gridNumber.order2.hdf -tf=$tf  -tp=.01 -rhoBeam=$rhob -E=$E -numElem=$nElem -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=100. -ps=newmark2Implicit -cs=newmarkCorrector -smoothInterfaceVelocity=1 -nis=$nSmooths -smoothBeam=0  -rampInflow=1  -recomputeGVOnCorrection=0 -ff=$ff -show=$runName.show -go=go > $runName.out &";






### todo: make this a function so that it can be used by other runs in the future
#=================================================== common stuff ==============================
use threads;
use threads::shared;

share($ENV);

$dir=$ENV{PWD};
print "pwd=$dir\n";
$home=$ENV{HOME};
$bin="$home/cg.g/ins/bin/";


foreach(@runDirs){
    # add absolute location to runDirs
    $_=$resultsDir.$_;
}




if($cleanResults){
    chdir $dir;
    system("rm -r @runDirs");
}


$numRuns=@runCmds;
print "\n\nNumber of runs = $numRuns\n\n";
$counter = 0;
for(my $i=0; $i<$numRuns; $i++)
{
    $counter++;
    $date=localtime(); #here is date
    print "$date\nRun $counter\n";
    print "==============================================\n";
    if($verbose eq 1){print "cd $dir\n"};
    chdir "$dir" or die "cannot change: $!\n";  # back to the run (commands and grids) dir
    $runDir  = $runDirs[$i];
    if($verbose eq 1){ print "mkdir -p $runDir\n";};
    system("mkdir -p $runDir");
    if($verbose eq 1){print "cd $runDir\n"};
    chdir "$runDir" or die "cannot change: $!\n"; # go to $runDir
    msl(@files); # make soft linkes in $runDir
    $runCmd=$bin.$runCmds[$i];
    print "cmd:\n$runCmd\n";
    if($runTheCommand){
	# use threads
	print "executing cmd ------>\n";
	$thr=threads->create('msc',"$runCmd"); 
	$thr->join();
	# do not use threads
	# msc("nohup $runCmd");
    }
    print "==============================================\n\n";
}


sub msc{ ## make system call
  system( @_ );
}


sub msl{## make soft links
    if($verbose eq 1){ print("****creating symbolic links for cmd and grid files****\n");}
    foreach(@_){
	$cmd="ln -s $dir/$_ $_";
	if($verbose eq 1){print "$cmd\n";}
	system("$cmd");
    }
}
