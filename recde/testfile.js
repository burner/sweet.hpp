{
	"Token" : [
		{ "Name" : "Var", "Regex" : "var", "ConvertFunction" : "void" },
		{ "Name" : "Int", "Regex" : "int", "ConvertFunction" : "void" },
		{ "Name" : "Identifier", "Regex" : "[a-zA-Z]+", "ConvertFunction" : "void" },
		{ "Name" : "Float", "Regex" : "float", "ConvertFunction" : "void" },
		{ "Name" : "Colon", "Regex" : ":", "ConvertFunction" : "void" }
	],
	"Rules" : [
		{ "Name" : "VarDecl", "Expression" : [
			{ "Rule" : "Var ; Identifier(identifier) ; Colon ; Type(type)" , "Id" : "ComplexVarDecl" },
			{ "Rule" : "Var ; Type(type)" , "Id" : "JustType" }
			]
		},
		{ "Name" : "Type", "Expression" : [
			{ "Rule" : "Int(value)" , "Id" : "Integer" },
			{ "Rule" : "Float(value)", "Id" : "Float" }
			]
		},
		{ "Name" : "Start", "Expression" : [
			{ "Rule" : "VarDecl(varDecl)", "Id" : "Start" }
			]
		}
	]
}
