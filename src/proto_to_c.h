//==-- proto_to_cxx.h - Protobuf-C++ conversion ----------------------------==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Defines functions for converting between protobufs and C++.
//
//===----------------------------------------------------------------------===//

#include <cstdint>
#include <cstddef>
#include <string>
#include "../lib/ccg/ccg.h"
#include "util.h"


namespace clang_fuzzer {
class MainFunction;
class LoopFunction;

//extern std::vector<int> retVec;
std::vector<int> MainFunctionToString(const MainFunction &main, unsigned seed_ccg);
//std::string FunctionToString(const Function1 &input);
//std::string ProtoToCxx(const uint8_t *data, size_t size);
std::string LoopFunctionToString(const LoopFunction &input);
//std::string LoopProtoToCxx(const uint8_t *data, size_t size);

}

//static void init(void);
//static void printProgram(void);

#define CCG_VERSION "0.3"

#define foreach(v, t) for((v = (t)); (v); (v = v->next))

#define INTEGERTYPE_SIZE(type) ((type == _i8 || type == _u8 ? 8 : (type == _i16 || type == _u16 ? 16 : (type == _i32 || type == _u32 ? 32 : 64))))

#define IS_UNSIGNED_INTEGERTYPE(type) (type == _u8 || type == _u16 || type == _u32 || type == _u64)

#define RANDOM_BITNESS (8 * pow(2, rand() % 3))

#define USABLE_ID(var) (var->type == _pointer ? maxDerefdPointer(var) : var->name)

extern CommandlineOpt cmdline;
extern Program program;
extern char const * const inttype2str[_inttypemax];
extern char const * const testop2str[_testopmax];
extern char const * const arithop2str[_arithopmax];
extern char const * const assignop2str[_assignopmax];
extern unsigned seed_ccg;

#ifdef __cplusplus
extern "C" {
#endif
#include "../lib/ccg/ccg.h"
#ifdef __cplusplus
};
#endif






