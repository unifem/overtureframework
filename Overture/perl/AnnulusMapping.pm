#**************************************************************
#** perl script defining subroutines for creating annulus mappings
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
package AnnulusMapping;

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

our $className = "AnnulusMapping";
sub new() {
    my $type = shift;
    my $self = Mapping->new;
    $self->{CENTER} = "0,0,0";
    $self->{INNER_RADIUS} = 0.5;
    $self->{OUTER_RADIUS} = 1.;
    $self->{ELLIPSE_RATIO} =  "";
    $self->{MAKE_3D} = "";
    $self->{START_ANGLE} = 0.;
    $self->{END_ANGLE} = 1.;
    $self->{MAPPINGNAME} = $className;
    bless $self, $type;
    return $self;
}

sub cmdString() {
    my $self = shift;
    my $str = "annulus\n";
    my $tab = "  ";

    $str .= $tab."centre for annulus\n$self->{CENTER}\n";
    $str .= $tab."inner and outer radii\n$self->{INNER_RADIUS}, $self->{OUTER_RADIUS}\n";
    $str .= $tab."start and end angles\n$self->{START_ANGLE}, $self->{END_ANGLE}\n";
    if ( $self->{ELLIPSE_RATIO} ) {
	$str .= $tab."ellipse ratio\n$self->{ELLIPSE_RATIO}\n";
    }
    if ( $self->{MAKE_3D} ) 
    {
	$str .= $tab."make 3d (toggle)\n$self->{MAKE_3D}\n";
    }
    $str .= $self->SUPER::cmdString();
    $str .= "exit\n";
    return $str;
}

END { };

# print out the test string if we are executing this file
## e.g. 'perl -I$Overture/include AnnulusMapping.pm' will result in the printing of a test string
if ( ($0 =~ /AnnulusMapping.pm/) )
{
    my $annulus = AnnulusMapping->new();

    $annulus->{INNER_RADIUS} = .75;
    $annulus->{END_ANGLE} = .25;
    $annulus->{ELLIPSE_RATIO} = .5;
    $annulus->{LINES} = "21 41 ";
    $annulus->{BC} = "1 2 3 4 ";
    $annulus->{SHARE} = "7 8 9 10 ";
    $annulus->{MAPPINGNAME} = "TestAnnulus";
    print $annulus->cmdString();
}

1;
