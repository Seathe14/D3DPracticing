#pragma once
#include <exception>
#include <string_view>

class BaseException : public std::exception
{
public:
	BaseException(int line, std::string_view file);
	char const* what() const override;
	virtual const char* GetType() const;

	int GetLine() const;
	const std::string& GetFile() const;
	std::string GetOriginString() const;
private:
	int line{ -1 };
	std::string file;
protected:
	mutable std::string whatBuffer;
};