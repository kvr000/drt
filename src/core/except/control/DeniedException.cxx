/*drt
 * include:	dr/Exception.hxx
 *
 * ns:		dr
 */

#include <dr/DeniedException.hxx>
#include "_gen/DeniedException-all.hxx"

DR_NS_BEGIN


/*drt
 * class:	DeniedException
 * type:	exception
 * ancestor:	dr::Exception
 *
 * at:	String				failed_check;
 */


DR_MET(public)
DeniedException::DeniedException(const String &failed_check_):
	failed_check(failed_check_)
{
}

DR_MET(public virtual)
DR_EXPORT_MET String DeniedException::stringify() const
{
	return String("permission denied, failed for ").append(failed_check);
}


DR_NS_END
