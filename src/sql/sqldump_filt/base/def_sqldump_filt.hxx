#ifndef dr__sql__sqldump_filt__def_sqldump_filt__hxx__
# define dr__sql__sqldump_filt__def_sqldump_filt__hxx__

#include <dr/def.hxx>


#ifdef COMPILING_sqldump_filt
# define DR_SQL_SQLDUMP_FILT_PUB	DR_EXPORT_CLS
# define DR_SQL_SQLDUMP_FILT_FUN	DR_EXPORT_FUN
# define DR_SQL_SQLDUMP_FILT_MET	DR_EXPORT_MET
# define DR_SQL_SQLDUMP_FILT_MTS	DR_EXPORT_MTS
# define DR_SQL_SQLDUMP_FILT_DAT	DR_EXPORT_DAT
# define DR_SQL_SQLDUMP_FILT_DTC	DR_EXPORT_DTC
# define DR_SQL_SQLDUMP_FILT_CLS	DR_EXPORT_CLS
#else
# define DR_SQL_SQLDUMP_FILT_PUB	DR_IMPORT_CLS
# define DR_SQL_SQLDUMP_FILT_FUN	DR_IMPORT_FUN
# define DR_SQL_SQLDUMP_FILT_MET	DR_IMPORT_MET
# define DR_SQL_SQLDUMP_FILT_MTS	DR_IMPORT_MTS
# define DR_SQL_SQLDUMP_FILT_DAT	DR_IMPORT_DAT
# define DR_SQL_SQLDUMP_FILT_DTC	DR_IMPORT_DTC
# define DR_SQL_SQLDUMP_FILT_CLS	DR_IMPORT_CLS
#endif

#define DR_SQL_SQLDUMP_FILT_NS_USE using namespace dr; using namespace dr::sql::sqldump_filt;
#define DR_SQL_SQLDUMP_FILT_NS dr::sql::sqldump_filt
#define DR_SQL_SQLDUMP_FILT_NSP(rest) dr::sql::sqldump_filt::rest
#define DR_SQL_SQLDUMP_FILT_NS_STR "dr::sql::sqldump_filt::"
#define DR_SQL_SQLDUMP_FILT_NS_STRP(rest) "dr::sql::sqldump_filt::"rest
#define DR_SQL_SQLDUMP_FILT_NS_BEGIN namespace dr \
{ \
	namespace sql \
	{ \
		namespace sqldump_filt \
		{
#define DR_SQL_SQLDUMP_FILT_NS_END }; }; };


DR_SQL_SQLDUMP_FILT_NS_BEGIN
DR_SQL_SQLDUMP_FILT_NS_END

#endif
