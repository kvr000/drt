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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/Time.hxx>
#include <dr/TimeoutExcept.hxx>
#include <dr/EndOfDataExcept.hxx>

#include <dr/io/StreamBuffer.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, StreamBuffer, Object);
DR_OBJECT_IMPL_SIMPLE(StreamBuffer);


StreamBuffer::StreamBuffer(Handle *handle_):
	handle(handle_, true),
	cache_suggest(4096),
	time_limit(0)
{
}

StreamBuffer::~StreamBuffer()
{
}

void StreamBuffer::reinit(Handle *handle_)
{
	handle.setDoref(handle_);
	cache.setEmpty();
	time_limit = 0;
}

Blob StreamBuffer::readLine()
{
	for (size_t req = 1; ;) {
		Blob *c;
		if ((c = extendCache(req))->getSize() < req) {
			if (c->getSize() == 0)
				return Null();
			return c->cleanGet();
		}
		else {
			for (; ; req++) {
				if ((*c)[req-1] == '\n') {
					return c->removeLeftGet(req);
				}
				else if (req == c->getSize()) {
					req += 1;
					break;
				}
			}
		}
	}
}

Blob StreamBuffer::tryReadLine()
{
	ssize_t p;
	if ((p = tryReadUntilChar('\n')) < 0) {
		if (cache.getSize() > 0)
			return cache.cleanGet();
		else
			return Null();
	}
	return cache.removeLeftGet(p+1);
}

/**
 * @param search
 * 	character to search
 *
 * @return -1
 * 	when reached the end of stream and character was not found
 * @return >= 0
 * 	position of the requested character
 */
ssize_t StreamBuffer::tryReadUntilChar(char search)
{
	ssize_t p = 0;
	for (;;) {
		ssize_t n;
		if ((n = cache.find(search, p)) >= 0)
			return n;
		p = cache.getSize();
		if (tryExtendCacheAdd(1) == 0)
			return -1;
	}
}

ssize_t StreamBuffer::readUntilChar(char search)
{
	ssize_t p = 0;
	for (;;) {
		ssize_t n;
		if ((n = cache.find(search, p)) >= 0)
			return n;
		p = cache.getSize();
		extendCacheAdd(256);
	}
}

ssize_t StreamBuffer::read(Blob *data, size_t maxsize)
{
	if (cache.isNullEmpty()) {
		if (cache.isNull())
			xthrownew(EndOfDataExcept("byte", "stream"));
		if (time_limit != 0) {
			SysTime cur_time = Time::getTime();
			if ((cur_time = time_limit-cur_time) < 0)
				cur_time = 0;
			if (handle->waitData(Handle::M_READ, (Sint64)cur_time*1000000000) == 0) {
				xthrownew(TimeoutExcept());
			}
		}
		if (handle->read(data, maxsize, 0) == 0) {
			cache.setNull();
		}
	}
	else if (maxsize < cache.getSize()) {
		*data = cache.removeLeftGet(maxsize);
	}
	else {
		*data = cache;
		cache.setEmpty();
	}
	return data->getSize();
}

ssize_t StreamBuffer::readExtendAdd(Blob *data, size_t add_size)
{
	if (cache.isNullEmpty()) {
		if (cache.isNull())
			xthrownew(EndOfDataExcept("byte", "stream"));
		if (time_limit != 0) {
			SysTime cur_time = Time::getTime();
			if ((cur_time = time_limit-cur_time) < 0)
				cur_time = 0;
			if (handle->waitData(Handle::M_READ, (Sint64)cur_time*1000000000) == 0) {
				xthrownew(TimeoutExcept());
			}
		}
		if ((add_size = handle->read(data, add_size, data->getSize())) == 0) {
			cache.setNull();
		}
	}
	else if (add_size < cache.getSize()) {
		*data += cache.removeLeftGet(add_size);
	}
	else {
		add_size = cache.getSize();
		*data += cache;
		cache.setEmpty();
	}
	return add_size;
}

ssize_t StreamBuffer::readFull(Blob *data, size_t size)
{
	if (size == 0) {
		data->setEmpty();
		return 0;
	}
	read(data, size);
	while (data->getSize() < size) {
		readExtendAdd(data, size-data->getSize());
	}
	return data->getSize();
}

ssize_t StreamBuffer::write(const Blob &data)
{
	return handle->write(data);
}

ssize_t StreamBuffer::writeFull(const Blob &data)
{
	const char *b = data.toStr();
	const char *e = b+data.getSize();
	while (b < e) {
		b += handle->write(b, e-b);
	}
	return data.getSize();
}

Blob *StreamBuffer::extendCache(size_t min_size)
{
	size_t cur = cache.getSize();
	if (cur < min_size) {
		if (time_limit != 0) {
			SysTime cur_time = Time::getTime();
			if ((cur_time = time_limit-cur_time) < 0)
				cur_time = 0;
			if (handle->waitData(Handle::M_READ, (Sint64)cur_time*1000000000) == 0) {
				xthrownew(TimeoutExcept());
			}
		}
		size_t toread = cur < cache_suggest ? cache_suggest-cur : cur > 2048 ? 4096 : cur*2;
		char *l = cache.lock(cur+toread);
		toread = handle->read(l+cur, toread);
		if (toread == 0) {
			xthrownew(EndOfDataExcept("byte", "stream"));
		}
		cache.unlock(cur+toread);
	}
	return &cache;
}

Blob *StreamBuffer::extendCacheAdd(size_t add_size)
{
	return extendCache(cache.getSize()+add_size);
}

size_t StreamBuffer::tryExtendCache(size_t min_size)
{
	size_t cur = cache.getSize();
	while (cur < min_size) {
		if (time_limit != 0) {
			SysTime cur_time = Time::getTime();
			if ((cur_time = time_limit-cur_time) < 0)
				cur_time = 0;
			if (handle->waitData(Handle::M_READ, (Sint64)cur_time*1000000000) == 0) {
				xthrownew(TimeoutExcept());
			}
		}
		size_t toread = cur < cache_suggest ? cache_suggest-cur : cur > 2048 ? 4096 : cur*2;
		toread = handle->read(cache.lockKeep(cur+toread)+cur, toread);
		cur += toread;
		cache.unlock(cur);
		if (toread == 0) {
			break;
		}
	}
	return cache.getSize();
}

size_t StreamBuffer::tryExtendCacheAdd(size_t add_size)
{
	size_t orig_size = cache.getSize();
	return tryExtendCache(cache.getSize()+add_size)-orig_size;
}

void StreamBuffer::setCacheSize(size_t suggested_size)
{
	cache_suggest = suggested_size;
}


DR_IO_NS_END
