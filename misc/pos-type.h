#ifndef __STACKENING_MISC_POSITION_TYPE_H__
#define __STACKENING_MISC_POSITION_TYPE_H__

namespace misc {

    struct pos_type {
        int const line;

        explicit pos_type(int ln)
            : line(ln)
        {}

        bool operator==(pos_type const& rhs) const
        {
            return line == rhs.line;
        }
    };

}

template <typename _OS>
_OS& operator<<(_OS& os, misc::pos_type const& pos)
{
    os << "Line: " << pos.line;
    return os;
}

#endif /* __STACKENING_MISC_POSITION_TYPE_H__ */
