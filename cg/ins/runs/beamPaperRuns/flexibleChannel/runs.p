#!/usr/bin/perl
# perl script to run fourBeamsInAChannel
# 
#
use threads;
use threads::shared;

@runDirs;
@runCmds;


$cleanResults=1;
#=============================== AMP schemes ===============================================================
# # AMP+FD0+NBNB
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD0_NBNB";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=0 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";


# # AMP+FD0+ABAM 
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD0_ABAM";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=0 -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";

# # AMP+FD1+NBNB
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD1_NBNB";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";


# # AMP+FD1+ABAM # not work
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD1_ABAM";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";

# # AMP+FEM+NBNB
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FEM_NBNB";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FEM -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#==================================================================================================================

#=============================== TP schemes ===============================================================
# #TP+FD1+NBNB 
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_NBNB";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateTPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

# #TP+FD1+ABAM
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_ABAM";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateTPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";


# #TP+FEM+NBNB 
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FEM_NBNB";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateTPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FEM -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

# #TP+FEM+NBNB+Kxxt0p01
# $runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FEM_NBNB_Kxxt0p01";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -Kxxt=0.01 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateTPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FEM -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#=========================== FD TP scheme with beam smoother ====================================
#TP+FD1+NBNB_bSmoother 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_NBNB_bSmoother";
push @runDirs,$runName;
push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateTPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -smoothBeam=1 -numberOfBeamSmooths=4 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#TP+FD1+ABAM_bSmoother 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_ABAM_bSmoother";
push @runDirs,$runName;
push @runCmds,"cgins -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateTPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -smoothBeam=1 -numberOfBeamSmooths=4  -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";



$numRuns=@runCmds;

print "number of runs = $numRuns\n";

$dir=$ENV{PWD};
print "pwd=$dir\n";
$home=$ENV{HOME};
$bin="$home/cg.g/ins/bin/";

share($ENV);


# files need a soft link inside of each run folder
@files=("flexiblePartition.cmd","flexiblePartitionGride2.order2.hdf");


if($cleanResults){
    chdir $dir;
    system("rm -r @runDirs");
}


for(my $i=0; $i<$numRuns; $i++)
{
    chdir "$dir" or die "cannot change: $!\n";  # back to the main Dir
    $runDir  = $runDirs[$i];
    $runCmd=$bin.$runCmds[$i];
    print "mkdir -p $runDir\n";
    system("mkdir -p $runDir");
    chdir "$dir/$runDir" or die "cannot change: $!\n"; # go to $runDir
    msl(@files); # make soft linkes in $runDir
    print "$runCmd\n";
    $thr=threads->create('msc',"$runCmd"); 
    $thr->join();
    # msc("nohup $runCmd"); #without using threads
}


sub msc{ ## make system call
  system( @_ );
}


sub msl{##make soft links
   foreach(@_){
     $cmd="ln -s $dir/$_ $_";
     system("$cmd");
   }
}
