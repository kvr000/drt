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
 * include:	dr/serv/io/def.hxx
 * include:	dr/Time.hxx
 * include:	dr/io/File.hxx
 *
 * ns:		dr::serv::io
 */

#include <dr/x_kw.hxx>
#include <dr/io/FileNotFoundException.hxx>
#include <dr/io/Directory.hxx>

#include <dr/serv/io/DirProcessingWriter.hxx>

#include "_gen/DirProcessingWriter-all.hxx"

DR_SERV_IO_NS_BEGIN

DR_NS_USE;


/*drt
 * class:	DirProcessingWriter
 * ancestor:	dr::Object
 *
 * at:	String			dirname_input;
 * at:	String			dirname_tmp;
 * at:	int			minor_length;
 *
 * at:	Time::SysTime		last_time_sec;
 * at:	int			minor_counter;
 *
 * doc:{
 * 	Directory batch processing support - writer
 * }doc
 */

DR_MET(public)
DirProcessingWriter::DirProcessingWriter(const String &dirname_input_, const String &dirname_tmp_, int minor_length_):
	dirname_input(dirname_input_+"/"),
	dirname_tmp(dirname_tmp_+"/"),
	minor_length(minor_length_),
	last_time_sec(0)
{
	if (dr::io::File::getType(dirname_input) != dr::io::Handle::HT_Directory)
		xthrownew(dr::io::FileNotFoundException(dirname_input, "isdir", -1, -1));
	if (dr::io::File::getType(dirname_tmp) != dr::io::Handle::HT_Directory)
		xthrownew(dr::io::FileNotFoundException(dirname_tmp, "isdir", -1, -1));
}

DR_MET(public static)
DirProcessingWriter *DirProcessingWriter::createRooted(const String &root, int minor_length)
{
	if (dr::io::File::getType(root) != dr::io::Handle::HT_Directory)
		xthrownew(dr::io::FileNotFoundException(root, "isdir", -1, -1));
	xsafe(dr::io::Directory::mkdir(root+"/input"));
	xsafe(dr::io::Directory::mkdir(root+"/tmp"));
	return new DirProcessingWriter(root+"/input", root+"/tmp", minor_length);
}

DR_MET(public virtual)
dr::io::File *DirProcessingWriter::createFile(String *name)
{
	if (last_time_sec != Time::toSeconds(Time::getTimePrecSec())) {
		last_time_sec = Time::toSeconds(Time::getTimePrecSec());
		minor_counter = 0;
	}
	name->format("%ld-%0*d", (long)last_time_sec, minor_length, minor_counter++);
	return new dr::io::File(dirname_tmp+*name, dr::io::File::M_CREATE|dr::io::File::M_WRITE|dr::io::File::M_EXCL);
}

DR_MET(public virtual)
void DirProcessingWriter::closeFile(const String &name)
{
	dr::io::File::rename(dirname_tmp+name, dirname_input+name);
}

DR_MET(public virtual)
void DirProcessingWriter::putContent(const Blob &content)
{
	String name;
	ERef<dr::io::File> fd(createFile(&name));
	for (size_t off = 0; off < content.getSize(); )
		off += fd->write(content.toStr()+off, content.getSize()-off);
	fd->close();
	closeFile(name);
}


DR_SERV_IO_NS_END
