#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <limits>
#include <regex>

using namespace std;

void zero(string signal)
{
	signal == "0" ? cout << "0x0000000000000000" << endl : cout << "0x8000000000000000" << endl;

	cout << signal << " | " << "0000000000" << " | "
		<< "000000000000000000000000000000000000000000000000000\n\n";
}

void infinity(string signal)
{
	signal == "0" ? cout << "0x7FF0000000000000" << endl : cout << "0xFFF0000000000000" << endl;

	cout << signal << " | " << "1111111111" << " | "
		<< "000000000000000000000000000000000000000000000000000\n\n";
}

void not_a_number()
{
	cout << "0x7FFFFFFFFFFFFFFF" << endl;

	cout << "0 | " << "1111111111" << " | "
		<< "111111111111111111111111111111111111111111111111111\n\n";
}

string exponent_to_bin(int n)
{
	ostringstream out;

	for (auto i = 0x400; i != 0; i >>= 1)
	{
		out << ((n & i) == 0 ? 0 : 1);
	}

	return out.str();
}

void print_dfp(string signal, string exponent, string mantissa)
{
	bitset<64> set(signal + exponent + mantissa);

	cout << "0x" << hex << uppercase << set.to_ullong() << endl;
	cout << signal << " | " << exponent << " | " << mantissa << endl << endl;
}

void decimal_to_dfp(string d)
{
	string			signal;
	string			exponent;
	ostringstream	mantissa;
	double			decimal;
	regex			nan("[a-zA-Z]+");

	signal = d.front() == '-' ? "1" : "0";
	
	if (d == "infinity" || d == "-infinity")
	{
		infinity(signal);
		//decimal = pow(2, 1023);
	}
	else if (regex_match(d, nan))
	{
		not_a_number();
		//decimal = numeric_limits<double>::max(); //(1 + (1 - pow(2, -52))) * pow(2, 1023);
	}
	else
	{
		decimal = abs(stod(d));

		if (decimal == 0)
		{
			zero(signal);
		}
		else
		{
			auto exp_count = 0;
			double aux;
			double fract_part;

			if (decimal > 0 && decimal < 1)
			{
				fract_part = modf(decimal, &aux);
				while (aux == 0)
				{
					fract_part *= 2;
					fract_part = modf(fract_part, &aux);
					exp_count++;
				}
				exp_count = -exp_count;
				decimal = fract_part;
			}
			else
			{
				while (decimal >= 2)
				{
					decimal /= 2;
					exp_count++;
				}
			}

			exponent = exponent_to_bin(exp_count + 1023);

			double int_part;
			auto filled_bits = 0;
			fract_part = modf(decimal, &int_part);

			while (fract_part != 0)
			{
				fract_part *= 2;
				fract_part = modf(fract_part, &int_part);
				mantissa << static_cast<int>(int_part);
				filled_bits++;
			}

			for (auto i = 0; i < 52 - filled_bits; i++)
			{
				mantissa << '0';
			}

			print_dfp(signal, exponent, mantissa.str());
		}
	}
}

void dfp_to_decimal(string b)
{
	string	signal;
	auto exponent = -1023;
	auto mantissa = 1.0;

	signal = b.front() == '0' ? "" : "-";

	for (auto i = 1; i < 12; i++)
	{
		exponent += (b[i] - 48) * pow(2.0, 11 - i);
	}

	for (auto i = 12; i < 65; i++)
	{
		mantissa += (b[i] - 48) * pow(2.0, 12 - (i + 1));
	}

	if (exponent < 0)
	{
		for (auto i = 0; i < abs(exponent); i++)
		{
			mantissa /= 2;
		}
	}
	else
	{
		for (auto i = 0; i < exponent; i++)
		{
			mantissa *= 2;
		}
	}

	cout << "Decimal: " << signal << mantissa << endl << endl;
}

int main(int argc, char **argv)
{
	string entry;
	
	while (cin >> entry)
	{
		transform(entry.begin(), entry.end(), entry.begin(), ::tolower);
		if (entry.back() == 'b')
		{
			dfp_to_decimal(entry);
		}
		else
		{
			decimal_to_dfp(entry);
		}
	}
	

	return 0;
}
