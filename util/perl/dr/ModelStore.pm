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

	dr::Util::doDie("invalid format of drtag: $val") unless ($val =~ m/^(\w+)(\((\w+)\))?\s+(\S.*)$/);
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
		comment			=> [],
		drtag			=> dr::ModelStore::Drtag->new(),
	}, $class;

	$owner->{class_hash}->{"$basic->{package}::$basic->{name}"} = $this;
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

our %BASE_MAPPER = (
	drtag			=> \&readBaseDrtag,
	comment			=> \&readBaseComment,
);

sub getSubModel
{
	my $this		= shift;
	my $pname		= shift;

	if ($this->{owner}->{class_hash}->{$pname}) {
		return $this->{owner}->{class_hash}->{$pname};
	}
	return $this->{owner}->loadModel($this->{location}, $pname);
}

sub getFinalType
{
	my $this		= shift;

	return $this;
}

sub getFinalTypeWithTagger
{
	my $this		= shift;

	return ( $this, $this->{drtag}->cloneTagger() );
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

	return $this->{drtag}->getSpec($spec);
}

sub postLoad
{
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
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{drtag}->addTag($val);
}

sub readBaseComment
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$this->{comment}}, $val);
}


package dr::ModelStore::Primitive;

use strict;
use warnings;

use base "dr::ModelStore::ClassBase";

sub checkPrimitive
{
	my $owner		= shift;
	my $type		= shift;

	if ($type =~ m/::/) {
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

	$this->{attr_list} = [];

	return $this;
}

sub getPrimary
{
	my $this		= shift;

	if (!defined $this->{primary}) {
		$this->{primary} = [];
		foreach my $attr (@{$this->{attr_list}}) {
			if ($attr->{stype} eq "compos") {
				push(@{$this->{primary}}, $attr);
			}
			elsif (my $role = $attr->getRole()) {
				push(@{$this->{primary}}, $attr) if (defined $role->{primary});
			}
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
			$this->dieContext("invalid index format, expected (unique|nonunique) (col0 [asc|desc], col1...): $def->{value}") unless ($def->{value} =~ m/^(\w+)\s*\(\s*((\w+(\s+(asc|desc))?\s*,\s*)*(\w+(\s+(asc|desc))?))\s*\)\s*$/);
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

our %CLASS_MAPPER = (
	%dr::ModelStore::ClassBase::BASE_MAPPER,
	attr			=> \&readClassAttr,
	assoc			=> \&readClassAssoc,
	compos			=> \&readClassCompos,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%CLASS_MAPPER);

	$this->postLoad();
}

sub readClassAttr
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $attr = dr::ModelStore::Attr->new($this, { stype => "attr", name => $val });
	$attr->load($base->getSubLeveler());

	push(@{$this->{attr_list}}, $attr);
}

sub readClassAssoc
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $assoc = dr::ModelStore::Assoc->new($this, { stype => "assoc", name => $val });
	$assoc->load($base->getSubLeveler());

	push(@{$this->{attr_list}}, $assoc);
}

sub readClassCompos
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $compos = dr::ModelStore::Compos->new($this, { stype => "compos", name => $val });
	$compos->load($base->getSubLeveler());

	push(@{$this->{attr_list}}, $compos);
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

sub getFinalType
{
	my $this		= shift;

	if (my $type = dr::ModelStore::Primitive::checkPrimitive($this->{owner}, $this->{base})) {
		return $type;
	}
	else {
		eval { return $this->getSubModel($this->{base})->getFinalType(); }
			or $this->dieContext("failed to open final type", $@);
	}
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
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $basic		= shift;

	my $this = $class->SUPER::new($owner, $basic);

	$this->{lit_list} = [];

	return $this;
}

our %ENUM_MAPPER = (
	%dr::ModelStore::ClassBase::BASE_MAPPER,
	enum			=> \&readEnumLiteral,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

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

	push(@{$enum->{comment}}, $val);
}

sub readDrtag
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$this->{drtag}->addTag($val);
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
	}

	return $this->{role};
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

sub postLoad
{
	my $this		= shift;

	dr::Util::doDie("$this->{owner}->{full}.$this->{name}: mandatory undefined") unless (defined $this->{mandatory});
}

sub load
{
	my $this		= shift;
	my $reader		= shift;

	$this->{file_context} = $reader->getContext();

	dr::ModelStore::Util::genericLoad($this, $reader, \%MODEL_ATTRBASE_MAPPER);

	$this->postLoad();
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

	$this->{drtag}->addTag($val);
}

sub readClassAttrComment
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$this->{comment}}, $val);
}


package dr::ModelStore::Attr;

use strict;
use warnings;

use base "dr::ModelStore::AttrBase";

sub getFinalType
{
	my $this		= shift;

	if (my $type = dr::ModelStore::Primitive::checkPrimitive($this->{owner}, $this->{type})) {
		return $type;
	}
	else {
		eval { return $this->{owner}->getSubModel($this->{type})->getFinalType(); }
			or $this->dieContext("failed to open final type", $@);
	}
}

sub getFinalTypeWithTagger
{
	my $this		= shift;

	if (my $type = dr::ModelStore::Primitive::checkPrimitive($this->{owner}, $this->{type})) {
		return ( $type, $this->{drtag} );
	}
	else {
		my ( $type, $tagger );
		eval {
			( $type, $tagger ) = $this->{owner}->getSubModel($this->{type})->getFinalTypeWithTagger();
			$tagger->mergeReplacing($this->{drtag});
			1;
		}
			or $this->dieContext("failed to open final type $this->{type}", $@);
		return ( $type, $tagger );
	}
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

sub expandAssocAttrs
{
	my $this		= shift;

	if (caller(1024)) {
		dr::Util::doDie("deep recursion");
	}
	my $target = $this->getAssocTarget();
	my @primary = $target->getPrimary();
	$this->dieContext("no primary key for $target->{full}") unless (@primary);

	my @exp_primary;

	foreach my $pa (@primary) {
		if ($pa->{stype} eq "attr") {
			push(@exp_primary, { name => (@primary == 1 ? $this->{name} : $pa->{name}), attr => $pa });
		}
		elsif ($pa->{stype} eq "assoc" || $pa->{stype} eq "compos") {
			my @targ_exp = $pa->expandAssocAttrs();
			if (@targ_exp == 1) {
				push(@exp_primary, { name => (@primary == 1 ? $this->{name} : $pa->{name}), attr => $targ_exp[0]->{attr} });
			}
			else {
				STDERR->print("$this->{owner}->{full}: adding $pa->{name} with multi\n");
				foreach my $tpa (@targ_exp) {
					push(@exp_primary, { name => ($pa->getAssocPrefix() || "").$tpa->{name}, attr => $tpa->{attr} });
				}
			}
		}
		else {
			$this->dieContext("invalid attribute stype: $pa->{stype}");
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


package dr::ModelStore;

use strict;
use warnings;

use dr::IndentReader;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $this = bless {
		class_hash		=> {},
		fixiers			=> [],
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
	my $this		= shift;
	my $sub			= shift;

	push(@{$this->{fixiers}}, $sub);
}

sub loadModel
{
	my $this		= shift;
	my $location		= shift;
	my $pname		= shift;

	my $fname = $pname;
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
			elsif ($k eq "type") {
				$stype = $v;
			}
			else {
				die "unexpected key found while still missing basic information: $k";
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


1;
