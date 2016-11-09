#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <limits>

using namespace std;

void its_a_trap(string signal, string d)
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

void decimal_to_dfp(string d)
{
	string			signal;
	string			exponent;
	ostringstream	mantissa;
	double			decimal;

	signal = d.front() == '-' ? "1" : "0";

	if (d == "inf" || d == "-inf")
	{
		decimal = numeric_limits<double>::max();
	}
	else {
		decimal = abs(stod(d));
	}

	cout << decimal << endl;

	if (decimal == 0)
	{
		its_a_trap(signal, d);
	} 
	else
	{
		int exp = 0;
		double aux;
		double fract;

		// normalize
		if (decimal > 0 && decimal < 1)
		{
			fract = modf(decimal, &aux);
			while (aux == 0)
			{
				fract *= 2;
				fract = modf(fract, &aux);
				exp++;
			}
			exp *= -1;
			decimal = fract;
		}
		else
		{
			while (decimal >= 2)
			{
				decimal /= 2;
				exp++;
			}
		}

		exponent = exponent_to_bin(exp + 1023);

		double int_part;
		int filled_bits = 0;
		fract = modf(decimal, &aux);

		while (fract != 0)
		{
			fract *= 2;
			fract = modf(fract, &int_part);
			mantissa << static_cast<int>(int_part);
			filled_bits++;
		}

		for (auto i = 0; i < 52 - filled_bits; i++)
		{
			mantissa << '0';
		}

		string res = signal + exponent + mantissa.str();
		stringstream hex_out;

		bitset<64> set(res);
		hex_out << hex << uppercase << set.to_ullong();

		cout << "Number: " << d << endl;
		cout << "0x" << hex_out.str() << endl;
		cout << signal << " | " << exponent << " | " << mantissa.str() << endl << endl;
	}
}

void dfp_to_decimal(string b)
{
	string	signal;
	int		exponent = 0;
	double	mantissa = 0;

	signal = b.front() == '0' ? ' ' : '-';

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

	string entry;// { argv[1] };

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
