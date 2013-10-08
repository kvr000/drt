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

package dr::TagWriter;

use strict;
use warnings;

sub new
{
	my $class = shift; $class = ref($class) || $class;
	my $fd			= shift;

	my $this = bless {
		fd				=> $fd,
		tags				=> [],
		basePath			=> "",
	}, $class;

	return $this;
}

sub setBasePath
{
	my $this			= shift;
	my $basePath			= shift;

	$this->{basePath} = $basePath;

	return $this;
}

sub getBaseFilename
{
	my $this			= shift;
	my $filename			= shift;

	return substr($filename, length($this->{basePath}));
}

sub flush
{
	my $this		= shift;

	foreach my $tagline (sort({ $a cmp $b } @{$this->{tags}})) {
		$this->{fd}->printf("%s\n", $tagline);
	}
}

sub expandClassNames($)
{
	my $this		= shift;
	my $cld			= shift;

	my @classes;
	for (my $c = $cld->{class}; defined $c; $c = ($c =~ m/^(\w+)(::|\.)(.*)$/) ? $3 : undef) {
		push(@classes, $c);
	}
	return ( @classes );
}

sub escapeTagFind($)
{
	my $this		= shift;
	my $s			= shift;

	$s =~ s/([\/])/\\$1/g;
	return $s;
}

sub addClass($)
{
	my $this		= shift;
	my $cld			= shift;

	foreach my $cname ($this->expandClassNames($cld)) {
		push(@{$this->{tags}}, sprintf("%s\t%s\t/^%s\$/;\"\tc\tinherits:%s", $cname, $this->getBaseFilename($cld->{context}->{file}), $this->escapeTagFind($cld->{context}->{line}), join(",", (defined $cld->{ancestor} ? $cld->{ancestor} : (), @{$cld->{ifaces}}))));
	}
}

sub addMethod($$)
{
	my $this		= shift;
	my $cld			= shift;
	my $md			= shift;

	foreach my $cname ($this->expandClassNames($cld)) {
		push(@{$this->{tags}}, sprintf("%s\t%s\t/^%s\$/;\"\tf\tclass:%s\tsignature:%s", $md->{decl}, $this->getBaseFilename($md->{context}->{file}), $this->escapeTagFind($md->{context}->{line}), $cld->{class}, "()"));
	}
}

sub addAttribute($$)
{
	my $this		= shift;
	my $cld			= shift;
	my $ad			= shift;

	if ($ad->{def} =~ m/^([^\t]*)\s+(\w+);\s*$/) {
		my $name = $2;
		push(@{$this->{tags}}, sprintf("%s\t%s\t/^%s\$/;\"\tm\tclass:%s\taccess:%s", $name, $this->getBaseFilename($ad->{context}->{file}), $this->escapeTagFind($ad->{context}->{line}), $cld->{class}, $ad->{access}));
		foreach my $cname ($this->expandClassNames($cld)) {
			push(@{$this->{tags}}, sprintf("%s.%s\t%s\t/^%s\$/;\"\tm\tclass:%s\taccess:%s", $cname, $name, $this->getBaseFilename($ad->{context}->{file}), $this->escapeTagFind($ad->{context}->{line}), $cld->{class}, $ad->{access}));
		}
	}
}

sub addConst($$)
{
	my $this		= shift;
	my $cld			= shift;
	my $cnd			= shift;

	push(@{$this->{tags}}, sprintf("%s\t%s\t/^%s\$/;\"\tm\tclass:%s\taccess:%s", $cnd->{name}, $this->getBaseFilename($cnd->{context}->{file}), $this->escapeTagFind($cnd->{context}->{line}), $cld->{class}, $cnd->{access}));
	foreach my $cname ($this->expandClassNames($cld)) {
		push(@{$this->{tags}}, sprintf("%s.%s\t%s\t/^%s\$/;\"\tm\tclass:%s\taccess:%s", $cname, $cnd->{name}, $this->getBaseFilename($cnd->{context}->{file}), $this->escapeTagFind($cnd->{context}->{line}), $cld->{class}, $cnd->{access}));
	}
}

1;
