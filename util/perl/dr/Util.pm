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
our @EXPORT_OK = qw(doDie dumpSimple defvalue tablength tabalign escapeString unescapeString escapeStringContent);

use Scalar::Util qw(isweak reftype);

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

# dump a structure, but avoiding weak references
sub dumpSimple
{
	my $var			= shift;
	my $level		= shift || 0;

	return "" if ($level > 4);

	my $out = "";

	return "undef" if (!defined $var);
	my $type = reftype($var) || "";
	SW: {
		if ($type eq "SCALAR") {
			$out .= "\\".(isweak($$var) ? "__WEAK__" : dumpSimple($$var, $level));
			last SW;
		}
		if ($type eq "ARRAY") {
			$out .= "[\n";
			for (my $i = 0; $i < @$var; $i++) {
				$out .= ("\t" x ($level+1)).(isweak(${$var}[$i]) ? "__WEAK__" : dumpSimple(${$var}[$i], $level+1)).",\n";
			}
			$out .= ("\t" x $level)."]";
			last SW;
		}
		if ($type eq "HASH") {
			$out .= "{\n";
			foreach my $i (keys %$var) {
				$out .= ("\t" x ($level+1)).tabalign($i, 16)." => ".(isweak(${$var}{$i}) ? "__WEAK__" : dumpSimple(${$var}{$i}, $level+1)).",\n";
			}
			$out .= ("\t" x $level)."}";
			last SW;
		}
		$out .= "$var";
	}

	return $out;
}

sub openStringStream
{
	my $lines		= shift;

	return $INC{"IO/String.pm"} ? IO::String->new($lines) : IO::Scalar->new(\join("", @$lines));
}

sub createStringStream
{
	return $INC{"IO/String.pm"} ? IO::String->new() : IO::Scalar->new();
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

sub findEndParenthesis($)
{
	my $str			= shift;

	my $level = 1;
	my $p;
	for ($p = 0; $level > 0; $p++) {
		my $o = index($str, "(", $p); $o = length($str) if ($o < 0);
		my $c = index($str, ")", $p); $c = length($str) if ($c < 0);
		my $q = index($str, "\"", $p); $q = length($str) if ($q < 0);
		my $a = index($str, "\'", $p); $a = length($str) if ($a < 0);

		if ($c <= $o && $c <= $q && $c <= $a) {
			die "cannot find matching end parenthesis in string $str" if ($c == length($str));
			$p = $c;
			$level--;
		}
		elsif ($o < $q && $o < $a) {
			$level++;
			$p = $o;
		}
		elsif ($q < $a) {
			for ($p = $q;;) {
				die "cannot find end of quote in $str" if (($q = index($str, "\"", $p+1)) < 0);
				my $b = index($str, "\\", $p+1);
				last if ($b < 0 || $b > $q);
				$p = $b+1;
			}
			$p = $q;
		}
		else {
			for ($p = $a;;) {
				die "cannot find end of apostrophe in $str" if (($a = index($str, "\"", $p+1)) < 0);
				my $b = index($str, "\\", $p+1);
				last if ($b < 0 || $b > $a);
				$p = $b+1;
			}
			$p = $a;
		}
	}
	return $p;
}

sub escapeString($)
{
	my $s				= shift;

	$s =~ s/\\/\\\\/g;
	$s =~ s/\n/\\n/g;
	$s =~ s/\t/\\t/g;
	$s =~ s/"/\\"/g;
	return $s;
}

sub unescapeString($)
{
	my $s				= shift;

	my $n = "";
	while ($s =~ m/^(.*?)\\(.)(.*)$/s) {
		my $r;
		if ($2 eq "\\") {
			$r = "\\";
		}
		elsif ($2 eq "n") {
			$r = "\n";
		}
		elsif ($2 eq "t") {
			$r = "\t";
		}
		elsif ($2 eq "\"") {
			$r = "\"";
		}
		else {
			doDie("failed to process string, wrong escape: \\$2");
		}
		$n .= "$1$r";
		$s = $3;
	}
	return "$n$s";
}

sub escapeStringContent($)
{
	my $s				= shift;

	return ($s =~ m/^"(.*)"$/) ? "\"".escapeString($1)."\"" : $s;
}


1;
