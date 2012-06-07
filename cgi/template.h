#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#include "common/common.h"

/**********************************
HTML template class.
 **********************************/

class Template
{
public:
	// Blank file.
	Template();
	// Load a file.
	Template(const string& file);

	// Load a file, returns success.
	bool Load(const string& file);

	// Show a section.
	// All sections start hidden unless they are written thus: [[+sectionname  ...
	void ShowSection(const string& section);
	// Hide a section.
	void HideSection(const string& section);

	// Set a variable, all default to "".
	void SetVar(const string& key, const string& value);

	// Add elements of a variable for repeated sections.
	void AppendVar(const string& repeated_key, const string& value);

	// Set the number of times a section is repeated.
	void SetRepeats(const string& repeated_section, int number);
	
	// Add an http header. Content-type is there already by default. This is mainly for cookies.
	void AddHeader(const string& header);

	// Print the page. If printHeaders is true, print HTTP headers.
	void Print(bool printHeaders = false);

private:

	// Helper functions.
	void HandleOpen(stringpos& pos);
	void PrintSection(stringpos& pos);
	void SkipSection(stringpos& pos);
	void PrintVariable(stringpos& pos);

	void PrintRepeatedSection(stringpos& pos);
	void PrintRepeatedVariable(stringpos& pos, int index);

	string contents;
	map<string, bool> sections;
	map<string, string> variables;
	map<string, int> repeated_sections;
	map<string, vector<string> > repeated_variables;
	vector<string> headers;
};

/**

[[section_name Section Content.]] - Show/hide section.
[[+initially_visible_section_name Section Content.]] - Same but initally visible.
[[*repeated_section_name Section Content.]] - Section that is repeated. Default count is 0.
[[single_value_variable]] - must not be surrounded by space (e.g. [[var ]])
[[*repeated_value_variable]]

Repeated sections cannot contain other repeated sections. If a variable is not set it
defaults to "".

Names must be unique, and [a-zA-Z_0-9].

Example template:

<html>
<head>
[[title <title>Simple example</title>]]
</head>
<body>
<h1>[[page_title]]</h1>
[[+table
<table>
[[*table_row <tr><td>[[*author]]</td><td>[[*book_title]]</td></td>]]
</table>
]]
</body>
</html>

And some code that uses it:

HTML t("index.template.html");

t.ShowSection("title");
t.SetSectionRepeats("table", 5);
t.AppendRepeatedVariable("author", "Jimmy Jim");
t.AppendRepeatedVariable("author", "Johnny");
t.AppendRepeatedVariable("author", "Davey Dave");
t.AppendRepeatedVariable("author", "Big Mike");
t.AppendRepeatedVariable("author", "Most Excellent Ted");
t.AppendRepeatedVariable("book_title", "101");
t.AppendRepeatedVariable("book_title", "Stuff");
t.AppendRepeatedVariable("book_title", "Lots of things");
t.AppendRepeatedVariable("book_title", "How to eat");
t.AppendRepeatedVariable("book_title", "How not to eat");
t.SetVariable("page_title", "A list");

t.Print(true);

Note that repeated sections cannot contain other repeated sections. If they contain
normal sections, then those are hidden/shown the same for every repetition.

*/
