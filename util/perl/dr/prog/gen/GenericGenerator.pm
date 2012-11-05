package dr::prog::gen::GenericGenerator;

use strict;
use warnings;

sub new
{
	my $ref			= shift; my $classname = ref($ref) || $ref;
	my $file_trans		= shift;

	my $this = bless {
		file_trans		=> $file_trans,
		gen_dir			=> shift,
		in_dir			=> shift,
		in_fname		=> shift,
	}, $ref;

	$this->{in_parser} = dr::FileParser->new("$this->{in_dir}/$this->{in_fname}");

	return $this;
}

sub process
{
	die "abstract method";
}


1;
