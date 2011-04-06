#ifndef __STACKENING_UTILITY_POINTER_H__
#define __STACKENING_UTILITY_POINTER_H__

#include <memory>
#include <string>

namespace util {

    struct id {
        explicit id(void const* i)
            : _id(i)
        {}

        std::string str() const;

        bool operator<(id const& rhs) const;
        bool operator==(id const& rhs) const;
        bool operator!=(id const& rhs) const;
    private:
        void const* const _id;
    };

    template <typename _RawType>
    struct sref {
        typedef typename std::unique_ptr<_RawType>::pointer pointer;

        explicit sref(pointer ptr)
            : _ptr(ptr)
        {}

        template <typename _ConvertableType>
        sref(sref<_ConvertableType> rhs)
            : _ptr(rhs.convert<_RawType>()._ptr)
        {}

        template <typename _ConvertableType>
        sref operator=(sref<_ConvertableType> rhs)
        {
            _ptr = rhs.convert<_RawType>()._ptr;
            return *this;
        }

        template <typename _TargetType>
        sref<_TargetType> convert() const
        {
            return sref<_TargetType>(_ptr);
        }

        bool operator==(sref rhs) const
        {
            return *_ptr == *rhs._ptr;
        }

        bool operator!=(sref rhs) const
        {
            return *_ptr != *rhs._ptr;
        }

        bool operator<(sref rhs) const
        {
            return *_ptr < *rhs._ptr;
        }

        explicit operator bool() const
        {
            return NULL != _ptr;
        }

        operator _RawType() const
        {
            return *_ptr;
        }

        pointer operator->() const
        {
            return _ptr;
        }

        util::id id() const
        {
            return util::id(_ptr);
        }

        _RawType cp() const
        {
            return *_ptr;
        }

        template <typename _OS>
        _OS& print(_OS& os) const
        {
            os << *_ptr;
            return os;
        }
    private:
        pointer _ptr;
    };

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

        sref<_RawType> operator*() const
        {
            return sref<_RawType>(base_type::get());
        }

        util::id id() const
        {
            return util::id(base_type::get());
        }

        std::string str() const
        {
            return id().str();
        }

        _RawType cp() const
        {
            return *base_type::get();
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

    template <typename _RawType>
    sref<_RawType> mkref(_RawType& obj)
    {
        return sref<_RawType>(&obj);
    }

}

template <typename _OS, typename _RawType>
_OS& operator<<(_OS& os, util::sref<_RawType> ref)
{
    return ref.print(os);
}

#endif /* __STACKENING_UTILITY_POINTER_H__ */
