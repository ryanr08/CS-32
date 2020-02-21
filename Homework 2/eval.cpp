#include "Set.h"
#include <stack>
#include <string>
#include <cassert>
#include <iostream>
using namespace std;

bool isOperator(char c);
bool isValidExpression(string infix);
bool hasHigherPrecedence(char l, char r);
void applyBooleanOperation(bool& operand1, bool& operand2, char ch, bool& result);
bool evaluatePostFix(string postfix, const Set& trueValues, const Set& falseValues);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	if (!isValidExpression(infix))
		return 1;

	postfix = "";
	stack<char> operators;
	for (unsigned long i = 0; i < infix.size(); i++)                              // convert infix expression to postfix
	{
		if (islower(infix[i]))
		{
			postfix += infix[i];
		}
		else if (infix[i] == '(')
		{
			operators.push('(');
		}
		else if (infix[i] == '!')
		{
			operators.push('!');
		}
		else if (infix[i] == ')')
		{
			while (operators.top() != '(')
			{
				postfix += operators.top();
				operators.pop();
			}
			operators.pop();
		}
		else if (isOperator(infix[i]))
		{
			while (!operators.empty() && operators.top() != '(' && !hasHigherPrecedence(infix[i], operators.top()))
			{
				postfix += operators.top();
				operators.pop();
			}
			operators.push(infix[i]);
		}
	}
	while (!operators.empty())
	{
		postfix += operators.top();
		operators.pop();
	}


	for (unsigned long i = 0; i < infix.size(); i++)                  // check if every operand letters appears in either trueValues or falseValues
	{
		if (islower(infix[i]))
		{
			if (trueValues.contains(infix[i]))
			{
				if (falseValues.contains(infix[i]))
					return 3;
			}
			else if (!falseValues.contains(infix[i]))
				return 2;
		}
	}

	result = evaluatePostFix(postfix, trueValues, falseValues);
	return 0;
}

bool evaluatePostFix(string postfix, const Set& trueValues, const Set& falseValues)
{
	char t;
	trueValues.get(0, t);
	char f;
	falseValues.get(0, f);
	stack<char> Operands;
	for (unsigned long i = 0; i < postfix.size(); i++)               // evaluating the postfix expression
	{
		if (isalpha(postfix[i]))
			Operands.push(postfix[i]);
		else if (postfix[i] == '|' || postfix[i] == '&')
		{
			bool tempResult = true;
			bool operand2 = trueValues.contains(Operands.top());
			Operands.pop();
			bool operand1 = trueValues.contains(Operands.top());
			Operands.pop();
			applyBooleanOperation(operand1, operand2, postfix[i], tempResult);
			if (tempResult == true)
				Operands.push(t);
			else
				Operands.push(f);
		}
		else
		{
			bool operand1 = trueValues.contains(Operands.top());
			Operands.pop();
			operand1 = !operand1;
			if (operand1)
				Operands.push(t);
			else
				Operands.push(f);
		}
	}
	if (trueValues.contains(Operands.top()))
		return true;
	else
		return false;
}

void applyBooleanOperation(bool& operand1, bool& operand2, char ch, bool& result)
{
	switch (ch)
	{
	case '|':
		result = (operand1 || operand2);
		return;
	case '&':
		result = (operand1 && operand2);
		return;
	}
	return;
}

bool hasHigherPrecedence(char l, char r)
{
	if (l == r)
		return false;
	switch (l)
	{
	case '!':
		return true;
	case '&':
		if (r == '!')
			return false;
		else
			return true;
	case '|':
		return false;
	}
	return false;
}


bool isOperator(char c)
{
	if (c == '&' || c == '|')
		return true;
	return false;
}

bool isValidExpression(string infix)
{
	if (infix.size() == 0)
		return false;
	string newFix;
	int newSize = 0;
	for (unsigned long i = 0; i < infix.size(); i++)                               //remove spaces from infix
	{
		if (isOperator(infix[i]) || islower(infix[i]) || infix[i] == ')' || infix[i] == '(' || infix[i] == '!')
		{
			newFix += infix[i];
			newSize++;
		}
		else if (infix[i] != ' ')
			return false;
	}

	for (int i = 0; i < newSize; i++)                          // check if infix is valid
	{
		if (i == 0)
		{
			if (isOperator(newFix[i]))
				return false;
		}
		if (i == (newSize - 1))
		{
			if (!islower(newFix[i]) && newFix[i] != ')')
				return false;
		}
		else if (islower(newFix[i]))
		{
			if (!isOperator(newFix[i + 1]) && newFix[i + 1] != ')')
				return false;
		}
		else if (isOperator(newFix[i]))
		{
			if (!islower(newFix[i + 1]) && newFix[i + 1] != '!' && newFix[i + 1] != '(')
				return false;
		}
		else if (newFix[i] == '(')
		{
			if (newFix[i + 1] == ')')
				return false;
		}
	}
	int count1 = 0;
	int count2 = 0;

	for (int i = 0; i < newSize; i++)                // check if number of open and closed parenthesis are equal
	{
		if (newFix[i] == '(')
			count1++;
		else if (newFix[i] == ')')
			count2++;
	}
	return (count1 == count2);
}

int main()
{
	string trueChars = "tywz";
	string falseChars = "fnx";
	Set trues;
	Set falses;
	for (int k = 0; k < trueChars.size(); k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k < falseChars.size(); k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;
	assert(evaluate("t&!(f|t&t|f)|!!!(f&t&f)", trues, falses, pf, answer) == 0 && answer);
	assert(evaluate("t|f&f ", trues, falses, pf, answer) == 0 && answer);
	assert(evaluate(" !f|t ", trues, falses, pf, answer) == 0 && answer);
	assert(evaluate("!(f|t) ", trues, falses, pf, answer) == 0 && !answer);
	assert(evaluate("t & !f", trues, falses, pf, answer) == 0 && answer);
	assert(evaluate("&t|f", trues, falses, pf, answer) == 1);
	assert(evaluate(")t|f( & !f", trues, falses, pf, answer) == 1);
	assert(evaluate("((((((t|f)))", trues, falses, pf, answer) == 1);
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0 && pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);
	cout << "Passed all tests" << endl;
}