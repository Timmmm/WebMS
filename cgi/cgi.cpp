#include "cgi.h"

#include <iostream>
#include <vector>
#include "../common/common.h"


bool IsHex(char C)
{
	return isxdigit(C);
}

void Translate(string& S)
{
	stringpos N; // Percent index.
	stringpos M = 0; // Search from.

	for (unsigned int i = 0; i < S.length(); ++i)
		if (S[i] == '+')
			S[i] = ' ';

	while (true)
	{
		N = S.find_first_of('%', M);

		if (N == string::npos || N + 3 > S.length())
			break;

		if (IsHex(S[N + 1]) && IsHex(S[N + 2]))
		{
			char Hex[3] = { S[N + 1], S[N + 2], '\0' };
			int H = strtol(Hex, NULL, 16);
			S.replace(N, 3, 1, static_cast<char>(H));
		}

		M = N + 1;
	}
}

char ToHex(int C)
{
	switch (C)
	{
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'A';
	case 11: return 'B';
	case 12: return 'C';
	case 13: return 'D';
	case 14: return 'E';
	case 15: return 'F';
	default: break;
	}

	return '-';
}

string Encode(const string& S)
{
	string R; // Return string.
	for (unsigned int i = 0; i < S.length(); ++i)
	{
		if (isalnum(S[i]))
		{
			R += S[i];
		}
		else
		{
			R += '%';
			R += ToHex((S[i] >> 4) & 0xF);
			R += ToHex(S[i] & 0xF);
		}
	}
	return R;
}

bool ParseFormData(FormType& Fill)
{
	string QueryString;		// "foo=bar&baz=beh&meh=bleh"

//	if (GetEnv("REQUEST_METHOD") == "GET")
//	{
		QueryString = GetEnv("QUERY_STRING");
//	}
//	else if (GetEnv("REQUEST_METHOD") == "POST")
//	{
		int Length = StoI(GetEnv("CONTENT_LENGTH"));
		if (Length > 0)
		{
			char* Buffer = new char[Length + 1];
			fread(Buffer, 1, Length, stdin);
			Buffer[Length] = '\0';

			if (!QueryString.empty())
				QueryString += "&";
			QueryString += Buffer;
			delete[] Buffer;
		}
//	}

	vector<string> Pairs;
	// Split the query string into pairs.
	while (QueryString != "")
	{
		stringpos n = QueryString.find_last_of('&');
		if (n != string::npos)
		{
			if (n + 1 < QueryString.length())
				Pairs.push_back(QueryString.substr(n + 1));
			QueryString.erase(n);
		}
		else
		{
			Pairs.push_back(QueryString);
			break;
		}
	}

	// Do it backwards so things are concatenated in the right order.
	for (vector<string>::reverse_iterator i = Pairs.rbegin(); i != Pairs.rend(); ++i)
	{
		// Split into a key and value. Decode and add both to Fill.
		stringpos n = i->find_first_of('=');
		if (n == string::npos || n == 0)
			continue;
		string Key = i->substr(0, n);
		string Value;
		if (n + 1 < i->length())
			Value = i->substr(n + 1);

		Translate(Key);
		Translate(Value);

		Fill[Key] += Value;
	}

	return true;
}

bool ParseCookieData(CookieType& Fill)
{
	string CookieString = GetEnv("HTTP_COOKIE");

	vector<string> Pairs;
	// Split the cookie string into pairs.
	while (CookieString != "")
	{
		stringpos n = CookieString.find_last_of(';');
		if (n != string::npos)
		{
			if (n + 2 < CookieString.length())
				Pairs.push_back(CookieString.substr(n + 2));
			CookieString.erase(n);
		}
		else
		{
			Pairs.push_back(CookieString);
			break;
		}
	}

	// Do it backwards so things are concatenated in the right order.
	for (vector<string>::reverse_iterator i = Pairs.rbegin(); i != Pairs.rend(); ++i)
	{
		// Split into a key and value. Decode and add both to Fill.
		stringpos n = i->find_first_of('=');
		if (n == string::npos || n == 0)
			continue;
		string Key = i->substr(0, n);
		string Value;
		if (n + 1 < i->length())
			Value = i->substr(n + 1);

		Translate(Key);
		Translate(Value);

		Fill[Key] += Value;
	}
	return true;
}
