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
	@EXPORT = qw( $fuel_outer_bc  $fuel_top_bc  $fuel_bottom_bc  
		      $buffer_gas_inner_bc  $buffer_gas_outer_bc  $buffer_gas_top_bc  $buffer_gas_bottom_bc 
		      $cladding_inner_bc  $cladding_outer_bc  $cladding_top_bc  $cladding_bottom_bc 
		      $coolant_inner_bc  $coolant_top_bc  $coolant_bottom_bc );
        @EXPORT_OK   = qw( );
}

use BoxMapping;
use CylinderMapping;
use StandardBoundaryConditions;

our $fuel_outer_bc        = 1;
our $fuel_top_bc          = 2;
our $fuel_bottom_bc       = 3;
our $buffer_gas_inner_bc  = $fuel_outer_bc;
our $buffer_gas_outer_bc  = 4;
our $buffer_gas_top_bc    = $fuel_top_bc;
our $buffer_gas_bottom_bc = $fuel_bottom_bc;
our $cladding_inner_bc    = $buffer_gas_outer_bc;
our $cladding_outer_bc    = 5;
our $cladding_top_bc      = $fuel_top_bc;
our $cladding_bottom_bc   = $fuel_bottom_bc;
our $coolant_inner_bc     = $cladding_outer_bc;
our $coolant_top_bc       = $fuel_top_bc;
our $coolant_bottom_bc    = $fuel_bottom_bc;

sub new {
    my $type = shift;
    my $self = {};
    $self->{LENGTH} = 10;
    $self->{FUEL_RADIUS} = .4095;
    $self->{CLADDING_INNER_RADIUS} = .418;
    $self->{CLADDING_OUTER_RADIUS} = .475;
    $self->{NAME} = "rfa_rod";
    $self->{FUEL_GRID_NAMES} = ();
    $self->{CLADDING_GRID_NAMES} = ();
    $self->{BUFFER_GAS_GRID_NAMES} = ();
    $self->{DX} = $self->{FUEL_RADIUS}/10.;
    $self->{DZ_SCALE} = .25;
    $self->{CENTER_X} = 0.;
    $self->{CENTER_Y} = 0.;
    $self->{CENTER_Z} = 0.;
    return bless $self,$type;
}

sub cmdString() {
    my $self = shift;
    my $str = "";

    my $dx = $self->{DX};
    my $n_radial = 6;
    $pi = atan2(1,1)*4.;
    my $n_z = int($self->{DZ_SCALE}*($self->{LENGTH})/$dx) + 1;
    my $n_t = int($pi*($self->{FUEL_RADIUS})/$dx)+1;
#   build the fuel grids, one cylinder and one box
    my $fuel_cyl = CylinderMapping->new();
    $fuel_cyl->{CENTER} = "$self->{CENTER_X}, $self->{CENTER_Y}, $self->{CENTER_Z}";
    $fuel_cyl->{AXIAL_MIN} = $self->{CENTER_Z}-$self->{LENGTH}/2.;
    $fuel_cyl->{AXIAL_MAX} = $fuel_cyl->{AXIAL_MIN} + $self->{LENGTH};
    my $rmin = $self->{FUEL_RADIUS}-$dx*$n_radial;
    $fuel_cyl->{INNER_RADIUS} = $rmin;
    $fuel_cyl->{OUTER_RADIUS} = $self->{FUEL_RADIUS};
    $fuel_cyl->{MAPPINGNAME} = $self->{NAME}."_fuel_cyl";
    $fuel_cyl->{BC} = "$periodic_bc, $periodic_bc,$fuel_bottom_bc, $fuel_top_bc, $no_bc, $fuel_outer_bc";
    $fuel_cyl->{SHARE} = "$no_bc, $no_bc, $fuel_bottom_bc, $fuel_top_bc, $no_bc, $fuel_outer_bc";
    $fuel_cyl->{LINES} = "$n_t, $n_z, $n_radial";
    my $fuel_box = BoxMapping->new();
    $fuel_box->{X_MIN} = $self->{CENTER_X}-$rmin-$dx;
    $fuel_box->{X_MAX} = $self->{CENTER_X}+$rmin+$dx;
    $fuel_box->{Y_MIN} = $self->{CENTER_Y}-$rmin-$dx;
    $fuel_box->{Y_MAX} = $self->{CENTER_Y}+$rmin+$dx;
    $fuel_box->{Z_MIN} = $self->{CENTER_Z}-$self->{LENGTH}/2.;
    $fuel_box->{Z_MAX} = $self->{CENTER_Z}+$self->{LENGTH}/2.;
    $fuel_box->{MAPPINGNAME} = $self->{NAME}."_fuel_box";
    $fuel_box->{BC} = "$no_bc, $no_bc,$no_bc,$no_bc,$fuel_bottom_bc, $fuel_top_bc";
    $fuel_box->{SHARE} = "$no_bc, $no_bc,$no_bc,$no_bc,$fuel_bottom_bc, $fuel_top_bc";
    my $n_x = int(($fuel_box->{X_MAX}-$fuel_box->{X_MIN})/$dx) + 1;
    my $n_y = int(($fuel_box->{Y_MAX}-$fuel_box->{Y_MIN})/$dx) + 1;
    $fuel_box->{LINES} = "$n_x, $n_y, $n_z";
    push @{$self->{FUEL_GRID_NAMES}}, $fuel_box->{MAPPINGNAME};
    push @{$self->{FUEL_GRID_NAMES}}, $fuel_cyl->{MAPPINGNAME};
#   if needed, build the grid for the buffer gas, one cylinder
    my $bc_for_inner_cladding = $cladding_inner_bc;
    my $buffer_gas_cyl;
    if ( $self->{CLADDING_INNER_RADIUS} ne $self->{FUEL_RADIUS} ) {
	$buffer_gas_cyl = CylinderMapping->new();
	$buffer_gas_cyl->{CENTER} = "$self->{CENTER_X}, $self->{CENTER_Y}, $self->{CENTER_Z}";
	$buffer_gas_cyl->{AXIAL_MIN} = $self->{CENTER_Z}-$self->{LENGTH}/2.;
	$buffer_gas_cyl->{AXIAL_MAX} = $buffer_gas_cyl->{AXIAL_MIN} + $self->{LENGTH};
	$buffer_gas_cyl->{INNER_RADIUS} = $self->{FUEL_RADIUS};
	$buffer_gas_cyl->{OUTER_RADIUS} = $self->{CLADDING_INNER_RADIUS};
	$buffer_gas_cyl->{MAPPINGNAME} = $self->{NAME}."_buffer_gas_cyl";
	$buffer_gas_cyl->{BC} = "$periodic_bc, $periodic_bc, $buffer_gas_bottom_bc, $buffer_gas_top_bc, $buffer_gas_inner_bc, $buffer_gas_outer_bc";
	$buffer_gas_cyl->{SHARE} = "$no_bc, $no_bc, $buffer_gas_bottom_bc, $buffer_gas_top_bc, $buffer_gas_inner_bc, $buffer_gas_outer_bc";
	my $n_test = int(($self->{CLADDING_INNER_RADIUS}-$self->{FUEL_RADIUS})/$dx)+1;
	my $n_rad = $n_test<11 ? 11 : $n_test;
	$buffer_gas_cyl->{LINES} = "$n_t, $n_z, $n_rad";
	push @{$self->{BUFFER_GAS_GRID_NAMES}}, $buffer_gas_cyl->{MAPPINGNAME};
    } else {
	$bc_for_inner_cladding = $fuel_outer_bc;
    }
#   build the cylinder grid for the cladding
    my $cladding_cyl = CylinderMapping->new();
    $cladding_cyl->{CENTER} = "$self->{CENTER_X}, $self->{CENTER_Y}, $self->{CENTER_Z}";
    $cladding_cyl->{AXIAL_MIN} = $self->{CENTER_Z}-$self->{LENGTH}/2.;
    $cladding_cyl->{AXIAL_MAX} = $cladding_cyl->{AXIAL_MIN} + $self->{LENGTH};
    my $rmin = $self->{CLADDING_INNER_RADIUS};
    $cladding_cyl->{INNER_RADIUS} = $rmin;
    $cladding_cyl->{OUTER_RADIUS} = $self->{CLADDING_OUTER_RADIUS};
    $cladding_cyl->{MAPPINGNAME} = $self->{NAME}."_cladding_cyl";
    $cladding_cyl->{BC} = "$periodic_bc, $periodic_bc,$cladding_bottom_bc, $cladding_top_bc, $bc_for_inner_cladding, $cladding_outer_bc";
    $cladding_cyl->{SHARE} = "$no_bc, $no_bc, $cladding_bottom_bc, $cladding_top_bc, $bc_for_inner_cladding, $cladding_outer_bc";
    my $n_test = int(($self->{CLADDING_OUTER_RADIUS}-$self->{CLADDING_INNER_RADIUS})/$dx)+1;
    my $n_rad = $n_test<11 ? 11 : $n_test;
    $cladding_cyl->{LINES} = "$n_t, $n_z, $n_rad";
    push @{$self->{CLADDING_GRID_NAMES}}, $cladding_cyl->{MAPPINGNAME};
#   build the cylinder grid for the fluid outside the cylinder
    my $coolant_cyl = CylinderMapping->new();
    $coolant_cyl->{CENTER} = "$self->{CENTER_X}, $self->{CENTER_Y}, $self->{CENTER_Z}";
    $coolant_cyl->{AXIAL_MIN} = $self->{CENTER_Z}-$self->{LENGTH}/2.;
    $coolant_cyl->{AXIAL_MAX} = $coolant_cyl->{AXIAL_MIN} + $self->{LENGTH};
    my $rmax = $self->{FUEL_RADIUS}+$dx*$n_radial;
    $coolant_cyl->{INNER_RADIUS} = $self->{CLADDING_OUTER_RADIUS};
    $coolant_cyl->{OUTER_RADIUS} = $rmax;
    $coolant_cyl->{MAPPINGNAME} = $self->{NAME}."_coolant_cyl";
    $coolant_cyl->{BC} = "$periodic_bc, $periodic_bc,$coolant_bottom_bc, $coolant_top_bc, $coolant_inner_bc, $no_bc";
    $coolant_cyl->{SHARE} = "$no_bc, $no_bc, $coolant_bottom_bc, $coolant_top_bc, $coolant_inner_bc, $no_bc";
    $coolant_cyl->{LINES} = "$n_t, $n_z, $n_radial";
    push @{$self->{COOLANT_GRID_NAMES}}, $coolant_cyl->{MAPPINGNAME};

    $str .= $fuel_box->cmdString() . $fuel_cyl->cmdString();
    if ( $buffer_gas_cyl ) {
	$str .= $buffer_gas_cyl->cmdString();
    }
    $str .= $cladding_cyl->cmdString() . $coolant_cyl->cmdString();
    return $str;
}


END { };


#package main;
#my $fuel_rod = RFAFuelRod->new();
#print $fuel_rod->cmdString();
#use StandardBoundaryConditions;
#print "periodic_bc = $periodic_bc\n";
#print "no_bc = $no_bc\n";
