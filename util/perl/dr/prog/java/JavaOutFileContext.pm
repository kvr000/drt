package dr::prog::java::JavaOutFileContext;

use strict;
use warnings;

use Data::Dumper;
use dr::Util qw(defvalue tabalign dumpSimple);


sub new
{
	my $classname			= shift; $classname = ref($classname) || $classname;
	my $model_store			= shift;
	my $sysimport_ctx		= shift;
	my $appimport_ctx		= shift;
	my $code_ctx			= shift;

	dr::Util::doDie("code_ctx undefined") unless ($code_ctx);

	my $this = bless {
		model_store			=> $model_store,
		sysimport_ctx			=> $sysimport_ctx,
		appimport_ctx			=> $appimport_ctx,
		code_ctx			=> $code_ctx,
	}, $classname;

	return $this;
}

sub printSysimport
{
	my $this			= shift;
	my $line			= shift;

	$this->{sysimport_ctx}->printOnce($line);
}

sub printAppimport
{
	my $this			= shift;
	my $line			= shift;

	$this->{appimport_ctx}->printOnce($line);
}

sub printCode
{
	my $this			= shift;
	my $line			= shift;

	$this->{code_ctx}->print($line);
}

sub getCodeIndented
{
	my $this			= shift;

	return $this->{code_ctx}->indentContext(1);
}

sub getSubIndented
{
	my $this			= shift;

	return $this->new($this->{model_store}, $this->{sysimport_ctx}, $this->{appimport_ctx}, $this->getCodeIndented());
}

sub dupWithFileContext
{
	my $this			= shift;
	my $file_context		= shift;

	return $this->new($this->{model_store}, $this->{sysimport_ctx}, $this->{appimport_ctx}, $file_context);
}


1;
