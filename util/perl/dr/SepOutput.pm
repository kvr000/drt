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

package dr::SepOutput;

use strict;
use warnings;

use dr::Util qw(tabalign);

sub new
{
	my $class = shift; $class = ref($class) || $class;
	my $fd			= shift;
	my $options		= shift;

	my $this = bless {
		fd			=> $fd,
		sep_in			=> $options->{sep_in},
		sep_all			=> $options->{sep_all},
		state			=> 0,
		pending			=> "",
		add			=> undef,
	}, $class;

	return $this;
}

sub print
{
	my $this		= shift;

	$this->{fd}->print(@_);
}

sub printFlushing
{
	my $this		= shift;
	my $str			= shift;

	$this->printObj($str);
	$this->{fd}->print($this->{pending});
	$this->{pending} = "";
}

sub printObj
{
	my $this		= shift;
	my $str			= shift;

	if ($this->{state} == 0) {
		$this->{state} = 1;
	}
	else {
		$this->{pending} .= $this->{sep_in};
		if (defined $this->{add}) {
			if (defined $this->{align_add}) {
				$this->{pending} = tabalign($this->{pending}, $this->{align_add});
				undef $this->{align_add};
			}
			$this->{pending} .= $this->{add};
			undef $this->{add};
		}
		$this->{pending} .= $this->{sep_all};
		$this->{fd}->print($this->{pending});
		$this->{pending} = "";
	}

	$this->{pending} = $str;
}

sub printAdd
{
	my $this		= shift;
	my $str			= shift;

	$this->{add} = $str;
}

sub printAligned
{
	my $this		= shift;
	my $str			= shift;
	my $align		= shift;

	$this->{pending} = "" unless (defined $this->{pending});
	$this->{pending} = tabalign($this->{pending}, $align).$str;
}

sub printAlignedAdd
{
	my $this		= shift;
	my $str			= shift;
	my $align		= shift;

	if (defined $this->{add}) {
		$this->{add} .= " ".$str;
	}
	else {
		$this->{add} = $str;
		$this->{align_add} = $align;
	}
}

sub DESTROY
{
	my $this		= shift;

	if ($this->{state} != 0) {
		if (defined $this->{add}) {
			if (defined $this->{align_add}) {
				$this->{pending} = tabalign($this->{pending}, $this->{align_add});
				undef $this->{align_add};
			}
			$this->{pending} .= $this->{add};
			undef $this->{add};
		}
		$this->{fd}->print($this->{pending});
		$this->{fd}->print($this->{sep_all});
		$this->{pending} = "";
	}
	elsif (defined $this->{add}) {
		$this->{fd}->print($this->{add});
		undef $this->{add};
	}
}


1;
