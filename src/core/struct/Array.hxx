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

#ifndef dr__Array_hxx__
# define dr__Array_hxx__

#include <dr/types.hxx>
#include <dr/cmp.hxx>
#include <dr/Assert.hxx>
#include <dr/Ref.hxx>
#include <dr/Alloc.hxx>
#include <dr/Object.hxx>
#include <dr/SerializeEncoder.hxx>
#include <dr/SerializeDecoder.hxx>

DR_NS_BEGIN


/**
 * ancestor for arrays
 */
class DR_PUB Array_c: public Object
{
public:
	DR_RINLINE			Array_c()				{ len = 0; data = NULL; }
	DR_MINLINE virtual		~Array_c()				{ DR_Assert(data == NULL); } // do not call it
	void				destroy_g();				// must be called from  child's destructor

public: // interface to template
	virtual bool			el_eq(const void *d1, const void *d2) = 0;
	virtual int			el_cmp(const void *d1, const void *d2) = 0;
	virtual void *			reallocList(size_t osize, size_t nsize) = 0;
	virtual void			freeList(size_t osize) = 0;

public:
	void				resize_g(size_t nsize);
	void				sort_g(size_t tsize);
	void				sortF_g(size_t tsize, int (*f)(void *d1, void *d2));
	void				sortD_g(size_t tsize, int (*f)(void *d1, void *d2, void *data), void *data);
	void				rsort_g(size_t tsize);
	void				rsortF_g(size_t tsize, int (*f)(void *d1, void *d2));
	void				rsortD_g(size_t tsize, int (*f)(void *d1, void *d2, void *data), void *data);
	void				moveFrom_g(Array_c *source);

protected:
	void				sort_i(char *start, char *end, struct sortdata *sd);

protected:
	unsigned			len;
	char *				data;

private:
	Array_c &			operator=(const Array_c &);
};


/**
 * Simple element array
 */
template <typename V, typename Compar_ = Compar<V>, typename Allocator_ = Alloc>
class SArray: public Array_c, typestore<Compar_, 0>, typestore<Allocator_, 1>
{
public:
	typedef Allocator_ Allocator;
	typedef Compar_ Compar;
	static const size_t tsize = sizeof(V);

	struct iterator
	{
		V *v;
		DR_RINLINE		iterator(V *v_)				: v(v_) { }
		V &			operator*()				{ return *v; }
		const V &		operator*() const			{ return *v; }
		V *			operator->()				{ return v; }
		V &			operator++()				{ return *++v; }
		V &			operator++(int)				{ return *v++; }
		V &			operator--()				{ return *--v; }
		V &			operator--(int)				{ return *v--; }
		bool			operator==(const iterator &s)		{ return v == s.v; }
		bool			operator!=(const iterator &s)		{ return v != s.v; }
	};

	DR_RINLINE			SArray()				: Array_c() { }
	DR_RINLINE			~SArray()				{ p_trunc(0); destroy_g(); }
	DR_RINLINE V &			operator[](unsigned i)			{ DR_Assert(i*tsize < len); return ((V *)data)[i]; }
	DR_RINLINE const V &		operator[](unsigned i) const		{ DR_Assert(i*tsize < len); return ((V *)data)[i]; }
	DR_MINLINE unsigned		count()					{ return len/tsize; }

	DR_NINLINE void			resize(unsigned nsize);
	DR_NINLINE void			resizeInit(unsigned nsize, const V &iv);
	DR_NINLINE void			shrinkEnd(unsigned nsize)		{ p_trunc(count()-nsize); }
	DR_RINLINE void			append(const V &i);
	DR_RINLINE V *			accAppendingNew();
	DR_RINLINE void			remove(size_t idx);

	DR_MINLINE void			sort()					{ Array_c::sort_g(tsize); }
	DR_MINLINE void			sortF(int (*f)(void *d1, void *d2))	{ Array_c::sortF_g(tsize, f); }
	DR_MINLINE void			sortD(int (*f)(void *d1, void *d2, void *pass), void *pass)	{ Array_c::sortD_g(tsize, f, pass); }
	DR_MINLINE void			rsort()					{ Array_c::rsort_g(tsize); }
	DR_MINLINE void			rsortF(int (*f)(void *d1, void *d2))	{ Array_c::rsortF_g(tsize, f); }
	DR_MINLINE void			rsortD(int (*f)(void *d1, void *d2, void *data), void *pass)	{ Array_c::rsortD_g(tsize, f, pass); }

	DR_MINLINE void			moveFrom(SArray *source)		{ moveFrom_g(source); }

public:
	iterator			begin()					{ return (V *)data; }
	iterator			end()					{ return (V *)((char *)data+len); }

	// ancestor iface
public:
	virtual bool			el_eq(const void *d1, const void *d2)	{ return comp().teq(*(V *)d1, *(V *)d2); }
	virtual int			el_cmp(const void *d1, const void *d2)	{ return comp().tcmp(*(V *)d1, *(V *)d2); }

	virtual void *			reallocList(size_t nsize, size_t osize)	{ return allc().realloc(data, nsize); }
	virtual void			freeList(size_t osize)			{ allc().free(data); }

public:
	DR_MINLINE Compar &		comp()					{ return *reinterpret_cast<Compar *>((typestore<Compar, 0> *)this); }
	DR_MINLINE Allocator &		allc()					{ return *reinterpret_cast<Allocator *>((typestore<Allocator, 1> *)this); }

protected:
	DR_MINLINE void			p_trunc(unsigned nsize);
};

template <typename V>
class RArray: public Array_c
{
protected:
	typedef Ref<V>			SV;

public:
	static const size_t tsize = sizeof(Ref<V>);

	DR_RINLINE			RArray()				: Array_c() { }
	DR_RINLINE			~RArray()				{ p_trunc(0); destroy_g(); }
	DR_RINLINE Ref<V> &		operator[](unsigned i)			{ DR_Assert(i*tsize < len); return ((SV *)data)[i]; }
	DR_RINLINE const Ref<V> &	operator[](unsigned i) const		{ DR_Assert(i*tsize < len); return ((SV *)data)[i]; }
	DR_MINLINE unsigned		count()					{ return len/tsize; }

	DR_NINLINE void			resize(unsigned nsize);
	DR_NINLINE void			resizeInit(unsigned nsize, V *iv);
	DR_NINLINE void			shrinkEnd(unsigned nsize)		{ p_trunc(count()-nsize); }
	DR_RINLINE void			appendDoref(V *i);
	DR_RINLINE void			appendNoref(V *i);
	DR_RINLINE void			append(const Ref<V> &i);

	DR_MINLINE void			moveFrom(RArray *source)		{ moveFrom_g(source); }

protected: // ancestor iface
	virtual bool			el_eq(const void *d1, const void *d2)	{ return **(SV *)d1 == **(SV *)d2; }
	virtual int			el_cmp(const void *d1, const void *d2)	{ return **(SV *)d1 == **(SV *)d2 ? 0 : **(SV *)d1 < **(SV *)d2 ? -1 : 1; }

	virtual void *			reallocList(size_t nsize, size_t osize)	{ return MM::realloc(data, nsize); }
	virtual void			freeList(size_t osize)			{ MM::free(data); }

protected:
	DR_MINLINE void			p_trunc(unsigned nsize);
};

/**
 * Pointer-ed element array
 */
template <typename V, typename Compar_ = Compar<V>, typename Allocator_ = AllocTR<V> >
class PArray: public SArray<V *, Compar_, Allocator_>
{
public:
	typedef Allocator_ Allocator;
	typedef SArray<V *, Allocator_> Base;

public:
	DR_RINLINE			PArray()				: Base() { }
	DR_RINLINE			~PArray()				{ p_trunc(0); }

	DR_RINLINE bool			isNull(unsigned i)			{ return Base::operator[](i) == NULL; }
	DR_RINLINE void			setNull(unsigned i)			{ V *p; if ((p = Base::operator[](i))) { p->~V(); Base::allc().free(p, sizeof(V)); Base::operator[](i) = NULL; } }
	DR_MINLINE V &			operator[](unsigned i)			{ return *(Base::operator[](i)); }
	DR_MINLINE const V &		operator[](unsigned i) const		{ return *(Base::operator[](i)); }

	DR_MINLINE unsigned		count()					{ return Base::len/Base::tsize; }

	DR_NINLINE void			resize(unsigned nsize);
	DR_NINLINE void			resizeNull(unsigned nsize);
	DR_NINLINE void			resizeInit(unsigned nsize, const V &i);
	DR_RINLINE void			append(const V &i);

protected:
	virtual bool			el_eq(const void *d1, const void *d2)	{ return Base::comp().teq(**(V **)d1, **(V **)d2); }
	virtual int			el_cmp(const void *d1, const void *d2)	{ return Base::comp().tcmp(**(V **)d1, **(V **)d2); }

protected:
	DR_MINLINE void			p_trunc(unsigned nsize);
};

template <typename V, typename Compar_, typename Allocator_>
class SArraySerializer
{
	typedef SArray<V, Compar_, Allocator_> ArrayType;
public:
	void				serializeArray(ArrayType *array, SerializeEncoder *stream)	{ stream->writeArrayLength(array->count()); for (size_t i = 0; i < array->count(); i++) { stream->writeValue(ScalarPtr(&(*array)[i])); } stream->writeArrayEnd(); }
	void				unserializeArray(ArrayType *array, SerializeDecoder *stream)	{ V v; ScalarPtr vp(&v); ssize_t cnt = stream->readArrayLength(); while (stream->checkArrayNext(&cnt)) { stream->readValue(vp); } }
};

template <typename V, typename Compar_, typename Allocator_>
DR_MINLINE void SArray<V, Compar_, Allocator_>::p_trunc(unsigned nsize)
{
	unsigned i;
	nsize *= sizeof(V);
	for (i = len; i != nsize; ) {
		i -= sizeof(V);
		V *p = (V *)(data+i);
		p->~V();
	}
	Array_c::resize_g(nsize);
}

template <typename V, typename Compar_, typename Allocator_>
DR_NINLINE void SArray<V, Compar_, Allocator_>::resize(unsigned nsize)
{
	if (nsize*sizeof(V) < len) {
		p_trunc(nsize);
	}
	else {
		unsigned i = len;
		nsize *= sizeof(V);
		Array_c::resize_g(nsize);
#if 1
		for (; i != nsize; i += sizeof(V)) {
			V *p = (V *)(data+i);
			::new(p) V;
		}
#else
		new(data+i) V[(nsize-i)/sizeof(V)];
#endif
	}
}

template <typename V, typename Compar_, typename Allocator_>
DR_NINLINE void SArray<V, Compar_, Allocator_>::resizeInit(unsigned nsize, const V &iv)
{
	if (nsize*sizeof(V) < len) {
		p_trunc(nsize);
	}
	else {
		unsigned i = len/sizeof(V);
		Array_c::resize_g(nsize*sizeof(V));
#if 1
		for (; i < nsize; i++) {
			V *p = &((V *)data)[i];
			::new(p) V(iv);
		}
#else
		::new(&((V *)data)[i]) V[nsize-i](iv);
#endif
	}
}

template <typename V, typename Compar_, typename Allocator_>
DR_RINLINE void SArray<V, Compar_, Allocator_>::append(const V &iv)
{
	resizeInit(len/sizeof(V)+1, iv);
}

template <typename V, typename Compar_, typename Allocator_>
DR_RINLINE V *SArray<V, Compar_, Allocator_>::accAppendingNew()
{
	resize(len/sizeof(V)+1);
	return (V *)(data+len-sizeof(V));
}

template <typename V, typename Compar_, typename Allocator_>
DR_RINLINE void SArray<V, Compar_, Allocator_>::remove(size_t idx)
{
	((V *)(data+idx+sizeof(V)))->~V();
	memcpy(data+idx*sizeof(V), data+(idx+1)*sizeof(V), len-(idx+1)*sizeof(V));
	len -= sizeof(V);
}


template <typename V>
DR_MINLINE void RArray<V>::p_trunc(unsigned nsize)
{
	unsigned i;
	nsize *= sizeof(SV);
	for (i = len; i != nsize; ) {
		i -= sizeof(SV);
		SV *p = (SV *)(data+i);
		p->~SV();
	}
	Array_c::resize_g(nsize);
}

template <typename V>
DR_NINLINE void RArray<V>::resize(unsigned nsize)
{
	if (nsize*sizeof(SV) < len) {
		p_trunc(nsize);
	}
	else {
		unsigned i = len;
		nsize *= sizeof(SV);
		Array_c::resize_g(nsize);
		for (; i != nsize; i += sizeof(SV)) {
			SV *p = (SV *)(data+i);
			::new(p) SV();
		}
	}
}

template <typename V>
DR_NINLINE void RArray<V>::resizeInit(unsigned nsize, V *iv)
{
	if (nsize*sizeof(SV) < len) {
		p_trunc(nsize);
	}
	else {
		unsigned i = len;
		nsize *= sizeof(SV);
		Array_c::resize_g(nsize);
		for (; i < nsize; i += sizeof(SV)) {
			SV *p = (SV *)(data+i);
			::new(p) SV(iv, true);
		}
	}
}

template <typename V>
DR_RINLINE void RArray<V>::appendNoref(V *iv)
{
	resize(len/sizeof(SV)+1);
	operator[](len/sizeof(SV)-1).setNoref(iv);
}

template <typename V>
DR_RINLINE void RArray<V>::appendDoref(V *iv)
{
	resizeInit(len/sizeof(SV)+1, iv);
}

template <typename V>
DR_RINLINE void RArray<V>::append(const Ref<V> &iv)
{
	appendDoref(*iv);
}


template <typename V, typename Compar_, typename Allocator_>
DR_MINLINE void PArray<V, Compar_, Allocator_>::p_trunc(unsigned nsize)
{
	unsigned i;
	for (i = Base::len/Base::tsize; i-- > nsize; ) {
		V *p = ((V **)Base::data)[i];
		if (!p)
			continue;
		p->~V();
		Base::allc().free(p, sizeof(V));
	}
	Base::resize_g(nsize*sizeof(void *));
}

template <typename V, typename Compar_, typename Allocator_>
DR_NINLINE void PArray<V, Compar_, Allocator_>::resize(unsigned nsize)
{
	if (nsize*Base::tsize < Base::len) {
		p_trunc(nsize);
	}
	else {
		unsigned i = Base::len/Base::tsize;
		Base::resize(nsize);
		for (; i < nsize; i++)
			((V **)Base::data)[i] = Base::allc().allocT();
	}
}

template <typename V, typename Compar_, typename Allocator_>
DR_NINLINE void PArray<V, Compar_, Allocator_>::resizeInit(unsigned nsize, const V &iv)
{
	if (nsize*Base::tsize < Base::len) {
		p_trunc(nsize);
	}
	else {
		unsigned i = Base::len/Base::tsize;
		Base::resize(nsize);
		for (; i < nsize; i++) {
			V *p = ((V **)Base::data)[i];
			new(p) V(iv);
		}
	}
}

template <typename V, typename Compar_, typename Allocator_>
DR_NINLINE void PArray<V, Compar_, Allocator_>::resizeNull(unsigned nsize)
{
	if (nsize*Base::tsize < Base::len) {
		p_trunc(nsize);
	}
	else {
		Base::resizeInit(nsize, NULL);
	}
}


DR_NS_END

#endif
