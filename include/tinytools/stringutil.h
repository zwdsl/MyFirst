
#pragma 

#include <string>
#include <sstream>


template<class T>
std::string TypeToString(const T& v)
{
	std::ostringstream os;
	os << v;

	return os.str();
}

template<class T>
T StringToType( const std::string& s, T& v )
{
	std::istringstream is( s );
	is >> v;

	return v;
}

inline
std::string EscapeString(const std::string& s)
{
	std::string result;
	for (size_t i=0; i<s.length(); i++)
	{
		char c = s[i];
		if (c >= 0x20 && c <= 0x7e) 
		{
			result += c;
		}
		else 
		{
			switch (c) {
			case '\t':
				result += "\\t"; break;
			case '\n':
				result += "\\n"; break;
			case '\r':
				result += "\\r"; break;
			case '\\':
				result += "\\\\"; break;
			case '\0':
				result += "\\0"; break;
			default:
				{
					char buf[5];
					snprintf(buf, sizeof(buf), "\\x%02x", (unsigned char)c);
					result += buf;
				}
			}
		}
	}
	return result;
}

