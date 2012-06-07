#pragma once

#include "cgi/cgi.h"

// Authorisation is performed entirely client-side.
// The client calculates an authentication cookie in javascript, and when they load any
// page on the site the cookie is sent, and checked for validity and non-expiration.
// To log out, the client simply deletes its cookie!
//
// The cookie contains the following:
//
//  <seconds since epoch>, <user IP>, <a hash of [seconds since epoch, user ip, the password]>
//
// If this cookie is stolen it will (probably) not work due to the IP being wrong.
// It it is used after too long it will not work due to the time being wrong.
//
// No need for a salt since the time and IP change all the time.
//
// The current time, and user IP are sent to the client in every page with a log-in box.
//
// Also, 'the password' above can be actually the *hash* of a password, so that you can use a password you normally
// use. This doesn't really increase security much though, it merely prevents Eve finding out your *actual* password (i.e.
// the one you typed into the browser). She can still hack in to the website though if she finds the hash.

// TODO: Need a way to "refresh" the cookie so you are logged out N seconds after your last use,
// rather than N seconds after you logged in.

// So all IsAuthorised has to do is look for the cookie, check the time isn't too old, and the IP and hash are correct.
// We will also check the referer, just in case someone tries a CSRF.
bool IsAuthorised(CookieType& C);

