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

package dr::IndentReader;

use strict;
use warnings;

sub new
{
	my $class = shift; $class = ref($class) || $class;
	my $fd			= shift;

	my $this = bless {
		fd			=> $fd,
		level			=> "",
		pending			=> undef,
	}, $class;

	return $this;
}

sub advance
{
	my $this		= shift;

	$this->{level} .= "\t";
}

sub lower
{
	my $this		= shift;

	die "lowering level under 0" if ($this->{level} eq "");
	$this->{level} = substr($this->{level}, 0, -1);
}

sub getLine
{
	my $this		= shift;

	if (!defined $this->{pending}) {
		return unless (defined ($this->{pending} = <$this->{fd}>));
	}
	$this->{pending} =~ m/^(\t*)/;
	die "section $this->{pending} not read" if (length($1) > length($this->{level}));
	return unless (length($1) < length($this->{level}));
	die "invalid content: $this->{pending}" unless ($this->{pending} =~ m/^(\t*)(\w+)(\s+)(.*)$/);
	undef $this->{pending};
	return ( $2, $4 );
}

sub skipSubLevels
{
	my $this		= shift;

	for (;;) {
		if (!defined $this->{pending}) {
			return unless (defined ($this->{pending} = <$this->{fd}>));
		}
		$this->{pending} =~ m/^(\t*)/;
		return if (length($1) <= length($this->{level}));
		undef $this->{pending};
	}
}


1;
