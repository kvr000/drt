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

package dr::GenmapReader;

use strict;
use warnings;

use File::Basename;
use FileHandle;
use DB_File;
use dr::Util;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;
	my $listname		= shift;
	my $section		= shift;

	my $this = bless {
		listname		=> $listname,
		section			=> $section,
	}, $class;

	return $this;
}

sub open
{
	my $listname		= shift;
	my $section		= shift;

	if ($listname =~ m/\.db$/) {
		return dr::GenmapReader::Db->new($listname, $section);
	}
	else {
		return dr::GenmapReader::List->new($listname, $section);
	}
}

package dr::GenmapReader::List;

use strict;
use warnings;

use base qw(dr::GenmapReader);

sub new
{
	my $class = shift; $class = ref($class) || $class;

	my $this = $class->SUPER::new(@_);

	$this->{list_fd} = FileHandle->new($this->{listname}, "<")
		or dr::Util::doDie("failed to open $this->{listname}: $!");
	$this->{lineno} = 0;

	return $this;
}

sub getNext
{
	my $this		= shift;

	if (defined (my $line = $this->{list_fd}->getline())) {
		$this->{lineno}++;
		next if (m/^\s*(#.*|)$/);
		chomp($line);
		dr::Util::doDie("invalid format, need \"location classname\"") unless ($line =~ m/^(\S+)\s+((\w+::)*\w+)$/);
		return ( $1, $2 );
	}
}

sub getContext
{
	my $this		= shift;

	return "$this->{listname}:$this->{lineno}";
}


package dr::GenmapReader::Db;

use strict;
use warnings;

use base qw(dr::GenmapReader);

use DB_File;
use File::Basename;
use Data::Dumper;
use dr::Util;

sub new
{
	my $class = shift; $class = ref($class) || $class;

	my $this = $class->SUPER::new(@_);

	our %list_hash;
	$this->{list_db} = tie(%list_hash, 'DB_File', $this->{listname}, O_RDONLY, 0666, $DB_BTREE)
		or dr::Util::doDie("failed to open $this->{listname}");
	$this->{cur_key} = undef;

	return $this;
}

sub getNext
{
	my $this		= shift;

	if (!defined ($this->{cur_key})) {
		$this->{cur_key} = "$this->{section}\t";
		return if ($this->{list_db}->seq($this->{cur_key}, my $val, R_CURSOR) != 0);
	}
	else {
		return if ($this->{list_db}->seq($this->{cur_key}, my $val, R_NEXT) != 0);
	}
	if ($this->{cur_key} =~ m/^$this->{section}\t((\w+::)*\w+)$/) {
		return ( dirname($this->{listname}), $1 );
	}
	return;
}

sub getContext
{
	my $this		= shift;

	return "";
}


1;
