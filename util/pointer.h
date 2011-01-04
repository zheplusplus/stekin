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

        template <typename _ConvertableType>
        sptr(sptr<_ConvertableType>&& rhs)
            : base_type(std::move(rhs))
        {}

        template <typename _ConvertableType>
        sptr& operator=(sptr<_ConvertableType>&& rhs)
        {
            base_type::operator=(std::move(rhs));
            return *this;
        }

        pointer get() const = delete;
    };

    template <typename _RawType>
    sptr<_RawType> mkmptr(_RawType* ptr)
    {
        return sptr<_RawType>(ptr);
    }

    template <typename _RawType>
    sptr<_RawType const> mkptr(_RawType* ptr)
    {
        return sptr<_RawType const>(ptr);
    }

}

#endif /* __STACKENING_UTILITY_POINTER_H__ */
