package dr::prog::java::JavaOutFileContext;

use strict;
use warnings;

use Data::Dumper;
use dr::Util qw(defvalue tabalign dumpSimple);


sub new
{
	my $classname			= shift; $classname = ref($classname) || $classname;
	my $model_store			= shift;
	my $sysimportCtx		= shift;
	my $appimportCtx		= shift;
	my $codeCtx			= shift;

	dr::Util::doDie("codeCtx undefined") unless ($codeCtx);

	my $this = bless {
		model_store			=> $model_store,
		sysimportCtx			=> $sysimportCtx,
		appimportCtx			=> $appimportCtx,
		codeCtx				=> $codeCtx,
	}, $classname;

	return $this;
}

sub getSysimportCtx
{
	my $this			= shift;

	return $this->{sysimportCtx};
}

sub printSysimport
{
	my $this			= shift;
	my $line			= shift;

	$this->{sysimportCtx}->printOnce($line);
}

sub printAppimport
{
	my $this			= shift;
	my $line			= shift;

	$this->{appimportCtx}->printOnce($line);
}

sub printCode
{
	my $this			= shift;
	my $line			= shift;

	$this->{codeCtx}->print($line);
}

sub getCodeIndented
{
	my $this			= shift;

	return $this->{codeCtx}->indentContext(1);
}

sub getSubIndented
{
	my $this			= shift;

	return $this->new($this->{model_store}, $this->{sysimportCtx}, $this->{appimportCtx}, $this->getCodeIndented());
}

sub dupWithFileContext
{
	my $this			= shift;
	my $file_context		= shift;

	return $this->new($this->{model_store}, $this->{sysimportCtx}, $this->{appimportCtx}, $file_context);
}


1;
