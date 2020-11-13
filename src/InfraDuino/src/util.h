#include <Arduino.h>

boolean isStringDigit(const String & s)
{
    for (char c : s) if (!isDigit(c)) return (false);
    return (true);
}