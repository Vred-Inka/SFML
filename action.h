#pragma once
#include <string>

class Action
{
	std::string mName = "NONE";
	std::string mType = "NONE";

public:

	Action();
	Action(const std::string& name, const std::string& type);

	const std::string& GetName() const { return mName; };
	const std::string& GetType() const { return mType; };
	std::string ToString() const;


};

