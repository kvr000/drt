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

#include <dr/testenv/testenv.hxx>

#include <dr/IntEvaluator.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


#define TEST_EVAL

#ifdef TEST_EVAL
TESTNS(eval);
void test()
{
	Ref<Evaluator> evaluator;
	evaluator.setNoref(new IntEvaluator("4+5*7"));
	CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 39);

	evaluator.setNoref(new IntEvaluator("4*5+7"));
	CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 27);

	evaluator.setNoref(new IntEvaluator("-4*(5+8*7)+-4*(6/2)"));
	CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == -256);

	evaluator.setNoref(new IntEvaluator("!defined x && !defined \\1 && !defined a"));
	CHECK(evaluator->evaluate(tref(new Evaluator::ArgumentsHash())) == 1);

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
		CHECK(evaluator->evaluate(arguments) == -3);

		evaluator.setNoref(new IntEvaluator("!defined x && defined \\1 && !defined \\2 && defined a && defined seven"));
		CHECK(evaluator->evaluate(arguments) == 1);

		evaluator.setNoref(new IntEvaluator("a*c+e/c-g"));
		xtry {
			CHECK(evaluator->evaluate(arguments) == 0);
			DR_AssertInvalid();
		}
		xcatch (DataNotFoundException, ex) {
		}
		xend;
		evaluator.setNoref(new IntEvaluator("a*c+e/c-f+\\1/\\0"));
		CHECK(evaluator->evaluate(arguments) == 0);

		evaluator.setNoref(new IntEvaluator("a*c+e/c-f*\\2"));
		xtry {
			evaluator->evaluate(arguments);
			DR_AssertInvalid();
		}
		xcatch (DataNotFoundException, ex) {
		}
		xend;

		evaluator.setNoref(new IntEvaluator("a < b && (c > d || e < f)"));
		CHECK(evaluator->evaluate(arguments) != 0);

		evaluator.setNoref(new IntEvaluator("-f-+f-!f+f-f*f/f == f != f < (f > f <= f >= f && f || f ^^ f)"));
		evaluator->evaluate(arguments);
	}
}
TESTNSE(eval);
#endif

DR_TESTENV_MAIN()
{
	test_init();
#ifdef TEST_EVAL
	TESTRUN(eval);
#endif
	return 0;
}
