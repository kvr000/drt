package dr::prog::java::JavaGenerator;

use strict;
use warnings;

use Data::Dumper;
use dr::Util qw(defvalue tabalign dumpSimple);
use dr::prog::gen::ClikeGenerator qw(:CMT);

use base qw(dr::prog::gen::ClikeGenerator);

our %JAVA_TAGS = (
	model_subs		=> CMT_LINE,
	type			=> CMT_LINE,
	shortcut		=> CMT_LINE,
);

sub new
{
	my $classname		= shift;

	my $this = $classname->SUPER::new(@_);
	$this->{shortcut} = {};
	$this->{pending} = [];
	$this->{imports} = {};
	$this->{package_name} = undef;
	$this->{class_name} = undef;

	return $this;
}

our %JAVA_UML_TYPES = (
	void			=> "void",
	string			=> "String",
	int			=> "Integer",
	Integer			=> "Integer",
	long			=> "Long",
	Long			=> "Long",
	float			=> "Double",
	Float			=> "Double",
	double			=> "Double",
	Double			=> "Double",
	boolean			=> "Boolean",
	Boolean			=> "Boolean",
);

sub mapJavaType
{
	my $type			= shift;

	my $primtype = $type->{type};
	if (!defined $primtype) {
		return "int" if ($type->{stype} eq "enum");
		dr::Util::doDie("primtype undefined for $type->{full}");
	}
	elsif ($primtype =~ m/^(\w+)\((\d+)\)$/) {
		if (my $javat = $JAVA_UML_TYPES{$1}) {
			return $javat;
		}
		die "cannot map $primtype";
	}
	else {
		if (my $javat = $JAVA_UML_TYPES{$primtype}) {
			return $javat;
		}
		die "cannot map $primtype";
	}
}

sub mapJavaAttrType
{
	my $attr			= shift;

	my ( $atype, $atagger ) = $attr->getFinalTypeWithTagger();

	return mapJavaType($atype);
}

sub getFullClassname
{
	my $this			= shift;
	my $name			= shift;

	if (defined $this->{imports}->{$name}) {
		return $this->{imports}->{$name};
	}
	elsif ($name =~ m/^([a-z]+)\./) {
		return $name;
	}
	else {
		return "$this->{package}.$name";
	}
}

sub loadModel
{
	my $this			= shift;
	my $name			= shift;

	return $this->{model_store}->loadModel(undef, $this->getFullClassname($name));
}

sub openTargets
{
	my $this		= shift;

	return $this->{file_trans}->createTruncated("$this->{gen_dir}/$this->{in_fname}");
}

sub finish
{
	my $this		= shift;

	$this->SUPER::finish();
}

sub processLine
{
	my $this		= shift;
	my $line		= shift;

	if ($line =~ m/^package\s+((\w+\.)*\w+);/) {
		$this->{package_name} = $1;
		$this->printLine($line);
	}
	elsif ($line =~ m/^\s*import\s*((\S*)\.(\w+))\s*;\s*$/) {
		$this->processImport($line, $1);
	}
	elsif ($line =~ m/^{/) {
		return $this->processClassStart($line);
	}
	elsif ($line =~ m/^}/) {
		return $this->processClassEnd($line);
	}
	else {
		while ($line =~ m/^(.*?)DRTYPE\((\w+)\.(\w+)\)(.*)$/s) {
			my ( $start, $type, $attr, $end ) = ( $1, $2, $3, $4 );
			my $classmodel = $this->loadModel($type);
			my $translated = mapJavaAttrType($classmodel->getAttr($attr));
			$line = "$start$translated$end";
		}
		if ($line =~ m/^((public|private|protected)\s+|)((abstract)\s+|)class\s+(\w+)(\s*<.*>)?\s+extends\s+((\w+\.)*\w+)(\s+.*|)$/) {
			$this->{class_name} = $5;
			$this->{ancestor_name} = $7;
			$this->processClassDef($line);
		}
		elsif ($line =~ m/^\s*$/) {
			$this->printLine($line);
		}
		elsif ($line =~ m,^\s+.*\babstract\b\)\s*;\s*(//.*|/\*.*|)$,) {
			$this->processMethodAbstract($line);
		}
		elsif ($line =~ m,^\s+.*\)\s*;\s*(//.*|/\*.*|)$,) {
			$this->processMethodDef($line);
		}
		elsif ($line =~ m/^\s*((public|protected|private)\s+)*([a-zA-Z_.]+)\s+(\w+)(|\s*=.*);.*$/) {
			$this->processAttributeDef($line);
		}
		else {
			$this->processRegularLine($line);
		}
	}

	return 0;
}

sub processCommentLine
{
	my $this		= shift;
	my $cmtag		= shift;
	my $content		= shift;

	if ($cmtag eq "shortcut") {
		if ($content !~ m/^(\w+)=(.+?)\s*$/) {
			$this->printError("wrong shortcut definition: $content");
			return 1;
		}
		$this->{shortcut}->{$1} = $2;
	}
	elsif ($cmtag eq "model_subs") {
	}
	else {
		$this->SUPER::processCommentLine($cmtag, $content);
	}

	return 0;
} 
sub processCommentBlock
{
	my $this		= shift;
	my $cmtag		= shift;
	my $head		= shift;
	my $block		= shift;

	if (0) {
	}
	else {
		$this->SUPER::processCommentBlock($cmtag, $head, $block);
	}

	return 0;
}

sub processCommentContinuous
{
	my $this		= shift;
	my $cmtag		= shift;
	my $head		= shift;
	my $block		= shift;

	if (0) {
	}
	else {
		$this->SUPER::processCommentContinuous($cmtag, $head, $block);
	}

	return 0;
}

sub processCommentSingle
{
	my $this		= shift;
	my $cmtag		= shift;
	my $content		= shift;

	if ($cmtag eq "getset") {
		foreach (split(/\s+/, $content)) {
			push(@{$this->{pending}}, { context => $this->getContext(), name => "getset", value => $_ });
		}
	}
	else {
		$this->SUPER::processCommentSingle($cmtag, $content);
	}

	return 0;
}

sub getCommentType
{
	my $this			= shift;
	my $cmtag			= shift;
	my $content			= shift;

	return exists $JAVA_TAGS{$cmtag} ? $JAVA_TAGS{$cmtag} : CMT_UNKNOWN;
}

sub processImport
{
	my $this			= shift;
	my $line			= shift;

	dr::Util::doDie("did not find import specification: $line") unless ($line =~ m/^\s*import\s*((\S*)\.(\w+))\s*;\s*$/);
	$this->{imports}->{$3} = $1;
	$this->{import_ctx} = $this->{out_main}->rememberContext() if (!defined $this->{import_ctx});
	$this->{import_ctx}->printOnce("import $1;\n");

	return 0;
}

sub processClassDef
{
	my $this			= shift;
	my $line			= shift;

	$this->printLine($line);

	return 0;
}

sub processClassStart
{
	my $this			= shift;
	my $line			= shift;

	die "did not find class specification" unless (defined $this->{class_name});
	die "did not find ancestor specification" unless (defined $this->{ancestor_name});
	die "did not find package specification" unless (defined $this->{package_name});

	$this->{class_started} = 1;

	$this->printLine($line);

	return 0;
}

sub processClassEnd
{
	my $this		= shift;
	my $line		= shift;

	die "did not find class start" unless ($this->{class_started});

	$this->printLine($line);

	return 0;
}

sub processMethodDef
{
	my $this		= shift;
	my $line		= shift;

	$this->printLine($line);

	return 0;
}

our %ATTR_TAGS = (
	getset			=> "processAttributeDefGetset",
);

sub processAttributeDef
{
	my $this		= shift;
	my $line		= shift;

	$this->dieContext($this->getContext(), "failed to match attribute definition: $line") unless ($line =~ m/^\s*((public|protected|private)\s+)*([a-zA-Z_.]+)\s+(\w+)(|\s*=.*);.*$/);
	my ( $atype, $aname ) = ( $3, $4 );

	foreach my $pending (@{$this->{pending}}) {
		if (exists $ATTR_TAGS{$pending->{name}}) {
			my $func = $ATTR_TAGS{$pending->{name}};
			$this->$func($atype, $aname, $pending);
		}
		else {
			$this->dieContext($pending->{context}, "unknown tag $pending->{name}");
		}
	}
	$this->{pending} = [];
	$this->printLine($line);
}

sub processAttributeDefGetset
{
	my $this		= shift;
	my $atype		= shift;
	my $aname		= shift;
	my $oper		= shift;

	if ($oper->{value} eq "get") {
		$this->printIndented(tabalign("public $atype", 32)."get".ucfirst($aname)."()\n{\n\treturn this.$aname;\n}\n\n");
	}
	elsif ($oper->{value} eq "set") {
		$this->printIndented(tabalign("public void", 32)."set".ucfirst($aname)."($atype ${aname}_)\n{\n\tthis.$aname = ${aname}_;\n}\n\n");
	}
	else {
		$this->dieContext($oper->{context}, "invalid value for getset generator: $oper->{value}");
	}
}

sub processEnd
{
	my $this		= shift;

	if (@{$this->{pending}}) {
		foreach my $pending (@{$this->{pending}}) {
			$this->dieContext($pending->{context}, "found unprocessed tag $pending->{name}");
		}
	}
}


1;
