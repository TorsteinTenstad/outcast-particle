#pragma once
#include <deque>
#include <optional>
#include <string>

enum class ErrorNumber : int
{
	UNKNOWN_ERROR = 0,
	LOAD_FROM_FILE = 100,
	LOAD_LEVEL,
	LOAD_SHADER,
	LOAD_RECORDS,
	LOAD_WAV
};

class ErrorInfo
{
public:
	ErrorNumber error_number = ErrorNumber::UNKNOWN_ERROR;
	std::optional<std::string> message;
};

class Error
{
public:
	std::deque<ErrorInfo> error_infos;

	Error() {}
	Error(ErrorNumber error_number) :
		error_infos({ { error_number, std::nullopt } }) {}
	Error(ErrorNumber error_number, std::string message) :
		error_infos({ { error_number, message } }) {}

	operator bool() const
	{
		return error_infos.size() > 0;
	}

	Error operator+(const Error& other) const
	{
		Error result = *this;
		result.error_infos.insert(result.error_infos.end(), other.error_infos.begin(), other.error_infos.end());
		return result;
	}

	Error& operator+=(const Error& other)
	{
		this->error_infos.insert(this->error_infos.end(), other.error_infos.begin(), other.error_infos.end());
		return *this;
	}
};

#define ERROR Error(ErrorNumber::UNKNOWN_ERROR)
#define SUCCESS Error()