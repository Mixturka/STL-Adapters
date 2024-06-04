#pragma once
#include <vector>
#include <iostream>

struct map_tag {};
struct container_tag {};

template <typename T, typename = void>
struct is_container : std::false_type {};

template <typename T>
struct is_container<T, std::void_t<typename T::iterator,
                              typename T::const_iterator>> : std::true_type {};

template <typename T, typename = void>
struct is_associative_container : std::false_type {};

template <typename T>
struct is_associative_container<T, std::void_t<typename T::reverse_iterator>>
    : std::true_type {};

template <typename T, typename = void>
struct is_map_container : std::false_type {};

template <typename T>
struct is_map_container<T, std::void_t<typename T::key_type, typename T::mapped_type>> : std::true_type {};

template <typename T, typename = void>
struct is_set_container : std::false_type {};

template <typename T>
struct is_set_container<T, std::void_t<typename T::value_type, typename T::key_type>> : std::true_type {};

template <typename T>
struct is_pair_with_const_first : std::false_type {};

template <typename T, typename U>
struct is_pair_with_const_first<std::pair<const T, U>> : std::true_type {};

template <typename InIter, typename OutIter, typename Comparator>
OutIter Transform(InIter start, InIter end, OutIter start_r, Comparator comp, container_tag) {
  while (start != end) {
    *start_r++ = comp(*start++);
  }

  return start_r;
}

template <typename InIter, typename OutIter, typename Comparator>
OutIter Transform(InIter start, InIter end, OutIter start_r, Comparator comp, map_tag) {
  while (start != end) {
    *start_r++ = comp((start->second));
    ++start;
  }

  return start_r;
}

template <typename Predicate>
class TransformClass {
public:
  TransformClass(Predicate func) : func_(func) {}

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<!is_map_container<Container>::value, std::vector<typename Container::value_type>> {
    std::vector<typename Container::value_type> res;
    Transform(container.begin(), container.end(), std::inserter(res, res.begin()), this->func_, container_tag{});
    return res;
  }

  template <typename MapContainer>
  auto operator()(const MapContainer& container) -> std::enable_if_t<is_map_container<MapContainer>::value, std::vector<typename MapContainer::value_type::second_type>> {
    std::vector<typename MapContainer::value_type::second_type> res;
    Transform(container.begin(), container.end(), std::inserter(res, res.begin()), this->func_, map_tag{});
    return res;
  }

private:
  Predicate func_;
};

template <typename Container, typename Predicate> 
auto operator|(Container& container, TransformClass<Predicate>&& transformer) {
  static_assert(is_container<Container>::value, "Type must be a container");
  return transformer(container);
}

template <typename InIter, typename OutIter, typename Comparator>
OutIter Filter(InIter start, InIter end, OutIter start_r, Comparator comp, container_tag) {
  while (start != end) {
    if (comp(*start)) {
      *start_r++ = *start;
    }
    ++start;
  }

  return start_r;
}

template <typename InIter, typename OutIter, typename Comparator>
OutIter Filter(InIter start, InIter end, OutIter start_r, Comparator comp, map_tag) {
  while (start != end) {
    if (comp(start->second)) {
      *start_r++ = start->second;
    }
    ++start;
  }

  return start_r;
}

template <typename Predicate>
class FilterClass {
public:
  FilterClass(Predicate func) : func_(func) {}

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<!is_map_container<Container>::value, std::vector<typename Container::value_type>> {
    std::vector<typename Container::value_type> res;
    Filter(container.begin(), container.end(), std::inserter(res, res.begin()), this->func_, container_tag{});
    return res;
  }

  template <typename MapContainer>
  auto operator()(const MapContainer& container) -> std::enable_if_t<is_map_container<MapContainer>::value, std::vector<typename MapContainer::value_type::second_type>> {
    std::vector<typename MapContainer::value_type::second_type> res;
    Filter(container.begin(), container.end(), std::inserter(res, res.begin()), this->func_, map_tag{});
    return res;
  }

private:
  Predicate func_;
};

template <typename Container, typename Predicate> 
auto operator|(Container& container, FilterClass<Predicate>&& filter) {
  static_assert(is_container<Container>::value, "Type must be a container");
  return filter(container);
}

template <typename InIter, typename OutIter>
OutIter Take(InIter start, InIter end, OutIter start_r, uint64_t num, container_tag) {
  uint64_t counter = 0;
  while (start != end && counter < num) {
    *start_r++ = *start++;
    ++counter;
  }

  return start_r;
}

template <typename InIter, typename OutIter>
OutIter Take(InIter start, InIter end, OutIter start_r, uint64_t num, map_tag) {
  uint64_t counter = 0;
  while (start != end && counter < num) {
    *start_r++ = start->second;
    ++start;
    ++counter;
  }

  return start_r;
}

class TakeClass {
public:
  TakeClass(uint64_t count) : count_(count) {}

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<!is_map_container<Container>::value, std::vector<typename Container::value_type>> {
    std::vector<typename Container::value_type> res;
    Take(container.begin(), container.end(), std::inserter(res, res.begin()), this->count_, container_tag{});
    return res;
  }

  template <typename MapContainer>
  auto operator()(const MapContainer& container) -> std::enable_if_t<is_map_container<MapContainer>::value, std::vector<typename MapContainer::value_type::second_type>> {
    std::vector<typename MapContainer::value_type::second_type> res;
    Take(container.begin(), container.end(), std::inserter(res, res.begin()), this->count_, map_tag{});
    return res;
  }

private:
  uint64_t count_;
};

template <typename Container> 
auto operator|(Container& container, TakeClass&& taker) {
  static_assert(is_container<Container>::value, "Type must be a container");
  return taker(container);
}

template <typename InIter, typename OutIter>
OutIter Drop(InIter start, InIter end, OutIter start_r, uint64_t num, container_tag) {
  uint64_t counter = 0;
  while (start != end) {
    if (counter >= num) {
      *start_r++ = *start;
    }
    ++start;
    ++counter;
  }

  return start_r;
}

template <typename InIter, typename OutIter>
OutIter Drop(InIter start, InIter end, OutIter start_r, uint64_t num, map_tag) {
  uint64_t counter = 0;
  while (start != end) {
    if (counter >= num) {
      *start_r++ = start->second;
    }

    ++start;
    ++counter;
  }

  return start_r;
}

class DropClass {
public:
  DropClass(uint64_t count) : count_(count) {}

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<!is_map_container<Container>::value, std::vector<typename Container::value_type>> {
    std::vector<typename Container::value_type> res;
    Drop(container.begin(), container.end(), std::inserter(res, res.begin()), this->count_, container_tag{});
    return res;
  }

  template <typename MapContainer>
  auto operator()(const MapContainer& container) -> std::enable_if_t<is_map_container<MapContainer>::value, std::vector<typename MapContainer::value_type::second_type>> {
    std::vector<typename MapContainer::value_type::second_type> res;
    Drop(container.begin(), container.end(), std::inserter(res, res.begin()), this->count_, map_tag{});
    return res;
  }

private:
  uint64_t count_;
};

template <typename Container> 
auto operator|(Container& container, DropClass&& taker) {
  static_assert(is_container<Container>::value, "Type must be a container");
  return taker(container);
}

template <typename InIter, typename OutIter>
OutIter Reverse(InIter start, InIter end, OutIter start_r, container_tag) {
  while (start != end) {
    *start_r++ = *--end;
  }

  return start_r;
}

template <typename InIter, typename OutIter>
OutIter Reverse(InIter start, InIter end, OutIter start_r, map_tag) {
  while (start != end) {
    --end;
    *start_r++ = end->second;
  }

  return start_r;
}

class ReverseClass {
public:
  ReverseClass() = default;

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<!is_map_container<Container>::value, std::vector<typename Container::value_type>> {
    std::vector<typename Container::value_type> res;
    Reverse(container.begin(), container.end(), std::inserter(res, res.begin()), container_tag{});
    return res;
  }

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<is_map_container<Container>::value, std::vector<typename Container::value_type::second_type>> {
    std::vector<typename Container::value_type::second_type> res;
    Reverse(container.begin(), container.end(), std::inserter(res, res.begin()), map_tag{});
    return res;
  }
};

template <typename Container> 
auto operator|(Container& container, ReverseClass&& reverser) {
  static_assert(is_container<Container>::value, "Type must be a container");
  static_assert(is_associative_container<Container>::value, "Type must have reverse iterator");
  return reverser(container);
}

template <typename InIter, typename OutIter>
OutIter GetKeys(InIter start, InIter end, OutIter start_r, map_tag) {
  while (start != end) {
    *start_r++ = start->first;
    ++start;
  }

  return start_r;
}

template <typename InIter, typename OutIter>
OutIter GetKeys(InIter start, InIter end, OutIter start_r, container_tag) {
  while (start != end) {
    *start_r++ = *start++;
  }

  return start_r;
}

class KeyClass {
public:
  KeyClass() = default;

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<is_map_container<Container>::value, std::vector<typename Container::key_type>> {
    std::vector<typename Container::key_type> res;
    res.resize(container.size());
    GetKeys(container.begin(), container.end(), res.begin(), map_tag{});
    return res;
  }

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<is_set_container<Container>::value && !is_map_container<Container>::value, std::vector<typename Container::key_type>> {
    std::vector<typename Container::value_type> res;
    GetKeys(container.begin(), container.end(), std::inserter(res, res.begin()), container_tag{});
    return res;
  }
};

template <typename Container> 
auto operator|(Container& container, KeyClass&& keyser) {
  static_assert(is_container<Container>::value, "Type must be a container");
  return keyser(container);
}

template <typename InIter, typename OutIter>
OutIter GetValues(InIter start, InIter end, OutIter start_r, map_tag) {
  while (start != end) {
    *start_r++ = start->second;
    ++start;
  }

  return start_r;
}

template <typename InIter, typename OutIter>
OutIter GetValues(InIter start, InIter end, OutIter start_r, container_tag) {
  while (start != end) {
    *start_r++ = *start++;
  }

  return start_r;
}

class ValueClass {
public:
  ValueClass() = default;

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<is_map_container<Container>::value, std::vector<typename Container::value_type::second_type>> {
    std::vector<typename Container::value_type::second_type> res;
    GetValues(container.begin(), container.end(), std::inserter(res, res.begin()), map_tag{});
    return res;
  }

  template <typename Container>
  auto operator()(const Container& container) -> std::enable_if_t<!is_map_container<Container>::value && is_set_container<Container>::value, std::vector<typename Container::key_type>> {
    std::vector<typename Container::value_type> res;
    GetValues(container.begin(), container.end(), std::inserter(res, res.begin()), container_tag{});
    return res;
  }
};

template <typename Container> 
auto operator|(Container& container, ValueClass&& valueser) {
  static_assert(is_container<Container>::value, "Type must be a container");
  return valueser(container);
}
