#**************************************************************
#** perl script defining subroutines for creating stretched mappings
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
package StretchTransform;

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

our $className = "StretchTransform";

sub new {
    $#_ == 1 || die "a mapping to stretch must be given to StretchTransform->new\n";
    my $type = shift;
    my $self = Mapping->new;
    my $map_to_str = shift;
    $self->{STRETCH_TYPE} = ("","","");
    $self->{STRETCH_PARAMS} = ((),(),());
    $self->{MAPPING_TO_STRETCH} = $map_to_str;
    $self->{MAPPINGNAME} = "Stretched-".$map_to_str->{MAPPINGNAME};
    bless $self, $type;
    return $self;
}

sub cmdString {
    my $self = shift;
    my $str = "stretch coordinates\n";
    $str .= "transform which mapping?\n".$self->{MAPPING_TO_STRETCH}->{MAPPINGNAME}."\n";
    for (my $i=0; $i<= $#{$self->{STRETCH_TYPE}}; $i++ ) {
	my $stype = $self->{STRETCH_TYPE}[$i];
	if ($stype) {
	    my $ii = $i+1;
	    $str .= "Stretch r$ii:".$stype . "\n";
	    my @params = @{$self->{STRETCH_PARAMS}[$i]};
	    my $prefix = "STP:stretch r$ii $stype: ";
	    for my $p ( @params ) {
		$str .= "$prefix$p\n";
	    }
	}
    }
    $str .= $self->SUPER::cmdString();
    $str .= "stretch grid\n";
    $str .= "exit\n";
    return $str;
}

END {}

# print out the test string if we are executing this file, perl -I$Overture/include StretchTransform.pm
if ( ($0 =~ /StretchTransform.pm/) )
{
    use SquareMapping;
    $smap = SquareMapping->new();
    print $smap->cmdString();
    my $stretch = StretchTransform->new($smap);
    $stretch->{STRETCH_TYPE}[1] = "exp";
    push @{$stretch->{STRETCH_PARAMS}[1]},"cluster at r=1";
    $stretch->{STRETCH_TYPE}[0] = "itanh";
    push @{$stretch->{STRETCH_PARAMS}[0]},("layer 1 .5 10 0",
					   "layer 2 .5 20 .5");

    $stretch->{LINES} = "21 41 ";
    $stretch->{BC} = "1 2 3 4 ";
    $stretch->{SHARE} = "7 8 9 10 ";
#    $stretch->{MAPPINGNAME} = "TestStretch";
    print $stretch->cmdString();
}

1;
