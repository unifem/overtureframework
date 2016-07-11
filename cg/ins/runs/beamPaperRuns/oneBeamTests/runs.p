#!/usr/bin/perl
# perl script to run fourBeamsInAChannel
# 
#

@runDirs;
@runCmds;


$cleanResults=1;
$tf=3;


### try one 07/02/2016####
#FD1_ABAM Re=1000
# for(my $i=4;$i<5;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E20_NBS2_AMP_FD1_ABAM_G$gridNumber";
#     $nElem=7*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInAChannelGride$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=20. -numElem=$nElem -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -ps=adamsBashforth2 -cs=adamsMoultonCorrector -smoothBeam=1 -numberOfBeamSmooths=2 -show=$runName.show -go=go > $runName.out &" ;
# }


#FD1_ABAM Re=10
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E20_Re10_NBS2_AMP_FD1_ABAM_G$gridNumber";
#     $nElem=7*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInAChannelGride$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=20. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -ps=adamsBashforth2 -cs=adamsMoultonCorrector -smoothBeam=1 -numberOfBeamSmooths=2 -show=$runName.show -go=go > $runName.out &" ;
# }

#FD1_ABAM Re=10 E=10 Longer channel
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E10_Re10_Longer_NBS2_AMP_FD1_ABAM_G$gridNumber";
#     $nElem=7*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInALongerChannelGride$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=10. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -ps=adamsBashforth2 -cs=adamsMoultonCorrector -smoothBeam=1 -numberOfBeamSmooths=2 -show=$runName.show -go=go > $runName.out &" ;
# }

#FD1_NBNB Re=10 E=10 cfls=3 Longer channel variable NBS for finer grids
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E10_Re10_cfls3_Longer_NBS2_AMP_FD1_NBNB_vNBS_G$gridNumber";
#     $nElem=7*$gridNumber;
#     $NBS=2*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInALongerChannelGride$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=10. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=3. -ps=newmark2Implicit -cs=newmarkCorrector -smoothBeam=1 -numberOfBeamSmooths=$NBS -show=$runName.show -go=go > $runName.out &" ;
# }


#FEM_NBNB Re=10 E=10 cfls=3 Longer channel no beamSmoother for finer grids
for(my $i=0;$i<4;$i++)
{
    $gridNumber=2**$i;
    $runName="rhos10_E10_Re10_cfls3_Longer_AMP_FEM_noBSmoother_NBNB_G$gridNumber";
    $nElem=7*$gridNumber;
    push @runDirs,$runName;
    push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FEM  -g=oneBeamInALongerChannelGride$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=10. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=3. -ps=newmark2Implicit -cs=newmarkCorrector -smoothBeam=0  -show=$runName.show -go=go > $runName.out &" ;
}

#FD1_NBNB Re=10 E=10 cfls=2 Longer channel variable NBS for finer grids fixed width for hyperbolic martching
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E10_Re10_cfls2_Longer_AMP_FD1_NBNB_vNBS_fixedWidth_G$gridNumber";
#     $nElem=7*$gridNumber;
#     $NBS=2*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInALongerChannelFixRGridFixede$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=10. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=2. -ps=newmark2Implicit -cs=newmarkCorrector -smoothBeam=1 -numberOfBeamSmooths=$NBS -show=$runName.show -go=go > $runName.out &" ;
# }


#FD1_NBNB Re=10 E=10 cfls=2 newgrids Longer channel variable NBS for finer grids
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E10_Re10_cfls2_newGrids_Longer_NBS2_AMP_FD1_NBNB_vNBS_G$gridNumber";
#     $nElem=7*$gridNumber;
#     $NBS=2*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInALongerChannelGridNewe$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=10. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=2. -ps=newmark2Implicit -cs=newmarkCorrector -smoothBeam=1 -numberOfBeamSmooths=$NBS -show=$runName.show -go=go > $runName.out &" ;
# } 

#FD1_ABAM Re=10 E=10 Longer channel new grid more grids clustered around the beam tip
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;+
#     $runName="rhos10_E10_Re10_newGrid_Longer_NBS2_AMP_FD1_ABAM_G$gridNumber";
#     $nElem=7*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FD  -g=oneBeamInALongerChannelGridNewe$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=10. -numElem=$nElem -outflowOption=neumann -nu=1.e-1  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -ps=adamsBashforth2 -cs=adamsMoultonCorrector -smoothBeam=1 -numberOfBeamSmooths=2 -show=$runName.show -go=go > $runName.out &" ;
# }				


#FEM_NBNB
# for(my $i=0;$i<4;$i++)
# {
#     $gridNumber=2**$i;
#     $runName="rhos10_E20_AMP_FEM_NBNB_G$gridNumber";
#     $nElem=7*$gridNumber;
#     push @runDirs,$runName;
#     push @runCmds,"cgins -noplot oneBeamInAChannel -BM1=FEM  -g=oneBeamInAChannelGride$gridNumber.order2.hdf -tf=$tf. -tp=.01 -rhoBeam=1. -E=20. -numElem=$nElem -outflowOption=neumann -nu=1.e-3  -addedMass=1 -ampProjectVelocity=1 -useApproximateAMPcondition=0 -useSameStencilSize=1 -numberOfCorrections=1 -saveProbe=1 -cfls=1. -ps=newmark2Implicit -cs=newmarkCorrector -smoothBeam=0 -show=$runName.show -go=go > $runName.out &" ;
# }




# make soft links for these files in each run directories
@files=("oneBeamInAChannel.cmd","oneBeamInAChannelGride1.order2.hdf","oneBeamInAChannelGride2.order2.hdf","oneBeamInAChannelGride4.order2.hdf","oneBeamInAChannelGride8.order2.hdf", "oneBeamInAChannelGride16.order2.hdf","oneBeamInALongerChannelGride1.order2.hdf", "oneBeamInALongerChannelGride2.order2.hdf","oneBeamInALongerChannelGride4.order2.hdf","oneBeamInALongerChannelGride8.order2.hdf","oneBeamInALongerChannelGridNewe1.order2.hdf","oneBeamInALongerChannelGridNewe2.order2.hdf","oneBeamInALongerChannelGridNewe4.order2.hdf","oneBeamInALongerChannelGridNewe8.order2.hdf","oneBeamInALongerChannelFixRGridFixede1.order2.hdf","oneBeamInALongerChannelFixRGridFixede2.order2.hdf","oneBeamInALongerChannelFixRGridFixede4.order2.hdf","oneBeamInALongerChannelFixRGridFixede8.order2.hdf");




########## Common stuff ##########
use threads;
use threads::shared;


$numRuns=@runCmds;

print "number of runs = $numRuns\n";

$dir=$ENV{PWD};
print "pwd=$dir\n";
$home=$ENV{HOME};
$bin="$home/cg.g/ins/bin/";

share($ENV);




if($cleanResults){
    chdir $dir;
    print("rm -r @runDirs\n");
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
    # use threads
    # $thr=threads->create('msc',"$runCmd"); 
    # $thr->join();
    # use processes
    msc($runCmd);
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
