package OverBlownConvergenceTestOptions;
require Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(getPDEString formGridName getTwilightZoneString getPDEOptions);

sub getPDEString {

    my $arg = shift(@_);
    if ( $arg eq "cns" ) {
	return "compressible Navier Stokes (Godunov)";
    } elsif ( $arg eq "icns" ) {
	return "compressible Navier Stokes (implicit)";
    } elsif ( $arg eq "icns_axi_swirl" ) {
	return "compressible Navier Stokes (implicit)\naxisymmetric flow with swirl 1";
    } elsif ( $arg eq "icns_axi" ) {
	return "compressible Navier Stokes (implicit)";
    } elsif ( $arg eq "ncns" ) {
	return "steady-state compressible Navier Stokes (newton)";
    } elsif ( $arg eq "ncns_axi_swirl" ) {
	return "steady-state compressible Navier Stokes (newton)\naxisymmetric flow with swirl 1";
    } elsif ( $arg eq "ncns_axi" ) {
	return "steady-state compressible Navier Stokes (newton)";
    } elsif ( $arg eq "jcns" ) {	
	return "compressible Navier Stokes (Jameson)";
    } elsif ( $arg eq "asf" ) {
	return "all speed Navier Stokes";
    } 

    return "incompressible Navier Stokes";
}

sub formGridName {
    my $baseGrid = shift(@_);
    my $res = shift(@_);
    my $bc  = shift(@_);
    # the following if block was taken from obConvTest.p but I am not sure how up to date it is
    if ($baseGrid eq "square")
    {
	if ( $bc ne "periodic" ){
	    @gridName=("square10","square20","square30","square40");
	} else {
	    @gridName=("square10p","square20p","square30p","square40p");
	}	    
#	@gridName=("square20","square30","square40","square80");
    }
    elsif ($baseGrid eq "shifted_square")
    {
	@gridName=("shifted_square10","shifted_square20","shifted_square40","shifted_square80");
    }
#================================================================================
    elsif ( $baseGrid eq "cic" )
    {
	@gridName=("cic1","cic2","cic3","cic4");
    }
#================================================================================
    elsif ( $baseGrid eq "cici" )
    {
	@gridName=("cici1.order2","cici2.order2","cici3.order2","cici4.order2");
    }
#================================================================================
    elsif ( $baseGrid eq "cic_shifted" )
    {
	@gridName=("cic_shifted.1","cic_shifted.2","cic_shifted.3","cic_shifted.4");
    }
#=============================================================================================
    elsif ( $baseGrid eq "box" )
    {
	@gridName=("box5","box10","box20");
    }
#=============================================================================================
    elsif ( $baseGrid eq "sib" )
    {
	@gridName=("sib1","sib2","sib2a");
    }
#=============================================================================================
    elsif ( $baseGrid eq "sic" )
    {
	@gridName=("sic1","sic2","sic3");
    }
#=============================================================================================
    elsif ( $baseGrid eq "annulus" )
    {
	@gridName=("annulus20","annulus40","annulus60","annulus80");
    }
    elsif ( $baseGrid eq "sisi" )
    {
	@gridName=("sisi1.order2","sisi2.order2","sisi3.order2","sisi4.order2");
    }
#=============================================================================================
    else
    {
	printf ("ERROR: unknown baseGrid name = [$baseGrid]\n");
	exit;
    }

    my $ngrids = @gridName;
    ( $ngrids >= $res ) or die "ERROR : there are only $ngrids grids available for grid '$baseGrid'\n";
    ($res > 0) or die "ERROR : grid resolution index must be greater than 0, res=$res !\n";

    return $gridName[$res-1];
}

sub getTwilightZoneString {
    my $tztype = shift(@_);

    if ( $tztype eq "trig" ) 
    {
	return "OBTZ:trigonometric";
    } 
    elsif ( $tztype eq "poly" )
    {
        return "OBTZ:polynomial";
    } 
    else
    {
        return "OBTZ:pulse";
    }
}

sub getPDEOptions {

    my $arg = shift(@_);
    if ( $arg eq "icns" || $arg eq "icns_axi_swirl" || $arg eq "icns_axi" ) {
	my $s = "implicit factor $ifac \n";
	$s .= "OBPDE:av2,av4 .0,$av4\n";
	$s .= "implicit time step solver options\n";
	$s .= "choose best direct solver\n";
	$s .= "choose best iterative solver\n";
#	$s .= "harwell\n";
	$s .= "    matrix cutoff\n    1e-20\n";
	$s .= "exit\n";
	if ( $arg eq "icns_axi" ) {
	    $s .= "turn on axisymmetric flow";
	}
	return $s;
    } elsif ( $arg eq "ncns" || $arg eq "ncns_axi" || $arg eq "ncns_axi_swirl" ) {
	my $s = "implicit factor $ifac \n";
	$s .= "OBPDE:av2,av4 .0,$av4\n";
	$s .= "implicit time step solver options\n";
	$s .= "choose best direct solver\n";
	$s .= "    matrix cutoff\n    1e-20\n";
	$s .= "exit\n";
	if ( $arg eq "ncns_axi" ) {
	    $s .= "turn on axisymmetric flow";
	}
	return $s;
    }
    return "** no pde solver options";

}
