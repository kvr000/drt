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

#ifndef dr__MM_default__hxx__
# define dr__MM_default__hxx__

#include <dr/Hash.hxx>
#include <dr/Mutex.hxx>

DR_NS_BEGIN


static inline long hash(void *p)
{
	return (SintPtr)p^((SintPtr)p>>16);
}

/**
 * Default memory management implementation
 */
class DR_PUB MM_default
{
public:
	struct RH_value
	{
		char *	bt;
		size_t	size;
	};
	class DR_PUB RememberHash: public THash<void *, RH_value, HashCompar<void *, RH_value>, THashAlloc<void *, RH_value, AllocNat> >
	{
	};

	class DR_PUB StrCompar
	{
	public:
		DR_MINLINE long		khash(const char *s)			{ long h; for (h = 0; *s != '\0'; h = h*31+*s++) ; return h; }
		DR_MINLINE bool		keq(const char *s1, const char *s2)	{ return strcmp(s1, s2) == 0; }
	};
	typedef THash<char *, bool, StrCompar, THashAlloc<char *, bool, AllocNat> > StringCache;

public:
	static void *			impl_alloc(size_t size);
	static void			impl_free(void *m);
	static void *			impl_regrow(void *m, size_t size);
	static size_t			impl_size(void *m);
	static void			impl_closingThread();

#if DR_SIZE_PTR < DR_ALIGN_ALLOC
	void *				alsmall_ptr;
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
	void *				alsmall_ptr2;
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif
#endif
#endif

protected:
	static void			insertGlobalCache(void *volatile *queue, void *first, void *last);

protected:
	void *				alsmall_small[128/DR_ALIGN_ALLOC];
	void *				alsmall_bigger[6];

	RememberHash *			remember_hash;
	StringCache *			string_cache;
	Mutex *				remember_mutex;
	char *				str;
	size_t				str_len;

public:
	static void			setup(const char *opts, int level);
	MM_default();
	~MM_default();

public:
	static const unsigned char	guard_beg[32];
	static const unsigned char	guard_fin[48];

public:
	static MM_default		instance;
};


DR_NS_END

#endif
