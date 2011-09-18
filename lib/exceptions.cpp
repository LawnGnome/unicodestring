#include "ustring.h"

ICUError::ICUError(const std::ostringstream &ss) : std::runtime_error(ss.str()) {}
