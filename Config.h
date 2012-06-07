#pragma once

#include <string>

// Editing password. TODO: Store this hashed! (We may end up with a hash of a hash of a hash!)
const std::string CONFIG_PASSWORD = "asdf1234";

// How long the password is remembered for, in seconds.
const int CONFIG_AUTH_TIMEOUT = 60 * 60 * 24; // 1 day.

const std::string CONFIG_DOMAIN = "http://example.com/";
