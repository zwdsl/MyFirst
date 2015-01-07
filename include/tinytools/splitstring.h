
#ifndef __PUBLIC_SPLITSTRING_H__
#define __PUBLIC_SPLITSTRING_H__

#include <vector>
#include <string>

class CSplitString
{
private:
	std::vector<std::string> _list;
	std::string _string;

public:
	CSplitString( void )	
	{
	}

	CSplitString( const char *str, const char *spt = "," )
	{
		Split( str, spt );
	}

	void Split( const char *str, const char *spt = "," )
	{
		_string = str; _list.clear();

		std::string st = str; std::string sp = spt;
		std::string::size_type spl = sp.length();
		std::string::size_type pos = st.find(spt);

		while ( pos != std::string::npos )
		{
			_list.push_back( st.substr(0,pos) );
			st.erase( 0, pos+spl );
			pos = st.find( spt );
		}

		if ( st.length() >= 0 ) _list.push_back( st );
	}

	std::string::size_type getCount( void )
	{
		return _list.size( );
	}

	const std::string& get(int idx) const
	{
		return _list[idx];
	}
};


#endif // __PUBLIC_SPLITSTRING_H__
