package dr::prog::java::JavaOutputGenerator;

use strict;
use warnings;

use Data::Dumper;
use dr::Util qw(defvalue tabalign dumpSimple);

sub new
{
	my $classname			= shift;
	my $out_context			= shift;

	my $this = bless {
		out_context			=> $out_context,
	}, $classname;

	return $this;
}

sub generateNested
{
	my $this			= shift;
	my $model			= shift;

	if ($model->{stype} eq "typedef") {
		# ignore, java (unfortunately) does not have typedefs
	}
	elsif ($model->{stype} eq "enum") {
		$this->generateEnum($model);
	}
	else {
		dr::Util::doDie("unknown nested stype: $model->{stype} for class $model->{full}");
	}
}

sub generateEnum
{
	my $this			= shift;
	my $model			= shift;

	my $out_context = $this->{out_context};

	$out_context->printCode("public static class $model->{name}\n{\n");
	my $inner = $out_context->getCodeIndented();
	foreach my $literal (@{$model->getLiterals()}) {
		$inner->print(tabalign("public static final int", 32).tabalign("$literal->{name}", 32)."= $literal->{value};\n");
	}
	$out_context->printCode("};\n");
}


1;
