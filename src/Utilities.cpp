#include "Utilities.h"

#include <unistd.h>
#include <iostream>

bool HK::Ghost::utils::FileExists(const char* filename) {
	bool exists = access(filename, F_OK) != -1;
	if(!exists) {
		std::cerr << filename << " not found or inaccessible." << std::endl;
	}
	return exists;
}

std::string HK::Ghost::utils::GetEnvironmentVariableWithDefault(const char* variable,
                                                                const char* default_value) {
	std::string env;
	char* env_temp = std::getenv(variable);
	if(env_temp == NULL || !env_temp[0]) {  // make sure it's non-empty
		std::cout << "Environment variable $" << variable << " not set." << std::endl
		          << " Using default value of " << default_value << std::endl;
		env = default_value;
	}
	else {
		env = env_temp;
		std::cout << "Found environment variable $" << variable << " = " << env << std::endl;
	}
	return env;
}

std::string HK::Ghost::utils::GetConfigFilename(Store& variables,
                                                const char* config_param,
                                                const char* default_filename) {

	std::string filename;
	if(!variables.Get(config_param, filename))
		filename = default_filename;
	if(!utils::FileExists(filename.c_str())) {
		std::cerr << "File: " << filename << " not found." << std::endl
		          << "Please give a valid option in the config file to: " << config_param << std::endl
		          << "Exiting..." << std::endl;
		exit(-1);
	}
	std::cout << "Using filename: " << filename << " for parameter: " << config_param << std::endl;
	return filename;
}
