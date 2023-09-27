#include "utils/string_manip.hpp"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

std::string GetDateTimeIdentifier()
{
	auto now = std::chrono::system_clock::now();
	std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm* localTime = std::localtime(&nowTime);

	std::ostringstream oss;
	oss << std::put_time(localTime, "%Y%m%d_%H%M%S");

	return oss.str();
}

void RemoveChars(std::string& s, const std::string& chars)
{
	size_t pos = 0;
	while ((pos = s.find_first_of(chars, pos)) != std::string::npos)
	{
		s.erase(pos, 1);
	}
}

#define KEY_TO_STRING_CASE(k) \
	case sf::Keyboard::k: return #k

std::string HumanName(sf::Keyboard::Key key)
{
	switch (key)
	{
		KEY_TO_STRING_CASE(Unknown);
		KEY_TO_STRING_CASE(A);
		KEY_TO_STRING_CASE(B);
		KEY_TO_STRING_CASE(C);
		KEY_TO_STRING_CASE(D);
		KEY_TO_STRING_CASE(E);
		KEY_TO_STRING_CASE(F);
		KEY_TO_STRING_CASE(G);
		KEY_TO_STRING_CASE(H);
		KEY_TO_STRING_CASE(I);
		KEY_TO_STRING_CASE(J);
		KEY_TO_STRING_CASE(K);
		KEY_TO_STRING_CASE(L);
		KEY_TO_STRING_CASE(M);
		KEY_TO_STRING_CASE(N);
		KEY_TO_STRING_CASE(O);
		KEY_TO_STRING_CASE(P);
		KEY_TO_STRING_CASE(Q);
		KEY_TO_STRING_CASE(R);
		KEY_TO_STRING_CASE(S);
		KEY_TO_STRING_CASE(T);
		KEY_TO_STRING_CASE(U);
		KEY_TO_STRING_CASE(V);
		KEY_TO_STRING_CASE(W);
		KEY_TO_STRING_CASE(X);
		KEY_TO_STRING_CASE(Y);
		KEY_TO_STRING_CASE(Z);
		KEY_TO_STRING_CASE(Num0);
		KEY_TO_STRING_CASE(Num1);
		KEY_TO_STRING_CASE(Num2);
		KEY_TO_STRING_CASE(Num3);
		KEY_TO_STRING_CASE(Num4);
		KEY_TO_STRING_CASE(Num5);
		KEY_TO_STRING_CASE(Num6);
		KEY_TO_STRING_CASE(Num7);
		KEY_TO_STRING_CASE(Num8);
		KEY_TO_STRING_CASE(Num9);
		KEY_TO_STRING_CASE(Escape);
		KEY_TO_STRING_CASE(LControl);
		KEY_TO_STRING_CASE(LShift);
		KEY_TO_STRING_CASE(LAlt);
		KEY_TO_STRING_CASE(LSystem);
		KEY_TO_STRING_CASE(RControl);
		KEY_TO_STRING_CASE(RShift);
		KEY_TO_STRING_CASE(RAlt);
		KEY_TO_STRING_CASE(RSystem);
		KEY_TO_STRING_CASE(Menu);
		KEY_TO_STRING_CASE(LBracket);
		KEY_TO_STRING_CASE(RBracket);
		KEY_TO_STRING_CASE(Semicolon);
		KEY_TO_STRING_CASE(Comma);
		KEY_TO_STRING_CASE(Period);
		KEY_TO_STRING_CASE(Quote);
		KEY_TO_STRING_CASE(Slash);
		KEY_TO_STRING_CASE(Backslash);
		KEY_TO_STRING_CASE(Tilde);
		KEY_TO_STRING_CASE(Equal);
		KEY_TO_STRING_CASE(Hyphen);
		KEY_TO_STRING_CASE(Space);
		KEY_TO_STRING_CASE(Enter);
		KEY_TO_STRING_CASE(Backspace);
		KEY_TO_STRING_CASE(Tab);
		KEY_TO_STRING_CASE(PageUp);
		KEY_TO_STRING_CASE(PageDown);
		KEY_TO_STRING_CASE(End);
		KEY_TO_STRING_CASE(Home);
		KEY_TO_STRING_CASE(Insert);
		KEY_TO_STRING_CASE(Delete);
		KEY_TO_STRING_CASE(Add);
		KEY_TO_STRING_CASE(Subtract);
		KEY_TO_STRING_CASE(Multiply);
		KEY_TO_STRING_CASE(Divide);
		KEY_TO_STRING_CASE(Left);
		KEY_TO_STRING_CASE(Right);
		KEY_TO_STRING_CASE(Up);
		KEY_TO_STRING_CASE(Down);
		KEY_TO_STRING_CASE(Numpad0);
		KEY_TO_STRING_CASE(Numpad1);
		KEY_TO_STRING_CASE(Numpad2);
		KEY_TO_STRING_CASE(Numpad3);
		KEY_TO_STRING_CASE(Numpad4);
		KEY_TO_STRING_CASE(Numpad5);
		KEY_TO_STRING_CASE(Numpad6);
		KEY_TO_STRING_CASE(Numpad7);
		KEY_TO_STRING_CASE(Numpad8);
		KEY_TO_STRING_CASE(Numpad9);
		KEY_TO_STRING_CASE(F1);
		KEY_TO_STRING_CASE(F2);
		KEY_TO_STRING_CASE(F3);
		KEY_TO_STRING_CASE(F4);
		KEY_TO_STRING_CASE(F5);
		KEY_TO_STRING_CASE(F6);
		KEY_TO_STRING_CASE(F7);
		KEY_TO_STRING_CASE(F8);
		KEY_TO_STRING_CASE(F9);
		KEY_TO_STRING_CASE(F10);
		KEY_TO_STRING_CASE(F11);
		KEY_TO_STRING_CASE(F12);
		KEY_TO_STRING_CASE(F13);
		KEY_TO_STRING_CASE(F14);
		KEY_TO_STRING_CASE(F15);
		KEY_TO_STRING_CASE(Pause);
		default: return "Invalid key";
	}
}
#undef KEY_TO_STRING_CASE

std::string LeftPad(const std::string& original, size_t total_length, char pad_char)
{
	if (total_length <= original.size())
	{
		assert(false);
		return original;
	}

	std::string padding(total_length - original.size(), pad_char);
	return padding + original;
}

std::string RightPad(const std::string& original, size_t total_length, char pad_char)
{
	if (total_length <= original.size())
	{
		assert(false);
		return original;
	}

	std::string padding(total_length - original.size(), pad_char);
	return original + padding;
}

std::string BoolToStringAsEnabledOrDisabled(bool x)
{
	if (x)
	{
		return "Enabled";
	}
	else
	{
		return "Disabled";
	}
}

std::string CreateBadgeText(float f, int precision)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision);

	if (f > -10.f && f < 10.f)
	{
		ss << std::setw(2 + precision + (precision > 0 ? 1 : 0)) << std::setfill('0');
	}

	ss << f;
	return ss.str();
}