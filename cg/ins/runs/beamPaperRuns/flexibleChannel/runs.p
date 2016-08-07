#!/usr/bin/perl
# perl script to run fourBeamsInAChannel
# 
#



$resultsDir="/data1/Longfei/beamPaperResults/flexibleChannel/"; # location for results
$cleanResults=1; # delete all existing results
$runTheCommand=1; # runs the cmds or just print out the cmds
$verbose=0; 
# create symbolic links for this files in runDir
@files=("flexiblePartition.cmd","flexiblePartitionGride1.order2.hdf","flexiblePartitionGride2.order2.hdf","flexiblePartitionGride4.order2.hdf","flexiblePartitionGride8.order2.hdf","flexiblePartitionGride16.order2.hdf","flexiblePartition0thicknessGride1.order2.hdf","flexiblePartition0thicknessGride2.order2.hdf","flexiblePartition0thicknessGride4.order2.hdf","flexiblePartition0thicknessGride8.order2.hdf","flexiblePartitionsmallthicknessGride1.order2.hdf","flexiblePartitionsmallthicknessGride2.order2.hdf","flexiblePartitionsmallthicknessGride4.order2.hdf","flexiblePartitionsmallthicknessGride8.order2.hdf","flexiblePartitionsmallthicknessGride16.order2.hdf");


@runDirs;
@runCmds;

#====================== beam thickness = 0.1 =====================================================================
$runName="beamUnderPressure_h0p1_EI0p2_Rhos0p1_CFLs1_AMP_FD1_NBNB";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

# AMP+FD1+NBNB 1e-1 thickness
# heavy beam first: rhos=1000
$tf=5;
for(my $i=1;$i<5;$i++)
{
    $gridNumber=2**$i;
    $runName="beamUnderPressure_h0p1_EI0p2_Rhos1000_CFLs1_AMP_FD1_NBNB_G$gridNumber";
    $nElem=10*$gridNumber;
    push @runDirs,$runName;
    push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionGride$gridNumber.order2.hdf -nu=.025 -tf=$tf -tp=.01 -rhoBeam=1000 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=$nElem -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &" ;
}


# below are thin beams beam thickness = 1e-5
# redone on 08/02/2016 after fix a bug in BeamModel::interpolateSolution
#=============================== AMP schemes ===============================================================
# AMP+FD0+NBNB
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD0_NBNB";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=0 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";


# AMP+FD0+ABAM 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD0_ABAM";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=0 -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";

# AMP+FD1+NBNB
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD1_NBNB";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";


# AMP+FD1+ABAM 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FD1_ABAM";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";

# AMP+FEM+NBNB
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_AMP_FEM_NBNB";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=50. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FEM -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#==================================================================================================================

#=============================== TP schemes ===============================================================
#TP+FD1+NBNB 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_NBNB";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#TP+FD1+ABAM
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_ABAM";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";


#TP+FEM+NBNB 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FEM_NBNB";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FEM -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#TP+FEM+NBNB+Kxxt0p01
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FEM_NBNB_Kxxt0p01";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -Kxxt=0.01 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FEM -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#=========================== FD TP scheme with beam smoother ====================================
#TP+FD1+NBNB_bSmoother 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_NBNB_bSmoother";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -smoothBeam=1 -numberOfBeamSmooths=4 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &";

#TP+FD1+ABAM_bSmoother 
$runName="beamUnderPressure_EI0p2_Rhos0p1_CFLs1_TP_FD1_ABAM_bSmoother";
push @runDirs,$runName;
push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride2.order2.hdf -nu=.025 -tf=10. -tp=.01 -rhoBeam=.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=40 -p0=1 -addedMass=0 -useTP=1 -addedMassRelaxation=0.01 -addedMassTol=1e-5 -numberOfCorrections=500 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=0 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -smoothBeam=1 -numberOfBeamSmooths=4  -ps=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go > $runName.out &";



#=========================== self convergence test at t=5 ====================================
$tf=5;				


#===== small thickness beam ======
#AMP+FD1+NBNB 1e-5 thickness
#heavy beam first: rhos=1000
for(my $i=1;$i<5;$i++)
{
    $gridNumber=2**$i;
    $runName="beamUnderPressure_EI0p2_Rhos1000_hsmall_CFLs1_AMP_FD1_NBNB_G$gridNumber";
    $nElem=10*$gridNumber;
    push @runDirs,$runName;
    push @runCmds,"cgins -abortOnEnd  -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride$gridNumber.order2.hdf -nu=.025 -tf=$tf -tp=.01 -rhoBeam=1000 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=$nElem -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &" ;
}

# # # AMP+FD1+NBNB 1e-5 thickness
# # light beam first: rhos=1e-2
# for(my $i=1;$i<5;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="beamUnderPressure_EI0p2_Rhos0p1_hsmall_CFLs1_AMP_FD1_NBNB_G$gridNumber";
#     $nElem=10*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -abortOnEnd   -noplot flexiblePartition -g=flexiblePartitionsmallthicknessGride$gridNumber.order2.hdf -nu=.025 -tf=$tf -tp=.01 -rhoBeam=0.1 -E=.2 -tension=0. -thickness=.1 -Kt=1 -numElem=$nElem -p0=1 -addedMass=1 -useApproximateAMPcondition=0 -fluidOnTwoSides=1 -ampProjectVelocity=1 -projectNormalComponent=1 -option=beamUnderPressure -smoothInterfaceVelocity=1 -nis=4 -probePosition=.5  -cfls=1. -BM=FD -useSameStencilSize=1 -ps=newmark2Implicit -cs=newmarkCorrector -show=$runName.show -go=go > $runName.out &" ;
# }







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
