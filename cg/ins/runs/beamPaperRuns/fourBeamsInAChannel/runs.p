#!/usr/bin/perl
# perl script to run fourBeamsInAChannel
# 
#
use threads;
use threads::shared;

@runDirs;
@runCmds;


$cleanResults=1;
$runTheCommand=0;

#FEM on G2 
#$runName="FEM1234_LongerChannel_AMP_NB_G2";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FEM -BM2=FEM -BM3=FEM -BM4=FEM -useSameStencilSize=1 -g=fourBeamsInALongerChannelGride1.order2.hdf -tf=10. -tp=.01 -rhoBeam1=1000. -rhoBeam2=10. -rhoBeam3=1. -rhoBeam4=100. -E1=30 -E2=10 -E3=20 -E4=30 -numElem=7 -outflowOption=neumann -uIn=2. -nu=1.e-3 -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=4. -smoothBeam=0  -smoothInterfaceVelocity=1 -nis=10 -rampInflow=1 -show=$runName.show -go=go >  $runName.out &";


#same problem computed using FEM and FD on G8
$tf=20.;
$grid="fourBeamsInALongerChannelGride8.order2.hdf";
$cfls=1.;
$uIn=1.5;
$runName="FEM1234_LongerChannel_AMP_NB_G8";
$numElem=28;
$E1=30.;$rho1=1000.;
$E2=10.;$rho2=10.;
$E3=20.;$rho3=1.;
$E4=30.;$rho4=100;
$nu=1e-3;
$rampInflow=1;

$runName="FEM1234_LongerChannel_AMP_NB_G8";
push @runDirs,$runName;
push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FEM -BM2=FEM -BM3=FEM -BM4=FEM -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1  -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=0  -smoothInterfaceVelocity=1 -nis=10 -rampInflow=$rampInflow -show=$runName.show -go=go >  $runName.out &";


$runName="FD1234_LongerChannel_AMP_NB_G8";
push @runDirs,$runName;
push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -useSameStencilSize=1 -g=$grid -tf=$tf. -tp=.01 -rhoBeam1=$rho1 -rhoBeam2=$rho2 -rhoBeam3=$rho3 -rhoBeam4=$rho4 -E1=$E1 -E2=$E2 -E3=$E3 -E4=$E4 -numElem=$numElem -outflowOption=neumann -uIn=$uIn -nu=$nu -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=$cfls -smoothBeam=1 -numberOfBeamSmooths=30 -smoothInterfaceVelocity=1 -nis=30 -rampInflow=$rampInflow -show=$runName.show -go=go >  $runName.out &";



# ### done 07/05/2016####
# $runName="FD1234_AMP_NB_G8";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot  fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -useSameStencilSize=1 -g=fourBeamsInAChannelGride8.order2.hdf -tf=10. -tp=.01 -rhoBeam=1. -E=5. -numElem=21 -outflowOption=neumann -nu=5.e-2 -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -smoothBeam=1 -numberOfBeamSmooths=10 -smoothInterfaceVelocity=1 -nis=10 -show=$runName.show -go=go >  $runName.out &" ;


# ### done 07/01/2016####
# $runName="FD1234_AMP_ABAM_G8";
# push @runDirs,$runName;
# push @runCmds,"cgins -noplot  fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -useSameStencilSize=1 -g=fourBeamsInAChannelGride8.order2.hdf -tf=10. -tp=.01 -rhoBeam=1. -E=5. -numElem=21 -outflowOption=neumann -nu=5.e-2 -addedMass=1 -ampProjectVelocity=0 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -smoothBeam=1 -numberOfBeamSmooths=10 -smoothInterfaceVelocity=1 -nis=10 -pc=adamsBashforth2 -cs=adamsMoultonCorrector -show=$runName.show -go=go >  $runName.out &" ;

###-------- to do -----------
# push @runDirs,"FD34FEM12_AMP_NB_rhos1_G8";
# push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FEM -BM2=FEMls -BM3=FD -BM4=FD -g=fourBeamsInAChannelGride8.order2.hdf -tf=10. -tp=.01 -rhoBeam=1. -E=20 -numElem=21 -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -numberOfCorrections=1 -saveProbe=1 -show=FD34FEM12_AMP_NB_rhos1_G8.show -go=go >  FD34FEM12_AMP_NB_rhos1_G8.out &";


# push @runDirs,"FD1234_TP_NB_rhos1_G8";
# push @runCmds,"cgins -noplot fourBeamsInAChannel -BM1=FD -BM2=FD -BM3=FD -BM4=FD -g=fourBeamsInAChannelGride8.order2.hdf -tf=10. -tp=.01 -rhoBeam=1. -E=20 -numElem=21 -outflowOption=neumann -nu=1.e-3 -useTP=1 -addedMassRelaxation=.1 -addedMassTol=1.e-5   -addedMass=0 -ampProjectVelocity=0 -useApproximateAMPcondition=0 -numberOfCorrections=500 -saveProbe=1 -show=FD1234_TP_NB_rhos1_G8.show -go=go >  FD1234_TP_NB_rhos1_G8.out &";

$numRuns=@runCmds;

print "number of runs = $numRuns\n";

$dir=$ENV{PWD};
print "pwd=$dir\n";
$home=$ENV{HOME};
$bin="$home/cg.g/ins/bin/";

share($ENV);



@files=("fourBeamsInAChannel.cmd","beam2.h","beam3.h","beam4.h","fourBeamsInAChannelGride2.order2.hdf",
	"fourBeamsInAChannelGride4.order2.hdf","fourBeamsInAChannelGride8.order2.hdf",
        "fourBeamsInALongerChannelGride1.order2.hdf", "fourBeamsInALongerChannelGride2.order2.hdf",
        "fourBeamsInALongerChannelGride4.order2.hdf", "fourBeamsInALongerChannelGride8.order2.hdf");


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
    if($runTheCommand){
	$thr=threads->create('msc',"$runCmd"); 
	$thr->join();
    }
}

# foreach(@thr)
# {
#     $_->join();
# }


sub msc{ ## make system call
  system( @_ );
}


sub msl{##make soft links
   foreach(@_){
     $cmd="ln -s $dir/$_ $_";
     system("$cmd");
   }
}
