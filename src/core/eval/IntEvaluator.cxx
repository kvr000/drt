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
#include <dr/Array.hxx>
#include <dr/Hash.hxx>
#include <dr/List.hxx>
#include <dr/DataNotFoundException.hxx>
#include <dr/InvalidFormatException.hxx>

#include <dr/IntEvaluator.hxx>

#ifdef DR_LIBC_VC
# define strtoll _strtoi64
#endif

DR_NS_BEGIN

DR_OBJECT_DEF(DR_NS_STR, IntEvaluator, Object);
DR_OBJECT_IMPL_SIMPLE(IntEvaluator);


DR_OBJECT_DEF(DR_NS_STR, IntEvaluator::IntUnaryExpression, Object);
DR_OBJECT_IMPL_SIMPLE(IntEvaluator::IntUnaryExpression);


DR_OBJECT_DEF(DR_NS_STR, IntEvaluator::IntBinaryExpression, Object);
DR_OBJECT_IMPL_SIMPLE(IntEvaluator::IntBinaryExpression);


Sint64 IntEvaluator::IntUnaryExpression::evaluate(Arguments *args)
{
	switch (operation_id) {
	case IntEvaluator::OP_UnPlus:
		return operand->evaluate(args);
	case IntEvaluator::OP_UnMinus:
		return -operand->evaluate(args);
	case IntEvaluator::OP_UnNot:
		return !operand->evaluate(args);
	case IntEvaluator::OP_UnBitNot:
		return ~operand->evaluate(args);
	default:
		xthrownew(DataNotFoundException("unary operand", String::createNumber(operation_id)));
	}
	return 0;
}

Sint64 IntEvaluator::IntBinaryExpression::evaluate(Arguments *args)
{
	switch (operation_id) {
	case IntEvaluator::OP_BinPlus:
		return operands[0]->evaluate(args)+operands[1]->evaluate(args);
	case IntEvaluator::OP_BinMinus:
		return operands[0]->evaluate(args)-operands[1]->evaluate(args);
	case IntEvaluator::OP_BinMul:
		return operands[0]->evaluate(args)*operands[1]->evaluate(args);
	case IntEvaluator::OP_BinDiv:
		return operands[0]->evaluate(args)/operands[1]->evaluate(args);
	case IntEvaluator::OP_BinEqual:
		return operands[0]->evaluate(args) == operands[1]->evaluate(args);
	case IntEvaluator::OP_BinNonEq:
		return operands[0]->evaluate(args) != operands[1]->evaluate(args);
	case IntEvaluator::OP_BinLower:
		return operands[0]->evaluate(args) < operands[1]->evaluate(args);
	case IntEvaluator::OP_BinGreater:
		return operands[0]->evaluate(args) > operands[1]->evaluate(args);
	case IntEvaluator::OP_BinLowerEq:
		return operands[0]->evaluate(args) <= operands[1]->evaluate(args);
	case IntEvaluator::OP_BinGreaterEq:
		return operands[0]->evaluate(args) >= operands[1]->evaluate(args);
	case IntEvaluator::OP_BinLShift:
		return operands[0]->evaluate(args) << operands[1]->evaluate(args);
	case IntEvaluator::OP_BinRShift:
		return operands[0]->evaluate(args) >> operands[1]->evaluate(args);
	case IntEvaluator::OP_BinByteSwap:
		{
			Sint64 l = operands[0]->evaluate(args), r = operands[1]->evaluate(args);
			switch (r) {
			case 1:
				l = ((l>>1)&0x5555555555555555ULL)|((l<<2)&0xaaaaaaaaaaaaaaaaULL);
				break;
			case 2:
				l = ((l>>2)&0x3333333333333333ULL)|((l<<2)&0xccccccccccccccccULL);
				break;
			case 4:
				l = ((l>>4)&0x0f0f0f0f0f0f0f0fULL)|((l<<4)&0xf0f0f0f0f0f0f0f0ULL);
				break;
			case 16:
				l = ((l>>8)&0x00ff)|((l<<8)&0xff00);
				break;
			case 32:
				l = ((l>>8)&0x00ff00ffUL)|((l<<8)&0xff00ff00UL);
				l = ((l>>16)&0x0000ffffUL)|((l<<16)&0xffff0000UL);
				break;
			case 64:
				l = ((l>>8)&0x00ff00ff00ff00ffULL)|((l<<8)&0xff00ff00ff00ff00ULL);
				l = ((l>>16)&0x0000ffff0000ffffULL)|((l<<16)&0xffff0000ffff0000ULL);
				l = ((l>>32)&0x00000000ffffffffULL)|((l<<32)&0xffffffff00000000ULL);
				break;
			default:
				xthrownew(InvalidFormatException("right operand for binary swap", String::createNumber(r)));
			}
			return l;
		}
	case IntEvaluator::OP_BinBitAnd:
		return operands[0]->evaluate(args) & operands[1]->evaluate(args);
	case IntEvaluator::OP_BinBitOr:
		return operands[0]->evaluate(args) | operands[1]->evaluate(args);
	case IntEvaluator::OP_BinAnd:
		return operands[0]->evaluate(args) && operands[1]->evaluate(args);
	case IntEvaluator::OP_BinOr:
		return operands[0]->evaluate(args) || operands[1]->evaluate(args);
	case IntEvaluator::OP_BinXor:
		return !operands[0]->evaluate(args) ^ !operands[1]->evaluate(args);
	default:
		xthrownew(InvalidFormatException("binary operand", String::createNumber(operation_id)));
	}
	return 0;
}


unsigned char IntEvaluator::priorities[OP_OperatorCount] = {
	255,	//OP_Unknown,
	15,	//OP_UnPlus,
	15,	//OP_UnMinus,
	15,	//OP_UnNot,
	15,	//OP_UnBitNot,
	40,	//OP_BinPlus,
	40,	//OP_BinMinus,
	30,	//OP_BinMul,
	30,	//OP_BinDiv,
	25,	//OP_BinPower,
	60,	//OP_BinEqual,
	60,	//OP_BinNonEq,
	60,	//OP_BinLower,
	60,	//OP_BinGreater,
	60,	//OP_BinLowerEq,
	60,	//OP_BinGreaterEq,
	70,	//OP_BinLShift,
	70,	//OP_BinRShift,
	70,	//OP_BinByteSwap,
	80,	//OP_BinBitAnd,
	80,	//OP_BinBitOr,
	80,	//OP_BinAnd,
	90,	//OP_BinOr,
	90,	//OP_BinXor,
	5,	//OP_Parenthesis,
		//OP_OperatorCount,
};

Evaluator::TokenType IntEvaluator::parseNextToken(const char **expr, ParserState state, unsigned *num_args, Expression **ret_expression, int *prio)
{
	while (isspace(**expr))
		++*expr;
	switch (**expr) {
	case '\0':
		return TT_Finish;

	case '(':
		++*expr;
		return TT_OpenParenthesis;

	case ')':
		++*expr;
		return TT_CloseParenthesis;

	case '+':
		++*expr;
		if (state == PS_BinOperator) {
			DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinPlus));
			*num_args = 2;
			*prio = priorities[OP_BinPlus];
			return TT_Operator;
		}
		else if (isdigit(**expr)) {
			Sint64 val = strtoll(*expr, (char **)expr, 0);
			DR_REF_XCHG(ret_expression, (Expression *)new DirectExpression(val));
			return TT_Expression;
		}
		else {
			DR_REF_XCHG(ret_expression, (Expression *)new IntUnaryExpression(OP_UnPlus));
			*num_args = 1;
			*prio = priorities[OP_UnPlus];
			return TT_Operator;
		}
		break;

	case '-':
		++*expr;
		if (state == PS_BinOperator) {
			DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinMinus));
			*num_args = 2;
			*prio = priorities[OP_BinMinus];
			return TT_Operator;
		}
		else if (isdigit(**expr)) {
			Sint64 val = strtoll(*expr-1, (char **)expr, 0);
			DR_REF_XCHG(ret_expression, (Expression *)new DirectExpression(val));
			return TT_Expression;
		}
		else {
			DR_REF_XCHG(ret_expression, (Expression *)new IntUnaryExpression(OP_UnMinus));
			*num_args = 1;
			*prio = priorities[OP_UnMinus];
			return TT_Operator;
		}
		break;

	case '=':
		++*expr;
		if (**expr == '=') {
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinEqual));
				*num_args = 2;
				*prio = priorities[OP_BinEqual];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator =", "missing left operand"));
			}
		}
		else {
			xthrownew(InvalidFormatException("token", String(*expr)));
		}
		break;

	case '!':
		++*expr;
		switch (**expr) {
		case '=':
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinNonEq));
				*num_args = 2;
				*prio = priorities[OP_BinNonEq];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator =", "missing left operand"));
			}
			break;

		default:
			if (state == PS_Expression) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntUnaryExpression(OP_UnNot));
				*num_args = 1;
				*prio = priorities[OP_UnNot];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("unary operator !", "unexpected left operand"));
			}
			break;
		}
		break;

	case '/':
		++*expr;
		if (state == PS_BinOperator) {
			DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinDiv));
			*num_args = 2;
			*prio = priorities[OP_BinDiv];
			return TT_Operator;
		}
		else {
			xthrownew(InvalidFormatException("binary operator /", "missing left operand"));
		}
		break;

	case '*':
		++*expr;
		if (state == PS_BinOperator) {
			DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinMul));
			*num_args = 2;
			*prio = priorities[OP_BinMul];
			return TT_Operator;
		}
		else {
			xthrownew(InvalidFormatException("binary operator /", "missing left operand"));
		}
		break;

	case '<':
		++*expr;
		switch (**expr) {
		case '=':
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinLowerEq));
				*num_args = 2;
				*prio = priorities[OP_BinLowerEq];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator <=", "missing left operand"));
			}
			break;

		case '<':
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinLShift));
				*num_args = 2;
				*prio = priorities[OP_BinLShift];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator <<", "missing left operand"));
			}
			break;

		default:
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinLower));
				*num_args = 2;
				*prio = priorities[OP_BinLower];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator <", "missing left operand"));
			}
			break;
		}
		break;

	case '>':
		++*expr;
		switch (**expr) {
		case '=':
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinGreaterEq));
				*num_args = 2;
				*prio = priorities[OP_BinGreaterEq];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator >=", "missing left operand"));
			}
			break;

		case '>':
			++*expr;
			switch (**expr) {
			case '<':
				switch ((*expr)[1]) {
				case '<':
					*expr += 2;
					if (state == PS_BinOperator) {
						DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinByteSwap));
						*num_args = 2;
						*prio = priorities[OP_BinByteSwap];
						return TT_Operator;
					}
					else {
						xthrownew(InvalidFormatException("binary operator >>", "missing left operand"));
					}

				default:
					break;
				}
				// ignore whole rest, continue to basic >>
			default:
				if (state == PS_BinOperator) {
					DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinRShift));
					*num_args = 2;
					*prio = priorities[OP_BinRShift];
					return TT_Operator;
				}
				else {
					xthrownew(InvalidFormatException("binary operator >>", "missing left operand"));
				}
			}
			break;

		default:
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinGreater));
				*num_args = 2;
				*prio = priorities[OP_BinGreater];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("binary operator <", "missing left operand"));
			}
			break;
		}
		break;

	case '&':
		++*expr;
		if (**expr == '&') {
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinAnd));
				*num_args = 2;
				*prio = priorities[OP_BinAnd];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("operator &&", "missing operator"));
			}
		}
		else {
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinBitAnd));
				*num_args = 2;
				*prio = priorities[OP_BinBitAnd];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("operator &", "missing operator"));
			}
		}

	case '|':
		++*expr;
		if (**expr == '|') {
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinOr));
				*num_args = 2;
				*prio = priorities[OP_BinOr];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("operator ||", "missing operator"));
			}
		}
		else {
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinBitOr));
				*num_args = 2;
				*prio = priorities[OP_BinBitOr];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("operator |", "missing operator"));
			}
		}

	case '^':
		++*expr;
		if (**expr == '^') {
			++*expr;
			if (state == PS_BinOperator) {
				DR_REF_XCHG(ret_expression, (Expression *)new IntBinaryExpression(OP_BinXor));
				*num_args = 2;
				*prio = priorities[OP_BinXor];
				return TT_Operator;
			}
			else {
				xthrownew(InvalidFormatException("operator &&", "missing operator"));
			}
		}
		else {
			xthrownew(InvalidFormatException("token", String(*expr)));
		}

	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		if (state == PS_Expression) {
			Sint64 val = strtoll(*expr, (char **)expr, 0);
			DR_REF_XCHG(ret_expression, (Expression *)new DirectExpression(val));
			return TT_Expression;
		}
		else {
			xthrownew(InvalidFormatException("constant", "missing operator"));
		}
		break;

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_':
		if (state == PS_Expression) {
			const char *start = *expr;
			while (isalnum(**expr) || **expr == '_' || **expr == '.')
				++*expr;
			if (memcmp(start, "defined", 7) == 0 && *expr-start == 7) {
				while (isspace(**expr))
					++*expr;
				start = *expr;
				if (isalpha(*start) || *start == '_') {
					for (*expr = start+1; isalnum(**expr) || **expr == '_' || **expr == '.'; ++*expr) ;
					DR_REF_XCHG(ret_expression, (Expression *)new DefinedVariableExpression(String(start, *expr-start)));
					return TT_Expression;
				}
				else if (*start == '\\') {
					for (*expr = ++start; isdigit(**expr); ++*expr) ;
					if (*expr == start)
						xthrownew(InvalidFormatException("substitute", "missing id"));
					DR_REF_XCHG(ret_expression, (Expression *)new DefinedValueExpression(strtoll(start, NULL, 0)));
					return TT_Expression;
				}
				else {
					xthrownew(InvalidFormatException("defined", "missing variable or value"));
				}
			}
			else {
				DR_REF_XCHG(ret_expression, (Expression *)new VariableExpression(String(start, *expr-start)));
				return TT_Expression;
			}
		}
		else {
			xthrownew(InvalidFormatException("variable", "missing operator"));
		}

	case '\\':
		++*expr;
		if (state == PS_Expression) {
			if (!isdigit(**expr)) {
				xthrownew(InvalidFormatException("substitute", "missing id"));
			}
			DR_REF_XCHG(ret_expression, (Expression *)new ValueExpression(strtol(*expr, (char **)expr, 0)));
			return TT_Expression;
		}
		else {
			xthrownew(InvalidFormatException("substitute", "missing operator"));
		}

	default:
		xthrownew(InvalidFormatException("token", String(*expr)));
	}
	return TT_Finish;
}

IntEvaluator::IntEvaluator(const String &expression_):
	Evaluator(expression_, None())
{
	parseExpression();
}


DR_NS_END
