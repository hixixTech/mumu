#pragma once
#include <string>
#include <vector>
#include <memory>
class mu_method
{
public:
	enum Type
	{
		SIGNAL_METHOD,
		SLOT_METHOD
	};
public:
	mu_method(std::string strMethod, Type nType);
	~mu_method();
public:
	std::string get_name();
	unsigned int get_type_count();
	std::string get_type(int nIndex);
	Type get_method_type();
private:
	std::vector<std::string> m_strMethodTypes;
	std::string m_strName;
	Type m_nType;
};

typedef std::shared_ptr<mu_method> mu_method_ptr;

