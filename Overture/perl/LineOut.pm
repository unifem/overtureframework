#**************************************************************
#** perl script defining subroutines for line outs in PlotIt::contour
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 071203 - initial version
#**************************************************************
#**************************************************************
#*
#
#
package LineOut;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;

        @ISA         = qw(Exporter);
        @EXPORT      = qw( );#&createSmoothedPolygon);
        %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $className = "LineOut";

sub new() {
    my $type = shift;
    my $self = {};
    $self->{COMPONENTS} = ();
    $self->{NUMBER_OF_POINTS} = 21;
    $self->{USE_ACTUAL_DISTANCE} = 0;
    $self->{LINE_ENDPOINTS} = ();
    $self->{MATLAB_FILE} = "";
    return bless $self,$type;
}

sub cmdString() {
    my $self = shift;
    my $str = "";
    $str .= "line plots\n";
    if ( $self->{USE_ACTUAL_DISTANCE} ) {
	$str .= "use actual distance\n";
    }
    $str .= "specify lines\n"; 
    my $np = $self->{NUMBER_OF_POINTS};
    my @lines = @{$self->{LINE_ENDPOINTS}};
    my $nl = @lines;
    $str .= "$nl,$np\n";
    foreach my $l (@lines) {
	$str .= "$l\n";
    }
    my @comps = @{$self->{COMPONENTS}};
    foreach my $c (@comps) {
	for ( my $l=0; $l<$nl; $l++ ) {
	    $str .= "add $c\n";
	}
    }

    if ( $self->{MATLAB_FILE} ) {
	$str.= "save results to a matlab file\n";
	my $f =$self->{MATLAB_FILE}; 
	$str.= "$f\n";
    }
    $str .= "pause\n";
    $str .= "exit this menu\n";
    return $str;
}

sub addLine() {
    my $self = shift;
    my $line = shift;
    push(@{$self->{LINE_ENDPOINTS}},$line);
}

END { };

if ( $0 =~ /LineOut.pm/ )
{
    my $lo = LineOut->new();
    push @{$lo->{COMPONENTS}}, ("rho","u","v","T");
    $lo->{NUMBER_OF_POINTS} = 101;
    $lo->{MATLAB_FILE} = "test.m";
    push @{$lo->{LINE_ENDPOINTS}}, "0,0,1,1";
    push @{$lo->{LINE_ENDPOINTS}}, ".25, 0.1, .7, .9";
    print $lo->cmdString();
}

1;
