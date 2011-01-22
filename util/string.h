#ifndef __STACKENING_UTILITY_STRING_H__
#define __STACKENING_UTILITY_STRING_H__

#include <string>

namespace util {

    std::string replace_all(std::string src, std::string const& origin_text, std::string const& replacement);

    std::string str(int i);
    std::string str(double d);

}

#endif /* __STACKENING_UTILITY_STRING_H__ */
