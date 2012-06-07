#include <cgi/cgi.h>
#include <common/common.h>

#include "Query.h"
#include "Sha1.h"

void Debug(const FormType& F, const CookieType& C)
{
	cout << "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	cout << "<html><body>";

	cout << "<h1>Form Data</h1><table><tbody>";
	for (FormType::const_iterator i = F.begin(); i != F.end(); ++i)
	{
		cout << "<tr><td>" << i->first << "</td><td>" << i->second << "</td><tr>";
	}
	cout << "</tbody></table>";

	cout << "<h1>Cookie Data</h1><table><tbody>";
	for (FormType::const_iterator i = C.begin(); i != C.end(); ++i)
	{
		cout << "<tr><td>" << i->first << "</td><td>" << i->second << "</td><tr>";
	}
	cout << "</tbody></table>";

	cout << "</body></html>\r\n";
}

int main(int argc, char *argv[])
{
	// Parse the form data.
	FormType F;
	if (!ParseFormData(F))
	{
		PrintError("Failed to parse form data.");
		return 1;
	}

	CookieType C;
	if (!ParseCookieData(C))
	{
		PrintError("Failed to parse cookie data.");
		return 2;
	}

	if (F["method"] == "save") // Save
	{
		SavePage(F, C);
	}
	else
	{
		// View
		ShowPage(F, C);
	}
	return 0;
}
