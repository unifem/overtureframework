#**************************************************************
#** perl script defining subroutines to for creating cylinder mappings
#**      in ogen
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 070713 - initial version
#**************************************************************
#**************************************************************
#*
#
#
package CylinderMapping;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
	use Mapping;
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;

        @ISA         = qw(Mapping Exporter);
        @EXPORT      = qw( );#&createSmoothedPolygon);
        %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $className = "CylinderMapping";
sub new() {
    my $type = shift;
    my $self = Mapping->new;
    $self->{IS_SURFACE} = 0;
    $self->{INNER_RADIUS} = .2;
    $self->{OUTER_RADIUS} = 1;
    $self->{AXIAL_MIN} = 0;
    $self->{AXIAL_MAX} = 1;
    $self->{THETA_MIN} = 0;
    $self->{THETA_MAX} = 1;
    $self->{ORIENTATION} = "0,1,2";
    $self->{CENTER} = "0,0,0";
    $self->{MAPPINGNAME} = $className;
    bless $self,$type;
    return $self;
}

sub cmdString() {
    my $self = shift;
    my $str = "cylinder\n";
    my $tab = "  ";
    if ( $self->{IS_SURFACE} ne 0 ) {
	$str .= $tab."surface or volume (toggle)\n";
    }
    $str .= $tab."orientation\n$tab$tab$self->{ORIENTATION}\n";
    $str .= $tab."centre for cylinder\n$tab$tab$self->{CENTER}\n";
    $str .= $tab."bounds on theta\n$tab$tab$self->{THETA_MIN},$self->{THETA_MAX}\n";
    $str .= $tab."bounds on the axial variable\n$tab$tab$self->{AXIAL_MIN},$self->{AXIAL_MAX}\n";
    $str .= $tab."bounds on the radial variable\n$tab$tab$self->{INNER_RADIUS},$self->{OUTER_RADIUS}\n";
    $str .= $self->SUPER::cmdString();
    $str .="exit\n";
    return $str;
}

END { };

# print out the test string if we are executing this file
if ( ($0 =~ /CylinderMapping.pm/) )
{
    my $cyl = CylinderMapping->new();
    $cyl->{AXIAL_MIN} = -2;
    $cyl->{THETA_MAX} = .5;
    $cyl->{CENTER} = "0,1,2";
    $cyl->{LINES} = "21 41 41";
    $cyl->{BC} = "1 2 3 4 5 6";
    $cyl->{SHARE} = "7 8 9 10 11 12";
    $cyl->{MAPPINGNAME} = "TestCylinder";
    print $cyl->cmdString();
}

1;
