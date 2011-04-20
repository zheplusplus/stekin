#ifndef __STEKIN_MISCELLANY_POSITION_TYPE_H__
#define __STEKIN_MISCELLANY_POSITION_TYPE_H__

#include <string>

namespace misc {

    struct pos_type {
        int const line;

        explicit pos_type(int l)
            : line(l)
        {}

        std::string str() const;

        bool operator==(pos_type const& rhs) const;
    };

}

#endif /* __STEKIN_MISCELLANY_POSITION_TYPE_H__ */
