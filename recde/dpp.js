{
	"Token" : [
		{ "Name" : "Var", "Regex" : "var", "ConvertFunction" : "void" },
		{ "Name" : "Int", "Regex" : "int", "ConvertFunction" : "void" },
		{ "Name" : "Identifier", "Regex" : "", "ConvertFunction" : "void" },
		{ "Name" : "Float", "Regex" : "float", "ConvertFunction" : "void" }
		{ "Name" : "Lparen", "Regex" : "(", "ConvertFunction" : "void" }
		{ "Name" : "Rparen", "Regex" : ")", "ConvertFunction" : "void" }
		{ "Name" : "Dot", "Regex" : ".", "ConvertFunction" : "void" }
	],
	"Rules" : [
		{ "Name" : "VarDecl", "Expression" : [
			{ "Rule" : "Var ; Identifier(identifier) ; Colon ; Type(type)" , "Id" : "ComplexVarDecl" },
			{ "Rule" : "Var ; Type(type)" , "Id" : "JustType" }
			]
		},
		{ "Name" : "PrimaryExpression", "Expression" : [
			{ "Rule" : "Identifier(identifier)", "Id" : "PrimaryExpressionIdentifier" },
			{ "Rule" : "Lparen ; Expression(expr) ; Rparen", "Id" : "PrimaryExpressionExpression" }
			]
		},
		{ "Name" : "PostfixExpression", "Expression" : [
			{ "Rule" : "PrimaryExpression(primaryExpr)", "Id" : "PrimaryExpressionIdentifier" },
			{ "Rule" : "Lparen ; Expression(expr) ; Rparen", "Id" : "PrimaryExpressionExpression" },
			{ "Rule" : "PrimaryExpression(primaryExpr) ; Dot ; PostfixExpression(next)", 
				"Id" : "Dot" },
			]
		},
postfix
	: primary
	| primary postnext

postnext
	: . postnext
	| ( ) 
	| ( ) postnext
	| ++
	| [ expr ] 
	| [ expr ] postnext

	]
}
