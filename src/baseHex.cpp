#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string stringToHex(const std::string &input)
{
	std::ostringstream os;
	for (std::string::size_type i = 0; i < input.length(); ++i)
	{
		os << std::hex << std::setw(2) << std::setfill('0') << (int)input[i];
	}
	return os.str();
}

unsigned char hexCharToDec(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	// Caractere inválido
	throw std::invalid_argument("Caractere hexadecimal inválido");
}

std::string hexToString(const std::string &hex)
{
	if (hex.length() % 2 != 0)
	{
		// A string hexadecimal deve ter um número par de dígitos
		throw std::invalid_argument("String hexadecimal inválida");
	}

	std::string result;
	for (size_t i = 0; i < hex.length(); i += 2)
	{
		unsigned char high = hexCharToDec(hex[i]);
		unsigned char low = hexCharToDec(hex[i + 1]);
		unsigned char decoded = (high << 4) | low;
		result += decoded;
	}

	return result;
}
