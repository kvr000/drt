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
			STDERR->print(Dumper($splitter));
			die "unknown key $k";
		}
	}
}

sub genericDrcom
{
	my $val			= shift;

	die "invalid format of drcom: $val" unless ($val =~ m/^(\S+)\s+(\S.*)$/);
	return {
		name			=> $1,
		value			=> $2,
	};
}


package dr::ModelStore::ClassBase;

use strict;
use warnings;

use Scalar::Util;
use Data::Dumper;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

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
		drcom_list		=> [],
		drcom_hash		=> {},
	}, $class;

	$owner->{class_hash}->{"$basic->{package}::$basic->{name}"} = $this;
	Scalar::Util::weaken($this->{owner});

	return $this;
}

our %BASE_MAPPER = (
	drcom			=> \&readBaseDrcom,
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

sub load
{
	my $this		= shift;
	my $reader		= shift;

	dr::ModelStore::Util::genericLoad($this, $reader, \%BASE_MAPPER);
}

sub readBaseDrcom
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $drcom = dr::ModelStore::Util::genericDrcom($val);
	push(@{$this->{drcom_list}}, $drcom);
	$this->{drcom_hash}->{$drcom->{name}} = $drcom;
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

sub getFinalType
{
	my $this		= shift;

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

our %CLASS_MAPPER = (
	%dr::ModelStore::ClassBase::BASE_MAPPER,
	attr			=> \&readClassAttr,
	assoc			=> \&readClassAssoc,
	compos			=> \&readClassCompos,
);

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

sub load
{
	my $this		= shift;
	my $reader		= shift;

	dr::ModelStore::Util::genericLoad($this, $reader, \%CLASS_MAPPER);
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
		return $this->getSubModel($this->{base})->getFinalType();
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

	dr::ModelStore::Util::genericLoad($this, $reader, \%TYPEDEF_MAPPER);
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

	dr::ModelStore::Util::genericLoad($this, $reader, \%ENUM_MAPPER);
}

our %ENUM_LITERAL_MAPPER = (
	value			=> \&readEnumLiteralDirect,
	comment			=> \&readEnumLiteralComment,
);

sub readEnumLiteral
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $enum = {
		name			=> $val,
		value			=> undef,
		comment			=> [],
	};

	dr::ModelStore::Util::genericLoad($enum, $base->getSubLeveler(), \%ENUM_LITERAL_MAPPER);

	push(@{$this->{literal_list}}, $enum);
}

sub readEnumLiteralDirect
{
	my $enum		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$enum->{value} = $val;
}

sub readEnumLiteralComment
{
	my $enum		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$enum->{comment}}, $val);
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
		comment			=> [],
		drcom_list		=> [],
		drcom_hash		=> {},
	}, $class;

	Scalar::Util::weaken($this->{owner});

	return $this;
}

sub getRole
{
	my $this		= shift;

	if (!defined $this->{role}) {
		$this->{role} = {};
		if (my $com = $this->{drcom_hash}->{role}) {
			foreach my $r (split(/\s+/, $com->{value})) {
				$this->{role}->{$r} = 1;
			}
		}
	}

	return $this->{role};
}

sub getCom
{
	my $this		= shift;
	my $com			= shift;

	if (defined (my $val = $this->{drcom_hash}->{$com})) {
		return $val->{value};
	}
	return;
}

our %MODEL_ATTRBASE_MAPPER = (
	drcom			=> \&readClassAttrDrcom,
	comment			=> \&readClassAttrComment,
	type			=> \&readClassAttrDirect,
);

sub postLoad
{
	my $this		= shift;
}

sub load
{
	my $this		= shift;
	my $reader		= shift;

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

sub readClassAttrDrcom
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $drcom = dr::ModelStore::Util::genericDrcom($val);
	push(@{$this->{drcom_list}}, $drcom);
	$this->{drcom_hash}->{$drcom->{name}} = $drcom;
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
		return $this->{owner}->getSubModel($this->{type})->getFinalType();
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

	return $this->{owner}->getSubModel($this->{ref});
};

sub expandAssocAttrs
{
	my $this		= shift;

	my $target = $this->getAssocTarget();
	my @primary = $target->getPrimary();
	dr::Util::doDie("no primary for $target->{full}") unless (@primary);

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
			dr::Util::doDie("invalid attribute stype: $pa->{stype}");
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

	if (defined ($this->{assoc_prefix} = $this->{drcom_hash}->{assoc_prefix})) {
		$this->{assoc_prefix} = "" if ($this->{assoc_prefix} eq "NULL");
	}

	$this->SUPER::postLoad();
}

sub load
{
	my $this		= shift;
	my $reader		= shift;

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
