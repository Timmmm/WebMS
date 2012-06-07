#include "Page.h"

#include <sstream>
#include <fstream>

Error Page::load(string name)
{
	// TODO: Use database to avoid all kinds of race condition type things.
	// Not that they will really be a problem.
	string mkdName = "mkds/" + Securify(name, Alpha + Num + Space + Underscore) + ".mkd";
	string htmlName = "mkds/" + Securify(name, Alpha + Num + Space + Underscore) + ".html";

	{
		stringstream ss;
		// Try to load the file.
		ifstream is(mkdName.c_str());
		if (is)
		{
			ss << is.rdbuf();
		}
		markdown = ss.str();
		if (markdown.empty())
			return Error("Non-existant or empty page.");
	}

	{
		stringstream ss;
		// Try to load the file.
		ifstream is(htmlName.c_str());
		if (is)
		{
			ss << is.rdbuf();
		}
		html = ss.str();
//		if (html.empty())
//			return Error("Non-existant or empty page.");
	}

	return Success;
}

string Page::markdownContent() const
{
	return markdown;
}
string Page::htmlContent() const
{
	return html;
}

void Page::setMarkdownContent(string s)
{
	markdown = s;
}

void Page::setHtmlContent(string s)
{
	html = s;
}


Error Page::save(string name)
{
	string mkdName = "mkds/" + Securify(name, Alpha + Num + Space + Underscore) + ".mkd";
	string htmlName = "mkds/" + Securify(name, Alpha + Num + Space + Underscore) + ".html";

	// Try to save the file.
	{
		ofstream os(mkdName.c_str());
		if (!os)
			return Error("Couldn't open file for writing.");
		os << markdown;
	}
	{
		ofstream os(htmlName.c_str());
		if (!os)
			return Error("Couldn't open file for writing.");
		os << html;
	}
	return Success;
}
