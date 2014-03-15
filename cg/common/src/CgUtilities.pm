#**************************************************************
#** perl module for Cg
#**************************************************************
#**************************************************************
#** Kyle Chand
#** 091022 - initial version
#**************************************************************
#**************************************************************
package CgUtilities;

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
        @EXPORT = qw( set generate_docs add_module 
		      twilightCmd
		      $axis1 $axis2 $axis3 $side1 $side2 
		      $ON $OFF $TRUE $FALSE
		      $TZ_Trig $TZ_Poly $TZ_Pulse );
        @EXPORT_OK   = qw( );
}

our @cg_modules; # list containing the modules to generate docs for

our $axis1 = 0;
our $axis2 = 1;
our $axis3 = 2;
 
our $side1 = 0;
our $side2 = 1;

our $ON = "on";
our $OFF = "off";
our $TRUE = 1;
our $FALSE = 0;

our $TZ_Trig = "OBTZ:trigonometric";
our $TZ_Poly = "OBTZ:polynomial";
our $TZ_Pulse= "OBTZ:pulse";
add_module(twilight);

${DOC_SHORT}{twilight} = "Parameters that specify twilight zone forcing";
##${DOC}{TWILIGHT}
$twilight{TWILIGHT_ZONE} = $OFF;
$twilight{DOC_SHORT}{TWILIGHT_ZONE} = "if set to \\\$ON, activate the twilight zone forcing";
$twilight{TWILIGHT_TYPE} = $TZ_Poly;
$twilight{DOC_SHORT}{TWILIGHT_TYPE} = "set to one of \\\$TZ_Trig, \\\$TZ_Poly, or \\\$TZ_Pulse";
$twilight{TWILIGHT_DEGREE_TIME} = 2;
$twilight{DOC_SHORT}{TWILIGHT_DEGREE_TIME} = "polynomial degree for time dependent part of the forcing";
$twilight{TWILIGHT_DEGREE_SPACE} = 2;
$twilight{DOC_SHORT}{TWILIGHT_DEGREE_SPACE} = "polynomial degree for space forcing";
@{$twilight{TWILIGHT_FREQUENCIES}} = (2, 2, 2, 2);
$twilight{DOC_SHORT}{TWILIGHT_FREQUENCIES} = "frequencies for the trigonometric forcing, in order of axis1, axis2, axis3, time";
$twilight{VERBATIM_CMD} = "# ";
$twilight{DOC_SHORT}{VERBATIM_CMD} = "custom commands given directly to the interpreter";

sub twilightCmd() {
    $str =<<END_OF_STRING;
    turn $twilight{TWILIGHT_ZONE} twilight
         OBTZ:degree in space $twilight{TWILIGHT_DEGREE_SPACE}
         OBTZ:degree in time $twilight{TWILIGHT_DEGREE_TIME}
         OBTZ:frequencies (x,y,z,t) $twilight{TWILIGHT_FREQUENCIES}[0] $twilight{TWILIGHT_FREQUENCIES}[1] $twilight{TWILIGHT_FREQUENCIES}[2] $twilight{TWILIGHT_FREQUENCIES}[3]
         OBTZ:$twilight{TWILIGHT_TYPE}
         $twilight{VERBATIM_CMD};
END_OF_STRING
    return $str;
}

######
## set the value(s) of a module's variable, the syntax is:
## scalars:
##   set module_name, VARIABLE_NAME, value;
##   - or -
##   set(module_name, VARIABLE_NAME, value);
## arrays:
##   set module_name, VARIABLE_NAME, val0, val1, val2;
##   - or -
##   set(module_name, VARIABLE_NAME, val0, val1, val2);
sub set($$@)  {

    my $hash = shift;
    my $key  = shift;
    my @val  = @_;
    $hash ne "" or die "ERROR : ($hash) no input grouping was specified, maybe there is an erroneous comma after the keyword set?\n";

    exists ${$hash}{$key} or die "ERROR : could not find a parameter $key in input parameter group $hash!\n";

    my $num_vals = @val;
    if ( $num_vals==1 )
    {
	${$hash}{$key} = $val[0];
    }
    else
    {
	my $orig_num = @{${$hash}{$key}};
	if ( $orig_num==0 )
	{
	    push @{${$hash}{$key}}, @val;
	}
	else
	{
	    @{${$hash}{$key}} = @val;
	}
    }
}
######

########
## add a module (string) to the list of modules for automatic doc generation
## note that the module should correspond to a hash table name
sub add_module($) {
    my $mod = shift;
    push @cg_modules, $mod;
}
######

######
## Generate latex documentation for the variables in each module.
## This subroutine expects documentation strings (i.e. latex strings) in the following form:
##  ${DOC_SHORT}{$module_name} = "a short description of the module (i.e. subsection heading)"
##  ${DOC}{$module_name} =  "a long description of the module"
##  $module_name{DOC_SHORT}{$variable_name} = "a short description of the variable or function
##  $module_name{DOC}{$variable_name} = "a long description of the variable or function"
sub generate_docs() {

    foreach my $mod ( @cg_modules )
    {
	my $doc_short = ${DOC_SHORT}{$mod};
	my $doc = ${DOC}{$mod};
	print "\\subsection{{\\tt $mod}: $doc_short}\n";
	print $doc."\n";
	my @keys = sort keys %{$mod};
	foreach my $key (@keys)
	{
	    if ( $key ne "DOC" && $key ne "DOC_SHORT") 
	    {
		my $doc_short = exists ${$mod}{DOC_SHORT}{$key} ? ${$mod}{DOC_SHORT}{$key} : "";
		my $doc = exists ${$mod}{DOC}{$key} ? ${$mod}{DOC}{$key} : "";
		if ( $doc_short ne "" || $doc ne "" )
		{
		    my $default_value = "${$mod}{$key}";
		    if ( !$default_value ) { $default_value = "<none>";}
		    my $is_array = "";
		    if ( $default_value =~ /ARRAY/ )
		    { # here is a dumb way to find out if we have a scalar or array, is there a better way?
			$is_array  = 1;
			my @default_values = @{${$mod}{$key}};
			my $nvals = @default_values;
			$default_value = "";
			if ( $nvals ) {
			    for ( my $v=0; $v<$nvals-1; $v++ )
			    {
				$default_value .= "\"$default_values[$v]\", ";
			    }
			    $default_value .= "\"$default_values[$#default_values]\"";
			}else {
			    $default_value = "<none>";
			}
		    }
		    
					 
		    $_ = $key;
		    s/_/\\_/g ;
		    my $key_tex = $_;
		    $_ = $default_value;
		    s/_/\\_/g ;
		    $default_value_no_under = $_;
		    print "\\subsubsection[$key_tex]{$key_tex : $doc_short}\n\n";
		    print "{\\noindent\\it default value: }{\\tt $default_value_no_under}\\\\ \n";
		    print $doc."\n";
		    if ( $default_value ne "<none>" ) {
			print "\\noindent\n{\\it Examples: } \n\\begin{alltt}\\leftskip=1in\n";
			print "set $mod, $key, $default_value;\n";
			if ( !$is_array )
			{
			    print "\$$mod\\{$key\\} = $default_value;\n";
			}
			else
			{
			    print "\@{\$$mod\\{$key\\}} = ($default_value);\n";
			}
			print "\\leftskip=0in\n\\end{alltt}\%$\n \n\\noindent\n";
		    } # there is a default value
		} # there is documentation 
	    } # if there is documentation
	} # for each key in the module's hash
    } # for each input module

}
######

END { };

## if this script is executed directly then print stuff out for testing
if ( ($0 =~ /CgUtilities.pm/) )
{
    print ("$axis1\n$TZ_Poly\n");
    print(twilightCmd());
#    generate_docs();
}

# the following line must be here or else the import will fail
1; 
