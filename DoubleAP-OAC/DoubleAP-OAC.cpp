#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <limits>
#include <regex>

using namespace std;

void zero(string signal, string d)
{
	cout << "Number: " << d << endl;
	signal == "0" ? cout << "0x0000000000000000" << endl : cout << "0x8000000000000000" << endl;

	cout << signal << " | ";

	for (auto i = 0; i < 11; i++)
	{
		cout << "0";
	}

	cout << " | ";

	for (auto i = 0; i < 52; i++)
	{
		cout << "0";
	}

	cout << endl << endl;
}

void infinity(string signal)
{
	cout << "Infinity" << endl;
	signal == "0" ? cout << "0x7FF0000000000000" << endl : cout << "0xFFF0000000000000" << endl;

	cout << signal << " | ";

	for (auto i = 0; i < 11; i++)
	{
		cout << "1";
	}

	cout << " | ";

	for (auto i = 0; i < 52; i++)
	{
		cout << "0";
	}

	cout << endl << endl;
}

void not_a_number()
{
	cout << "NaN" << endl;
	cout << "0x7FFFFFFFFFFFFFFF" << endl;

	cout << "0 | ";

	for (auto i = 0; i < 11; i++)
	{
		cout << "1";
	}

	cout << " | ";

	for (auto i = 0; i < 52; i++)
	{
		cout << "1";
	}

	cout << endl << endl;
}

string exponent_to_bin(int n)
{
	ostringstream out;
	for (auto i = 0; i < 11; i++)
	{
		out << n % 2;
		n >>= 1;
	}
	string aux = out.str();
	reverse(aux.begin(), aux.end());
	return aux;
}

void print_dfp(string signal, string exponent, string mantissa)
{
	stringstream hex_out;
	bitset<64> set(signal + exponent + mantissa);
	hex_out << hex << uppercase << set.to_ullong();

	cout << "0x" << hex_out.str() << endl;
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
			zero(signal, d);
		}
		else
		{
			int exp_count = 0;
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
				exp_count *= -1;
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
			int filled_bits = 0;
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
	int		exponent = 0;
	double	mantissa = 0;

	signal = b.front() == '0' ? "" : "-";

	for (auto i = 1; i < 12; i++)
	{
		exponent += (b[i] - 48) * pow(2.0, 11 - i);
	}

	exponent -= 1023;

	for (auto i = 12; i < 65; i++)
	{
		mantissa += (b[i] - 48) * pow(2.0, 12 - (i + 1));
	}
	
	mantissa += 1;

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

	cout << signal << mantissa << endl;
}

int main(int argc, char **argv)
{
	/*if (argc != 2)
	{
		cout << "Número de parametros errado" << endl;
		return(1);
	}*/

	string entry;// = argv[1];

	while (true)
	{
		cin >> entry;

		if (entry.back() == 'B' || entry.back() == 'b')
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
