#!/usr/bin/perl
# perl script to run fourBeamsInAChannel
# 
#

$resultsDir="/data1/Longfei/beamPaperResults/fourBeamsInAChannel/"; # location for results
$cleanResults=1; # delete all existing results
$runTheCommand=1; # runs the cmds or just print out the cmds
$verbose=0; 
# create symbolic links for this files in runDir
@files=("fourBeamsInAChannel.cmd","beam2.h","beam3.h","beam4.h","fourBeamsInALongerChannelGride1.order2.hdf","fourBeamsInALongerChannelGride2.order2.hdf","fourBeamsInALongerChannelGride4.order2.hdf","fourBeamsInALongerChannelGride8.order2.hdf");


@runDirs;
@runCmds;


#============================= runs =============================================
$tf=20.;
$cfls=10.;
$uIn=2.;
$gn=4;
$numElem=7*$gn;
$E1=30.;$rho1=1000.;
$E2=10.;$rho2=10.;
$E3=20.;$rho3=1.;
$E4=30.;$rho4=100;
$nu=1e-3;
$rampInflow=1;

$grid="fourBeamsInALongerChannelGride$gn.order2.hdf";

# use yale solver (default)
# $runName="FEM1234_LongerChannel_AMP_NB_G$gn";
# push @runDirs,$runName;
# push @runCmds,"cgins -abortOnEnd -noplot fourBeamsInAChannel -BM1=FEM -BM2=FEM -BM3=FEM -BM4=FEM -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1  -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=0  -smoothInterfaceVelocity=1 -nis=10 -rampInflow=$rampInflow -recomputeGVOnCorrection=0 -show=$runName.show -go=go  >  $runName.out &";

# $runName="FD1234_LongerChannel_AMP_NB_G$gn";
# push @runDirs,$runName;
# push @runCmds,"cgins  -abortOnEnd  -noplot fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -useSameStencilSize=1 -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1 -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=1 -numberOfBeamSmooths=10 -smoothInterfaceVelocity=1 -nis=10 -rampInflow=$rampInflow -recomputeGVOnCorrection=0 -show=$runName.show -go=go >  $runName.out &";

# use best psolver
# $runName="FEM1234_LongerChannel_AMP_NB_bestPSolver_G$gn";
# push @runDirs,$runName;
# push @runCmds,"cgins -abortOnEnd -noplot fourBeamsInAChannel -BM1=FEM -BM2=FEM -BM3=FEM -BM4=FEM -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1  -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=0  -smoothInterfaceVelocity=1 -nis=10 -rampInflow=$rampInflow -recomputeGVOnCorrection=0  -psolver=best -rtolp=1.e-5 -atolp=1.e-6  -show=$runName.show -go=go  >  $runName.out &";

# $runName="FD1234_LongerChannel_AMP_NB_bestPSolver_G$gn";
# push @runDirs,$runName;
# push @runCmds,"cgins  -abortOnEnd  -noplot fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -useSameStencilSize=1 -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1 -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=1 -numberOfBeamSmooths=10 -smoothInterfaceVelocity=1 -nis=10 -rampInflow=$rampInflow -recomputeGVOnCorrection=0  -psolver=best -rtolp=1.e-5 -atolp=1.e-6  -show=$runName.show -go=go >  $runName.out &";


## try cfls=10 for FDBeamModel with more bSmoother
$runName="FD1234_cfls10_LongerChannel_AMP_NB_bestPSolver_G$gn";
push @runDirs,$runName;
push @runCmds,"cgins  -abortOnEnd  -noplot fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -useSameStencilSize=1 -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1 -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=1 -numberOfBeamSmooths=50 -smoothInterfaceVelocity=1 -nis=50 -rampInflow=$rampInflow -recomputeGVOnCorrection=0  -psolver=best -rtolp=1.e-5 -atolp=1.e-6  -show=$runName.show -go=go >  $runName.out &";






###-------- to do -----------
# push @runDirs,"FD34FEM12_AMP_NB_rhos1_G8";
# push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FEM -BM2=FEMls -BM3=FD -BM4=FD -g=fourBeamsInAChannelGride8.order2.hdf -tf=10. -tp=.01 -rhoBeam=1. -E=20 -numElem=21 -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -show=FD34FEM12_AMP_NB_rhos1_G8.show -go=go >  FD34FEM12_AMP_NB_rhos1_G8.out &";


# push @runDirs,"FD1234_TP_NB_rhos1_G8";
# push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -g=fourBeamsInAChannelGride8.order2.hdf -tf=10. -tp=.01 -rhoBeam=1. -E=20 -numElem=21 -outflowOption=neumann -nu=1.e-3 -useTP=1 -addedMassRelaxation=.1 -addedMassTol=1.e-5   -addedMass=0 -ampProjectVelocity=0 -useApproximateAMPcondition=0 -numberOfCorrections=500 -saveProbe=1 -show=FD1234_TP_NB_rhos1_G8.show -go=go >  FD1234_TP_NB_rhos1_G8.out &";


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
