#include <common/hareg_error.h>

int operator <<( std::ostream& o, const HaregError& err){
			o << "HaregError: " << err.message << " file: " << err.file_name << " line: " << err.line_in_file << '\n';
			return 0;
}
