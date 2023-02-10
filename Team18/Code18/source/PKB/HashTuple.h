#pragma once

#include "ParsedQuery.h"
#include "PKBQueryArg.h"

#include <tuple>

namespace spa {
template <class T>
inline void hash_combine(std::size_t& seed, T const& v) {
  seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct TupleHash {
  std::size_t operator()(std::tuple<RelationshipType, PKBQueryArgType,
                                    PKBQueryArgType> const& t) const noexcept {
    std::size_t result = 0;
    hash_combine<std::size_t>(result, static_cast<size_t>(std::get<0>(t)));
    hash_combine<std::size_t>(result, static_cast<size_t>(std::get<1>(t)));
    hash_combine<std::size_t>(result, static_cast<size_t>(std::get<2>(t)));
    return result;  // or use boost::hash_combine
  }
};

struct TupleEquality {
  bool operator()(const std::tuple<RelationshipType, PKBQueryArgType, PKBQueryArgType>& t1,
                  const std::tuple<RelationshipType, PKBQueryArgType, PKBQueryArgType>& t2) const {
    bool relEq = std::get<0>(t1) == std::get<0>(t2);
    bool arg1Eq = std::get<1>(t1) == std::get<1>(t2);
    bool arg2Eq = std::get<2>(t1) == std::get<2>(t2);
    return relEq && arg1Eq && arg2Eq;  // or use boost::hash_combine
  }
};
}  // namespace spa
