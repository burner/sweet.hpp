{
	"Token" : [
		{ "Name" : "Var", "Regex" : "var", "ConvertFunction" : "void" },
		{ "Name" : "Int", "Regex" : "int", "ConvertFunction" : "void" },
		{ "Name" : "Int_Value", "Regex" : "[0-9]+", "ConvertFunction" : "void" },
		{ "Name" : "Identifier", "Regex" : "", "ConvertFunction" : "void" },
		{ "Name" : "Float", "Regex" : "float", "ConvertFunction" : "void" },
		{ "Name" : "Lparen", "Regex" : "(", "ConvertFunction" : "void" },
		{ "Name" : "Rparen", "Regex" : ")", "ConvertFunction" : "void" },
		{ "Name" : "Lbrack", "Regex" : "[", "ConvertFunction" : "void" },
		{ "Name" : "Rbrack", "Regex" : "]", "ConvertFunction" : "void" },
		{ "Name" : "Dot", "Regex" : ".", "ConvertFunction" : "void" },
		{ "Name" : "Plusplus", "Regex" : "++", "ConvertFunction" : "void" },
		{ "Name" : "Minusminux", "Regex" : "--", "ConvertFunction" : "void" },
		{ "Name" : "Cast", "Regex" : "cast", "ConvertFunction" : "void" },
		{ "Name" : "Star", "Regex" : "*", "ConvertFunction" : "void" },
		{ "Name" : "Div", "Regex" : "/", "ConvertFunction" : "void" },
		{ "Name" : "Modulo", "Regex" : "%", "ConvertFunction" : "void" }
		{ "Name" : "Minus", "Regex" : "-", "ConvertFunction" : "void" }
		{ "Name" : "Plus", "Regex" : "+", "ConvertFunction" : "void" }
		{ "Name" : "Concat", "Regex" : "~", "ConvertFunction" : "void" }
		{ "Name" : "Star", "Regex" : "*", "ConvertFunction" : "void" }
		{ "Name" : "Bang", "Regex" : "~", "ConvertFunction" : "void" }
		{ "Name" : "And", "Regex" : "&", "ConvertFunction" : "void" }
		{ "Name" : "Leftshift", "Regex" : "<<", "ConvertFunction" : "void" }
		{ "Name" : "Rightshift", "Regex" : ">>", "ConvertFunction" : "void" }
		{ "Name" : "Less", "Regex" : "<", "ConvertFunction" : "void" }
		{ "Name" : "Lessequal", "Regex" : "<=", "ConvertFunction" : "void" }
		{ "Name" : "Greater", "Regex" : ">", "ConvertFunction" : "void" }
		{ "Name" : "Greaterequal", "Regex" : ">=", "ConvertFunction" : "void" }
		{ "Name" : "Equal", "Regex" : "==", "ConvertFunction" : "void" }
		{ "Name" : "Notequal", "Regex" : "!=", "ConvertFunction" : "void" }
		{ "Name" : "Is", "Regex" : "is", "ConvertFunction" : "void" }
		{ "Name" : "Bangis", "Regex" : "!is", "ConvertFunction" : "void" }
		{ "Name" : "And", "Regex" : "&", "ConvertFunction" : "void" }
		{ "Name" : "Logicaland", "Regex" : "&&", "ConvertFunction" : "void" }
		{ "Name" : "Or", "Regex" : "|", "ConvertFunction" : "void" }
		{ "Name" : "Logicalor", "Regex" : "||", "ConvertFunction" : "void" }
		{ "Name" : "Xor", "Regex" : "^", "ConvertFunction" : "void" }
		{ "Name" : "Questionmark", "Regex" : "?", "ConvertFunction" : "void" }
		{ "Name" : "Colon", "Regex" : ":", "ConvertFunction" : "void" }
		{ "Name" : "Assign", "Regex" : "=", "ConvertFunction" : "void" }
		{ "Name" : "Var", "Regex" : "var", "ConvertFunction" : "void" }
	],
	"Rules" : [
		{ "Name" : "Start", "Expression" : [
			{ "Rule" : "Expression(start)", "Id" : "Start" }
			]
		},
		{ "Name" : "Expression", "Expression" : [
			{ "Rule" : "AssignmentExpression(expr)" , "Id" : "Next" }
			]
		},
		{ "Name" : "Type", "Expression" : [
			{ "Rule" : "BasicType(type)" , "Id" : "BasicType" },
			{ "Rule" : "BasicType(type) ; TypeFollow(follow)" , "Id" : "BasicTypeFollow" },
			]
		},
		{ "Name" : "BasicType", "Expression" : [
			{ "Rule" : "Int" , "Id" : "Int" }
			{ "Rule" : "Float" , "Id" : "Float" }
			]
		},
		{ "Name" : "TypeFollow", "Expression" : [
			{ "Rule" : "Lbrack ; Rbrack" , "Id" : "DynamicArray" },
			{ "Rule" : "Lbrack ; Rbrack ; TypeFollow(follow)" , "Id" : "DynamicArrayFollow" }
			]
		},
		{ "Name" : "VarDecl", "Expression" : [
			{ "Rule" : "Var ; Identifier(identifier) ; Lparen ; OrOrExpression(init) ; Rparen" , "Id" : "VarExpr" },
			{ "Rule" : "Var ; Identifier(identifier) ; Colon ; Type(type)" , "Id" : "VarType" }
			]
		},
		{ "Name" : "AssignmentExpression", "Expression" : [
			{ "Rule" : "ConditionalExpression(drain)" , "Id" : "Cond" },
			{ "Rule" : "ConditionalExpression(drain) ; Assign ; AssignmentExpression(source)" , "Id" : "Assign" },
			]
		},
		{ "Name" : "ConditionalExpression", "Expression" : [
			{ "Rule" : "OrOrExpression(orOrExpr)" , "Id" : "OrOr" },
			{ "Rule" : 
				"OrOrExpression(orOrExpr) ; Questionmark ; Expression(trueBranch) ; Colon ; ConditionalExpression(otherBranch) " , "Id" : "Ternary" },
			]
		},
		{ "Name" : "OrOrExpression", "Expression" : [
			{ "Rule" : "AndAndExpression(andAndExpr)" , "Id" : "AndAnd" },
			{ "Rule" : "AndAndExpression(andAndExpr) ; Logicalor ; OrOrExpression(follow)" ,
				   	"Id" : "LogicalOr" 
			}
			]
		},
		{ "Name" : "AndAndExpression", "Expression" : [
			{ "Rule" : "OrExpression(orExpr)" , "Id" : "Or" },
			{ "Rule" : "OrExpression(orExpr) ; Logicaland ; AndAndExpression(follow)" , "Id" : "LogicalAnd" }
			]
		},
		{ "Name" : "OrExpression", "Expression" : [
			{ "Rule" : "XorExpression(xorExpr)" , "Id" : "Xor" },
			{ "Rule" : "XorExpression(xorExpr) ; Xor ; OrExpression(follow)" , "Id" : "Or" }
			]
		},
		{ "Name" : "XorExpression", "Expression" : [
			{ "Rule" : "AndExpression(andExpr)" , "Id" : "And" },
			{ "Rule" : "AndExpression(andExpr) ; Xor ; XorExpression(follow)" , "Id" : "Xor" }
			]
		},
		{ "Name" : "AndExpression", "Expression" : [
			{ "Rule" : "EqualityExpression(equal)" , "Id" : "Equality" },
			{ "Rule" : "EqualityExpression(equal) ; And ; AndExpression(follow)" , "Id" : "Equal" }
			]
		},
		{ "Name" : "EqualityExpression", "Expression" : [
			{ "Rule" : "RelExpression(rel)" , "Id" : "Rel" },
			{ "Rule" : "RelExpression(rel) ; Equal ; EqualityExpression(follow)" , "Id" : "Equal" },
			{ "Rule" : "RelExpression(rel) ; Bangequal ; EqualityExpression(follow)" , "Id" : "NotEqual" },
			{ "Rule" : "RelExpression(rel) ; Is ; EqualityExpression(follow)" , "Id" : "Is" },
			{ "Rule" : "RelExpression(rel) ; Bangis ; EqualityExpression(follow)" , "Id" : "NotIs" },
			]
		},
		{ "Name" : "RelExpression", "Expression" : [
			{ "Rule" : "ShiftExpression(shift)" , "Id" : "Shift" },
			{ "Rule" : "ShiftExpression(shift) ; Less ; RelExpression(follow)" , "Id" : "Less" },
			{ "Rule" : "ShiftExpression(shift) ; Lessequal ; RelExpression(follow)" , "Id" : "Lessequal" },
			{ "Rule" : "ShiftExpression(shift) ; Greater ; RelExpression(follow)" , "Id" : "Greater" },
			{ "Rule" : "ShiftExpression(shift) ; Greaterequal ; RelExpression(follow)" , "Id" : "Greaterequal" },
			]
		},
		{ "Name" : "ShiftExpression", "Expression" : [
			{ "Rule" : "AddExpression(add)" , "Id" : "Add" },
			{ "Rule" : "AddExpression(add) ; Leftshift ; ShiftExpression(follow)" , "Id" : "LeftShift" },
			{ "Rule" : "AddExpression(add) ; Rightshift ; ShiftExpression(follow)" , "Id" : "RightShift" },
			]
		},
		{ "Name" : "AddExpression", "Expression" : [
			{ "Rule" : "MulExpression(mul)" , "Id" : "Mul" },
			{ "Rule" : "MulExpression(mul) ; Plus ; AddExpression(follow)" , "Id" : "Plus" },
			{ "Rule" : "MulExpression(mul) ; Minus ; AddExpression(follow)" , "Id" : "Minus" },
			]
		},
		{ "Name" : "MulExpression", "Expression" : [
			{ "Rule" : "CastExpression(cast)" , "Id" : "Cast" },
			{ "Rule" : "CastExpression(cast) ; Star ; MulExpression(follow)" , "Id" : "Multi" },
			{ "Rule" : "CastExpression(cast) ; Div ; MulExpression(follow)" , "Id" : "Div" },
			{ "Rule" : "CastExpression(cast) ; Modulo ; MulExpression(follow)" , "Id" : "Modulo" },
			]
		},
		{ "Name" : "CastExpression", "Expression" : [
			{ "Rule" : " UnaryExpression(unaryFollow)" , "Id" : "Unary" },
			{ "Rule" : " Cast ; Lparen ;  Identifier(type) ; Rparen ; CastExpression(follow)" , "Id" : "Cast" },
			]
		},
		{ "Name" : "UnaryExpression", "Expression" : [
			{ "Rule" : "PostfixExpression(expr)" , "Id" : "Post" },
			{ "Rule" : "Plusplus ; UnaryExpression(follow)" , "Id" : "IncUnary" },
			{ "Rule" : "Plus ; UnaryExpression(follow)" , "Id" : "PlusUnary" },
			{ "Rule" : "Minus ; UnaryExpression(follow)" , "Id" : "MinusUnary" },
			{ "Rule" : "Bang ; UnaryExpression(follow)" , "Id" : "BangUnary" },
			{ "Rule" : "Concat ; UnaryExpression(follow)" , "Id" : "ConcatUnary" },
			{ "Rule" : "And ; UnaryExpression(follow)" , "Id" : "AndUnary" },
			{ "Rule" : "Star ; UnaryExpression(follow)" , "Id" : "StarUnary" },
			{ "Rule" : "Minusminus ; UnaryExpression(follow)" , "Id" : "DecUnary" }
			]
		},
		{ "Name" : "PostfixExpression", "Expression" : [
			{ "Rule" : "PrimaryExpression(primaryExpr)", "Id" : "Ident" },
			{ "Rule" : "PrimaryExpression(primaryExpr) ; PostfixNextExpression(follow)", 
				"Id" : "IdentPostfixNext" },
			]
		},
		{ "Name" : "PostfixNextExpression", "Expression" : [
			{ "Rule" : "Dot ; PostfixNextExpression(followPost)", "Id" : "DotPostfixNext" },
			{ "Rule" : "Dot ; PrimaryExpression(followPrimary)", "Id" : "DotPrimary" },
			{ "Rule" : "Dot ; PrimaryExpression(followPrimary) ; PostfixNextExpression(followPost)", 
				"Id" : "DotPrimaryPostfixNext" 
			},
			{ "Rule" : "Lparen ; Rparen", "Id" : "Call" },
			{ "Rule" : "Lparen ; Rparen ; PostfixNextExpression(followPost)", "Id" : "CallPostFixNext" },
			{ "Rule" : "Lbrack ; Expression(expr) ; Rbrack", "Id" : "ArrayExpr" },
			{ "Rule" : "Lbrack ; Expression(expr) ; Rbrack ; PostfixNextExpression(followPost)", 
				"Id" : "ArrayExprPostfixNext" }
			]
		},
		{ "Name" : "PrimaryExpression", "Expression" : [
			{ "Rule" : "Identifier(value)", "Id" : "PrimaryExpressionIdentifier" },
			{ "Rule" : "Int_Value(value) ", "Id" : "Value" },
			{ "Rule" : "Lparen ; Expression(expr) ; Rparen", "Id" : "PrimaryExpressionExpression" }
			]
		}
	]
}
