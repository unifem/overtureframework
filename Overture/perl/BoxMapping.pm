#**************************************************************
#** perl script defining subroutines for creating box mappings
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
package BoxMapping;

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

our $className = "BoxMapping";
sub new() {
    my $self = Mapping->new;
    $self->{X_MIN} = -1;
    $self->{X_MAX} =  1;
    $self->{Y_MIN} = -1;
    $self->{Y_MAX} =  1;
    $self->{Z_MIN} = -1;
    $self->{Z_MAX} =  1;
    $self->{ROTATION_AXIS} = 0;
    $self->{ROTATION_ANGLE} = 0;
    $self->{MAPPINGNAME} = $className;
    bless $self;
    return $self;
}

sub setCorners($$$$$$) {
    my $self = shift;
    $$self{X_MIN} = shift;
    $$self{X_MAX} = shift;
    $$self{Y_MIN} = shift;
    $$self{Y_MAX} = shift;
    $$self{Z_MIN} = shift;
    $$self{Z_MAX} = shift;
}

sub cmdString() {
    my $self = shift;
    my $str = "box\n";
    my $tab = "  ";
    $str .= $tab."set corners\n$tab$tab$self->{X_MIN},$self->{X_MAX},$self->{Y_MIN},$self->{Y_MAX},$self->{Z_MIN},$self->{Z_MAX}\n";
    if ( $self->{ROTATION_ANGLE} ne 0 ) {
	$str.=$tab."rotate\n$tab$tab$self->{ROTATION_AXIS}, $self->{ROTATION_ANGLE}\n";
    }
    $str .= $self->SUPER::cmdString();
    $str .= "exit\n";
    return $str;
}

END { };

# print out the test string if we are executing this file
if ( ($0 =~ /BoxMapping.pm/) )
{
    my $box = BoxMapping->new();
    $box->{Y_MIN} = -2;
    $box->{LINES} = "21 41 41";
    $box->{BC} = "1 2 3 4 5 6";
    $box->{SHARE} = "7 8 9 10 11 12";
    $box->{MAPPINGNAME} = "TestBox";
    print $box->cmdString();
}

1;
