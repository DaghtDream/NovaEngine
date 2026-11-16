#pragma once


#include <string>

namespace Rendering
{
	class ReadingFile
	{
	public:
		static std::string ReadFileToString(const std::string& filepath);
	};
}