#pragma once

#include "common/common.h"

#include <string>

using namespace std;

// This class represents a single page. It is loaded from a file on disk which stores
// both the HTML and corresponding Markdown code.
class Page
{
public:
	// Load a file. Name is the name of the page, e.g. "Android Apps", and is secured in this function.
	Error load(string name);

	string markdownContent() const;
	string htmlContent() const;

	void setMarkdownContent(string s);
	void setHtmlContent(string s);

	Error save(string name);
private:
	string markdown;
	string html;
};
