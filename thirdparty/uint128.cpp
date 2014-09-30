#include "uint128.h"

std::ostream &operator<<(std::ostream &o, const uint128 &n)
{
	switch(o.flags() & (std::ios_base::hex | std::ios_base::dec | std::ios_base::oct)) {
	case std::ios_base::hex:
		o << n.to_string(16);
		break;
	case std::ios_base::dec:
		o << n.to_string(10);
		break;
	case std::ios_base::oct:
		o << n.to_string(8);
		break;
	default:
		o << n.to_string();
		break;
	}
	return o;
}