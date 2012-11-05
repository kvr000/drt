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
 * include: 	dr/Array.hxx
 * include: 	dr/Hash.hxx
 *
 * ns:		dr
 */

#include "_gen/Variant-all.hxx"

#ifdef DR_LIBC_VC
# define strtoll _strtoi64
#endif

DR_NS_BEGIN


/*drt
 * class:	Variant
 * ancestor:	dr::Object
 *
 * co:public{
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
 * 		VT_Array,
 * 		VT_Hash,
 * 		//VT_List,
 * 		//VT_Avl,
 * 	};
 * }co
 *
 * co:protected{
 * 	int				vtype;
 * 	union
 * 	{
 * 		bool				bool_val;
 * 		Sint64				int_val;
 * 		double				double_val;
 *		Blob::Data *			binary_val;
 *		String::Data *			string_val;
 *		Object *			object_val;
 *		RArray<Variant> *		array_val;
 *		RHash<String, Variant> *	hash_val;
 * 	};
 * }co
 *
 * doc:{
 * 	Holder of generic values
 * }doc
 */

DR_MET(public)
Variant::Variant():
	vtype(VT_Invalid)
{
}

DR_MET(public)
Variant::Variant(const Null &):
	vtype(VT_Null)
{
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
	if (binary_val_.isNull())
		vtype = VT_Null;
	else
		new (&binary_val) Blob(binary_val_);
}

DR_MET(public)
Variant::Variant(const String &string_val_):
	vtype(VT_String)
{
	if (string_val_.isNull())
		vtype = VT_Null;
	else
		new (&string_val) String(string_val_);
}

DR_MET(public)
Variant::Variant(Iface *object_val_):
	vtype(VT_Object)
{
	if (object_val_ == NULL)
		vtype = VT_Null;
	else
		object_val = object_val_->ref();
}

DR_MET(public)
Variant::Variant(RArray<Variant> *array_):
	vtype(VT_Array)
{
	if (array_ == NULL)
		vtype = VT_Null;
	else
		array_val = (RArray<Variant> *)array_->ref();
}

DR_MET(public)
Variant::Variant(RHash<String, Variant> *hash_):
	vtype(VT_Hash)
{
	if (hash_ == NULL)
		vtype = VT_Null;
	else
		hash_val = (RHash<String, Variant> *)hash_->ref();
}

DR_MET(public virtual)
Variant::VType Variant::getType() const
{
	return (VType)vtype;
}

DR_MET(protected)
Variant::~Variant()
{
	switch ((VType)vtype) {
	case VT_Invalid:
	case VT_Null:
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

DR_MET(public virtual)
long Variant::hash() const
{
	switch ((VType)vtype) {
	case VT_Invalid:
		return -2;

	case VT_Null:
		return -1;

	case VT_Bool:
		return bool_val;

	case VT_Int:
		return (long)int_val;

	case VT_Double:
		if (double_val == 0)
			return 0;
		if (sizeof(long) >= sizeof(double_val)) {
			const void *dvp = &double_val;
			return *(const long *)dvp;
		}
		else {
			const void *dvp = &double_val;
			long h = *(const long *)dvp;
			for (size_t i = 1; i < sizeof(double_val)/sizeof(long); i++) {
				h ^= ((long *)(void *)&double_val)[i];
			}
			return h;
		}

	case VT_Binary:
		return ((Blob *)binary_val)->getHash();

	case VT_String:
		return ((String *)string_val)->getHash();

	case VT_Object:
	case VT_Hash:
	case VT_Array:
		if (object_val == NULL)
			return -1;
		return object_val->hash();
	}
	DR_AssertInvalid();
	return 0;
}

DR_MET(public virtual)
bool Variant::eq(const Iface *vs_) const
{
	if (!vs_) {
		return false;
	}
	else if (Variant *vs = (Variant *)vs_->accCheckFinal(comp_name)) {
		switch ((VType)vtype) {
		case VT_Invalid:
			return vs->vtype == VT_Invalid;

		case VT_Null:
			if (vs->vtype == VT_Null)
				return true;
			return vs->isNull();

		case VT_Bool:
			if (vs->vtype == VT_Bool) {
				return vs->bool_val == bool_val;
			}
			break;

		case VT_Int:
			if (vs->vtype == VT_Int) {
				return vs->int_val == int_val;
			}
			else if (vs->vtype == VT_Double) {
				return vs->double_val == int_val;
			}
			break;

		case VT_Double:
			if (vs->vtype == VT_Double) {
				return vs->double_val == double_val;
			}
			else if (vs->vtype == VT_Int) {
				return vs->int_val == double_val;
			}
			break;

		case VT_Binary:
			if (((Blob *)&binary_val)->isNull() && vs->isNull())
				return true;
			if (vs->vtype == VT_Binary) {
				return *(Blob *)&vs->binary_val == *(Blob *)&binary_val;
			}
			break;

		case VT_String:
			if (((String *)&string_val)->isNull() && vs->isNull())
				return true;
			if (vs->vtype == VT_String) {
				return *(String *)&vs->string_val == *(String *)&string_val;
			}
			break;

		case VT_Object:
		case VT_Array:
		case VT_Hash:
			if (object_val == NULL && vs->isNull())
				return true;
			if (vs->vtype == VT_Object && vs->object_val != NULL) {
				return object_val->eq(vs->object_val);
			}
			return false;
		}
		return !vs->isNull() && toBinary() == vs->toBinary();
	}
	else {
		return false;
	}
}

DR_MET(public virtual)
int Variant::cmp(const Iface *vs_) const
{
	if (!vs_) {
		return 1;
	}
	else if (Variant *vs = (Variant *)vs_->accCheckFinal(comp_name)) {
		switch ((VType)vtype) {
		case VT_Invalid:
			if (vs->vtype == VT_Invalid)
				return 0;
			return -1;

		case VT_Null:
			if (vs->vtype == VT_Null)
				return 0;
			if (vs->isNull())
				return 0;
			return -1;

		case VT_Bool:
			if (vs->vtype == VT_Bool) {
				return vs->bool_val != bool_val ? bool_val-vs->bool_val : 0;
			}
			break;

		case VT_Int:
			if (vs->vtype == VT_Int) {
				return vs->int_val != int_val ? int_val > vs->int_val ? 1 : -1 : 0;
			}
			else if (vs->vtype == VT_Double) {
				return vs->double_val != int_val ? int_val > vs->double_val ? 1 : -1 : 0;
			}
			break;

		case VT_Double:
			if (vs->vtype == VT_Double) {
				return vs->double_val == double_val;
			}
			else if (vs->vtype == VT_Int) {
				return vs->int_val != double_val ? double_val > vs->int_val ? 1 : -1 : 0;
			}
			break;

		case VT_Binary:
			if (((Blob *)&binary_val)->isNull() && vs->isNull())
				return 0;
			if (vs->vtype == VT_Binary) {
				return ((Blob *)&vs->binary_val)->bcmp(*(Blob *)&binary_val);
			}
			break;

		case VT_String:
			if (((String *)&string_val)->isNull() && vs->isNull())
				return 0;
			if (vs->vtype == VT_String) {
				return string_val->bcmp(string_val);
			}
			break;

		case VT_Object:
		case VT_Array:
		case VT_Hash:
			if (object_val == NULL && vs->isNull())
				return 0;
			if (vs->vtype == VT_Object) {
				if (vs->object_val == NULL)
					return 1;
				return object_val->cmp(vs->object_val);
			}
			break;
		}
		if (vs->isNull())
			return 1;
		return toBinary().bcmp(vs->toBinary());
	}
	else {
		return -1;
	}
}

DR_MET(public virtual)
String Variant::getName() const
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
	case VT_Array:
	case VT_Hash:
		return object_val == NULL ? DR_STR(null) : object_val->classname();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
bool Variant::isNull() const
{
	return vtype == VT_Null;
}

DR_MET(public virtual)
bool Variant::toBool() const
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
	case VT_Array:
	case VT_Hash:
		return object_val != NULL;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return false;
}

DR_MET(public virtual)
Sint64 Variant::toInt() const
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
	case VT_Array:
	case VT_Hash:
		return (SintPtr)object_val;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return 0;
}

DR_MET(public virtual)
double Variant::toDouble() const
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
	case VT_Array:
	case VT_Hash:
		return (double)(SintPtr)object_val;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return 0;
}

DR_MET(public virtual)
String Variant::toString() const
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
	case VT_Array:
	case VT_Hash:
		return object_val ? Null() : object_val->stringify();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
BString Variant::toUtf8() const
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
	case VT_Array:
	case VT_Hash:
		return object_val ? Null() : object_val->stringify().utf8();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
Blob Variant::toBinary() const
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
	case VT_Array:
	case VT_Hash:
		return object_val ? Null() : object_val->stringify().utf8();
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return Null();
}

DR_MET(public virtual)
Object *Variant::toObject() const
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
	case VT_Array:
	case VT_Hash:
		return object_val ? object_val->ref() : NULL;
	}

	DR_AssertMsg("Invalid type for Variant Type");
	return NULL;
}


DR_NS_END
