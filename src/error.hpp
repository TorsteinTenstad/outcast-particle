#pragma once

class Error_t
{
private:
	bool is_error_;

public:
	Error_t() :
		is_error_(false) {}
	Error_t(bool is_error) :
		is_error_(is_error) {}
	operator bool() const
	{
		return is_error_;
	}
	Error_t operator+(const Error_t& other) const
	{
		return Error_t(is_error_ || other.is_error_);
	}
	Error_t& operator+=(const Error_t& other)
	{
		is_error_ = is_error_ || other.is_error_;
		return *this;
	}
};

#define ERROR Error_t(true)
#define SUCCESS Error_t(false)