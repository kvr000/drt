/**
 * drt library
 *
 * drt multiplatform development toolkit
 *
 * ----------------------------------------------------------------------------------
 *
 * Copyright (C) 2004-2008 Zbyněk Vyškovský
 *
 * ----------------------------------------------------------------------------------
 *
 * LICENSE:
 *
 * This file is part of drt
 *
 * drt is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * drt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with drt; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @copyright	2004-2008 Zbyněk Vyškovský
 * @link	mailto:kvr@matfyz.cz
 * @link	http://kvr.matfyz.cz/drt/
 * @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
 **/

/*drt
 * include: 	dr/Object.hxx
 * include: 	dr/Blob.hxx
 * include: 	dr/String.hxx
 * ns:		dr
 * class:	Variant
 * ancestor:	dr::Object
 * ifaces:	
 *
 * co:{
 * public:
 * 	enum VType
 * 	{
 * 		VT_Invalid,
 * 		VT_Null,
 * 		VT_Bool,
 * 		VT_Int,
 * 		VT_Double,
 * 		VT_Binary,
 * 		VT_String,
 * 		VT_Object,
 * 		//VT_Array,
 * 		//VT_Hash,
 * 		//VT_List,
 * 		//VT_Avl,
 * 	};
 * }co
 *
 * co:{
 * 	int				vtype:7;
 * 	int				is_null:1;
 * 	union
 * 	{
 * 		bool				bool_val;
 * 		Sint64				int_val;
 * 		double				double_val;
 *		Blob::Data *			binary_val;
 *		String::Data *			string_val;
 *		Object *			object_val;
 * 	};
 * }co
 */

#include "_gen/Variant-all.hxx"

DR_NS_BEGIN


DR_MET(public)
Variant::Variant():
	vtype(VT_Invalid),
	is_null(1)
{
}

DR_MET(public)
Variant::Variant(const Null &):
	vtype(VT_Null)
{
	is_null = 1;
}

DR_MET(public)
Variant::Variant(bool bool_val_):
	vtype(VT_Bool)
{
	bool_val = bool_val_;
}

DR_MET(public)
Variant::Variant(Sint64 int_val_):
	vtype(VT_Int)
{
	int_val = int_val_;
}

DR_MET(public)
Variant::Variant(double double_val_):
	vtype(VT_Double)
{
	double_val = double_val_;
}

DR_MET(public)
Variant::Variant(const Blob &binary_val_):
	vtype(VT_Binary)
{
	*(Blob *)&binary_val = binary_val_;
}

DR_MET(public)
Variant::Variant(const String &string_val_):
	vtype(VT_String)
{
	*(String *)&string_val = string_val_;
}

DR_MET(public)
Variant::Variant(Iface *object_val_):
	vtype(VT_Object)
{
	object_val = object_val_ == NULL ? NULL : object_val_->ref();
}

DR_MET(public virtual)
Variant::VType Variant::getType()
{
	return (VType)vtype;
}

DR_MET(public virtual)
String Variant::getName()
{
	switch ((VType)vtype) {
	case VT_Invalid:
		return DR_STR(invalid);

	case VT_Null:
		return DR_STR(null);

	case VT_Bool:
		return DR_STR(bool);

	case VT_Int:
		return DR_STR(int);

	case VT_Double:
		return DR_STR(double);

	case VT_Binary:
		return DR_STR(binary);

	case VT_String:
		return DR_STR(string);

	case VT_Object:
		return object_val == NULL ? DR_STR(null) : object_val->classname();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
bool Variant::isNull()
{
	switch ((VType)vtype) {
	case VT_Invalid:
		return false;

	case VT_Null:
		return true;

	case VT_Bool:
	case VT_Int:
	case VT_Double:
		return false;

	case VT_Binary:
		return ((Blob *)&binary_val)->isNull();

	case VT_String:
		return ((String *)&string_val)->isNull();

	case VT_Object:
		return object_val == NULL;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return false;
}

DR_MET(public virtual)
bool Variant::toBool()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
		return false;

	case VT_Bool:
		return bool_val;

	case VT_Int:
		return int_val != 0;

	case VT_Double:
		return double_val != 0;

	case VT_Binary:
		return strtoll(((Blob *)&binary_val)->toStr(), NULL, 10) != 0;

	case VT_String:
		return strtoll(((String *)&string_val)->utf8().toStr(), NULL, 10) != 0;

	case VT_Object:
		return object_val != NULL;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return false;
}

DR_MET(public virtual)
Sint64 Variant::toInt()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
		return 0;

	case VT_Bool:
		return (int)bool_val;

	case VT_Int:
		return int_val;

	case VT_Double:
		return (Sint64)double_val;

	case VT_Binary:
		return strtoll(((Blob *)&binary_val)->toStr(), NULL, 10);

	case VT_String:
		return strtoll(((String *)&string_val)->utf8().toStr(), NULL, 10);

	case VT_Object:
		return (SintPtr)object_val;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return 0;
}

DR_MET(public virtual)
double Variant::toDouble()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
		return 0;

	case VT_Bool:
		return (double)bool_val;

	case VT_Int:
		return (double)int_val;

	case VT_Double:
		return double_val;

	case VT_Binary:
		return strtod(((Blob *)&binary_val)->toStr(), NULL);

	case VT_String:
		return strtod(((String *)&string_val)->utf8().toStr(), NULL);

	case VT_Object:
		return (double)(SintPtr)object_val;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return 0;
}

DR_MET(public virtual)
String Variant::toString()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
		return Null();

	case VT_Bool:
		return bool_val ? DR_STR(1) : DR_STR(0);

	case VT_Int:
		return String::createNumber(int_val);

	case VT_Double:
		return String::createNumber(double_val);

	case VT_Binary:
		return String::createUtf8(*(Blob *)&binary_val);

	case VT_String:
		return *(String *)&string_val;

	case VT_Object:
		return object_val ? Null() : object_val->stringify();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
BString Variant::toUtf8()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
		return Null();

	case VT_Bool:
		return bool_val ? DR_BSTR(1) : DR_BSTR(0);

	case VT_Int:
		return BString::createNumber(int_val);

	case VT_Double:
		return BString::createNumber(double_val);

	case VT_Binary:
		return *(Blob *)&binary_val;

	case VT_String:
		return ((String *)&string_val)->utf8();

	case VT_Object:
		return object_val ? Null() : object_val->stringify().utf8();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
Blob Variant::toBinary()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
		return Null();

	case VT_Bool:
		return bool_val ? DR_BSTR(1) : DR_BSTR(0);

	case VT_Int:
		return BString::createNumber(int_val);

	case VT_Double:
		return BString::createNumber(double_val);

	case VT_Binary:
		return *(Blob *)&binary_val;

	case VT_String:
		return ((String *)&string_val)->utf8();

	case VT_Object:
		return object_val ? Null() : object_val->stringify().utf8();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
Object *Variant::toObject()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
	case VT_Bool:
	case VT_Int:
	case VT_Double:
	case VT_Binary:
	case VT_String:
		return NULL;

	case VT_Object:
		return object_val ? object_val->ref() : NULL;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return NULL;
}

DR_MET(protected)
Variant::~Variant()
{
	switch ((VType)vtype) {
	case VT_Invalid:
		break;

	case VT_Bool:
	case VT_Int:
	case VT_Double:
		vtype = VT_Invalid;
		break;

	case VT_Binary:
		vtype = VT_Invalid;
		((Blob *)&binary_val)->~Blob();
		break;

	case VT_String:
		vtype = VT_Invalid;
		((String *)&string_val)->~String();
		break;

	case VT_Object:
		vtype = VT_Invalid;
		object_val->unref();
		break;

	default:
		DR_AssertMsg("Invalid type for Variant Type");
	}
}


DR_NS_END
