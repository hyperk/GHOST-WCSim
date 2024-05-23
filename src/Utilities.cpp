#include "Utilities.h"

#include <unistd.h>
#include <iostream>

bool Ghost::Utils::FileExists(const char* filename) {
	bool exists = access(filename, F_OK) != -1;
	if(!exists) {
		std::cerr << filename << " not found or inaccessible." << std::endl;
	}
	return exists;
}

std::string Ghost::Utils::GetEnvironmentVariableWithDefault(const char* variable, const char* default_value) {
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

std::string Ghost::Utils::GetConfigFilename(Store& variables,
                                            const char* config_param,
                                            const char* default_filename) {

	std::string filename;
	if(!variables.Get(config_param, filename))
		filename = default_filename;
	if(!Ghost::Utils::FileExists(filename.c_str())) {
		std::cerr << "File: " << filename << " not found." << std::endl
		          << "Please give a valid option in the config file to: " << config_param << std::endl
		          << "Exiting..." << std::endl;
		exit(-1);
	}
	std::cout << "Using filename: " << filename << " for parameter: " << config_param << std::endl;
	return filename;
}

std::fstream Ghost::Utils::OpenFileStream(std::string fileName) {
	std::fstream inputFile;
	inputFile.open(fileName, std::fstream::in);
	if(!inputFile.is_open()) {
		std::cout << "File " << fileName << " not found" << std::endl;
		throw std::ios_base::failure("");
	}
	return inputFile;
}

std::vector<std::string> Ghost::Utils::readInLine(std::fstream& inFile, int lineSize, char* inBuf) {
	// Read in line break it up into tokens
	// Any line starting with # is ignored
	while(true) {
		if(inFile.getline(inBuf, lineSize)) {
			if(inBuf[0] != '#') {
				return Ghost::Utils::tokenize(" $\r", inBuf);
			}
		}
		else {
			if(inFile.fail())
				std::cerr << "Failed to read line. Is the buffer size large enough?" << std::endl;
			std::vector<std::string> nullLine;
			return nullLine;
		}
	}
}

// Returns a vector with the tokens
std::vector<std::string> Ghost::Utils::tokenize(std::string separators, std::string input) {
	std::size_t startToken = 0, endToken;  // Pointers to the token pos
	std::vector<std::string> tokens;       // Vector to keep the tokens

	if(separators.size() > 0 && input.size() > 0) {
		while(startToken < input.size()) {
			// Find the start of token
			startToken = input.find_first_not_of(separators, startToken);

			// If found...
			if(startToken != input.npos) {
				// Find end of token
				endToken = input.find_first_of(separators, startToken);
				if(endToken == input.npos)
					// If there was no end of token, assign it to the end of string
					endToken = input.size();

				// Extract token
				tokens.push_back(input.substr(startToken, endToken - startToken));

				// Update startToken
				startToken = endToken;
			}  // if token is found
		}      // while we've not completed tokenising the string
	}          // if input is valid

	return tokens;
}
