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

#ifndef dr__Evaluator_hxx__
# define dr__Evaluator_hxx__

#include <dr/Hash.hxx>
#include <dr/Array.hxx>
#include <dr/Ref.hxx>
#include <dr/DataNotFoundException.hxx>

DR_NS_BEGIN


class DR_PUB Evaluator: public Object
{
	DR_OBJECT_DECL_SIMPLE(Evaluator, Object);

public:
	enum TokenType {
		TT_Finish,
		TT_Expression,
		TT_Operator,
		TT_OpenParenthesis,
		TT_CloseParenthesis,
		TT_Function,
	};

	enum ParserState {
		PS_Finished,
		PS_Expression,
		PS_BinOperator,
	};

public:
	DR_CONSTRUCT			Evaluator(const String &expression);

public:
	class DR_PUB Arguments: public Object
	{
	public:
		DR_OBJECT_DECL_SIMPLE(Arguments, Object);

	public:
		typedef THash<String, Sint64>	Constants;
		typedef THash<unsigned, Sint64>	Values;

	public:
		DR_CONSTRUCT			Arguments(Constants *constants_, Values *values_): constants(constants_, true), values(values_, true) {}
		DR_CONSTRUCT			Arguments(Constants *constants_): constants(constants_, true), values(new Values(), false) {}
		DR_CONSTRUCT			Arguments(Values *values_)	: constants(new Constants(), false), values(values_, true) {}
		DR_CONSTRUCT			Arguments()			: constants(new Constants(), false), values(new Values(), false) {}

		Ref<Constants>			constants;
		Ref<Values>			values;
	};

public:
	virtual Sint64				evaluate(Arguments *args);

public:
	virtual String				getExpression();

protected:
	class DR_PUB Expression: public Object
	{
	public:
		DR_OBJECT_DECL_SIMPLE(Expression, Object);

	public:
		DR_CONSTRUCT			Expression()			{}

	public:
		virtual void			setOperands(const Ref<Expression> *operands_) { DR_AssertInvalid(); }

	public:
		virtual Sint64			evaluate(Arguments *args) = 0;
	};

	class DR_PUB MarkFunctionDummyExpression: public Expression
	{
	public:
		DR_OBJECT_DECL_SIMPLE(MarkFunctionDummyExpression, Expression);

	public:
		DR_CONSTRUCT			MarkFunctionDummyExpression()	{}

	public:
		virtual Sint64			evaluate(Arguments *args)	{ DR_AssertInvalid(); return 0; }
	};

	class DR_PUB ConstantExpression: public Expression
	{
	public:
		DR_OBJECT_DECL_SIMPLE(ConstantExpression, Expression);

	public:
		DR_CONSTRUCT			ConstantExpression(Sint64 value_): value(value_) {}

	public:
		virtual Sint64			evaluate(Arguments *args)	{ return value; }

	protected:
		Sint64				value;
	};

	class DR_PUB VariableExpression: public Expression
	{
		DR_OBJECT_DECL_SIMPLE(VariableExpression, Expression);

	public:
		DR_CONSTRUCT			VariableExpression(const String &var_name_): var_name(var_name_) {}

	public:
		virtual Sint64			evaluate(Arguments *args)	{ if (Sint64 *v = args->constants->accValue(var_name)) { return *v; } xthrownew(DataNotFoundException("constant", var_name)); return 0; }

	protected:
		String				var_name;
	};

	class DR_PUB ValueExpression: public Expression
	{
		DR_OBJECT_DECL_SIMPLE(ValueExpression, Expression);
	public:
		DR_CONSTRUCT			ValueExpression(unsigned column_id_): column_id(column_id_) {}

	public:
		virtual Sint64			evaluate(Arguments *args)	{ if (Sint64 *v = args->values->accValue(column_id)) { return *v; } xthrownew(DataNotFoundException("value", String::createNumber(column_id))); return 0;}

	protected:
		unsigned			column_id;
	};

	class DR_PUB OperExpression: public Expression
	{
		DR_OBJECT_DECL_SIMPLE(OperExpression, Expression);

	public:
		DR_CONSTRUCT			OperExpression(int operation_id_): operation_id(operation_id_) {}

	public:
		virtual void			setOperands(const Ref<Expression> *operands_) = 0;

	protected:
		int				operation_id;
	};

	class DR_PUB UnaryExpression: public OperExpression
	{
		DR_OBJECT_DECL_SIMPLE(UnaryExpression, OperExpression);

	public:
		DR_CONSTRUCT			UnaryExpression(int operation_id_): OperExpression(operation_id_) {}

	public:
		virtual void			setOperands(const Ref<Expression> *operands_)	{ operand = operands_[0]; }

	protected:
		Ref<Evaluator::Expression>	operand;
	};

	class DR_PUB BinaryExpression: public OperExpression
	{
		DR_OBJECT_DECL_SIMPLE(BinaryExpression, OperExpression);

	public:
		DR_CONSTRUCT			BinaryExpression(int operation_id_): OperExpression(operation_id_) {}

	public:
		virtual void			setOperands(const Ref<Expression> *operands_)	{ operands[0] = operands_[0]; operands[1] = operands_[1]; }

	public:
		Ref<Expression>	operands[2];
	};


protected:
	DR_CONSTRUCT			Evaluator(const String &expression, const None &none);

protected:
	virtual Evaluator::TokenType	parseNextToken(const char **expr, ParserState state, unsigned *num_args, Expression **expression, int *prio) = 0;

protected:
	virtual void			parseExpression();

protected:
	String				expression_str;
	Ref<Expression>			expression_tree;

public:
	friend struct			ParseElement;
};


DR_NS_END

#endif
