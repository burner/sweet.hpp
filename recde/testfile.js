{
	"Token" : [
		{ "Name" : "Var", "Regex" : "var", "ConvertFunction" : "void" },
		{ "Name" : "Int", "Regex" : "int", "ConvertFunction" : "void" },
		{ "Name" : "Identifier", "Regex" : "", "ConvertFunction" : "void" },
		{ "Name" : "Float", "Regex" : "float", "ConvertFunction" : "void" }
	],
	"Rules" : [
		{ "Name" : "VarDecl", "Expression" : [
			{ "Rule" : "Var ; Identifier(identifier) ; Colon ; Type(type)" },
			{ "Rule" : "Var ; Type(type)" }
			]
		},
		{ "Name" : "Type", "Expression" : [
			{ "Rule" : "Int(integer)" },
			{ "Rule" : "Float(floatingpoint)" }
			]
		},
		{ "Name" : "Start", "Expression" : [
			{ "Rule" : "VarDecl" }
			]
		}
	]
}
