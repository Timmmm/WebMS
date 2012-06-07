#include "template.h"


Template::Template()
{
}
Template::Template(const string& file)
{
	// TODO: Just store the filename, and read it in on Print().
	ifstream in(file.c_str());
	if (in)
	{
		stringstream ss;

		ss << in.rdbuf();
		contents = ss.str();
	}
}
bool Template::Load(const string& file)
{
	ifstream in(file.c_str());
	if (!in)
		return false;

	stringstream ss;

	ss << in.rdbuf();
	contents = ss.str();

	return true;
}
void Template::ShowSection(const string& section)
{
	sections[section] = true;
}
void Template::HideSection(const string& section)
{
	sections[section] = false;
}
void Template::SetVar(const string& key, const string& value)
{
	variables[key] = value;
}
void Template::AppendVar(const string& repeated_key, const string& value)
{
	repeated_variables[repeated_key].push_back(value);
}
void Template::SetRepeats(const string& repeated_section, int number)
{
	if (number < 0)
		number = 0;

	repeated_sections[repeated_section] = number;
}
void Template::AddHeader(const string& header)
{
	headers.push_back(header);
}
void Template::Print(bool printHeaders)
{
	if (printHeaders)
	{
		// This doesn't work for some reason.
		//		cout << "Content-Type: text/xhtml+xml; charset=UTF-8\r\n\r\n";
		cout << "Content-Type: text/html; charset=UTF-8\r\n\r\n";
		for (unsigned int i = 0; i < headers.size(); ++i)
			cout << headers[i] << "\r\n";
		cout << "\r\n";
	}
	

	// Section stack.

	// Algorithm:
	// Search for the next [[.
	// Switch type: (need to scan to next space or ]] for this.
	//  Single variable, replace it, done.
	//  Section, increment section counter and enter section if shown
	//    otherwise go to the corresponding ]].
	//  Repeated section, read in the whole section, and repeatedly handle it in
	//    a special other function.
	//  Repeated variable, ignore it - these are only allowed in repeated sections.

	// Just do normal variables first.
	// Now make it do sections too.
	// Now repeated sections and variables...
	stringpos pos = 0;
	while (true)
	{
		// Find the [[
		stringpos open = contents.find("[[", pos);
		// Print up to it.
		if (open == string::npos)
		{
			cout << contents.substr(pos) << flush;
			break;
		}

		cout << contents.substr(pos, open - pos);
		pos = open;

		HandleOpen(pos);

		if (pos >= contents.length())
			break;
	}
}

// Handle an open [[.
void Template::HandleOpen(stringpos& pos)
{
	// Find the next non-alpha-num-_ character.
	stringpos n = contents.find_first_not_of("abcdefghijklmnopqrstuvwxyz"
	                                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                                           "0123456789_+*", pos + 2);

	if (n == string::npos)
	{
		pos = string::npos;
		cout << "Error: Missing ]]" << endl;
		return;
	}

	if (isspace(contents[n]))
	{
		if (contents[pos + 2] == '*')
		{
			PrintRepeatedSection(pos);
		}
		else
		{
			// Normal section - work out whether to show it.
			bool show = false;
			if (contents[pos + 2] == '+')
			{
				if (sections.count(contents.substr(pos + 3, n - (pos + 3))) == 0)
				{
					show = true;
				}
				else
				{
					show = sections[contents.substr(pos + 3, n - (pos + 3))];
				}
			}
			else
			{
				if (sections.count(contents.substr(pos + 2, n - (pos + 2))) == 1)
				{
					show = sections[contents.substr(pos + 2, n - (pos + 2))];
				}
			}
			if (show)
			{
				PrintSection(pos);
			}
			else
			{
				SkipSection(pos);
			}
		}
	}
	else
	{
		// It is a variable.
		PrintVariable(pos);
	}
}

void Template::PrintSection(stringpos& pos)
{
	// Find the next [[ or ]].

	// TODO: Use isspace.
	pos = contents.find_first_not_of("abcdefghijklmnopqrstuvwxyz"
	                                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
												"0123456789_+*-.", pos + 2);
	stringpos start = pos;
	while (true)
	{
		pos = contents.find_first_of("[]", pos);
		if (pos == string::npos || pos + 1 >= contents.length())
		{
			pos = string::npos;
			cout << "Error: Missing ]]" << endl;
			return;
		}

		if (contents[pos + 1] == contents[pos])
		{
			// Print up to it.
			cout << contents.substr(start, pos - start) << flush;

			if (contents[pos] == '[')
			{
				HandleOpen(pos);
				start = pos;
			}
			else
			{
				pos += 2;
				return;
			}
		}
		else
		{
			++pos;
		}

		if (pos >= contents.length())
			break;
	}
}

// This starts, pointing to the first [ in the opening [[.
void Template::SkipSection(stringpos& pos)
{
	pos += 2;

	int levels = 1;
	while (levels > 0)
	{
		pos = contents.find_first_of("[]", pos);
		if (pos + 1 >= contents.length())
		{
			pos = string::npos;
			cout << "Error: Missing ]]" << endl;
			return;
		}
		if (contents[pos + 1] == contents[pos])
		{
			if (contents[pos] == '[')
				++levels;
			else
				--levels;
			pos += 2;
		}
		else
		{
			++pos;
		}
	}
}

// This starts, pointing to the first [ in the opening [[.
void Template::PrintVariable(stringpos& pos)
{
	// Find the ]].
	stringpos close = contents.find("]]", pos + 2);
	if (close == string::npos)
	{
		pos = string::npos;
		cout << "Error: Missing ]]" << endl;
		return;
	}

	cout << variables[contents.substr(pos + 2, close - (pos + 2))] << flush;
	pos = close + 2;
}

void Template::PrintRepeatedSection(stringpos& pos)
{
	// The first character of the actual normal section - normally a space.
	int begin = contents.find_first_not_of("abcdefghijklmnopqrstuvwxyz"
	                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                                       "0123456789_+*", pos + 2);

	// The section name.
	string name = contents.substr(pos + 3, begin - (pos + 3));

	if (repeated_sections.count(name) == 0 || repeated_sections[name] < 1)
	{
		SkipSection(pos);
		return;
	}

	// Repeat the number of times.
	for (int i = 0; i < repeated_sections[name]; ++i)
	{
		// Go to the start of the section.
		stringpos start = begin;
		pos = start;
		while (true)
		{
			// Find the next [[ or ]].
			pos = contents.find_first_of("[]", pos);
			if (pos + 1 >= contents.length())
			{
				pos = string::npos;
				cout << "Error: Missing ]]" << endl;
				return;
			}

			if (contents[pos + 1] == contents[pos])
			{
				// Print up to it.
				cout << contents.substr(start, pos - start) << flush;

				// See if it is an open [[.
				if (contents[pos] == '[')
				{
					if (pos + 2 > contents.length())
						return;
					// A repeated variable?
					if (contents[pos + 2] == '*')
						PrintRepeatedVariable(pos, i);
					else
						PrintVariable(pos);

					// Now start after the ]].
					start = pos;
				}
				else
				{
					// It is a close ]].
					pos += 2;
					goto end;
				}
			}
			else
			{
				++pos;
			}

			if (pos >= contents.length())
				return;
		}
	end:
		;
	}
}

void Template::PrintRepeatedVariable(stringpos& pos, int index)
{
	// Find the ]].
	stringpos close = contents.find("]]", pos + 2);
	if (close == string::npos)
	{
		pos = string::npos;
		cout << "Error: Missing ]]" << endl;
		return;
	}

	// +3 now to avoid the *
	if (repeated_variables[contents.substr(pos + 3, close - (pos + 3))].size() > index)
		cout << repeated_variables[contents.substr(pos + 3, close - (pos + 3))][index] << flush;
	pos = close + 2;
}
