#include "action.h"

#include <sstream>

Action::Action()
{
}

Action::Action(const std::string& name, const std::string& type)
	:m_Name(name)
	, m_Type(type)
{
}
