#include "mu_method.h"


mu_method::mu_method(std::string strMethod, Type nType) :m_nType(nType)
{
	int len = strMethod.length();
	std::string strTemp;
	for (int i = 0; i < len; ++i)
	{
		if (m_strName.empty())
		{
			if (strMethod[i] == '(')
			{
				m_strName = strTemp;
				strTemp = "";
				continue;
			}
			else if (strMethod[i] == ' ')
			{
				continue;
			}
		}
		else
		{
			if (strMethod[i] == ',' || strMethod[i] == ')')
			{
				std::string strStars;
				while (!strTemp.empty())
				{
					if (strTemp.back() == '*' || strTemp.back() == '&')
					{
						strStars = strTemp.back() + strStars;
						strTemp.pop_back();
						continue;
					}
					else if (strTemp.back() == ' ')
					{
						strTemp.pop_back();
						continue;
					}
					break;
				}
				while (!strTemp.empty())
				{
					if (strTemp.front() == ' ')
					{
						strTemp.erase(0, 1);
						continue;
					}
					break;
				}
				for (size_t t = 0; t < strTemp.length(); ++t)
				{
					if (strTemp[t] == ' ')
					{
						if (strTemp[t - 1] == ' ')
						{
							strTemp.erase(t, 1);
							t = t - 1;
						}
					}

				}
				strTemp += strStars;
				m_strMethodTypes.push_back(strTemp);
				strTemp = "";
			}
			if (strMethod[i] == ')')
			{
				return;
			}
			else if (strMethod[i] == ',')
			{
				continue;
			}
		}
		strTemp += strMethod[i];
	}
}


mu_method::~mu_method()
{
}

std::string mu_method::get_name()
{
	return m_strName;
}

unsigned int mu_method::get_type_count()
{
	return m_strMethodTypes.size();
}

std::string mu_method::get_type(int nIndex)
{
	return m_strMethodTypes[nIndex];
}

mu_method::Type mu_method::get_method_type()
{
	return m_nType;
}
