###
## drt library
##
## drt multiplatform development toolkit
##
## ----------------------------------------------------------------------------------
##
## Copyright (C) 2004-2008 Zbyněk Vyškovský
##
## ----------------------------------------------------------------------------------
##
## LICENSE:
##
## This file is part of drt
##
## drt is free software; you can redistribute it and/or modify it under the
## terms of the GNU Lesser General Public License as published by the Free
## Software Foundation; either version 3 of the License, or (at your option)
## any later version.
##
## drt is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
## more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with drt; if not, write to the Free Software Foundation, Inc., 51
## Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##
## @copyright	2004-2008 Zbyněk Vyškovský
## @link	mailto:kvr@matfyz.cz
## @link	http://kvr.matfyz.cz/drt/
## @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
###

package dr::ModelStore::Util;

use strict;
use warnings;

use Scalar::Util;
use Data::Dumper;

sub genericLoad
{
	my $obj			= shift;
	my $reader		= shift;
	my $splitter		= shift;

	while (my ($k, $v) = $reader->getPair()) {
		if (my $func = $splitter->{$k}) {
			&$func($obj, $reader, $k, $v);
		}
		else {
			dr::Util::doDie("unknown key '$k': ".Dumper($splitter));
		}
	}
}

sub genericDrtag
{
	my $val			= shift;

	dr::Util::doDie("invalid format of drtag: $val") unless ($val =~ m/^(\w+)(\((\w+)\))?\s+(\S.*)$/s);
	return {
		name			=> $1,
		spec			=> $3,
		value			=> $4,
	};
}


package dr::ModelStore::Drtag;

use strict;
use warnings;

sub new
{
	my $ref			= shift; my $class = ref($ref) || $ref;

	my $this = bless {
		drtag_list		=> [],
		drtag_hash		=> {},
		drtag_spec		=> {},
		drtag_cnt		=> 0,
	}, $class;

	return $this;
}

sub addTag
{
	my $this		= shift;
	my $val			= shift;

	my $drtag = dr::ModelStore::Util::genericDrtag($val);
	$drtag->{order} = ++$this->{drtag_cnt};
	if (defined $drtag->{spec}) {
		$this->{drtag_spec}->{$drtag->{name}}->{$drtag->{spec}} = $drtag;
	}
	else {
		push(@{$this->{drtag_list}}, $drtag);
		$this->{drtag_hash}->{$drtag->{name}} = $drtag;
	}
}

sub getTag
{
	my $this		= shift;
	my $tag			= shift;

	if (defined (my $val = $this->{drtag_hash}->{$tag})) {
		return $val;
	}
	dr::Util::doDie("tag '$tag' not found");
}

sub getTagValue
{
	my $this		= shift;
	my $tag			= shift;

	if (defined (my $val = $this->{drtag_hash}->{$tag})) {
		return $val->{value};
	}
	dr::Util::doDie("tag '$tag' not found");
}

sub checkTag
{
	my $this		= shift;
	my $tag			= shift;

	return $this->{drtag_hash}->{$tag} if (defined $this->{drtag_hash}->{$tag});
	return;
}

sub checkTagValue
{
	my $this		= shift;
	my $tag			= shift;

	return $this->{drtag_hash}->{$tag}->{value} if (defined $this->{drtag_hash}->{$tag});
	return;
}

sub getSpec
{
	my $this		= shift;
	my $tag			= shift;

	if (defined (my $spec = $this->{drtag_spec}->{$tag})) {
		return $spec;
	}
	return {};
}

sub cloneTagger
{
	my $this		= shift;

	my $dup = $this->new();

	$dup->{drtag_list} = [ @{$this->{drtag_list}} ];
	$dup->{drtag_hash} = { %{$this->{drtag_hash}} };
	foreach (keys %{$this->{drtag_spec}}) {
		$dup->{drtag_spec}->{$_} = { %{$this->{drtag_spec}->{$_}} };
	}
	$dup->{drtag_cnt} = $this->{drtag_cnt};

	return $dup;
}

sub mergeReplacing
{
	my $this		= shift;
	my $merge		= shift;

	foreach (@{$merge->{drtag_list}}) {
		if ($this->{drtag_hash}->{$_->{name}}) {;
			for (my $i = 0; ; $i++) {
				splice(@{$this->{drtag_list}}, $i, 1), last if ($this->{drtag_list}->[$i]->{name} eq $_->{name});
			}
		}
		unshift(@{$this->{drtag_list}}, $_);
		$this->{drtag_hash}->{$_->{name}} = $_;
	}
	foreach (keys %{$merge->{drtag_spec}}) {
		my $spec = $merge->{drtag_spec};
		my $tspec;
		$this->{drtag_spec} = $tspec = {} unless (defined ($tspec = $this->{drtag_spec}->{$spec}));
		foreach (keys %{$spec}) {
			next if (defined $tspec->{$_});
			$tspec->{$_} = $spec->{$_};
		}
	}
}

sub mergeAdding
{
	my $this		= shift;
	my $merge		= shift;

	foreach (@{$merge->{drtag_list}}) {
		next if ($this->{drtag_hash}->{$_->{name}});
		push(@{$this->{drtag_list}}, $_);
		$this->{drtag_hash}->{$_->{name}} = $_;
	}
	foreach (keys %{$merge->{drtag_spec}}) {
		my $spec = $merge->{drtag_spec};
		my $tspec;
		$this->{drtag_spec} = $tspec = {} unless (defined ($tspec = $this->{drtag_spec}->{$spec}));
		foreach (keys %{$spec}) {
			next if (defined $tspec->{$_});
			$tspec->{$_} = $spec->{$_};
		}
	}
}


package dr::ModelStore::ClassBase;

use strict;
use warnings;

use Data::Dumper;

sub new
{
	my $ref			= shift; my $class = ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = bless {
		owner			=> $owner,
		location		=> $basic->{location},
		package			=> $basic->{package},
		name			=> $basic->{name},
		filename		=> $basic->{filename},
		full			=> "$basic->{package}::$basic->{name}",
		stype			=> $basic->{stype},
		nested_names		=> [],
		comment			=> [],
		drtag			=> dr::ModelStore::Drtag->new(),
	}, $class;

	$owner->{class_hash}->{"$basic->{package}::$basic->{name}"} = $this;
	Scalar::Util::weaken($this->{owner});

	return $this;
}

sub dieContext
{
	my $this			= shift;
	my $msg				= shift;
	my $cause			= shift;

	dr::Util::doDie(((defined $cause)) ? "$this->{file_context}: $msg\n$cause" : "$this->{file_context}: $msg");
}

our %BASE_MAPPER = (
	nested				=> \&readNested,
	drtag				=> \&readBaseDrtag,
	comment				=> \&readBaseComment,
);

sub getPackageName
{
	my $this			= shift;
	my $separator			= shift;

	my $package = $this->{package};
	$package =~ s/::/$separator/g;
	return $package;
}

sub getPackageDotName
{
	return shift->getPackageName(".");
}

sub getPackageCxxName
{
	return shift->getPackageName("::");
}

sub getPackageBSlashName
{
	return shift->getPackageName("\\");
}

sub getFullDotName
{
	return shift->getFullName(".");
}

sub getFullCxxName
{
	return shift->getFullName("::");
}

sub getFullBSlashname
{
	return shift->getFullName("\\");
}

sub getFullName
{
	my $this			= shift;
	my $separator			= shift;

	my $fullname = $this->{full};
	$fullname =~ s/::/$separator/g if (defined $separator);
	return $fullname;
}

sub getBaseName
{
	my $this			= shift;

	return $this->{name};
}

sub getSubModel
{
	my $this			= shift;
	my $modelName			= shift;

	if ($modelName =~ m/^\.(.*)$/) {
		$modelName = "$this->{package}::$1";
	}
	if ($this->{owner}->{class_hash}->{$modelName}) {
		return $this->{owner}->{class_hash}->{$modelName};
	}
	return $this->{owner}->loadModel($this->{location}, $modelName);
}

sub formatTyperef
{
	my $this			= shift;
	my $typeref			= shift;

	if ($typeref =~ m/^\.(.*)$/) {
		return "$this->{package}::$1";
	}
	else {
		return $typeref;
	}
}

sub checkSubModel
{
	my $this		= shift;
	my $modelName		= shift;

	dr::Util::doDie("modelName undefined") unless (defined $modelName);

	if ($this->{owner}->{class_hash}->{$modelName}) {
		return $this->{owner}->{class_hash}->{$modelName};
	}
	return $this->{owner}->checkModel($this->{location}, $modelName);
}

sub getFinalTypeWithTagger
{
	my $this		= shift;

	return ( $this, $this->{drtag}->cloneTagger() );
}

sub getSubFinalTypeWithTagger
{
	my $this			= shift;
	my $subname			= shift;
	my $subtype			= shift;

	if (my $type = dr::ModelStore::Primitive::checkPrimitive($this, $subtype)) {
		return ( $type, $this->{drtag} );
	}
	elsif ($subtype =~ m/ref:\s*(()|((.+)\.([^.]+))|((.+)\.)|(\.(.+)))\s*$/) {
		if (defined $2) {
			if (defined (my $typeref = $this->checkDrTagValue("typeref"))) {
				dr::Util::doDie("reference target required") unless (defined $subname);
				return $this->getSubModel($this->formatTyperef($typeref))->getAttr($subname)->getFinalTypeWithTagger();
			}
			else {
				dr::Util::doDie("reference target required") unless (defined $subname);
				return $this->getSubModel($this->{package})->getAttr($subname)->getFinalTypeWithTagger();
			}
		}
		elsif (defined $3) {
			return $this->getSubModel($4)->getAttr($5)->getFinalTypeWithTagger();
		}
		elsif (defined $6) {
			dr::Util::doDie("reference target required") unless (defined $subname);
			return $this->getSubModel($6)->getAttr($subname)->getFinalTypeWithTagger();
		}
		elsif (defined $8) {
			my $field_name = $9;
			if (defined (my $typeref = $this->checkDrTagValue("typeref"))) {
				return $this->getSubModel($this->formatTyperef($typeref))->getAttr($field_name)->getFinalTypeWithTagger();
			}
			else {
				return $this->getSubModel($this->{package})->getAttr($field_name)->getFinalTypeWithTagger();
			}
		}
		else {
			die "unexpected regexp result";
		}
	}
	else {
		return $this->getSubModel($subtype)->getFinalTypeWithTagger();
	}
}

sub getDrTagger
{
	my $this		= shift;

	return $this->{drtag};
}

sub getDrTag
{
	my $this		= shift;
	my $tag			= shift;

	eval {
		return $this->{drtag}->getTag($tag);
	}
		or $this->dieContext($@);
}

sub getDrTagValue
{
	my $this		= shift;
	my $tag			= shift;

	eval {
		return $this->{drtag}->getTagValue($tag);
	}
		or $this->dieContext($@);
}

sub checkDrTagValue
{
	my $this		= shift;
	my $tag			= shift;

	return $this->{drtag}->checkTagValue($tag);
}

sub getParentName
{
	my $this			= shift;

	#dr::Util::doDie("classname $this->{name} does not have prefix") unless ($this->{name} =~ m/^(.*)::.*$/);
	return $this->{package};
}

sub checkDrTagHierarchicalValue
{
	my $this			= shift;
	my $tag				= shift;

	for (my $classdef = $this; $classdef; $classdef = $this->checkSubModel($classdef->getParentName())) {
		if (defined (my $value = $this->checkDrTagValue($tag))) {
			return $value;
		}
	}
	return undef;
}

sub getNestedNames
{
	my $this			= shift;

	return $this->{nested_names};
}

sub getDrSpecs
{
	my $this		= shift;
	my $spec		= shift;

	return $this->{drtag}->getSpec($spec);
}

sub postLoad
{
	my $this			= shift;
}

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%BASE_MAPPER);

	$this->postLoad();
}

sub readBaseDrtag
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	$this->{drtag}->addTag(dr::Util::unescapeString($val));
}

sub readBaseComment
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	push(@{$this->{comment}}, dr::Util::unescapeString($val));
}

sub readNested
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	push(@{$this->{nested_names}}, dr::Util::unescapeString($val));
}


package dr::ModelStore::Primitive;

use strict;
use warnings;

use base "dr::ModelStore::ClassBase";

sub checkPrimitive
{
	my $owner		= shift;
	my $type		= shift;

	if ($type =~ m/^ref:/) {
		return;
	}
	elsif ($type =~ m/::/) {
		return;
	}
	else {
		return dr::ModelStore::Primitive->new($owner, $type, $type);
	}
}

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $name		= shift;
	my $type		= shift;

	my $this = $class->SUPER::new($owner, { package => "", name => $name, stype => "primitive" });

	$this->{type} = $type;

	return $this;
}


package dr::ModelStore::Class;

use strict;
use warnings;

use Data::Dumper;

use base "dr::ModelStore::ClassBase";

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = $class->SUPER::new($owner, $basic);

	$this->{field_list} = [];
	$this->{field_hash} = {};
	$this->{action_list} = [];
	$this->{oper_list} = [];
	$this->{view_list} = [];
	$this->{compos} = undef;

	return $this;
}

sub checkCompos
{
	my $this		= shift;

	return $this->{compos};
}

sub getCompos
{
	my $this		= shift;

	dr::Util::doDie("getting compos which is not defined for $this->{name}") if (!defined $this->{compos});

	return $this->{compos};
}

sub getPrimary
{
	my $this		= shift;

	if (!defined $this->{primary}) {
		$this->{primary} = [];
		foreach my $field (@{$this->{field_list}}) {
			push(@{$this->{primary}}, $field) if ($field->getRole()->{primary});
		}
	}

	return @{$this->{primary}};
}

sub getIndexes
{
	my $this		= shift;

	if (!defined $this->{index_list}) {
		$this->{index_list} = [];
		foreach my $def (sort({ $a->{order} <=> $b->{order} } values %{$this->{drtag}->getSpec("index")})) {
			$this->dieContext("invalid index format, expected (unique|nonunique) (col0 [asc|desc], col1...): $def->{value}") unless ($def->{value} =~ m/^(\w+)\s*\(\s*((\w+(\s+(asc|desc))?\s*,\s*)*(\w+(\s+(asc|desc))?))\s*\)\s*$/i);
			my $type = $1;
			my @fields = split(/\s*,\s*/, $2);
			push(@{$this->{index_list}}, {
					name			=> $def->{spec},
					type			=> $type,
					fields			=> \@fields,
				});
		}
	}
	return @{$this->{index_list}};
}

sub getAttrs
{
	my $this		= shift;

	return $this->{field_list};
}

our %CLASS_MAPPER = (
	%dr::ModelStore::ClassBase::BASE_MAPPER,
	compos			=> \&readClassCompos,
	field			=> \&readClassAttr,
	assoc			=> \&readClassAssoc,
	child			=> \&readClassChild,
	action			=> \&readClassAction,
	view			=> \&readClassView,
	oper			=> \&readClassOper,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%CLASS_MAPPER);

	$this->postLoad();
}

sub readClassCompos
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $compos = dr::ModelStore::Compos->new($this, { stype => "compos", name => $val });
	$compos->load($base->getSubLeveler());

	push(@{$this->{field_list}}, $compos);

	$this->{compos} = $compos;
}

sub readClassAttr
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $field = dr::ModelStore::Attr->new($this, { stype => "field", name => $val });
	$field->load($base->getSubLeveler());

	if (!$field->checkDrTagValue("disabled")) {
		push(@{$this->{field_list}}, $field);
		$this->{field_hash}{$field->{name}} = $field;
	}
}

sub readClassAssoc
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $assoc = dr::ModelStore::Assoc->new($this, { stype => "assoc", name => $val });
	$assoc->load($base->getSubLeveler());

	push(@{$this->{field_list}}, $assoc);
}

sub readClassChild
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $child = dr::ModelStore::Child->new($this, { stype => "child", name => $val });
	$child->load($base->getSubLeveler());

	push(@{$this->{field_list}}, $child);
}

sub readClassAction
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $action = dr::ModelStore::Action->new($this, { stype => "action", name => $val });
	$action->load($base->getSubLeveler());

	push(@{$this->{action_list}}, $action);
}

sub readClassView
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $view = dr::ModelStore::View->new($this, { stype => "view", name => $val });
	$view->load($base->getSubLeveler());

	push(@{$this->{view_list}}, $view);
}

sub readClassOper
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $view = dr::ModelStore::Oper->new($this, { stype => "oper", name => $val });
	$view->load($base->getSubLeveler());

	push(@{$this->{oper_list}}, $view);
}

sub getAttr
{
	my $this		= shift;
	my $field_name		= shift;

	$this->dieContext("field $field_name not found in $this->{name}") unless (defined $this->{field_hash}{$field_name});
	return $this->{field_hash}{$field_name}
}


package dr::ModelStore::Typedef;

use strict;
use warnings;

use base "dr::ModelStore::ClassBase";

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = $ref->SUPER::new($owner, $basic);

	$this->{base} = undef;

	return $this;
}

sub getFinalTypeWithTagger
{
	my $this		= shift;

	if (my $type = dr::ModelStore::Primitive::checkPrimitive($this->{owner}, $this->{base})) {
		return ( $type, $this->{drtag}->cloneTagger() );
	}
	else {
		my ( $type, $tagger );
		eval {
			( $type, $tagger ) = $this->getSubModel($this->{base})->getFinalTypeWithTagger();
			$tagger->mergeReplacing($this->{drtag});
			1;
		}
			or $this->dieContext("failed to open final type", $@);
		return ( $type, $tagger );
	}
}

our %TYPEDEF_MAPPER = (
	%dr::ModelStore::ClassBase::BASE_MAPPER,
	base			=> \&readTypedefDirect,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%TYPEDEF_MAPPER);

	$this->postLoad();
}

sub readTypedefDirect
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{$key} = $val;
}


package dr::ModelStore::Enum;

use strict;
use warnings;

use base "dr::ModelStore::ClassBase";

sub new
{
	my $ref				= shift;
	my $class			= ref($ref) || $ref;

	my $owner			= shift;
	my $basic			= shift;

	my $this = $class->SUPER::new($owner, $basic);

	$this->{literal_list} = [];

	return $this;
}

sub getLiterals
{
	my $this			= shift;

	return $this->{literal_list};
}

our %ENUM_MAPPER = (
	%dr::ModelStore::ClassBase::BASE_MAPPER,
	enum				=> \&readEnumLiteral,
);

sub load
{
	my $this			= shift;
	my $reader			= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%ENUM_MAPPER);

	$this->postLoad();
}

sub readEnumLiteral
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $enum = dr::ModelStore::Enum::Literal->new($this, { name => $val });
	$enum->load($base->getSubLeveler());

	push(@{$this->{literal_list}}, $enum);
}


package dr::ModelStore::Enum::Literal;

use strict;
use warnings;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = bless {
		owner			=> $owner,
		name			=> $basic->{name},
		value			=> undef,
		comment			=> [],
		drtag			=> dr::ModelStore::Drtag->new(),
	}, $class;

	Scalar::Util::weaken($this->{owner});

	return $this;
}

our %ENUM_LITERAL_MAPPER = (
	value			=> \&readDirect,
	comment			=> \&readComment,
	drtag			=> \&readDrtag,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%ENUM_LITERAL_MAPPER);

	$this->postLoad();
}

sub postLoad
{
	my $this		= shift;

	# no checks here
}

sub readDirect
{
	my $enum		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$enum->{value} = $val;
}

sub readComment
{
	my $enum		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$enum->{comment}}, dr::Util::unescapeString($val));
}

sub readDrtag
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{drtag}->addTag(dr::Util::unescapeString($val));
}

sub getDrTagger
{
	my $this		= shift;

	return $this->{drtag};
}


package dr::ModelStore::AttrBase;

use strict;
use warnings;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = bless {
		owner			=> $owner,
		name			=> $basic->{name},
		stype			=> $basic->{stype},
		mandatory		=> undef,
		comment			=> [],
		drtag			=> dr::ModelStore::Drtag->new(),
	}, $class;

	Scalar::Util::weaken($this->{owner});

	return $this;
}

sub dieContext
{
	my $this		= shift;
	my $msg			= shift;
	my $cause		= shift;

	dr::Util::doDie(((defined $cause)) ? "$this->{file_context}: $msg\n$cause" : "$this->{file_context}: $msg");
}

sub getRole
{
	my $this		= shift;

	if (!defined $this->{role}) {
		$this->{role} = {};
		if (defined (my $tag = $this->checkDrTagValue("role"))) {
			foreach my $r (split(/\s+/, $tag)) {
				$this->{role}->{$r} = 1;
			}
		}
		if (my $tag = $this->checkDrTagValue("primary")) {
			$this->{role}->{primary} = 1;
		}
	}

	return $this->{role};
}

sub isPrimary
{
	my $this		= shift;

	return !!$this->getRole()->{primary};
}

sub getDrTagger
{
	my $this		= shift;

	return $this->{drtag};
}

sub getDrTag
{
	my $this		= shift;
	my $tag			= shift;

	eval {
		return $this->{drtag}->getTag($tag);
	}
		or $this->dieContext($@);
}

sub getDrTagValue
{
	my $this		= shift;
	my $tag			= shift;

	eval {
		return $this->{drtag}->getTagValue($tag);
	}
		or $this->dieContext($@);
}

sub checkDrTagValue
{
	my $this		= shift;
	my $tag			= shift;

	return $this->{drtag}->checkTagValue($tag);
}

sub getDrSpecs
{
	my $this		= shift;
	my $spec		= shift;

	return $this->{drtag}->getSpecs($spec);
}

our %MODEL_ATTRBASE_MAPPER = (
	drtag			=> \&readClassAttrDrtag,
	comment			=> \&readClassAttrComment,
	type			=> \&readClassAttrDirect,
	mandatory		=> \&readClassAttrDirect,
	default			=> \&readClassAttrDirect,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_ATTRBASE_MAPPER);

	$this->postLoad();
}

sub postLoad
{
	my $this		= shift;

	dr::Util::doDie("$this->{owner}->{full}.$this->{name}: mandatory undefined") unless (defined $this->{mandatory});
}

sub readClassAttrDirect
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{$key} = $val;
}

sub readClassAttrDrtag
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{drtag}->addTag(dr::Util::unescapeString($val));
}

sub readClassAttrComment
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$this->{comment}}, dr::Util::unescapeString($val));
}


package dr::ModelStore::Attr;

use strict;
use warnings;

use base "dr::ModelStore::AttrBase";

use Data::Dumper;

sub getFinalTypeWithTagger
{
	my $this			= shift;

	my @r = eval {
		my ( $type, $tagger ) = $this->{owner}->getSubFinalTypeWithTagger($this->{name}, $this->{type});
		$tagger->mergeReplacing($this->{drtag});
		return ( $type, $tagger );
	};
	$this->dieContext("failed to process $this->{owner}->{full}.$this->{name}", $@) if ($@);
	return @r;
}


package dr::ModelStore::AssocBase;

use strict;
use warnings;

use base "dr::ModelStore::AttrBase";

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = $class->SUPER::new($owner, $basic);

	$this->{ref} = undef;

	return $this;
}

sub getAssocPrefix
{
	my $this		= shift;

	return $this->{assoc_prefix};
}

sub getAssocTarget
{
	my $this		= shift;

	eval { return $this->{owner}->getSubModel($this->{ref}); }
		or $this->dieContext("failed to open final type", $@);
};

sub getFinalTypeWithTagger
{
	my $this		= shift;

	my @refs = $this->expandAssocAttrs();
	if (@refs != 1) {
		$this->dieContext("getFinalTypeWithTagger called on reference to not single field: $this->{name}");
	}

	my ( $type, $tagger ) = $refs[0]->{field}->getFinalTypeWithTagger();
	$tagger->mergeReplacing($this->{drtag});

	return ( $type, $tagger );
}

sub expandAssocAttrs
{
	our $deeprec_check = 0;

	my $this		= shift;

	if (++$deeprec_check%1024  == 0 && caller(1024)) {
		dr::Util::doDie("deep recursion");
	}
	my $target = $this->getAssocTarget();
	my @primary = $target->getPrimary();
	$this->dieContext("no primary key for $target->{full}") unless (@primary);

	my @exp_primary;

	foreach my $pa (@primary) {
		if ($pa->{stype} eq "field") {
			push(@exp_primary, { name => (@primary == 1 ? $this->{name} : $pa->{name}), field => $pa });
		}
		elsif ($pa->{stype} eq "assoc" || $pa->{stype} eq "compos") {
			my @targ_exp = $pa->expandAssocAttrs();
			if (@targ_exp == 1) {
				push(@exp_primary, { name => (@primary == 1 ? $this->{name} : $pa->{name}), field => $targ_exp[0]->{field} });
			}
			else {
				#STDERR->print("$this->{owner}->{full}: adding $pa->{name} with multi\n");
				foreach my $tpa (@targ_exp) {
					push(@exp_primary, { name => ($pa->getAssocPrefix() || "").$tpa->{name}, field => $tpa->{field} });
				}
			}
		}
		else {
			$this->dieContext("invalid field stype: $pa->{stype}");
		}
	}

	return @exp_primary;
}

our %MODEL_ASSOCBASE_MAPPER = (
	%MODEL_ATTRBASE_MAPPER,
	ref			=> \&readClassAssocBaseRef,
);

sub postLoad
{
	my $this		= shift;

	dr::Util::doDie("$this->{owner}->{full}.$this->{name}: ref undefined") unless (defined $this->{ref});

	if (defined ($this->{assoc_prefix} = $this->{drtag}->checkTagValue("assoc_prefix"))) {
		$this->{assoc_prefix} = "" if ($this->{assoc_prefix} eq "NULL");
	}

	$this->SUPER::postLoad();
}

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_ASSOCBASE_MAPPER);

	$this->postLoad();
}

sub readClassAssocBaseRef
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{ref} = $val;
}


package dr::ModelStore::Assoc;

use strict;
use warnings;

use base "dr::ModelStore::AssocBase";


package dr::ModelStore::Compos;

use strict;
use warnings;

use base "dr::ModelStore::AssocBase";


package dr::ModelStore::Child;

use strict;
use warnings;

use base "dr::ModelStore::AssocBase";


package dr::ModelStore::OperInfo;

use strict;
use warnings;

use dr::Util qw(unescapeString);

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = bless {
		owner			=> $owner,
		name			=> $basic->{name},
		stype			=> $basic->{stype},
		comment			=> [],
		drtag			=> dr::ModelStore::Drtag->new(),
	}, $class;

	Scalar::Util::weaken($this->{owner});

	return $this;
}

sub getDrTagger
{
	my $this		= shift;

	return $this->{drtag};
}

sub getDrTag
{
	my $this		= shift;
	my $tag			= shift;

	eval {
		return $this->{drtag}->getTag($tag);
	}
		or $this->dieContext($@);
}

sub getDrTagValue
{
	my $this		= shift;
	my $tag			= shift;

	eval {
		return $this->{drtag}->getTagValue($tag);
	}
		or $this->dieContext($@);
}

sub checkDrTagValue
{
	my $this		= shift;
	my $tag			= shift;

	return $this->{drtag}->checkTagValue($tag);
}

sub getDrSpecs
{
	my $this		= shift;
	my $spec		= shift;

	return $this->{drtag}->getSpecs($spec);
}

sub dieContext
{
	my $this		= shift;
	my $msg			= shift;
	my $cause		= shift;

	dr::Util::doDie(((defined $cause)) ? "$this->{file_context}: $msg\n$cause" : "$this->{file_context}: $msg");
}

our %MODEL_OPERINFO_MAPPER = (
	drtag			=> \&readDrtag,
	comment			=> \&readComment,
);

sub postLoad
{
	my $this		= shift;
}

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_OPERINFO_MAPPER);

	$this->postLoad();
}

sub readComment
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$this->{comment}}, dr::Util::unescapeString($val));
}

sub readDrtag
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{drtag}->addTag(dr::Util::unescapeString($val));
}


package dr::ModelStore::OperBase;

use strict;
use warnings;

use base "dr::ModelStore::OperInfo";

sub new
{
	my $ref				= shift;
	my $class			= ref($ref) || $ref;

	my $owner			= shift;
	my $basic			= shift;

	my $this = $class->SUPER::new($owner, $basic);
	$this->{classify} = {};

	return $this;
}

our %MODEL_OPERATIONBASE_MAPPER = (
	%MODEL_OPERINFO_MAPPER,
	classify		=> \&readClassify,
);

sub load
{
	my $this			= shift;
	my $reader			= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_OPERATIONBASE_MAPPER);

	$this->postLoad();
}

sub readClassify
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	foreach (split(/,\s*/, $val)) {
		$this->{classify}->{$_} = 1;
	}
}

sub getReturnTypeWithTagger
{
	my $this			= shift;

	my @r = eval {
		my ( $type, $tagger ) = $this->{owner}->getSubFinalTypeWithTagger(undef, $this->{returntype});
		$tagger->mergeReplacing($this->{drtag});
		return ( $type, $tagger );
	};
	$this->dieContext("failed to process $this->{owner}->{full}.$this->{name}().returntype", $@) if ($@);
	return @r;
}

sub isStatic
{
	my $this			= shift;

	return $this->{classify}->{static} // 0;
}


package dr::ModelStore::OperParam;

use strict;
use warnings;

sub new
{
	my $ref				= shift;
	my $class			= ref($ref) || $ref;

	my $owner			= shift;
	my $basic			= shift;

	my $this = bless {
		owner				=> $owner,
		name				=> $basic->{name},
		stype				=> $basic->{stype},
		mandatory			=> undef,
		comment				=> [],
		drtag				=> dr::ModelStore::Drtag->new(),
	}, $class;

	Scalar::Util::weaken($this->{owner});

	return $this;
}

sub dieContext
{
	my $this			= shift;
	my $msg				= shift;
	my $cause			= shift;

	dr::Util::doDie(((defined $cause)) ? "$this->{file_context}: $msg\n$cause" : "$this->{file_context}: $msg");
}

sub getDrTagger
{
	my $this			= shift;

	return $this->{drtag};
}

sub getDrTag
{
	my $this			= shift;
	my $tag				= shift;

	eval {
		return $this->{drtag}->getTag($tag);
	}
		or $this->dieContext($@);
}

sub getDrTagValue
{
	my $this			= shift;
	my $tag				= shift;

	eval {
		return $this->{drtag}->getTagValue($tag);
	}
		or $this->dieContext($@);
}

sub checkDrTagValue
{
	my $this			= shift;
	my $tag				= shift;

	return $this->{drtag}->checkTagValue($tag);
}

sub getDrSpecs
{
	my $this			= shift;
	my $spec			= shift;

	return $this->{drtag}->getSpecs($spec);
}

our %MODEL_OPERATIONPARAM_MAPPER = (
	drtag			=> \&readOperParamDrtag,
	comment			=> \&readOperParamComment,
	type			=> \&readOperParamDirect,
	mandatory		=> \&readOperParamDirect,
	default			=> \&readOperParamDirect,
	direction		=> \&readOperParamDirect,
);

sub load
{
	my $this			= shift;
	my $reader			= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_OPERATIONPARAM_MAPPER);

	$this->postLoad();
}

sub postLoad
{
	my $this			= shift;
	my $reader			= shift;
}

sub readOperParamDirect
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	$this->{$key} = $val;
}

sub readOperParamDrtag
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	$this->{drtag}->addTag(dr::Util::unescapeString($val));
}

sub readOperParamComment
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	push(@{$this->{comment}}, dr::Util::unescapeString($val));
}

sub getFinalTypeWithTagger
{
	my $this			= shift;

	my @r = eval {
		my ( $type, $tagger ) = $this->{owner}->{owner}->getSubFinalTypeWithTagger($this->{name}, $this->{type});
		$tagger->mergeReplacing($this->{drtag});
		return ( $type, $tagger );
	};
	$this->dieContext("failed to process $this->{owner}->{owner}->{full}.$this->{owner}->{name}().$this->{name}", $@) if ($@);
	return @r;
}


package dr::ModelStore::Oper;

use strict;
use warnings;

use base "dr::ModelStore::OperBase";

sub new
{
	my $ref				= shift;
	my $class			= ref($ref) || $ref;

	my $owner			= shift;
	my $basic			= shift;

	my $this = $class->SUPER::new($owner, $basic);

	$this->{returntype} = undef;
	$this->{param_list} = [];

	return $this;
}

our %MODEL_OPERATION_MAPPER = (
	%MODEL_OPERATIONBASE_MAPPER,
	return				=> \&readOperReturn,
	param				=> \&readOperParam,
);

sub load
{
	my $this			= shift;
	my $reader			= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_OPERATION_MAPPER);

	$this->postLoad();
}

sub readOperReturn
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	$this->{returntype} = $val;
}

sub readOperParam
{
	my $this			= shift;
	my $base			= shift;
	my $key				= shift;
	my $val				= shift;

	my $param = dr::ModelStore::OperParam->new($this, { stype => "param", name => $val });
	$param->load($base->getSubLeveler());

	push(@{$this->{param_list}}, $param);
}


package dr::ModelStore::ActionBase;

use strict;
use warnings;

use base "dr::ModelStore::OperBase";

sub new
{
	my $ref				= shift;
	my $class			= ref($ref) || $ref;

	my $owner			= shift;
	my $basic			= shift;

	my $this = $class->SUPER::new($owner, $basic);

	return $this;
}

our %MODEL_ACTIONBASE_MAPPER = (
	%MODEL_OPERATIONBASE_MAPPER,
);

sub load
{
	my $this			= shift;
	my $reader			= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_ACTIONBASE_MAPPER);

	$this->postLoad();
}


package dr::ModelStore::Action;

use strict;
use warnings;

use base "dr::ModelStore::ActionBase";


package dr::ModelStore::View;

use strict;
use warnings;

use base "dr::ModelStore::OperInfo";

our %MODEL_VIEW_MAPPER = (
	%MODEL_OPERINFO_MAPPER,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_VIEW_MAPPER);

	$this->postLoad();
}


package dr::ModelStore;

use strict;
use warnings;

use dr::IndentReader;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;
	my $default_location	= shift;

	my $this = bless {
		class_hash		=> {},
		fixiers			=> [],
		default_location	=> $default_location,
	}, $class;

	return $this;
}

sub createClass
{
	my $this		= shift;
	my $basic		= shift;

	if ($basic->{stype} eq "class") {
		return dr::ModelStore::Class->new($this, $basic);
	}
	elsif ($basic->{stype} eq "typedef") {
		return dr::ModelStore::Typedef->new($this, $basic);
	}
	elsif ($basic->{stype} eq "enum") {
		return dr::ModelStore::Enum->new($this, $basic);
	}
	elsif ($basic->{stype} eq "association") {
		$basic->{stype} = "assoc";
		return dr::ModelStore::Class->new($this, $basic);
	}
	else {
		die "unknown stype: $basic->{stype}";
	}
}

sub registerFixier
{
	my $this			= shift;
	my $sub				= shift;

	push(@{$this->{fixiers}}, $sub);
}

sub loadModel
{
	my $this			= shift;
	my $location			= shift;
	my $modelName			= shift;

	dr::Util::doDie("modelName undefined") unless (defined $modelName);

	if (!defined $location) {
		if (!defined ($location = $this->{default_location})) {
			dr::Util::doDie("no location nor default location specified while loading $modelName");
		}
	}
	$modelName =~ s/\./::/g;
	my $fname = $modelName;
	$fname =~ s/::/\//g;
	$fname = "$location/$fname";
	my $reader = dr::IndentReader->new("$fname.clsdef")->getBaseLeveler();

	my $model;
	eval {
		my $package;
		my $name;
		my $stype;
		while (my ( $k, $v )= $reader->getPair()) {
			if ($k eq "package") {
				$package = $v;
			}
			elsif ($k eq "name") {
				$name = $v;
			}
			elsif ($k eq "stype") {
				$stype = $v;
			}
			else {
				die "unexpected key found while still missing basic information (package, name and stype): $k";
			}
			last if (defined $package && defined $name && defined $stype);
		}
		$model = $this->createClass({ filename => "$fname.clsdef", location => $location, package => $package, name => $name, stype => $stype });
		$model->load($reader);
		&$_($model) foreach (@{$this->{fixiers}});
		1;
	}
		or die "\n".$reader->getContext().": $@";

	return $model;
}

sub checkModel
{
	my $this			= shift;
	my $location			= shift;
	my $modelName			= shift;

	dr::Util::doDie("modelName undefined") unless (defined $modelName);

	if (!defined $location) {
		if (!defined ($location = $this->{default_location})) {
			dr::Util::doDie("no location nor default location specified while loading $modelName");
		}
	}
	$modelName =~ s/\./::/g;
	my $fname = $modelName;
	$fname =~ s/::/\//g;
	$fname = "$location/$fname.clsdef";
	return undef if (!-f $fname);

	return $this->loadModel($location, $modelName);
}


1;
