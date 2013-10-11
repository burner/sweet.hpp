#pragma once

enum RulePartType {
	RulePartType_Unknown,
	RulePartType_Token,
	RulePartType_Rule
};

struct RulePart {
	bool save;
	RulePartType type;

	inline RulePart() : save(false), type(RulePartType_Unknown) {
	}
};
