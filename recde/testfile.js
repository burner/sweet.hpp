{
	"Token" : [
		{ "Name" : "Semicolon", "Regex" : ";", "ConvertFunction" : "nothing" },
		{ "Name" : "Integer", "Regex" : "[0-9]+", "ConvertFunction" : "toInteger" }
	],
	"Rules" : [
		{ "Name" : "A", "Expression" : [
			{ "Rule" : "Whitespace; separated" },
			{ "Rule" : "Another; Whitespace; separated" }
			]
		},
		{ "Name" : "B", "Expression" : [
			{ "Rule" : " Whitespace ; separated" },
			{ "Rule" : "Another(condition); Whitespace; separated" }
			]
		}
	]
}
