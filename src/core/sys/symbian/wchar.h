#ifndef dros__symbian__wchar_hxx__
# define dros__symbian__wchar_hxx__

#include <dr/def_core.hxx>

#include <string.h>

typedef signed int 		wint_t;

extern "C" {

DR_PUB void	 		wmemcpy(wchar_t *dst, const wchar_t *src, size_t length);
DR_PUB void 			wmemmove(wchar_t *dst, const wchar_t *src, size_t length);
DR_PUB int			wmemcmp(const wchar_t *sl, const wchar_t *sr, size_t length);
DR_PUB void 			wmemset(wchar_t *dst, wchar_t c, size_t length);

};


#endif
