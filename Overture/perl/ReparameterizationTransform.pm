#**************************************************************
#** perl script defining subroutines for creating reparameterized mappings
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
package ReparameterizationTransform;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
	use Mapping;
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;

        @ISA         = qw(Mapping Exporter);
        @EXPORT      = qw( $IDENTITY $ORTHOGRAPHIC 
			   $RESTRICTION $EQUIDISTRIBUTION $REORIENT
			   $NORTH_POLE $SOUTH_POLE cmdString );
        %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $className = "ReparameterizationTransform";

our $IDENTITY = "";
our $ORTHOGRAPHIC = "orthographic";
our $NORTH_POLE = 1;
our $SOUTH_POLE = -1;
our $RESTRICTION = "restrict parameter space";
our $EQUIDISTRIBUTION = "equidistribution";
our $REORIENT = "reorient domain coordinates";

sub new {
    $#_ == 1 || die "a mapping to reparameterization must be given to ReparameterizationTransform->new\n";
    my $type = shift;
    my $self = Mapping->new;
    my $map_to_str = shift;

    $self->{TYPE} = $IDENTITY;
    %{$self->{ORTHOGRAPHIC}} = (SA=>1, SB=>1,
				POLE=>$NORTH_POLE,
				ANGULAR_AXIS=>"axis1");
    %{$self->{RESTRICTION}} = (RA=>0, RB=>1,
			       SA=>0, SB=>1,
			       TA=>0, TB=>1);
    %{$self->{EQUIDISTRIBUTION}} = ( ARCLENGTH_WGT=>1,
				     CURVATURE_WGT=>0,
				     NUM_SMOOTHS=>3 );
				  
    %{$self->{REORIENT}} = (DIR1=>0,
			    DIR2=>1,
			    DIR3=>2);

    $self->{MAPPING_TO_REPARAMETERIZE} = $map_to_str;
    $self->{MAPPINGNAME} = "Reparameterized-".$map_to_str->{MAPPINGNAME};
    bless $self, $type;
    return $self;
}

sub cmdString {
    my $self = shift;
    my $str = "reparameterize\n";
    $str .= "transform which mapping?\n".$self->{MAPPING_TO_REPARAMETERIZE}->{MAPPINGNAME}."\n";
	print "RESTRICTION = $RESTRICTION, $$self{TYPE}\n";
    if ( $self->{TYPE} eq $ORTHOGRAPHIC )
    {
	$str.=<<END_OF_STRING;
$ORTHOGRAPHIC
  specify sa,sb
  $self->{ORTHOGRAPHIC}{SA}, $self->{ORTHOGRAPHIC}{SB}
  choose north or south pole
  $self->{ORTHOGRAPHIC}{POLE}
  angular axis = $self->{ORTHOGRAPHIC}{ANGULAR_AXIS}
END_OF_STRING
    }
    elsif ( $self->{TYPE} eq $RESTRICTION )
    {
	my %rp = %{$self->{RESTRICTION}};
	$str.=<<END_OF_STRING;
$RESTRICTION
  set corners
  $rp{RA}, $rp{RB}, $rp{SA}, $rp{SB}, $rp{TA}, $rp{TB}
END_OF_STRING
    }
    elsif ( $self->{TYPE} eq $EQUIDISTRIBUTION )
    {
	my %rp = %{$self->{EQUIDISTRIBUTION}};
	$str.=<<END_OF_STRING;
$EQUIDISTRIBUTION
    arclength weight
    $rp{ARCLENGTH_WGT}
    curvature weight
    $rp{CURVATURE_WGT}
    number of smooths 
    $rp{NUM_SMOOTHS}
    re-evaluate equidistribution
END_OF_STRING
    }
    elsif ( $self->{TYPE} eq $REORIENT )
    {
	$str.=<<END_OF_STRING;
$REORIENT
    $self->{REORIENT}{DIR1}, $self->{REORIENT}{DIR2}, $self->{REORIENT}{DIR3}
END_OF_STRING
    }
    elsif ( $self->{TYPE} ne $IDENTITY )
    {
	die "Unknown reparameterization given to mapping $self->{MAPPINGNAME} : $self->{TYPE}!\n";
    }
    $str .="exit\n";
    $str .= $self->SUPER::cmdString();
    $str .= "exit\n";
    return $str;
}

END {}

# print out the test string if we are executing this file
if ( ($0 =~ /ReparameterizationTransform.pm/) )
{
    use SquareMapping;
    $smap = SquareMapping->new();
    print $smap->cmdString();
    my $reparam = ReparameterizationTransform->new($smap);
    $reparam->{TYPE} = $ORTHOGRAPHIC;
    $reparam->{ORTHOGRAPHIC}{SA} = .5;
    $reparam->{LINES} = "21 41 ";
    $reparam->{BC} = "1 2 3 4 ";
    $reparam->{SHARE} = "7 8 9 10 ";
#    $reparam->{MAPPINGNAME} = "TestReparameterization";
    print $reparam->cmdString();
    $reparam->{TYPE} = $RESTRICTION;
    $reparam->{RESTRICTION}{SB} = .5;
    print $reparam->cmdString();
    $reparam->{TYPE} = $EQUIDISTRIBUTION;
    $reparam->{EQUIDISTRIBUTION}{CURVATURE_WGT} = 1;
    print $reparam->cmdString();
    $reparam->{TYPE} = $REORIENT;
    $reparam->{REORIENT}{DIR1} = 1;
    $reparam->{REORIENT}{DIR2} = 2;
    $reparam->{REORIENT}{DIR3} = 0;
    print $reparam->cmdString();
    
}

1;
