#ifndef __STACKENING_UTILITY_POINTER_H__
#define __STACKENING_UTILITY_POINTER_H__

#include <memory>

namespace util {

    template <typename _RawType>
    struct sptr
        : public std::unique_ptr<_RawType>
    {
        typedef std::unique_ptr<_RawType> base_type;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::deleter_type deleter_type;

        sptr() = default;

        explicit sptr(pointer p)
            : base_type(p)
        {}

        sptr(sptr&& rhs) 
            : base_type(std::move(rhs))
        {}

        sptr& operator=(sptr&& rhs) = delete;
        pointer get() const = delete;
    };

}

#endif /* __STACKENING_UTILITY_POINTER_H__ */
