#include "test-common.h"
#include "../../misc/platform.h"

inst::type const test::WORD("word", platform::WORD_LENGTH_INBYTE);
inst::type const test::DWORD("dword", platform::WORD_LENGTH_INBYTE * 2);
inst::type const test::HALFWORD("halfword", platform::WORD_LENGTH_INBYTE / 2);