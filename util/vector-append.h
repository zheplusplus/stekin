#ifndef __STEKIN_UTILITY_VECTOR_APPEND_H__
#define __STEKIN_UTILITY_VECTOR_APPEND_H__

#include <vector>
#include <algorithm>

#include "pointer.h"

namespace util {

    template <typename _RawType>
    void ptrs_append(std::vector<sptr<_RawType>>& former, std::vector<sptr<_RawType>>&& latter)
    {
        std::for_each(latter.begin()
                    , latter.end()
                    , [&](sptr<_RawType>& p)
                      {
                          former.push_back(std::move(p));
                      });
    }

    template <typename _RawType>
    std::vector<sptr<_RawType>> ptrs_append(std::vector<sptr<_RawType>>&& former
                                          , std::vector<sptr<_RawType>>&& latter)
    {
        std::for_each(latter.begin()
                    , latter.end()
                    , [&](sptr<_RawType>& p)
                      {
                          former.push_back(std::move(p));
                      });
        return std::move(former);
    }

    template <typename _RawType>
    std::vector<sref<_RawType>> refs_append(std::vector<sref<_RawType>> former
                                          , std::vector<sref<_RawType>> latter)
    {
        std::for_each(latter.begin()
                    , latter.end()
                    , [&](sref<_RawType> const& p)
                      {
                          former.push_back(p);
                      });
        return former;
    }

}

#endif /* __STEKIN_UTILITY_VECTOR_APPEND_H__ */
