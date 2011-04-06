#ifndef __STACKENING_TEST_COMMON_H__
#define __STACKENING_TEST_COMMON_H__

#include <ostream>

#include "../misc/pos-type.h"

namespace test {

    std::ostream& operator<<(std::ostream& os, misc::pos_type const& pos); 

}

#endif /* __STACKENING_TEST_COMMON_H__ */
