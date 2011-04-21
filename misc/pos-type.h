#ifndef __STEKIN_MISCELLANY_POSITION_TYPE_H__
#define __STEKIN_MISCELLANY_POSITION_TYPE_H__

#include <string>

namespace misc {

    struct position {
        int const line;

        explicit position(int l)
            : line(l)
        {}

        std::string str() const;

        bool operator==(position const& rhs) const;
    };

}

#endif /* __STEKIN_MISCELLANY_POSITION_TYPE_H__ */
