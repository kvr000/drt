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

#include <dr/Const.hxx>

#include <dr/config/NoValueException.hxx>

DR_CONFIG_NS_BEGIN


DR_THROW_DEF(DR_CONFIG_NS_STR, NoValueException, Exception);
DR_THROW_IMPL_SIMPLE(NoValueException);

DR_EXPORT_MET NoValueException::NoValueException(Object *config_, const String &key_):
	config_obj(config_, true),
	key(key_)
{
}

DR_EXPORT_MET String NoValueException::stringify() const
{
	String desc("configuration exception: ");
	desc.append(key).append(" was not found");
	return desc;
}

DR_EXPORT_MET Object *NoValueException::getConfigObject()
{
	return config_obj.getDoref();
}


DR_EXPORT_MET String NoValueException::getKey()
{
	return key;
}


DR_CONFIG_NS_END
