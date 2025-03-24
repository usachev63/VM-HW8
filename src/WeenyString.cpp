#include "weenystring/WeenyString.h"

using namespace weenystring;

const char WeenyString::emptyString[] = "";

void WeenyString::dump() const {
  std::cerr << "WeenyString " << cstr();
  if (isUniqueRef())
    std::cerr << " UNIQUE link";
  else
    std::cerr << " ONE OF MANY links";
  std::cerr << " to addr " << (void *)cstr();
  std::cerr << std::endl;
}
