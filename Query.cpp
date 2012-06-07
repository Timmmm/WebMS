#include "Query.h"
#include "Page.h"

#include "common/common.h"

#include "Auth.h"
#include "Base64.h"

#include "EpochTime.h"

// Print an error page.
void PrintError(string S)
{
	cout << "Content-Type: text/html\r\n\r\n<html><body>" << HTMLEscape(S) << "</body></html>" << endl;
}

void ShowPage(FormType& F, CookieType& C)
{
	string pageName = F["page"];
	if (pageName.empty())
		pageName = "Index"; // Defualt page.

	Page pageBody;
	if (!pageBody.load(pageName))
	{
		// Show a page not found thing with a pi.
		pageBody.setHtmlContent("Page not found.");
	}

	// TODO: Allow editing this template in the web interface! Maybe.
	Template page;
	if (!page.Load("resources/template.html"))
	{
		PrintError("Couldn't load page template.");
		return;
	}

	page.SetVar("content", pageBody.htmlContent());

	// For updating the auth cookie.
	string ip = GetEnv("REMOTE_ADDR");
	page.SetVar("ip_address", ip);
	page.SetVar("epoch_time", ULLtoS(CurrentEpochTime()));

	if (IsAuthorised(C))
	{
		page.ShowSection("authorised");
		page.SetVar("page_name", pageName);
		page.SetVar("base64markdown", EncodeBase64(pageBody.markdownContent()));
	}
	else
	{
		// Show 'pi' button. When clicked it shows login form.
		// The login form has a hidden variable that remembers that we are going to this page when
		// we have succesfully logged in.
		page.ShowSection("unauthorised");
	}

	page.Print(true);
}


void SavePage(FormType& F, CookieType& C)
{
	string pageName = F["page"];
	if (pageName.empty())
	{
		PrintError("No page specified.");
		return;
	}

	if (IsAuthorised(C))
	{
		string mkdContent = F["mkd"];
		string htmlContent = F["html"];

		Page pageBody;
		pageBody.setMarkdownContent(mkdContent);
		pageBody.setHtmlContent(htmlContent);
		Error e = pageBody.save(pageName);
		if (!e)
		{
			PrintError("Couldn't save page: " + e.message());
			return;
		}
	}
	ShowPage(F, C);
}

//void UploadFile(FormType& F, CookieType& C)
//{
//
//}
