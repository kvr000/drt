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

package dr::XmiParser::Comment;

use strict;
use warnings;

sub new
{
	my $ref			= shift;
	my $class		= ref($ref) || $ref;

	my $this = bless {
		comment			=> [],
		dr_list			=> [],
		dr_hash			=> {},
	}, $class;

	return $this;
}

sub processComment
{
	my $this		= shift;
	my $body		= shift;

	foreach my $line (split("\n", $body)) {
		if ($line =~ m/^(drt?-)(\w+):\s*(.*?)\s*$/) {
			my $comment = { name => $2, value => $3 };
			push(@{$this->{dr_list}}, $comment);
			die "drcomment $2 already specified" if (defined $this->{dr_hash}->{$2});
			$this->{dr_hash}->{$comment->{name}} = $comment;
		}
		else {
			push(@{$this->{comment}}, "$line");
		}
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
}

sub addAttribute
{
	my $this		= shift;
	my $attr		= shift;

	push(@{$this->{attr_list}}, $attr);
	$this->{attr_hash}->{$attr->{name}} = $attr;
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
		return $this->{owner}->resolveDatatype($attr->{type_xmi_id});
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

	my $stack = "";
	for (my $i = 0; ; $i++) {
		my ($pack, $fn, $ln, $sub) = caller($i)
			or last;
		$stack .= "$sub ($fn:$ln)\n";
	}
	$stack =~ s/^/\t/gm;
	die "on element ".$reader->name().": $msg\nstack:\n$stack";
}

sub read_warnUnknown
{
	my $this		= shift;

	my $reader		= $this->{reader};

	$this->read_doDie("unexpected element");
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

	$this->read_warnUnknown();
	$this->read_skipNode();
}

sub read_needNodeEnd
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	while (my ($type, $name, $depth) = $this->readNode()) {
		return if ($depth == $exit_depth);
		$this->read_warnUnknown();
		$this->read_skipNode();
	}
	die "cannot get here";
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

	my $reader		= $this->{reader};

	while (my ($type, $name, $depth) = $this->readNode()) {
		if ($type == XML_READER_TYPE_ELEMENT) {
			if ($name eq "xmi:XMI") {
				$this->read_processXmi($depth);
				next;
			}
			else {
				$this->read_doDie("expected xmi:XMI as root element, got $name");
			}
		}
		else {
			$this->read_doDie("expected opening xmi:XMI, got ".$reader->nodeType." - ".$reader->name);
		}
	}
}

sub read_processXmi
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		return if ($depth == $exit_depth);
		if ($type == XML_READER_TYPE_ELEMENT) {
			if ($name eq "uml:Model") {
				$this->read_processModel();
				next;
			}
			elsif ($name eq "xmi:Documentation") {
				$this->read_skipNode();
				next;
			}
			else {
				$this->read_unknownNode();
			}
		}
	}
	die "cannot get here";
}

sub createGenericComment
{
	my $this		= shift;

	return dr::XmiParser::Comment->new();
}

sub read_processGenericComment
{
	my $this		= shift;
	my $comment		= shift;

	my $reader		= $this->{reader};

	eval {
		$comment->processComment($this->read_getMandatoryAttr("body"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd();
}

sub read_processModel
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		return if ($depth == $exit_depth);
		if ($type == XML_READER_TYPE_ELEMENT) {
			if ($name eq "packagedElement") {
				$this->read_processPackagedElement();
				next;
			}
			else {
				$this->read_unknownNode();
			}
		}
	}
	die "cannot get here";
}

sub read_processPackagedElement
{
	my $this		= shift;

	my $reader		= $this->{reader};

	my $type = $this->read_getMandatoryAttr("xmi:type");
	if ($type eq "uml:Package") {
		$this->read_processPackage();
	}
	elsif ($type eq "uml:Class") {
		$this->read_processClass();
	}
	elsif ($type eq "uml:AssociationClass") {
		$this->read_processAssociationClass();
	}
	elsif ($type eq "uml:Enumeration") {
		$this->read_processEnum();
	}
	elsif ($type eq "uml:Association") {
		$this->read_processAssociation();
	}
	elsif ($type eq "uml:Artifact") {
		$this->read_processArtifact();
	}
	elsif ($type eq "uml:DataType") {
		$this->read_processDataType();
	}
	else {
		$this->read_doDie("unknown type: $type");
	}
}

sub read_processPackage
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Package->new($this, "package");

	my @classes;

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "packagedElement") {
			$this->read_processPackagedElement();
			next;
		}
		elsif ($name eq "xmi:Extension") {
			$this->read_skipNode();
			next;
		}
		else {
			$this->read_unknownNode();
		}
	}
	push(@{$this->{packages}}, $this->{read_context});
	$this->{read_context} = $saved_context;
}

sub read_processClass
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	$this->read_doDie("class definition out of package context") unless (defined $this->{read_context});

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Class->new($this, "class");

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "xmi:Extension") {
			$this->read_processClassExtensions();
			next;
		}
		elsif ($name eq "ownedAttribute") {
			$this->read_processOwnedAttribute();
			next;
		}
		elsif ($name eq "ownedComment") {
			$this->read_processClassComment();
			next;
		}
		elsif ($name eq "nestedClassifier") {
			$this->read_processNestedClassifier();
			next;
		}
		else {
			$this->read_unknownNode();
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

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth();

	my $association = $this->read_getOptionalAttr("association");
	my $name = $this->read_getMandatoryAttr("name");

	if (defined $association && $name eq "") {
		$this->read_skipNode();
		return;
	}

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Class->new($this, "association");

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "xmi:Extension") {
			$this->read_processClassExtensions();
			next;
		}
		elsif ($name eq "ownedAttribute") {
			$this->read_processOwnedAttribute();
			next;
		}
		elsif ($name eq "ownedComment") {
			$this->read_processClassComment();
			next;
		}
		elsif ($name eq "nestedClassifier") {
			$this->read_processNestedClassifier();
			next;
		}
		elsif ($name eq "memberEnd") {
			$this->read_processClassMemberEnd();
			next;
		}
		else {
			$this->read_unknownNode();
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

	return $this->read_processPackagedElement();
}

sub read_processClassExtensions
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth;

	my $saved_extender = $this->{read_extender};
	$this->{read_extender} = $this->read_getMandatoryAttr("extender");

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "stereotype") {
			$this->read_processClassExtensionStereotype();
			next;
		}
		elsif ($name eq "typedef") {
			$this->read_processClassExtensionTypedef();
			next;
		}
		else {
			$this->read_unknownNode();
		}
	}

	$this->{read_extender} = $saved_extender;
}

sub read_processClassExtensionStereotype
{
	my $this		= shift;

	my $reader		= $this->{reader};

	$this->{read_context}->{stereotype} = $this->read_getMandatoryAttr("name");

	$this->read_needNodeEnd();
}

sub read_processClassExtensionTypedef
{
	my $this		= shift;

	my $reader		= $this->{reader};

	$this->{read_context}->{type} = "typedef";

	$this->{read_context}->{typedef} = { base_name => $this->read_getMandatoryAttr("base"), base_xid => $this->read_getOptionalAttr("xmi:idref") };

	$this->read_needNodeEnd();
}

sub read_processClassComment
{
	my $this		= shift;

	my $reader		= $this->{reader};

	eval {
		$this->{read_context}->{comment}->processComment($this->read_getMandatoryAttr("body"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd();
}

sub read_processClassMemberEnd
{
	my $this		= shift;

	$this->read_skipNode();
}

sub read_processOwnedAttribute
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth();

	my $association_xid = $this->read_getOptionalAttr("association");
	my $aggregation = $this->read_getOptionalAttr("aggregation");
	my $name = $this->read_getMandatoryAttr("name");

	if (defined $association_xid && $name eq "") {
		$this->read_skipNode();
		return;
	}

	my $saved_attr = $this->{read_attr};
	$this->{read_attr} = {
		association_xid		=> $association_xid,
		aggregation		=> $aggregation,
		name			=> $name,
		type_type		=> undef,
		type_primitive		=> undef,
		type_xmi_id		=> undef,
		default_value		=> undef,
		comment			=> dr::XmiParser::Comment->new(),
	};

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "type") {
			$this->read_processOwnedAttributeType();
			next;
		}
		if ($name eq "defaultValue") {
			$this->read_processOwnedAttributeDefaultValue();
			next;
		}
		elsif ($name eq "ownedComment") {
			$this->read_processOwnedAttributeComment();
			next;
		}
		elsif ($name eq "lowerValue") {
			$this->read_processOwnedAttributeLowerValue();
			next;
		}
		elsif ($name eq "upperValue") {
			$this->read_processOwnedAttributeUpperValue();
			next;
		}
		else {
			$this->read_unknownNode();
		}
	}

	$this->read_doDie("type unspecified") unless (defined $this->{read_attr}->{type_type});

	$this->{read_context}->addAttribute($this->{read_attr});

	$this->{read_attr} = $saved_attr;
}

sub read_processOwnedAttributeType
{
	my $this		= shift;

	my $reader		= $this->{reader};

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
		$this->{read_attr}->{type_xmi_id} = $this->read_getMandatoryAttr("xmi:idref");
	}
	else {
		$this->read_doDie("unknown type type: $type_type");
	}

	$this->read_needNodeEnd();
}

sub read_processOwnedAttributeDefaultValue
{
	my $this		= shift;

	my $reader		= $this->{reader};

	$this->{read_attr}->{default_value} = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd();
}

sub read_processOwnedAttributeComment
{
	my $this		= shift;

	my $reader		= $this->{reader};

	eval {
		$this->{read_attr}->{comment}->processComment($this->read_getMandatoryAttr("body"));
		1;
	}
		or $this->read_doDie($@);

	$this->read_needNodeEnd();
}

sub read_processOwnedAttributeLowerValue
{
	my $this		= shift;

	my $reader		= $this->{reader};

	$this->{read_attr}->{lower_value} = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd();
}

sub read_processOwnedAttributeUpperValue
{
	my $this		= shift;

	my $reader		= $this->{reader};

	$this->{read_attr}->{upper_value} = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd();
}

sub read_processEnum
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth();

	my $name = $this->read_getMandatoryAttr("name");

	my $saved_context = $this->{read_context};
	$this->{read_context} = dr::XmiParser::Class->new($this, "enum");
	$this->{read_context}->{stereotype} = "enum";
	$this->{read_context}->{literal_list} = [];

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "ownedComment") {
			$this->read_processGenericComment($this->{read_context}->{comment});
			next;
		}
		elsif ($name eq "xmi:Extension") {
			$this->read_processEnumExtension();
			next;
		}
		elsif ($name eq "ownedLiteral") {
			$this->read_processEnumLiteral();
			next;
		}
		else {
			$this->read_unknownNode();
		}
	}

	push(@{$saved_context->{class_list}}, $this->{read_context});
	$saved_context->{class_hash}->{$this->{read_context}->{name}} = $this->{read_context};

	$this->{read_context} = $saved_context;
}

sub read_processEnumExtension
{
	my $this		= shift;

	$this->read_skipNode();
}

sub read_processEnumLiteral
{
	my $this		= shift;

	my $reader		= $this->{reader};
	my $exit_depth		= $reader->depth();

	my $literal = {
		name			=> $this->read_getMandatoryAttr("name"),
		xmi_id			=> $this->read_getMandatoryAttr("xmi:id"),
		comment			=> $this->createGenericComment(),
		default			=> undef,
	};

	for (;;) {
		my ($type, $name, $depth) = $this->readNode()
			or $this->read_doDie("unexpected end of XML");
		last if ($depth == $exit_depth);
		if ($name eq "ownedComment") {
			$this->read_processGenericComment($literal->{comment});
			next;
		}
		elsif ($name eq "defaultValue") {
			$this->read_processEnumLiteralDefault(\$literal->{default});
			next;
		}
		else {
			$this->read_unknownNode();
		}
	}

	push(@{$this->{read_context}->{literal_list}}, $literal);
}

sub read_processEnumLiteralDefault
{
	my $this		= shift;
	my $default		= shift;

	$$default = $this->read_getMandatoryAttr("value");

	$this->read_needNodeEnd();
}

sub read_processArtifact
{
	my $this		= shift;

	$this->read_skipNode();
}

sub read_processAssociation
{
	my $this		= shift;

	$this->read_skipNode();
}

sub read_processDataType
{
	my $this		= shift;

	my $reader		= $this->{reader};

	my $datatype = {
		name			=> $this->read_getMandatoryAttr("name"),
		xmi_id			=> $this->read_getMandatoryAttr("xmi:id"),
	};

	push(@{$this->{datatypes}}, $datatype);

	$this->read_needNodeEnd();
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

sub postprocRefPackage
{
	my $this		= shift;
	my $package		= shift;

	my $resolved = 0;

	$this->{package_xi}->{$package->{xmi_id}} = $package;

	foreach my $ch_package (@{$package->{package_list}}) {
		Scalar::Util::weaken($ch_package->{parent} = $package);
		$this->postprocessPackage($ch_package);
	}
	foreach my $ch_class (@{$this->{class_list}}) {
		Scalar::Util::weaken($ch_class->{parent} = $package);
		$this->postprocessClass($ch_class);
	}

	return $resolved;
}

sub postprocRefClass
{
	my $this		= shift;
	my $class		= shift;

	my $resolved = 0;

	foreach my $attr (@{$class->{attr_list}}) {
		Scalar::Util::weaken($attr->{parent} = $class);
		$this->postprocessClassAttr($attr);
	}

	return $resolved;
}

sub resolveDatatype
{
	my $this		= shift;
	my $type_xi		= shift;

	if (exists $this->{datatype_xi}->{$type_xi}) {
		return $this->{datatype_xi}->{$type_xi};
	}
	elsif (exists $this->{class_xi}->{$type_xi}) {
		return $this->{class_xi}->{$type_xi};
	}
	die "xmi_id $type_xi not defined as datatype";
}

sub postprocRefClassAttr
{
	my $this		= shift;
	my $attr		= shift;

	my $resolved = 0;

	$attr->{datatype} = $this->resolveDatatype($attr->{type_xi});

	return $resolved;
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
