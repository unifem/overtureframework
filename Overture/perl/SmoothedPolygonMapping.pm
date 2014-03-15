#**************************************************************
#** perl script defining subroutines to for creating smoothed polygons
#**      for use in ogen
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 040127 - initial version
#**************************************************************
#**************************************************************
#*
#
#
package SmoothedPolygonMapping;

# most of this BEGIN block I got from:
# http://www.perldoc.com/perl5.8.0/pod/perlmod.html#Packages
BEGIN {
        use Exporter   ();
	use Mapping;
        our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
        # set the version for version checking
        $VERSION     = 1.00;
        # if using RCS/CVS, this may be preferred
        $VERSION = 1.00;
        @ISA         = qw(Mapping Exporter);
        @EXPORT      = qw( );
        %EXPORT_TAGS = ( );  
        # your exported package globals go here,
        # as well as any optionally exported functions
        @EXPORT_OK   = qw( );
}

our $className = "SmoothedPolygon";

sub new() {
    my $type = shift;
    my $self = Mapping->new;
    $self->{CORNERS} = [];
    $self->{SURFORCURVE} = 0; # 0=surf (default), 1=curve
    @{$self->{SHARPNESS}} = (); 
    @{$self->{T_STRETCH}} = (); 
    $self->{N_STRETCH} = "";
    $self->{NORMAL_DISTANCE} = ""; # only have a fixed normal distance for now
    $self->{MAPPINGNAME} = $className;
    $self->{CORRECT_CORNERS} =[]; # specify the 4 corners
    bless $self,$type;
    return $self;
}

sub ogenHeader {
    my $self = shift;
    return "smoothedPolygon\n";
};

sub updateString {
    my $self = shift;
    my $tab = "   ";
    my @crn = @{$self->{CORNERS}};
    my @shrp = @{$self->{SHARPNESS}};
    my @tstr = @{$self->{T_STRETCH}};
    my $nc = @crn;
    my $ns = @shrp;
    my $nts = @tstr;
    my $str = "$tab vertices\n$tab $nc\n";
    for my $p ( @crn ) {
	$str = $str.$tab."$p\n";
    }

    if ( $self->{SURFORCURVE}==1 ) {
	$str = $str.$tab."curve or area (toggle)\n";
    }

    if ( $ns == $nc ) {
	$str = $str.$tab."sharpness\n";
	for ( my $i=0; $i<$nc; $i++ ) {
	    my $s = $shrp[$i];
	    $str = $str."$tab$s\n";
	}
    } elsif ( $ns==1 ) {
	$str .= $tab."sharpness\n";
	my $s = $shrp[0];
	for ( my $i=0; $i<$nc; $i++ ) {
	    $str = $str."$tab$s\n";
	}
    }	

    if ( $nts == $nc ) {
	$str .= $tab."t-stretch\n";
	for ( my $i=0; $i<$nc; $i++ ) {
	    my $s = $tstr[$i];
	    $str = $str."$tab$s\n";
	}
    } elsif ( $nts==1 ) {
	$str = $str.$tab."t-stretch\n";
	my $s = $tstr[0];
	for ( my $i=0; $i<$nc; $i++ ) {
	    $str = $str."$tab$s\n";
	}
    }
    
    if ( $self->{N_STRETCH} )
    {
	$str.=$tab."n-stretch\n$tab$self->{N_STRETCH}\n";
    } 

    if ( $self->{NORMAL_DISTANCE} ) {
	$str .= $tab."n-dist\n${tab}fixed normal distance\n${tab}$self->{NORMAL_DISTANCE}\n";
    }

    if ( $self->{CORRECT_CORNERS} ) {
	my @ccrn = @{$self->{CORRECT_CORNERS}};
	my @x00 = $ccrn[0];
	my @x10 = $ccrn[1];
	my @x01 = $ccrn[2];
	my @x11 = $ccrn[3];
	$str.=<<END_OF_STRING;
	corners
	specify positions of corners
        $x00[0] $x00[1]
        $x01[0] $x01[1]
	$x10[0] $x10[1]
	$x11[0] $x11[1]
END_OF_STRING
    }

    $str .= $self->SUPER::cmdString();

    return $str;
} 

sub cmdString {
    my $self = shift;
    return $self->ogenHeader().$self->updateString()."exit\n";
}

END { };

# print out the test string if we are executing this file
if ( ($0 =~ /SmoothedPolygonMapping.pm/) )
{
    my $smoothedPolygon = SmoothedPolygonMapping->new();
    push @{$smoothedPolygon->{CORNERS}}, "0,0";
    push @{$smoothedPolygon->{CORNERS}}, "0,1";
    push @{$smoothedPolygon->{CORNERS}}, "1,1";
    $smoothedPolygon->{LINES} = "21 41 ";
    $smoothedPolygon->{BC} = "1 2 3 4 ";
    $smoothedPolygon->{SHARE} = "7 8 9 10 ";
    $smoothedPolygon->{MAPPINGNAME} = "TestSmoothedPolygon";
    $smoothedPolygon->{NORMAL_DISTANCE} = -.02;
    push @{$smoothedPolygon->{T_STRETCH}}, qw(1);
    print $smoothedPolygon->cmdString();
}

1;
