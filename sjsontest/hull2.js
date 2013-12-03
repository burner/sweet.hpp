{
	"Config" : {
		"system" : {
			"numNodes" : "32",
			"layout" : "2",
			"nodes" : [
				{ "name": 0, "avai": "0.9" },
				{ "name": "1", "avai": "0.87" },
				{ "name": "2", "avai": "0.87" },
				{ "name": "3", "avai": "0.87" },
				{ "name": "default", "avaiFunc": "srand(low, high)",
					"avaiVars" : [
						{"name" : "low", "value" : "0.8"},
						{"name" : "high", "value" : "1.0"}
					]
				}
			],

			edges : [
				{ "start": "2", "end" : "1", "avai": 0.17 },
				{ "start": "2", "end" : "3", "avai": "0.57" },
				{ "start": "3", "end" : "0", "avai": "0.77" },
				{ "name": "default", "avaiFunc": "srand(low, high)",
					"avaiVars" : [
						{"name" : "low", "value" : "0.8"},
						{"name" : "high", "value" : "1.0"}
					]
				}
			]
		},
	
		"jobs" : [
			{"jobname" : "createCompleteRectConnectedGrid", "number" : "0",
				"variables" : [
					{"name" : "size", "value" : "32"},
					{"name" : "ratio", "value" : "2.5"}
				]
			},
			{"jobname" : "mapNodeToGraph", "number" : "1"},
			{"jobname" : "mapEdgesToGraph", "number" : "2"},
			{"jobname" : "graphToDot", "number" : "3",
				"variables" : [
					{"name" : "filename", "value" : "graph.dot"}
				]
			},
			{"jobname" : "createConvexHull", "number" : "4"},
			{"jobname" : "createHull", "number" : "5"}
		]
	}
}
