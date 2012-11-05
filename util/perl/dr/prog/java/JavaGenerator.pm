package dr::prog::java::JavaGenerator;

use strict;
use warnings;

use Data::Dumper;
use dr::prog::gen::ClikeGenerator qw(:CMT);

use base qw(dr::prog::gen::ClikeGenerator);

our %JAVA_TAGS = (
	model_subs		=> CMT_LINE,
	type			=> CMT_LINE,
	shortcut		=> CMT_LINE,
);

sub new
{
	my $classname		= shift;

	my $this = $classname->SUPER::new(@_);
	$this->{shortcut} = {};

	return $this;
}

sub openTargets
{
	my $this		= shift;

	return $this->{file_trans}->createTruncated("$this->{gen_dir}/$this->{in_fname}");
}

sub finish
{
	my $this		= shift;

	$this->SUPER::finish();
}

sub processLine
{
	my $this		= shift;
	my $line		= shift;

	if ($line =~ m/^package\s+((\w+\.)*\w+);/) {
		$this->{package_name} = $1;
	}
	elsif ($line =~ m/^((public|private|protected)\s+|)class\s+(\w+)\s+extends\s+((\w+\.)*\w+)(\s+.*|)$/) {
		$this->{class_name} = $3;
		$this->{ancestor_name} = $4;
	}
	elsif ($line =~ m/^{/) {
		return $this->processClassStart($line);
	}
	elsif ($line =~ m/^}/) {
		return $this->processClassEnd($line);
	}

	$this->printLine($line);

	return 0;
}

sub processCommentLine
{
	my $this		= shift;
	my $cmtag		= shift;
	my $content		= shift;

	if ($cmtag eq "shortcut") {
		if ($content !~ m/^(\w+)=(.+?)\s*$/) {
			$this->printError("wrong shortcut definition: $content");
			return 1;
		}
		$this->{shortcut}->{$1} = $2;
	}
	elsif ($cmtag eq "model_subs") {
	}
	else {
		$this->SUPER::processCommentLine($cmtag, $content);
	}

	return 0;
} 
sub processCommentBlock
{
	my $this		= shift;
	my $cmtag		= shift;
	my $head		= shift;
	my $block		= shift;

	if (0) {
	}
	else {
		$this->SUPER::processCommentBlock($cmtag, $head, $block);
	}

	return 0;
}

sub processCommentContinuous
{
	my $this		= shift;
	my $cmtag		= shift;
	my $head		= shift;
	my $block		= shift;

	if (0) {
	}
	else {
		$this->SUPER::processCommentContinuous($cmtag, $head, $block);
	}

	return 0;
}

sub processCommentSingle
{
	my $this		= shift;
	my $cmtag		= shift;
	my $content		= shift;

	if (0) {
	}
	else {
		$this->SUPER::processCommentSingle($cmtag, $content);
	}

	return 0;
}

sub getCommentType
{
	my $this		= shift;
	my $cmtag		= shift;
	my $content		= shift;

	return exists $JAVA_TAGS{$cmtag} ? $JAVA_TAGS{$cmtag} : CMT_UNKNOWN;
}

sub processClassStart
{
	my $this		= shift;
	my $line		= shift;

	die "did not find class specification" unless (defined $this->{class_name});
	die "did not find ancestor specification" unless (defined $this->{ancestor_name});
	die "did not find package specification" unless (defined $this->{package_name});

	$this->{class_started} = 1;

	$this->printLine($line);

	return 0;
}

sub processClassEnd
{
	my $this		= shift;
	my $line		= shift;

	die "did not find class start" unless ($this->{class_started});

	$this->printLine($line);

	return 0;
}


1;
