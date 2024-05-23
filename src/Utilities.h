#include <string>

#include "Store.h"

namespace Ghost::Utils {
	//! Get an environment variable, falling back to a default value
	/*!
		\param variable The name of the environment variable
		\param default_value The default value, to use if the environment variable isn't set or is
		empty \return A string containing the environment variable if it is set and is not empty, else
		the default
	*/
	std::string GetEnvironmentVariableWithDefault(const char* variable, const char* default_value);

	//! Check if a file exists and is accessible
	/*!
		\param filename The name of the file
		\return True if it exists and is accessible, false if not
	*/
	bool FileExists(const char* filename);

	//! Get a filename from the config file
	/*!
		Also checks that the file exists. There is a hard exit if not
		\param variables The Store containing your Tools' config file options
		\param config_param The name of the config file variable
		\param default_filename A fallback
		\return The filename
	*/
	std::string GetConfigFilename(Store& variables,
																const char* config_param,
																const char* default_filename);

	//! Open a filestream from a filename
	std::fstream OpenFileStream(std::string fileName);

	//! Converts `std::string` to `double`
	/*!
		Wrapper to `std::atof` that accepts a `const char *`
	*/
	inline double atof(const std::string& str) {return std::atof( str.c_str() );}
	//! Converts `std::string` to `int`
	/*!
		Wrapper to `std::atoi` that accepts a `const char *`
	*/
	inline int    atoi(const std::string& str) {return std::atoi( str.c_str() );}

	//! Read in line break it up into tokens separated by whitespace
	/*!
		Any line starting with # is ignored (treated as a comment).
		\param inFile The input file stream to read a line from
		\param lineSize The size of the buffer (maximum length of the line in characters - 1)
		\param inBuf A buffer to store each line temporarily. Should be size `lineSize`
		\return A vector containing ntokens elements. Will be a vector of size 0 if no line can be found (e.g. end of file, all lines are comments)
	*/
	std::vector<std::string> readInLine(std::fstream& inFile, int lineSize, char* inBuf);

	//! Split a string into tokens based on various separators
	/*!
		\param separators String containing the items to split on
		\param input The string you want to tokenize
		\return A vector containing ntokens elements. Will be a vector of size 0 if either input is invalid. If there are no separators found in input, will be a vector of size 1 containing the input string
	*/
	std::vector<std::string> tokenize(std::string separators, std::string input);

} // namespace Ghost::Utils
