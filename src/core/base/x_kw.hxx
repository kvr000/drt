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

#ifndef dr__throw_kw__hxx__
# define dr__throw_kw__hxx__


/**
 * throws new instance of specified class
 */
#define xthrownew DR_THROWNEW

/**
 * throws existing exception, referencing it first
 */
#define xthrowref DR_THROWREF

/**
 * throws existing exception, loosing the passed pointer
 */
#define xthrowmove DR_THROWMOVE

/**
 * rethrows the exception
 */
#define xrethrow DR_RETHROW

/**
 * rethrows exception caught by xcatchany
 */
#define xrethrowany DR_RETHROWANY

/**
 * beginning of exception try block
 */
#define xtry DR_TRY

/**
 * end of exception mechanism
 */
#define xend DR_ENDTRY

/**
 * catches the exception
 */
#define xcatch DR_CATCH

/**
 * catches any exception
 */
#define xcatchany DR_CATCHANY

/**
 * forgets passed exception, the reference is lost
 */
#define xforget DR_FORGET

/**
 * holds the exception
 */
#define xhold DR_THOLD

/**
 * releases the exception hold
 */
#define xholdgo DR_THOLDGO

/**
 * ends exception holding block
 */
#define xendhold DR_THOLDEND

/**
 * safely executes the code inside, catching any exception
 */
#define xsafe DR_XSAFE

/**
 * safely executes the code inside, catching any exception of specified type
 */
#define xsafetype DR_XSAFETYPE


#endif
