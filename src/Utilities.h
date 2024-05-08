#include <string>

#include "Store.h"

namespace HK {
	namespace Ghost {
		namespace utils {
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
		}  // namespace utils
	}      // namespace Ghost
}  // namespace HK
