#include "Auth.h"

#include "common/common.h"

#include "EpochTime.h"
#include "Sha1.h"

#include "Config.h"

// So all IsAuthorised has to do is look for the cookie, check the time isn't too old, and the IP and hash are correct.
bool IsAuthorised(CookieType& C)
{
	string ip = GetEnv("REMOTE_ADDR");
	string referer = GetEnv("HTTP_REFERER");
	string cookie = C["auth"];

	// Check referer to prevent CSRF. It would be better to use a unique in-page code, but whatever.
	if (referer.length() < CONFIG_DOMAIN.length()
			|| referer.substr(0, CONFIG_DOMAIN.length()) != CONFIG_DOMAIN)
		return false;

	// Check cookie.
	vector<string> parts = Split(cookie, ",");
	if (parts.size() != 3)
		return false;

	// Check IP.
	if (parts[0] != ip)
		return false;

	// Time.
	unsigned long long t = StoULL(parts[1], 0);

	// Check time.
	if (t + CONFIG_AUTH_TIMEOUT < CurrentEpochTime())
		return false;

	// Hash previous two with password.
	string toHash = parts[0] + "," + ULLtoS(t) + "," + CONFIG_PASSWORD;

	string hashed = Hash(toHash);

	if (parts[2] != hashed)
		return false;

	return true;
}
