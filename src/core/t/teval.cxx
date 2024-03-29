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

#include <stdio.h>
#include <string.h>

#include <dr/x_kw.hxx>
#include <dr/Mem.hxx>
#include <dr/DataNotFoundException.hxx>

#include <dr/tenv/tenv.hxx>

#include <dr/IntEvaluator.hxx>

DR_NS_USE
DR_TENV_NS_USE


#define TEST_EVAL

#ifdef TEST_EVAL
TENV_NS(eval);
void test()
{
	Ref<Evaluator> evaluator;

	TENV_CHECK_EXCEPTION(evaluator.setNoref(new IntEvaluator(")(")); evaluator->evaluate(tref(new Evaluator::ArgumentsHash())), dr::Exception);

	evaluator.setNoref(new IntEvaluator("4+5*7"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 39);

	evaluator.setNoref(new IntEvaluator("4*5+7"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 27);

	evaluator.setNoref(new IntEvaluator("-4*(5+8*7)+-4*(6/2)"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == -256);

	evaluator.setNoref(new IntEvaluator("!defined x && !defined \\1 && !defined a"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 1);

	evaluator.setNoref(new IntEvaluator("5&1|8"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 9);

	evaluator.setNoref(new IntEvaluator("1024<<30"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == ((Sint64)1024)<<30);

	evaluator.setNoref(new IntEvaluator("1024*1024>>18"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == ((Sint64)1024*1024)>>18);

	evaluator.setNoref(new IntEvaluator("0x1234>><<16"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 0x3412);

	evaluator.setNoref(new IntEvaluator("0x12345678>><<32>><<4"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 0x87654321);

	evaluator.setNoref(new IntEvaluator("0x123456789abcdef0>><<64"));
	TENV_CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == (Sint64)0xf0debc9a78563412ULL);

	{
		ERef<Evaluator::ArgumentsHash::Constants> constants(new Evaluator::ArgumentsHash::Constants);
		ERef<Evaluator::ArgumentsHash::Values> values(new Evaluator::ArgumentsHash::Values);
		(**constants)["a"] = 0;
		(**constants)["b"] = 1;
		(**constants)["c"] = 2;
		(**constants)["d"] = 3;
		(**constants)["e"] = 4;
		(**constants)["f"] = 5;
		(**constants)["seven"] = 7;
		values->operator[](0) = 3;
		values->operator[](1) = 9;

		ERef<Evaluator::ArgumentsHash> arguments(new Evaluator::ArgumentsHash(constants, values));
		evaluator.setNoref(new IntEvaluator("a*c+e/c-f"));
		TENV_CHECK(evaluator->evaluate(arguments) == -3);

		evaluator.setNoref(new IntEvaluator("!defined x && defined \\1 && !defined \\2 && defined a && defined seven"));
		TENV_CHECK(evaluator->evaluate(arguments) == 1);

		evaluator.setNoref(new IntEvaluator("a*c+e/c-g"));
		xtry {
			TENV_CHECK(evaluator->evaluate(arguments) == 0);
			DR_AssertInvalid();
		}
		xcatch (DataNotFoundException, ex) {
		}
		xend;
		evaluator.setNoref(new IntEvaluator("a*c+e/c-f+\\1/\\0"));
		TENV_CHECK(evaluator->evaluate(arguments) == 0);

		evaluator.setNoref(new IntEvaluator("a*c+e/c-f*\\2"));
		xtry {
			evaluator->evaluate(arguments);
			DR_AssertInvalid();
		}
		xcatch (DataNotFoundException, ex) {
		}
		xend;

		evaluator.setNoref(new IntEvaluator("a < b && (c > d || e < f)"));
		TENV_CHECK(evaluator->evaluate(arguments) != 0);

		evaluator.setNoref(new IntEvaluator("-f-+f-!f+f-f*f/f == f != f < (f > f <= f >= f && f || f ^^ f)"));
		evaluator->evaluate(arguments);
	}
}
TENV_NSE(eval);
#endif

DR_TENV_MAIN()
{
	tenv_init();
#ifdef TEST_EVAL
	TENV_RUN(eval);
#endif
	return 0;
}
