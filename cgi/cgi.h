#pragma once

#include <string>
#include <map>

using namespace std;

/**********************************
 Functions for getting form data.
 **********************************/

// Form data - key, value.
typedef map<string, string> FormType;

// Fill 'Fill' with the form data. Return true if success.
bool ParseFormData(FormType& Fill);

/**********************************
 Functions for getting cookie data.
 **********************************/

// Cookie data - key, value.
typedef map<string, string> CookieType;

// Fill 'Fill' with the cookie data.
bool ParseCookieData(CookieType& Fill);

/**********************************
 URL manipulation functions.
 **********************************/

// Encode a string to a URL. It is very conservative, and replaces all non-alphanumeric
// characters with their %xx code.
string Encode(const string& S);
