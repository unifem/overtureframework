#**************************************************************
#** perl script defining subroutines for the Mapping base class
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 070713 - initial version
#**************************************************************
#**************************************************************
#
#
package Mapping;
# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;
        # if using RCS/CVS, this may be preferred
        $VERSION = do { my @r = (q$Revision: 1.2 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker
        @ISA         = qw(Exporter);
        @EXPORT      = qw( );#&createSmoothedPolygon);
        %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $className = "Mapping";

sub new() {
    my $type = shift;
    my $self = {};
    $self->{MAPPINGNAME} = "";
    $self->{LINES} = -1;
    $self->{BC} = -1;
    $self->{SHARE} = -1;
    $self->{PERIODICITY} = "";
    $self->{VERBATIM_COMMANDS} = "";
    return bless $self,$type;
}

sub cmdString() {
    my $self = shift;
    my $tab = "  ";
    my $str = "";
    if ( $self->{MAPPINGNAME} ) {
	$str .= $tab."mappingName\n$tab$tab$self->{MAPPINGNAME}\n";
    }
    if ( $self->{BC} ne -1 ) {
	$str.=$tab."boundary conditions\n$tab$tab$self->{BC}\n";
    }
    if ( $self->{LINES} ne -1 ) {
	$str.=$tab."lines\n$tab$tab$self->{LINES}\n";
    }
    if ( $self->{SHARE} ne -1 ) {
	$str.=$tab."share\n$tab$tab$self->{SHARE}\n";
    }
    if ( $self->{PERIODICITY} ) {
	$str.=$tab."periodicity\n".$self->{PERIODICITY}."\n";
    }
    if ( $self->{VERBATIM_COMMANDS} ) {
	$str.=$self->{VERBATIM_COMMANDS};
    }
    return $str;
}

END { };


# print out the test string if we are executing this file
if ( ($0 =~ /[^a-zA-Z]Mapping.pm/) )
{
    my $mp = Mapping->new();
    $mp->{LINES} = "21 41 41";
    $mp->{BC} = "1 2 3 4 5 6";
    $mp->{SHARE} = "7 8 9 10 11 12";
    $mp->{MAPPINGNAME} = "TestMapping";
    print $mp->cmdString();
}

1;
