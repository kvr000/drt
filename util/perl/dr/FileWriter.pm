package dr::FileWriter::Context;

use strict;
use warnings;

sub new
{
	my $class			= shift;
	my $owner			= shift;

	return bless {
		owner				=> $owner,
		indent				=> $owner->{indent},
		context				=> $owner->{current},
	}, $class;
}

sub print
{
	my $this			= shift;
	my $content			= shift;

	$content =~ s/^(.+)$/$this->{indent}$1/gm;
	$this->{context}->{content} .= $content;
}

sub printOnce
{
	my $this			= shift;
	my $content			= shift;

	my $owner = $this->{owner};

	return 0 if (defined $owner->{printed}->{$content});
	$owner->{printed}->{$content} = 1;
	$this->print($content);
	return 1;
}

sub indentContext
{
	my $this			= shift;
	my $level			= shift || 1;

	my $ind_ctx = dr::FileWriter::Context->new($this->{owner});
	$ind_ctx->{context} = $this->{context};
	$ind_ctx->{indent} = $this->{indent}.("\t" x $level);

	return $ind_ctx;
}

sub subContext
{
	my $this			= shift;

	$this->{owner}->print("");

	my $ind_ctx = dr::FileWriter::Context->new($this->{owner});
	$ind_ctx->{context} = { content => "", next => $this->{context}->{next} };
	$this->{context}->{next} = $ind_ctx->{context};
	$ind_ctx->{indent} = $this->{indent};

	return $ind_ctx;
}


package dr::FileWriter;

use strict;
use warnings;

use Data::Dumper;

sub new
{
	my $class			= shift;
	my $realfd			= shift;

	my $this = bless {}, $class;

	my $current_piece = { content => "", next => undef };
	$this->{realfd} = $realfd;
	$this->{pieces} = $current_piece;
	$this->{current} = $current_piece;
	$this->{printed} = {};
	$this->{indent} = "";

	return $this;
}

sub rememberContext
{
	my $this			= shift;

	$this->print("");

	return dr::FileWriter::Context->new($this);
}

sub print
{
	my $this			= shift;
	my $content			= shift;

	$this->{current} = ($this->{current}->{next} = { content => $content, next => undef });
}

sub printIndented
{
	my $this			= shift;
	my $content			= shift;

	$content =~ s/^(.+)$/$this->{indent}$1/gm;
	$this->print($content);
}

sub printAt
{
	my $this			= shift;
	my $context			= shift;
	my $content			= shift;

	$context->print($content);
}

sub printOnce
{
	my $this			= shift;
	my $context			= shift;
	my $content			= shift;

	$context->printOnce($content);
}

sub indent
{
	my $this			= shift;
	my $indent_change		= shift;

	if ($indent_change > 0) {
		$this->{indent} .= "\t" x $indent_change;
	}
	else {
		$this->{indent} = substr($this->{indent}, 0, $indent_change);
	}
}

sub flush
{
	my $this			= shift;

	if (defined $this->{current}) {
		for (my $context = $this->{pieces}; $context; $context = $context->{next}) {
			$this->{realfd}->print($context->{content});
		}
		$this->{current} = undef;
	}

	return $this->{realfd}->flush();
}

sub seek
{
	my $this			= shift;

	$this->flush();

	return $this->{realfd}->seek(@_);
}

sub read
{
	my $this			= shift;
	
	return $this->{realfd}->read(@_);
}

sub error
{
	my $this			= shift;

	return $this->{realfd}->error(@_);
}

sub binmode
{
	my $this			= shift;

	return $this->{realfd}->binmode(@_);
}

sub close
{
	my $this			= shift;

	return $this->{realfd}->close(@_);
}


1;
