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
#include <dr/BString.hxx>
#include <dr/EndOfDataException.hxx>
#include <dr/InvalidFormatException.hxx>

#include <dr/Evaluator.hxx>

DR_NS_BEGIN

DR_OBJECT_DEF(DR_NS_STR, Evaluator, Object);
DR_OBJECT_IMPL_SIMPLE(Evaluator);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::Arguments, Object);
DR_OBJECT_IMPL_SIMPLE(Evaluator::Arguments);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::ArgumentsHash, Evaluator::Arguments);
DR_OBJECT_IMPL_SIMPLE(Evaluator::ArgumentsHash);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::Expression, Object);
DR_OBJECT_IMPL_SIMPLE(Evaluator::Expression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::MarkFunctionDummyExpression, Evaluator::Expression);
DR_OBJECT_IMPL_SIMPLE(Evaluator::MarkFunctionDummyExpression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::ConstantExpression, Evaluator::Expression);
DR_OBJECT_IMPL_SIMPLE(Evaluator::ConstantExpression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::ValueExpression, ValueExpression);
DR_OBJECT_IMPL_SIMPLE(Evaluator::ValueExpression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::VariableExpression, VariableExpression);
DR_OBJECT_IMPL_SIMPLE(Evaluator::VariableExpression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::OperExpression, Object);
DR_OBJECT_IMPL_SIMPLE(Evaluator::OperExpression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::UnaryExpression, Object);
DR_OBJECT_IMPL_SIMPLE(Evaluator::UnaryExpression);


DR_OBJECT_DEF(DR_NS_STR, Evaluator::BinaryExpression, Object);
DR_OBJECT_IMPL_SIMPLE(Evaluator::BinaryExpression);


struct ParseElement
{
	enum Type {
		PE_None,
		//PE_Expression,
		PE_Operation,
		PE_Function,
		PE_OpenParenthesis,
	};
	DR_CONSTRUCT			ParseElement(): type(PE_None)			{}

	DR_DESTRUCT			~ParseElement()			{ if (/*type == PE_Expression ||*/ type == PE_Operation) { expression->unref(); } }

	void				initOperation(Evaluator::Expression *expression_, unsigned priority_, unsigned num_args_) { type = PE_Operation; (expression = expression_)->ref(); priority = priority_; num_args = num_args_; }
	//void				initExpression(Evaluator::Expression *expression_) { type = PE_Expression; (expression = expression_)->ref(); priority = 0; num_args = 0; }
	void				initParenthesis()		{ type = PE_OpenParenthesis; }
	void				initFunction(Evaluator::Expression *expression_, unsigned num_args_) { type = PE_Function; (expression = expression_)->ref(); num_args = num_args_; }

	Type				type;
	Evaluator::Expression *		expression;
	unsigned			priority;
	unsigned			num_args;
};

void Evaluator::parseExpression()
{
	BString expr_utf8(expression_str.utf8());
	const char *expr_str = expr_utf8.toStr();

	SList<ParseElement>		parse_stack;
	RArray<Expression>		operands;
	typedef SList<ParseElement>::Node StackNode;
	ParserState			state = PS_Expression;
	Ref<Expression>			expr_obj;
	unsigned			num_args;
	int				priority;
	TokenType			token_type;

	token_type = parseNextToken(&expr_str, state, &num_args, expr_obj.mem(), &priority);
	switch (token_type) {
	case TT_Finish:
		xthrownew(EndOfDataException("expression", "empty"));

	case TT_Operator:
		parse_stack.accAppendingNew()->initOperation(expr_obj, priority, num_args);
		state = PS_Expression;
		break;

	case TT_Expression:
		operands.appendDoref(expr_obj);
		state = PS_BinOperator;
		break;

	case TT_OpenParenthesis:
		parse_stack.accAppendingNew()->initParenthesis();
		state = PS_Expression;
		break;

	case TT_Function:
		parse_stack.accAppendingNew()->initFunction(expr_obj, num_args);
		state = PS_Expression;
		break;
		
	case TT_CloseParenthesis:
	default:
		xthrownew(InvalidFormatException("expression", "unary operator or number"));
	}

	for (;;) {
		token_type = parseNextToken(&expr_str, state, &num_args, expr_obj.mem(), &priority);
		switch (token_type) {
		case TT_Finish:
			{
				while (StackNode *last = parse_stack.iterLast()) {
					switch (last->v.type) {
					case ParseElement::PE_OpenParenthesis:
						xthrownew(InvalidFormatException("expression", "missing right parenthesis"));
						break;

					//case ParseElement::PE_Expression:
					case ParseElement::PE_Operation:
						if (last->v.num_args > 0) {
							if (last->v.num_args > operands.count()) {
								xthrownew(InvalidFormatException("expression", "missing operands"));
							}
							last->v.expression->setOperands(&operands[operands.count()-last->v.num_args]);
							operands.shrinkEnd(last->v.num_args);
						}
						operands.appendDoref(last->v.expression);
						parse_stack.remove(last);
						break;

					case ParseElement::PE_Function:
						if (last->v.num_args> 0) {
							if (last->v.num_args > operands.count()) {
								xthrownew(InvalidFormatException("expression", "missing parameters"));
							}
							last->v.expression->setOperands(&operands[operands.count()-last->v.num_args]);
							operands.shrinkEnd(last->v.num_args);
						}
						operands.appendDoref(last->v.expression);
						parse_stack.remove(last);
						break;

					default:
						DR_AssertInvalid();
					}
				}
				if (operands.count() != 1) {
					xthrownew(InvalidFormatException("expression", "missing operand?"));
				}
				expression_tree = operands[0];
				return;
			}
			break;

		case TT_Expression:
			{
				operands.appendDoref(expr_obj);
				state = PS_BinOperator;
			}
			break;

		case TT_Operator:
			{
				while (StackNode *last = parse_stack.iterLast()) {
					if (last->v.type != ParseElement::PE_Operation)
						break;
					if (priority > 0 ? last->v.priority > (unsigned)priority : last->v.priority >= (unsigned)-priority)
						break;
					if (last->v.num_args > 0) {
						if (last->v.num_args > operands.count()) {
							xthrownew(InvalidFormatException("expression", "missing operands"));
						}
						last->v.expression->setOperands(&operands[operands.count()-last->v.num_args]);
						operands.shrinkEnd(last->v.num_args);
					}
					operands.appendDoref(last->v.expression);
					parse_stack.remove(last);
					break;
				}
				parse_stack.accAppendingNew()->initOperation(expr_obj, priority < 0 ? -priority : priority, num_args);
			}
			state = PS_Expression;
			break;

		case TT_OpenParenthesis:
			{
				parse_stack.accAppendingNew()->initParenthesis();
			}
			state = PS_Expression;
			break;

		case TT_CloseParenthesis:
			{
				while (StackNode *last = parse_stack.iterLast()) {
					if (last->v.type == ParseElement::PE_OpenParenthesis) {
						parse_stack.remove(last);
						break;
					}
					else if (last->v.type == ParseElement::PE_Function) {
						if (last->v.num_args> 0) {
							if (last->v.num_args > operands.count()) {
								xthrownew(InvalidFormatException("expression", "missing parameters"));
							}
							last->v.expression->setOperands(&operands[operands.count()-last->v.num_args]);
							operands.shrinkEnd(last->v.num_args);
						}
						operands.appendDoref(last->v.expression);
						parse_stack.remove(last);
					}
					else if (last->v.type == ParseElement::PE_Operation) {
						if (last->v.num_args > 0) {
							if (last->v.num_args > operands.count()) {
								xthrownew(InvalidFormatException("expression", "missing operands"));
							}
							last->v.expression->setOperands(&operands[operands.count()-last->v.num_args]);
							operands.shrinkEnd(last->v.num_args);
						}
						operands.appendDoref(last->v.expression);
						parse_stack.remove(last);
					}
					else {
						DR_AssertInvalid();
					}
				}
			}
			state = PS_BinOperator;
			break;

		case TT_Function:
			{
				parse_stack.accAppendingNew()->initFunction(expr_obj, num_args);
			}
			state = PS_Expression;
			break;
		}
	}
}

Evaluator::Evaluator(const String &expression_):
	expression_str(expression_)
{
	Ref<Expression> cur_expr;
	parseExpression();
}

Evaluator::Evaluator(const String &expression_, const None &none):
	expression_str(expression_)
{
}

Sint64 Evaluator::evaluate(Arguments *args)
{
	return expression_tree->evaluate(args);
}

String Evaluator::getExpression()
{
	return expression_str;
}


DR_NS_END
