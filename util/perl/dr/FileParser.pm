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

package dr::FileParser;

use strict;
use warnings;

use FileHandle;
use dr::Util;

sub new
{
	my $class = shift; $class = ref($class) || $class;
	my $fname		= shift;

	my $fd = FileHandle->new($fname, "<")
		or die "failed to open $fname";

	my $this = bless {
		fname			=> $fname,
		lineno			=> 0,
		fd			=> $fd,
		pending			=> undef,
	}, $class;

	return $this;
}

sub getFd
{
	my $this		= shift;

	return $this->{fd};
}

sub readLine
{
	my $this		= shift;

	my $line;
	if (defined ($line = $this->{pending})) {
		undef $this->{pending};
	}
	elsif (defined ($line = $this->{fd}->getline())) {
		$this->{lineno}++;
	}
	return $line;
}

sub unreadLine
{
	my $this		= shift;
	my $line		= shift;

	dr::Util::doDie("File $this->{fname} already has unread line") if (defined $this->{pending});
	$this->{pending} = $line;
}

sub getContext
{
	my $this		= shift;

	return "$this->{fname}:$this->{lineno}";
}


1;
