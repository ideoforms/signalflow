#pragma once

#include <string>

void signal_init();
void signal_debug(char const *msg, ...);
void signal_warn(char const *msg, ...);
void signal_assert(bool equality, char const *msg, ...);
