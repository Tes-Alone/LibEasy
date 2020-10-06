#ifndef LEC_ASMLIB_H_
#define LEC_ASMLIB_H_

#include "BaseTypes.hpp"

using namespace Easy;

extern "C" e_ulong asm_hex_double(e_double d);
extern "C" e_uint  asm_hex_float(e_float f);
extern "C" e_double asm_round0(e_double d);

#endif // LEC_ASNLIB_H_