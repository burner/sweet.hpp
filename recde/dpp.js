{
	"Token" : [
		{ "Name" : "Var", "Regex" : "var", "ConvertFunction" : "void" },
		{ "Name" : "Int", "Regex" : "int", "ConvertFunction" : "void" },
		{ "Name" : "Identifier", "Regex" : "", "ConvertFunction" : "void" },
		{ "Name" : "Float", "Regex" : "float", "ConvertFunction" : "void" }
		{ "Name" : "Lparen", "Regex" : "(", "ConvertFunction" : "void" }
		{ "Name" : "Rparen", "Regex" : ")", "ConvertFunction" : "void" }
		{ "Name" : "Lbrack", "Regex" : "[", "ConvertFunction" : "void" }
		{ "Name" : "Rbrack", "Regex" : "]", "ConvertFunction" : "void" }
		{ "Name" : "Dot", "Regex" : ".", "ConvertFunction" : "void" }
	],
	"Rules" : [
		{ "Name" : "Start", "Expression" : [
			{ "Rule" : "Expression(start)", "Id" : "Start" }
			]
		},
		{ "Name" : "Expression", "Expression" : [
			{ "Rule" : "PostfixExpression(expr)" , "Id" : "Post" }
			]
		},
		{ "Name" : "PrimaryExpression", "Expression" : [
			{ "Rule" : "Identifier(identifier)", "Id" : "PrimaryExpressionIdentifier" },
			{ "Rule" : "Lparen ; Expression(expr) ; Rparen", "Id" : "PrimaryExpressionExpression" }
			]
		},
		{ "Name" : "PostfixExpression", "Expression" : [
			{ "Rule" : "PrimaryExpression(primaryExpr)", "Id" : "Ident" },
			{ "Rule" : "PrimaryExpression(primaryExpr) ; PostfixNextExpression(follow)", "Id" : "IdentPostfixNext" },
			]
		},
		{ "Name" : "PostfixNextExpression", "Expression" : [
			{ "Rule" : "Dot ; PostfixNextExpression(followPost)", "Id" : "DotPostfixNext" },
			{ "Rule" : "Dot ; PrimaryExpression(followPrimary)", "Id" : "DotPrimary" },
			{ "Rule" : "Dot ; PrimaryExpression(followPrimary) ; PostfixNextExpression(followPost)", "Id" : "DotPrimaryPostfixNext" },
			{ "Rule" : "Lparen ; Rparen", "Id" : "Call" },
			{ "Rule" : "Lbrack ; Expression(expr) ; Rbrack", "Id" : "ArrayExpr" },
			{ "Rule" : "Lbrack ; Expression(expr) ; Rbrack ; PostfixNextExpression(followPost)", "Id" : "ArrayExprPostfixNext" }
			]
		}
	]
}
