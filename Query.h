#pragma once

#include <cgi/cgi.h>
#include <cgi/template.h>

#include <string>

using namespace std;

// Print an error page.
void PrintError(string S);

void ShowPage(FormType& F, CookieType& C);
void SavePage(FormType& F, CookieType& C);
//void UploadFile(FormType& F, CookieType& C);
