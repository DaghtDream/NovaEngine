#include "ReadingFile.h"

#include <iostream>
#include <fstream>
#include <iterator>

namespace Rendering
{
	std::string ReadingFile::ReadFileToString(const std::string& filepath)
	{
		std::ifstream file(filepath);

		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filepath << std::endl;
			exit(1);
		}

		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		return content;
	}
}