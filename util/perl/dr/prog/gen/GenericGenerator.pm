package dr::prog::gen::GenericGenerator;

use strict;
use warnings;

sub new
{
	my $ref				= shift; my $classname = ref($ref) || $ref;
	my $file_trans			= shift;

	my $this = bless {
		file_trans			=> $file_trans,
		model_store			=> shift,
		gen_dir				=> shift,
		in_dir				=> shift,
		in_fname			=> shift,
	}, $ref;

	$this->{in_parser} = dr::FileParser->new("$this->{in_dir}/$this->{in_fname}");

	return $this;
}

sub process
{
	die "abstract method";
}

sub getContext
{
	my $this		= shift;

	return $this->{in_parser}->getContext();
}

sub dieContext
{
	my $this		= shift;
	my $context		= shift;
	my $msg			= shift;

	dr::Util::doDie("$context: $msg");
}


1;
