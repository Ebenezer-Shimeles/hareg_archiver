#pragma once
#include <ostream>
#include <string>

/*
   Encapsuates runtime errors 
   not perfect
    might need to add stack traces...
*/
class HaregError{
	public:
		std::string message;
		std::string file_name;
		int32_t line_in_file;
		HaregError(const std::string& msg, const std::string& file,const int32_t& line) : message(msg), file_name(file), line_in_file(line){}
		HaregError() = default;

	    
};
int operator <<( std::ostream& o, const HaregError& err);