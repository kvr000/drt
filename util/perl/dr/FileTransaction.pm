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

package dr::FileTransaction;

use strict;
use warnings;

sub new
{
	my $class = shift; $class = ref($class) || $class;

	return bless { opers => [], umask => eval { my $u = umask(0); umask($u); $u } }, $class;
}

sub commit
{
	my $this		= shift;

	foreach my $op (@{$this->{opers}}) {
		if ($op->{fd}) {
			$this->closeOp($op);
		}
	}

	$this->{opers} = [];
}

sub createTruncated
{
	my $this		= shift;
	my $fname		= shift;

	if (my @st = stat($fname)) {
		chmod(($st[2]&07777)|(0222&~$this->{umask}), $fname);
	}
	my $fd = FileHandle->new($fname, ">")
		or die "failed to open file $fname: $!";
	binmode($fd);
	push(@{$this->{opers}}, { op => "rm", fname => $fname, fd => $fd });
	return $fd;
}

sub closeFile
{
	my $this		= shift;
	my $fd			= shift;

	my $op;
	for (my $i = $#{$this->{opers}}; $i >= 0; $i--) {
		if (defined $this->{opers}[$i]->{fd} && $this->{opers}[$i]->{fd} == $fd) {
			$op = $this->{opers}[$i];
			unshift(@{$this->{opers}}, splice(@{$this->{opers}}, $i, 1)) if ($i != $#{$this->{opers}});
			$this->closeOp($op);
			return;
		}
	}
	die "fd $fd not opened within transaction";
}

sub closeOp
{
	my $this		= shift;
	my $op			= shift;

	$op->{fd}->flush();
	if ($op->{fd}->error()) {
		die "failed to write to $op->{fname}: ".$op->{fd}->error();
	}
	chmod((stat($op->{fd}))[2]&07555, $op->{fd}) if (defined $this->{umask});
	close($op->{fd});
	undef $op->{fd};
}

sub DESTROY
{
	my $this		= shift;

	foreach my $op (@{$this->{opers}}) {
		if ($op->{fd}) {
			close($op->{fd});
			undef $op->{fd};
		}
		if ($op->{op} eq "rm") {
			unlink($op->{fname});
		}
		else {
			die "unsupported operation: $op->{op}";
		}
	}
}


1;
