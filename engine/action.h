#pragma once
#include <string>

class Action
{
	std::string m_Name = "NONE";
	std::string m_Type = "NONE";

public:

	Action();
	Action(const std::string& name, const std::string& type);

	const std::string& GetName() const { return m_Name; };
	const std::string& GetType() const { return m_Type; };
	std::string ToString() const;


};

