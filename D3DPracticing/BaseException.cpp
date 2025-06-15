#include "BaseException.h"
#include <sstream>

BaseException::BaseException(int line, std::string_view file) : line(line), file(file.data())
{
}

char const* BaseException::what() const
{
        std::ostringstream oss;
        oss << GetType() << std::endl
                << GetOriginString();
        whatBuffer = oss.str();
        return whatBuffer.c_str();
}

const char* BaseException::GetType() const
{
	return "Base Exception";
}

int BaseException::GetLine() const
{
	return line;
}

const std::string& BaseException::GetFile() const
{
	return file;
}

std::string BaseException::GetOriginString() const
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
