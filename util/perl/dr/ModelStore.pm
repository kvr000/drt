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

package dr::ModelStore;

use strict;
use warnings;

package dr::ModelStore::Base;

use strict;
use warnings;

use Scalar::Util;
use Data::Dumper;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $package		= shift;
	my $name		= shift;
	my $type		= shift;

	my $this = bless {
		owner			=> $owner,
		package			=> $package,
		name			=> $name,
		type			=> $type,
		comment			=> [],
		drcom_list		=> [],
		drcom_hash		=> {},
	}, $class;

	$owner->{class_hash}->{"${package}::${name}"} = $this;
	Scalar::Util::weaken($this->{owner});

	return $this;
}

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

our %BASE_MAPPER = (
	drcom			=> \&readBaseDrcom,
	comment			=> \&readBaseComment,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	dr::ModelStore::Base::genericLoad($this, $reader, \%BASE_MAPPER);
}

sub readBaseDrcom
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $drcom = dr::ModelStore::Base::genericDrcom($val);
	push(@{$this->{drcom_list}}, $drcom);
	$this->{drcom_hash}->{$1} = $drcom;
}

sub readBaseComment
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$this->{comment}}, $val);
}


package dr::ModelStore::Class;

use strict;
use warnings;

use base "dr::ModelStore::Base";

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $package		= shift;
	my $name		= shift;
	my $type		= shift;

	my $this = $class->SUPER::new($owner, $package, $name, $type);

	$this->{attr_list} = [];

	return $this;
}

our %CLASS_MAPPER = (
	%dr::ModelStore::Base::BASE_MAPPER,
	attr			=> \&readClassAttr,
	assoc			=> \&readClassAssoc,
	compos			=> \&readClassCompos,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	dr::ModelStore::Base::genericLoad($this, $reader, \%CLASS_MAPPER);
}

our %MODEL_ATTR_FUNC = (
	drcom			=> \&readClassAttrDrcom,
	comment			=> \&readClassAttrComment,
	type			=> \&readClassAttrDirect,
);

sub readClassAttr
{
	my $this		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $attr = {
		name			=> $val,
	};

	dr::ModelStore::Base::genericLoad($attr, $base->getSubLeveler(), \%MODEL_ATTR_FUNC);

	push(@{$this->{attr_list}}, $attr);
}

sub readClassAttrDirect
{
	my $attr		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$attr->{$key} = $val;
}

sub readClassAttrDrcom
{
	my $attr		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	my $drcom = dr::ModelStore::Base::genericDrcom($val);
	push(@{$attr->{drcom_list}}, $drcom);
	$attr->{drcom_hash}->{$1} = $drcom;
}

sub readClassAttrComment
{
	my $attr		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	push(@{$attr->{comment}}, $val);
}

sub readClassAssoc
{
	my $model		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$base->getSubLeveler()->skipRecursive();
}

sub readClassCompos
{
	my $model		= shift;
	my $base		= shift;
	my $key			= shift;
	my $val			= shift;

	$base->getSubLeveler()->skipRecursive();
}


package dr::ModelStore::Typedef;

use strict;
use warnings;

use base "dr::ModelStore::Base";

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $package		= shift;
	my $name		= shift;
	my $type		= shift;

	my $this = $ref->SUPER::new($owner, $package, $name, $type);

	$this->{base} = undef;

	return $this;
}

our %TYPEDEF_MAPPER = (
	%dr::ModelStore::Base::BASE_MAPPER,
	base			=> \&readTypedefDirect,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	dr::ModelStore::Base::genericLoad($this, $reader, \%TYPEDEF_MAPPER);
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

use base "dr::ModelStore::Base";

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $owner		= shift;
	my $package		= shift;
	my $name		= shift;
	my $type		= shift;

	my $this = $class->SUPER::new($owner, $package, $name, $type);

	$this->{lit_list} = [];

	return $this;
}

our %ENUM_MAPPER = (
	%dr::ModelStore::Base::BASE_MAPPER,
	enum			=> \&readEnumLiteral,
);

sub load
{
	my $this		= shift;
	my $reader		= shift;

	dr::ModelStore::Base::genericLoad($this, $reader, \%ENUM_MAPPER);
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

	dr::ModelStore::Base::genericLoad($enum, $base->getSubLeveler(), \%ENUM_LITERAL_MAPPER);

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
	}, $class;

	return $this;
}

sub createType
{
	my $this		= shift;
	my $package		= shift;
	my $name		= shift;
	my $type		= shift;

	if ($type eq "class") {
		return dr::ModelStore::Class->new($this, $package, $name, $type);
	}
	elsif ($type eq "typedef") {
		return dr::ModelStore::Typedef->new($this, $package, $name, $type);
	}
	elsif ($type eq "enum") {
		return dr::ModelStore::Enum->new($this, $package, $name, $type);
	}
	elsif ($type eq "association") {
		return dr::ModelStore::Class->new($this, $package, $name, "class");
	}
	else {
		die "unknown type: $type";
	}
}

sub loadModel
{
	my $this		= shift;
	my $fname		= shift;

	my $reader = dr::IndentReader->new("$fname.clsdef")->getBaseLeveler();

	my $model;
	eval {
		my $package;
		my $name;
		my $type;
		while (my ( $k, $v )= $reader->getPair()) {
			if ($k eq "package") {
				$package = $v;
			}
			elsif ($k eq "name") {
				$name = $v;
			}
			elsif ($k eq "type") {
				$type = $v;
			}
			else {
				die "unexpected key found while still missing basic information: $k";
			}
			last if (defined $package && defined $name && defined $type);
		}
		$model = $this->createType($package, $name, $type);
		$model->load($reader);
		1;
	}
		or die "\n".$reader->getContext().": $@";

	return $model;
}


1;
