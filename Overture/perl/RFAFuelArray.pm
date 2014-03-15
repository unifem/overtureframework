#**************************************************************
#** perl module for generating Westinghouse RFA fuel rod grids
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 070712 - initial version
#**************************************************************
#**************************************************************
package RFAFuelRod;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.h

BEGIN {
        use Exporter   ();
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;
        @ISA         = qw(Exporter);
#        @EXPORT      = qw( &processBuildingFile);#
        %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
        # your exported package globals go here,
        # as well as any optionally exported functions
	@EXPORT = qw( );
        @EXPORT_OK   = qw( );
}

use RFAFuelRod;

sub new {
    my $type = shift;
    my $self = {};
    $self->{LENGTH} = 10;
    $self->{FUEL_RADIUS} = .4095;
    $self->{CLADDING_INNER_RADIUS} = .418;
    $self->{CLADDING_OUTER_RADIUS} = .475;
    $self->{NAME} = "rfa";
    $self->{FUEL_GRID_NAMES} = ();
    $self->{CLADDING_GRID_NAMES} = ();
    $self->{BUFFER_GAS_GRID_NAMES} = ();
    $self->{DX} = $self->{FUEL_RADIUS}/10.;
    $self->{CENTER_X} = 0.;
    $self->{CENTER_Y} = 0.;
    $self->{CENTER_Z} = 0.;
    $self->{ARRAY_DIM} = 1;
    $self->{ROD_SPACING} = .292;
    return bless $self,$type;
}

sub cmdString() {
    $self = shift;
    my $str = "";
    my $fuel_rod = RFAFuelRod->new();
    $fuel_rod->{LENGTH} = $self->{LENGTH};
    $fuel_rod->{FUEL_RADIUS} = $self->{FUEL_RADIUS};
    $fuel_rod->{CLADDING_INNER_RADIUS} = $self->{CLADDING_INNER_RADIUS};
    $fuel_rod->{CLADDING_OUTER_RADIUS} = $self->{CLADDING_OUTER_RADIUS};
    $fuel_rod->{DX} = $self->{DX};

    for ( my $m=0; $m<$self->{ARRAY_DIM}; $m++ ) {
	for ( my $n=0; $n<$self->{ARRAY_DIM}; $n++ ) {
	}
    }
	
    return $str;
}

END { }
