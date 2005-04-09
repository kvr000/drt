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

#ifndef dr__IntEvaluator_hxx__
# define dr__IntEvaluator_hxx__

#include <dr/Hash.hxx>
#include <dr/Ref.hxx>

#include <dr/Evaluator.hxx>

DR_NS_BEGIN


class DR_PUB IntEvaluator: public Evaluator
{
	DR_OBJECT_DECL_SIMPLE(IntEvaluator, Evaluator);

public:
	enum Operators {
		OP_Unknown,
		OP_UnPlus,
		OP_UnMinus,
		OP_UnNot,
		OP_UnBitNot,
		OP_BinPlus,
		OP_BinMinus,
		OP_BinMul,
		OP_BinDiv,
		OP_BinPower,
		OP_BinEqual,
		OP_BinNonEq,
		OP_BinLower,
		OP_BinGreater,
		OP_BinLowerEq,
		OP_BinGreaterEq,
		OP_BinLShift,
		OP_BinRShift,
		OP_BinByteSwap,
		OP_BinBitAnd,
		OP_BinBitOr,
		OP_BinAnd,
		OP_BinOr,
		OP_BinXor,
		OP_Parenthesis,
		OP_OperatorCount,
	};

	class DR_PUB IntUnaryExpression: public UnaryExpression
	{
		DR_OBJECT_DECL_SIMPLE(IntUnaryExpression, UnaryExpression);

	public:
		DR_CONSTRUCT			IntUnaryExpression(int operation_id_): UnaryExpression(operation_id_) {}

	public:
		virtual Sint64			evaluate(Arguments *args);
	};

	class DR_PUB IntBinaryExpression: public BinaryExpression
	{
		DR_OBJECT_DECL_SIMPLE(IntBinaryExpression, BinaryExpression);

	public:
		DR_CONSTRUCT			IntBinaryExpression(int operation_id_): BinaryExpression(operation_id_) {}

	public:
		virtual Sint64			evaluate(Arguments *args);
	};

public:
	DR_CONSTRUCT			IntEvaluator(const String &expression);

protected:
	virtual Evaluator::TokenType	parseNextToken(const char **expr, ParserState state, unsigned *num_args, Expression **expression, int *prio);

protected:
	static unsigned char		priorities[OP_OperatorCount];
};


DR_NS_END

#endif
