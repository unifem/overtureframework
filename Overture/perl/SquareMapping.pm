#**************************************************************
#** perl script defining subroutines for creating square mappings
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
package SquareMapping;

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

our $className = "SquareMapping";
sub new() {
    my $type = shift;
    my $self = Mapping->new;
    $self->{X_MIN} = -1;
    $self->{X_MAX} =  1;
    $self->{Y_MIN} = -1;
    $self->{Y_MAX} =  1;
    $self->{MAKE_3D} = "";
    $self->{MAPPINGNAME} = $className;
    return bless $self, $type;
}

sub cmdString() {
    my $self = shift;
    my $str = "rectangle\n";
    my $tab = "  ";
    $str .= $tab."set corners\n$tab$tab$self->{X_MIN},$self->{X_MAX},$self->{Y_MIN},$self->{Y_MAX}\n";
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
if ( ($0 =~ /SquareMapping.pm/) )
{
    my $square = SquareMapping->new();
    $square->{Y_MIN} = -2;
    $square->{LINES} = "21 41 ";
    $square->{BC} = "1 2 3 4 ";
    $square->{SHARE} = "7 8 9 10 ";
    $square->{MAPPINGNAME} = "TestSquare";
    print $square->cmdString();
}

1;
