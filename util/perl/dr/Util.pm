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

package dr::Util;

use strict;
use warnings;

use Exporter;

our @ISA = qw(Exporter);
our @EXPORT_OK = qw(doDie defvalue tablength tabalign);

our $TABSIZE = 8;

BEGIN
{
	eval { require IO::String; } || eval { require IO::Scalar; } || die "failed to load IO::String or IO::Scalar";
}

sub doDie($)
{
	my $msg			= shift;

	my $stack = "";
	for (my $i = 0; ; $i++) {
		my ($pack, $fn, $ln, $sub) = caller($i)
			or last;
		$stack .= "$sub ($fn:$ln)\n";
	}
	$stack =~ s/^/\t/gm;
	die "$msg\nstack:\n$stack";
}

# perl 5.10 // operator, unfortunately not everywhere is installed perl 5.10
sub defvalue($$)
{
	my $val			= shift;
	my $def			= shift;

	return defined $val ? $val : $def;
}

sub openStringStream
{
	my $lines		= shift;

	return $INC{"IO/String.pm"} ? IO::String->new($lines) : IO::Scalar->new(\join("", @$lines));
}

sub tablength($)
{
	my $str			= shift;

	my $pos = 0;
	while ($str =~ m/^(.*?)\t(\t*)(.*)$/) {
		$pos += length($1)+$TABSIZE+length($2)*$TABSIZE;
		$pos -= $pos%$TABSIZE;
		$str = $3;
	}
	$pos += length($str);
	return $pos;
}

sub tabalign($$)
{
	my $str			= shift;
	my $align		= shift;

	if ((my $l = tablength($str)) < $align) {
		$str .= "\t" x (($align-$l+$TABSIZE-1)/$TABSIZE);
	}
	else {
		$str .= " ";
	}
	return $str;
}

sub hashUndef
{
	my %hash		= @_;

	while (my ($k, $v) = each(%hash)) {
		delete $hash{$k} unless (defined $v);
	}
	return %hash;
}

sub hashTrue
{
	my %hash		= @_;

	while (my ($k, $v) = each(%hash)) {
		delete $hash{$k} unless ($v);
	}
	return %hash;
}

sub splitString($)
{
	my $s			= shift;

	my $o = "";
	for (;;) {
		if ($s !~ m/^(.*?)(['" \t\n])(.*)$/s) {
			$o .= $s;
			return ( $o, "" );
		}
		if ($2 eq "\"" || $2 eq "\'") {
			my $q = $2;
			$o .= "$1$q";
			$s = $3;
			for (;;) {
				my $b = index($s, "\\");
				my $e = index($s, $q);
				dr::Util::doDie("cannot find end quote $q in $s") if ($e < 0);
				if ($b < 0 || $e < $b) {
					$o .= substr($s, 0, $e+1);
					$s = substr($s, $e+1);
					last;
				}
				else {
					$o .= substr($s, 0, $b+2);
					$s = substr($s, $b+2);
				}
			}
		}
		else {
			$o .= $1;
			return ( $o, $3 );
		}
	}
}

sub splitAttributes($)
{
	my $content		= shift;

	my %ats;

	for (;;) {
		$content =~ s/^\s+//;
		last if ($content eq "");
		my $eq = index($content, "=");
		dr::Util::doDie("splitAttributes: missing equals in $content") if ($eq < 0);
		my $key = substr($content, 0, $eq);
		$key =~ s/^(\s*)(.*?)(\s*)$/$2/;
		dr::Util::doDie("splitAttributes: key is empty in $content") if ($key eq "");
		my $value;
		( $value, $content ) = dr::Util::splitString(substr($content, $eq+1));
		$ats{$key} = $value;
	}

	return \%ats;
}


1;
