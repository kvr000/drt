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

package dr::XmiParser;

use strict;
use warnings;

use dr::Util qw(defvalue dumpSimple);


package dr::XmiParser::Leveler;

use strict;
use warnings;

use dr::Util;
use XML::LibXML::Reader;
#BEGIN
#{
#	eval { require XML::LibXML::Reader; 1; } || eval { use XML::LibXML::Common qw(:libxml); 1; } || die "failed to load XML::LibXML::Reader or XML::LibXML::Common: $@";
#}

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;
	my $owner		= shift;
	my $level		= shift;

	my $this = bless {
		owner			=> $owner,
		level			=> $level,
	}, $class;

	return $this;
}

sub readNode
{
	my $this		= shift;

	my ( $type, $name, $depth ) = $this->{owner}->readNode();

	$this->{owner}->read_doDie("readed type undefined") unless (defined $type);
	$this->{owner}->read_doDie("readed depth undefined") unless (defined $depth);

	return if ($type == XML_READER_TYPE_END_ELEMENT && $depth < $this->{level});
	$this->{owner}->read_doDie("sub level not read") if ($depth > $this->{level});

	return $name;
}

sub skipNode
{
	my $this		= shift;

	while (defined (my $name = $this->readNode())) {
		$this->getSubLeveler()->skipNode();
	}
}

sub getMandatoryAttr
{
	my $this		= shift;
	my $name		= shift;

	my $value = $this->{owner}->{reader}->getAttribute($name);
	$this->{owner}->read_doDie("mandatory attribute $name not found in element") unless (defined $value);
	return $value;
}

sub getOptionalAttr
{
	my $this		= shift;
	my $name		= shift;

	return $this->{owner}->{reader}->getAttribute($name);
}

sub getSubLeveler
{
	my $this		= shift;

	return $this->new($this->{owner}, $this->{level}+1);
}


package dr::XmiParser::Comment;

use strict;
use warnings;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $this = bless {
		order			=> 0,
		comment			=> [],
		drtag_list		=> [],
		drtag_hash		=> {},
	}, $class;

	return $this;
}

sub processComment
{
	my $this		= shift;
	my $body		= shift;

	foreach my $line (split("\n", $body)) {
		if ($line =~ m/^(drt?-)(\w+(\(\w+\))?):\s*(.*?)\s*$/) {
			my $comment = { order => $this->{order}++, name => $2, value => $4 };
			push(@{$this->{drtag_list}}, $comment);
			die "drcomment $2 already specified" if (defined $this->{drtag_hash}->{$2});
			$this->{drtag_hash}->{$comment->{name}} = $comment;
		}
		elsif ($line =~ m/^drt?-$/) {
			die "invalid syntax for dr tag: $line";
		}
		else {
			push(@{$this->{comment}}, "$line");
		}
	}
}

sub processTag
{
	my $this		= shift;
	my $tag			= shift;
	my $value		= shift;

	if ($tag =~ m/^\s*(drt?-)(\w+)(-(\d+))?(\(\w+\))?$/) {
		my $comment = { order => defined $3 ? $4 : 0x7fffffff, name => $2.dr::Util::defvalue($5, ""), value => $value };
		push(@{$this->{drtag_list}}, $comment);
		die "drcomment $2 already specified" if (defined $this->{drtag_hash}->{$2});
		$this->{drtag_hash}->{$comment->{name}} = $comment;
	}
	elsif ($tag =~ m/^\s*drt?-$/) {
		die "invalid syntax for dr tag: $tag";
	}
}


package dr::XmiParser::Context;

use strict;
use warnings;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;
	my $owner		= shift;
	my $type		= shift;

	my $this = bless {
		type			=> $type,
		name			=> $owner->read_getMandatoryAttr("name"),
		xmi_id			=> $owner->read_getMandatoryAttr("xmi:id"),
		comment			=> dr::XmiParser::Comment->new(),
		stereotype		=> undef,
		typedef_list		=> [],
		typedef_hash		=> {},
		class_list		=> [],
		class_hash		=> {},
		package_list		=> [],
		package_hash		=> {},
		attr_list		=> [],
		attr_hash		=> {},
		oper_list		=> [],
		oper_hash		=> {},
	}, $class;

	Scalar::Util::weaken($this->{owner} = $owner);

	return $this;
}


package dr::XmiParser::Package;

use strict;
use warnings;

use base qw(dr::XmiParser::Context);


package dr::XmiParser::Class;

use strict;
use warnings;

use Data::Dumper;

use base qw(dr::XmiParser::Context);

sub new
{
	my $this		= shift;

	$this = $this->SUPER::new(@_);

	$this->{ancestor_xid} = undef;

	return $this;
}

sub addAttribute
{
	my $this		= shift;
	my $attr		= shift;

	push(@{$this->{attr_list}}, $attr);
	$this->{attr_hash}->{$attr->{name}} = $attr;
}

sub addOperation
{
	my $this		= shift;
	my $oper		= shift;

	push(@{$this->{oper_list}}, $oper);
	$this->{oper_hash}->{$oper->{name}} = $oper;
}

sub isTypedef
{
	my $this		= shift;

	return defined $this->{stereotype} && $this->{stereotype} eq "typedef";
}

sub getTypedefSource
{
	my $this		= shift;

	if (defined $this->{typedef}->{base_xid}) {
		return $this->{owner}->resolveDatatype($this->{typedef}->{base_xid});
	}
	else {
		return $this->{typedef}->{base_name};
	}
}

sub getAttrType
{
	my $this		= shift;
	my $attr		= shift;

	die "attribute $attr->{name} undefined for class $this->{name}" unless (defined $this->{attr_hash}->{$attr->{name}});
	if ($attr->{type_type} eq "primitive") {
		die Dumper($attr) unless (defined $attr->{type_primitive});
		return $attr->{type_primitive};
	}
	else {
		return $this->{owner}->resolveDatatype($attr->{type_xid});
	}
}


package dr::XmiParser;

use strict;
use warnings;

use FileHandle;
use XML::LibXML::Reader;
use Scalar::Util;
use Data::Dumper;


our %UML_PRIMITIVE_TYPES = (
	Boolean			=> "Boolean",
	Integer			=> "Integer",
	UnlimitedNatural	=> "Long",
	String			=> "String",
);


sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;
	my $fname		= shift;

	my $reader;
	if (0) {
		my $reader = XML::LibXML->new();

		my $doc = $reader->parse_file($fname)
			or die "failed to parse $fname";
		print(Dumper($doc));
	}
	else {
		$reader = XML::LibXML::Reader->new(location => $fname)
			or die "failed to create XML reader for XMI $fname";
		#print(Dumper($reader->document()));
	}

	my $this = bless {
		fname			=> $fname,
		reader			=> $reader,
		packages		=> [],
		datatypes		=> [],
		refs			=> {},
		package_xi		=> {},
		class_xi		=> {},
		datatype_xi		=> {},
	}, $class;

	eval {
		$this->read_processXml();
		#$this->read_processDump();
		undef $this->{reader};
		1;
	}
		or die sprintf("%s:%d:%d: %s", $this->{fname}, $this->{reader}->lineNumber(), $this->{reader}->columnNumber(), "failed to read XMI: $@");

	eval {
		$this->postprocess();
		1;
	}
		or die sprintf("%s:0:0: failed to postprocess: %s", $this->{fname}, $@);

	return $this;
}

sub read_doDie
{
	my $this		= shift;
	my $msg			= shift;

	my $reader		= $this->{reader};

	dr::Util::doDie("on element ".$reader->name().": $msg");
}

sub read_warnUnknown
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_doDie("unexpected element ".$this->{reader}->name()."");
}

sub readRelevant
{
	my $this		= shift;

	my $reader		= $this->{reader};

	if (defined $this->{read_pending_end}) {
		undef $this->{read_pending_end};
		return ( XML_READER_TYPE_END_ELEMENT, $reader->name, $reader->depth );
	}

	while (my $err = $reader->read()) {
		$this->read_doDie("failed to parse XML") if ($err < 0);
		next if ($reader->nodeType == XML_READER_TYPE_COMMENT);
		$this->{read_pending_end} = $reader->name if ($reader->isEmptyElement());
		return ( $reader->nodeType, $reader->name, $reader->depth );
	}
	return;
}

sub readNode
{
	my $this		= shift;

	my $reader		= $this->{reader};

	if (defined $this->{read_pending_end}) {
		undef $this->{read_pending_end};
		return ( XML_READER_TYPE_END_ELEMENT, $reader->name, $reader->depth );
	}

	while (my $err = $reader->read()) {
		$this->read_doDie("failed to parse XML") if ($err < 0);
		my $type = $reader->nodeType;
		next if ($type == XML_READER_TYPE_COMMENT || $type == XML_READER_TYPE_WHITESPACE || $type == XML_READER_TYPE_SIGNIFICANT_WHITESPACE);
		$this->{read_pending_end} = $reader->name if ($reader->isEmptyElement());
		return ( $type, $reader->name, $reader->depth );
	}
	return;
}

sub read_processDump
{
	my $this		= shift;

	my $reader		= $this->{reader};

	while (my ($type, $name, $depth) = $this->readRelevant()) {
		printf("%s%2d %s\n", "\t" x $depth, $type, $name);
	}
	die "done";
}

sub read_skipNode
{
	my $this		= shift;
	my $main_reader		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	while (my ($type, $name, $depth) = $this->readNode()) {
		return if ($depth == $exit_depth);
	}
	die "cannot get here";
}

sub read_unknownNode
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_warnUnknown($main_reader);
	$this->read_skipNode($main_reader);
}

sub read_needNodeEnd
{
	my $this		= shift;
	my $main_reader		= shift;
	my $reader		= $main_reader->getSubLeveler();

	while (defined (my $name = $reader->readNode())) {
		$this->read_warnUnknown($reader);
		$reader->skipNode();
	}
}

sub read_getMandatoryAttr
{
	my $this		= shift;
	my $attr		= shift;

	my $reader		= $this->{reader};

	my $value = $reader->getAttribute($attr);
	$this->read_doDie("mandatory attribute $attr not found in element") unless (defined $value);
	return $value;
}

sub read_getOptionalAttr
{
	my $this		= shift;
	my $attr		= shift;

	my $reader		= $this->{reader};

	my $value = $reader->getAttribute($attr);
	return $value;
}

sub read_processXml
{
	my $this		= shift;

	my $reader		= dr::XmiParser::Leveler->new($this, 0);

	if (defined (my $name = $reader->readNode())) {
		if ($name eq "xmi:XMI") {
			$this->read_processXmi($reader);
		}
		else {
			$this->read_doDie("expected xmi:XMI as root element, got $name");
		}
	}
	else {
		$this->read_doDie("unexpected end of XML");
	}
}

sub read_processXmi
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "uml:Model") {
			$this->read_processModel($reader);
			next;
		}
		elsif ($name eq "xmi:Documentation") {
			$this->read_skipNode($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
}

sub createGenericComment
{
	my $this		= shift;

	return dr::XmiParser::Comment->new();
}

sub read_processGenericComment
{
	my $this		= shift;
	my $main_reader		= shift;
	my $comment		= shift;

	eval {
		$comment->processComment($main_reader->getMandatoryAttr("body"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd($main_reader);
}

sub read_processGenericTagged
{
	my $this		= shift;
	my $main_reader		= shift;
	my $comment		= shift;

	eval {
		$comment->processTag($main_reader->getMandatoryAttr("tag"), $main_reader->getMandatoryAttr("value"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd($main_reader);
}

sub read_processModel
{
	my $this		= shift;

	my $reader		= shift->getSubLeveler();

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "packagedElement") {
			$this->read_processPackagedElement($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
}

sub read_processPackagedElement
{
	my $this		= shift;
	my $reader		= shift;

	my $type = $this->read_getMandatoryAttr("xmi:type");
	if ($type eq "uml:Package") {
		$this->read_processPackage($reader);
	}
	elsif ($type eq "uml:Class") {
		$this->read_processClass($reader);
	}
	elsif ($type eq "uml:AssociationClass") {
		$this->read_processAssociationClass($reader);
	}
	elsif ($type eq "uml:Enumeration") {
		$this->read_processEnum($reader);
	}
	elsif ($type eq "uml:Association") {
		$this->read_processAssociation($reader);
	}
	elsif ($type eq "uml:Artifact") {
		$this->read_processArtifact($reader);
	}
	elsif ($type eq "uml:DataType") {
		$this->read_processDataType($reader);
	}
	else {
		$this->read_doDie("unknown type: $type");
	}
}

sub read_processPackage
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Package->new($this, "package");

	my @classes;

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "packagedElement") {
			$this->read_processPackagedElement($reader);
			next;
		}
		elsif ($name eq "xmi:Extension") {
			$this->read_skipNode($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
	push(@{$this->{packages}}, $this->{read_context});
	$this->{read_context} = $saved_context;
}

sub read_processClass
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	$this->read_doDie("class definition out of package context") unless (defined $this->{read_context});

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Class->new($this, "class");

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "xmi:Extension") {
			$this->read_processClassExtensions($reader);
			next;
		}
		elsif ($name eq "ownedAttribute") {
			$this->read_processOwnedAttribute($reader);
			next;
		}
		elsif ($name eq "ownedOperation") {
			$this->read_processOwnedOperation($reader);
			next;
		}
		elsif ($name eq "ownedComment") {
			$this->read_processClassComment($reader);
			next;
		}
		elsif ($name eq "nestedClassifier") {
			$this->read_processNestedClassifier($reader);
			next;
		}
		elsif ($name eq "generalization") {
			$this->read_processClassGeneralization($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
			next;
		}
	}

	$this->{read_context}->{stereotype} = "class" unless (defined $this->{read_context}->{stereotype});

	push(@{$saved_context->{class_list}}, $this->{read_context});
	$saved_context->{class_hash}->{$this->{read_context}->{name}} = $this->{read_context};

	$this->{read_context} = $saved_context;
}

sub read_processAssociationClass
{
	my $this		= shift;
	my $main_reader		= shift;
	my $reader		= $main_reader->getSubLeveler();

	my $association = $main_reader->getOptionalAttr("association");
	my $name = $main_reader->getMandatoryAttr("name");

	if (defined $association && $name eq "") {
		$this->read_skipNode($reader);
		return;
	}

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Class->new($this, "association");

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "xmi:Extension") {
			$this->read_processClassExtensions($reader);
			next;
		}
		elsif ($name eq "ownedAttribute") {
			$this->read_processOwnedAttribute($reader);
			next;
		}
		elsif ($name eq "ownedOperation") {
			$this->read_processOwnedOperation($reader);
			next;
		}
		elsif ($name eq "ownedComment") {
			$this->read_processClassComment($reader);
			next;
		}
		elsif ($name eq "nestedClassifier") {
			$this->read_processNestedClassifier($reader);
			next;
		}
		elsif ($name eq "memberEnd") {
			$this->read_processClassMemberEnd($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	$this->{read_context}->{stereotype} = "class" unless (defined $this->{read_context}->{stereotype});

	push(@{$saved_context->{class_list}}, $this->{read_context});
	$saved_context->{class_hash}->{$this->{read_context}->{name}} = $this->{read_context};

	$this->{read_context} = $saved_context;
}

sub read_processNestedClassifier
{
	my $this		= shift;
	my $main_reader		= shift;

	return $this->read_processPackagedElement($main_reader);
}

sub read_processClassExtensions
{
	my $this		= shift;

	my $reader		= shift->getSubLeveler();

	my $saved_extender = $this->{read_extender};
	$this->{read_extender} = $this->read_getMandatoryAttr("extender");

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "stereotype") {
			$this->read_processClassExtensionStereotype($reader);
			next;
		}
		elsif ($name eq "typedef") {
			$this->read_processClassExtensionTypedef($reader);
			next;
		}
		elsif ($name eq "taggedValue") {
			$this->read_processClassTaggedValue($reader);
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	$this->{read_extender} = $saved_extender;
}

sub read_processClassTaggedValue
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_processGenericTagged($main_reader, $this->{read_context}->{comment});
}

sub read_processClassExtensionStereotype
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->{read_context}->{stereotype} = $this->read_getMandatoryAttr("name");

	$this->read_needNodeEnd($main_reader);
}

sub read_processClassExtensionTypedef
{
	my $this		= shift;
	my $main_reader		= shift;
	my $reader		= $main_reader->getSubLeveler();

	$this->{read_context}->{type} = "typedef";
	$this->{read_context}->{typedef} = { type_type => undef, type_primitive => undef, type_xid => undef };

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "base") {
			$this->read_processClassExtensionTypedefBase($reader);
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	$this->read_doDie("base undefined for typedef") unless (defined $this->{read_context}->{typedef}->{type_type});
}

sub read_processClassExtensionTypedefBase
{
	my $this		= shift;
	my $main_reader		= shift;
	my $reader		= $main_reader->getSubLeveler();

	my $type_type = $this->read_getMandatoryAttr("xmi:type");
	if ($type_type eq "uml:PrimitiveType") {
		$this->{read_context}->{typedef}->{type_type} = "primitive";
		if ($this->read_getMandatoryAttr("href") !~ m/.*#(\w+)$/) {
			$this->read_doDie("invalid format for type href");
		}
		if (!defined ($this->{read_context}->{typedef}->{base_name} = $UML_PRIMITIVE_TYPES{$1})) {
			$this->read_doDie("unknown UML primitive type: $1");
		}
	}
	elsif ($type_type eq "uml:Class") {
		$this->{read_context}->{typedef}->{type_type} = "class";
		$this->{read_context}->{typedef}->{base_xid} = $this->read_getMandatoryAttr("xmi:idref");
	}
	else {
		$this->read_doDie("unknown type type: $type_type");
	}
}

sub read_processClassComment
{
	my $this		= shift;
	my $main_reader		= shift;

	eval {
		$this->{read_context}->{comment}->processComment($this->read_getMandatoryAttr("body"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd($main_reader);
}

sub read_processClassGeneralization
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->{read_context}->{ancestor_xid} = $main_reader->getMandatoryAttr("general");

	$this->read_needNodeEnd($main_reader);
}

sub read_processClassMemberEnd
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_skipNode($main_reader);
}

sub read_processOwnedAttribute
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	my $association_xid = $this->read_getOptionalAttr("association");
	my $aggregation = $this->read_getOptionalAttr("aggregation");
	my $name = $this->read_getMandatoryAttr("name");

	if (defined $association_xid && $name eq "") {
		$this->read_skipNode($reader);
		return;
	}

	my $saved_attr = $this->{read_attr};
	$this->{read_attr} = {
		association_xid		=> $association_xid,
		aggregation		=> $aggregation,
		name			=> $name,
		type_type		=> undef,
		type_primitive		=> undef,
		type_xid		=> undef,
		default_value		=> undef,
		comment			=> dr::XmiParser::Comment->new(),
	};

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "type") {
			$this->read_processOwnedAttributeType($reader);
			next;
		}
		if ($name eq "defaultValue") {
			$this->read_processOwnedAttributeDefaultValue($reader);
			next;
		}
		elsif ($name eq "ownedComment") {
			$this->read_processOwnedAttributeComment($reader);
			next;
		}
		elsif ($name eq "lowerValue") {
			$this->read_processOwnedAttributeLowerValue($reader);
			next;
		}
		elsif ($name eq "upperValue") {
			$this->read_processOwnedAttributeUpperValue($reader);
			next;
		}
		elsif ($name eq "xmi:Extension") {
			$this->read_processOwnedAttributeExtension($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	if (defined $this->{read_attr}->{lower_value} && defined $this->{read_attr}->{upper_value}) {
		if ($this->{read_attr}->{lower_value} eq 0 && $this->{read_attr}->{upper_value} eq 0) {
			$this->{read_attr}->{mandatory} = 0;
		}
		elsif ($this->{read_attr}->{lower_value} eq 0 && $this->{read_attr}->{upper_value} eq 1) {
			$this->{read_attr}->{mandatory} = 0;
		}
		elsif ($this->{read_attr}->{lower_value} eq 1 && $this->{read_attr}->{upper_value} eq 1) {
			$this->{read_attr}->{mandatory} = 1;
		}
		elsif ($this->{read_attr}->{lower_value} eq "0" && $this->{read_attr}->{upper_value} eq "*") {
			$this->{read_attr}->{mandatory} = "*";
		}
		elsif ($this->{read_attr}->{lower_value} eq "1" && $this->{read_attr}->{upper_value} eq "*") {
			$this->{read_attr}->{mandatory} = "*";
		}
		elsif ($this->{read_attr}->{lower_value} eq "*" && $this->{read_attr}->{upper_value} eq "*") {
			$this->{read_attr}->{mandatory} = undef;
			if (defined $this->{read_attr}->{aggregation} && $this->{read_attr}->{aggregation} ne "none") {
				$this->read_doDie("unexpected combination of lower/upper: * - * when aggregation type is not none");
			}
		}
		else {
			$this->read_doDie("unexpected combination of lower/upper: $this->{read_attr}->{lower_value} - $this->{read_attr}->{upper_value}");
		}
	}
	else {
		$this->{read_attr}->{mandatory} = 1;
	}

	$this->read_doDie("type unspecified for $name") unless (defined $this->{read_attr}->{type_type});

	$this->{read_context}->addAttribute($this->{read_attr});

	$this->{read_attr} = $saved_attr;
}

sub read_processOwnedAttributeType
{
	my $this		= shift;
	my $main_reader		= shift;

	my $type_type = $this->read_getMandatoryAttr("xmi:type");
	if ($type_type eq "uml:PrimitiveType") {
		$this->{read_attr}->{type_type} = "primitive";
		if ($this->read_getMandatoryAttr("href") !~ m/.*#(\w+)$/) {
			$this->read_doDie("invalid format for type href");
		}
		if (!defined ($this->{read_attr}->{type_primitive} = $UML_PRIMITIVE_TYPES{$1})) {
			$this->read_doDie("unknown UML primitive type: $1");
		}
	}
	elsif ($type_type eq "uml:Class") {
		$this->{read_attr}->{type_type} = "class";
		$this->{read_attr}->{type_xid} = $this->read_getMandatoryAttr("xmi:idref");
	}
	else {
		$this->read_doDie("unknown type type: $type_type");
	}

	$this->read_needNodeEnd($main_reader);
}

sub read_processOwnedAttributeDefaultValue
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->{read_attr}->{default_value} = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd($main_reader);
}

sub read_processOwnedAttributeComment
{
	my $this		= shift;
	my $main_reader		= shift;

	eval {
		$this->{read_attr}->{comment}->processComment($this->read_getMandatoryAttr("body"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd($main_reader);
}

sub read_processOwnedAttributeLowerValue
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->{read_attr}->{lower_value} = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd($main_reader);
}

sub read_processOwnedAttributeUpperValue
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->{read_attr}->{upper_value} = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd($main_reader);
}

sub read_processOwnedAttributeExtension
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "taggedValue") {
			$this->read_processGenericTagged($reader, $this->{read_attr}->{comment});
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
}

sub read_processOwnedOperation
{
	my $this		= shift;
	my $main_reader		= shift;
	my $reader		= $main_reader->getSubLeveler();

	$this->{read_oper} = {
		name			=> $main_reader->getMandatoryAttr("name"),
		classify		=> { dr::Util::hashTrue(
			static			=> defvalue($main_reader->getOptionalAttr("isStatic"), "false") eq "true",
			abstract		=> defvalue($main_reader->getOptionalAttr("isAbstract"), "false") eq "true",
		)},
		comment			=> dr::XmiParser::Comment->new(),
	};

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "xmi:Extension") {
			$this->read_processOwnedOperationExtension($reader);
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	$this->{read_context}->addOperation($this->{read_oper});
}

sub read_processOwnedOperationExtension
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "taggedValue") {
			$this->read_processGenericTagged($reader, $this->{read_oper}->{comment});
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
}

sub read_processEnum
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Class->new($this, "enum");
	$this->{read_context}->{stereotype} = "enum";
	$this->{read_context}->{literal_list} = [];

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "ownedComment") {
			$this->read_processGenericComment($reader, $this->{read_context}->{comment});
			next;
		}
		elsif ($name eq "xmi:Extension") {
			$this->read_processEnumExtension($reader);
			next;
		}
		elsif ($name eq "ownedLiteral") {
			$this->read_processEnumLiteral($reader);
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	push(@{$saved_context->{class_list}}, $this->{read_context});
	$saved_context->{class_hash}->{$this->{read_context}->{name}} = $this->{read_context};

	$this->{read_context} = $saved_context;
}

sub read_processEnumExtension
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "stereotype") {
			$this->read_processEnumExtensionStereotype($reader);
		}
		elsif ($name eq "taggedValue") {
			$this->read_processGenericTagged($reader, $this->{read_context}->{comment});
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
}

sub read_processEnumExtensionStereotype
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_doDie("expected stereotype enum for enum") unless ($main_reader->getMandatoryAttr("name") eq "enum");

	$this->read_needNodeEnd($main_reader);
}

sub read_processEnumLiteral
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();

	my $literal = {
		name			=> $this->read_getMandatoryAttr("name"),
		xmi_id			=> $this->read_getMandatoryAttr("xmi:id"),
		comment			=> $this->createGenericComment(),
		default			=> undef,
	};

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "ownedComment") {
			$this->read_processGenericComment($reader, $literal->{comment});
			next;
		}
		elsif ($name eq "xmi:Extension") {
			$this->read_processEnumLiteralExtension($reader, $literal);
		}
		elsif ($name eq "defaultValue") {
			$this->read_processEnumLiteralDefault($reader, \$literal->{default});
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}

	push(@{$this->{read_context}->{literal_list}}, $literal);
}

sub read_processEnumLiteralDefault
{
	my $this		= shift;
	my $main_reader		= shift;
	my $default		= shift;

	$$default = $main_reader->getMandatoryAttr("value");

	$this->read_needNodeEnd($main_reader);
}

sub read_processEnumLiteralExtension
{
	my $this		= shift;
	my $reader		= shift->getSubLeveler();
	my $literal		= shift;

	while (defined (my $name = $reader->readNode())) {
		if ($name eq "taggedValue") {
			$this->read_processGenericTagged($reader, $literal->{comment});
			next;
		}
		else {
			$this->read_unknownNode($reader);
		}
	}
}

sub read_processArtifact
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_skipNode($main_reader);
}

sub read_processAssociation
{
	my $this		= shift;
	my $main_reader		= shift;

	$this->read_skipNode($main_reader);
}

sub read_processDataType
{
	my $this		= shift;
	my $main_reader		= shift;

	my $datatype = {
		name			=> $main_reader->getMandatoryAttr("name"),
		xmi_id			=> $main_reader->getMandatoryAttr("xmi:id"),
	};

	push(@{$this->{datatypes}}, $datatype);

	$this->read_needNodeEnd($main_reader);
}

sub postprocRegisterDatatype
{
	my $this		= shift;
	my $datatype		= shift;

	$this->{datatype_xi}->{$datatype->{xmi_id}} = $datatype;
}

sub postprocRegisterPackage
{
	my $this		= shift;
	my $package		= shift;

	$this->{package_xi}->{$package->{xmi_id}} = $package;

	foreach my $ch_package (@{$package->{package_list}}) {
		Scalar::Util::weaken($ch_package->{parent} = $package);
		$this->postprocRegisterPackage($ch_package);
	}
	foreach my $ch_class (@{$package->{class_list}}) {
		Scalar::Util::weaken($ch_class->{parent} = $package);
		$this->postprocRegisterClass($ch_class);
	}
}

sub postprocRegisterClass
{
	my $this		= shift;
	my $class		= shift;

	$this->{class_xi}->{$class->{xmi_id}} = $class;

	foreach my $ch_class (@{$class->{class_list}}) {
		Scalar::Util::weaken($ch_class->{parent} = $class);
		$this->postprocRegisterClass($ch_class);
	}

	foreach my $attr (@{$class->{attr_list}}) {
		Scalar::Util::weaken($attr->{parent} = $class);
		$this->postprocRegisterClassAttr($attr);
	}
}

sub postprocRegisterClassAttr
{
	my $this		= shift;
	my $attr		= shift;
}

sub resolveDatatype
{
	my $this		= shift;
	my $type_xi		= shift;

	dr::Util::doDie("type_xi undefined") unless (defined $type_xi);
	if (exists $this->{datatype_xi}->{$type_xi}) {
		return $this->{datatype_xi}->{$type_xi};
	}
	elsif (exists $this->{class_xi}->{$type_xi}) {
		return $this->{class_xi}->{$type_xi};
	}
	die "xmi_id $type_xi not defined as datatype";
}

sub postprocRefPackage
{
	my $this		= shift;
	my $package		= shift;

	my $resolved = 0;

	$this->{package_xi}->{$package->{xmi_id}} = $package;

	foreach my $ch_package (@{$package->{package_list}}) {
		$this->postprocRefPackage($ch_package);
	}
	foreach my $ch_class (@{$package->{class_list}}) {
		$this->postprocRefClass($ch_class);
	}

	return $resolved;
}

sub postprocRefClass
{
	my $this		= shift;
	my $class		= shift;

	my $resolved = 0;

	for (my $i = 0; $i < @{$class->{attr_list}}; $i++) {
		$i-- if ($this->postprocRefClassAttr(${$class->{attr_list}}[$i]));
	}
	foreach my $ch_class (@{$class->{class_list}}) {
		$this->postprocRefClass($ch_class);
	}
	if (defined $class->{ancestor_xid}) {
		Scalar::Util::weaken($class->{ancestor} = $this->resolveDatatype($class->{ancestor_xid}));
	}

	return $resolved;
}

sub postprocRefClassAttr
{
	my $this		= shift;
	my $attr		= shift;

	eval {
		$attr->{datatype} = $attr->{type_primitive} || $this->resolveDatatype($attr->{type_xid});
		1;
	}
		or die "$attr->{parent}->{name}.$attr->{name}: $@";

	if (!defined $attr->{mandatory}) {
		if ($attr->{aggregation} eq "none") {
			my $found_attr;
			foreach my $assoc_attr (@{$attr->{datatype}->{attr_list}}) {
				if (defined $assoc_attr->{association_xid} && $assoc_attr->{association_xid} eq $attr->{association_xid}) {
					$found_attr = $assoc_attr;
					last;
				}
			}
			if (!defined $found_attr) {
				dr::Util::doDie("$attr->{parent}->{name}.$attr->{name}: association target not found: $attr->{association_xid}");
			}
			if ($found_attr->{aggregation} ne "composite") {
				dr::Util::doDie("$attr->{parent}->{name}.$attr->{name}: association target is not composite as expected for * - * multiplicity: ".dumpSimple($found_attr));
			}
			$attr->{aggregation} = "child";
			$attr->{mandatory} = "*";
		}
		else {
			dr::Util::doDie("$attr->{parent}->{name}.$attr->{name}: mandatory not defined and aggregation is not none");
		}
	}

	return 0;
}

sub postprocess
{
	my $this		= shift;

	foreach my $datatype (@{$this->{datatypes}}) {
		$datatype->{parent} = undef;
		$this->postprocRegisterDatatype($datatype);
	}
	foreach my $package (@{$this->{packages}}) {
		$package->{parent} = undef;
		$this->postprocRegisterPackage($package);
	}

	foreach my $package (@{$this->{packages}}) {
		$this->postprocRefPackage($package);
	}
}

sub getPrefixName
{
	my $this		= shift;
	my $obj			= shift;

	return (defined $obj->{parent} ? $this->getFullName($obj->{parent}) : "");
}

sub getFullName
{
	my $this		= shift;
	my $obj			= shift;

	return (defined $obj->{parent} ? $this->getFullName($obj->{parent})."::" : "") . $obj->{name};
}

sub formatDatatype
{
	my $this		= shift;
	my $class		= shift;

	return ref($class) ? $this->getFullName($class) : $class;
}


1;
