package dr::prog::gen::CopyGenerator;

use strict;
use warnings;

use base qw(dr::prog::gen::GenericGenerator);

sub process
{
	my $this			= shift;

	my $output = $this->{file_trans}->updateChanged("$this->{gen_dir}/$this->{in_fname}");

	while (defined (my $line = $this->{in_parser}->readLine())) {
		$output->print($line);
	}
	$output->flush();

	return 0;
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
