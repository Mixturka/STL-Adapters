#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "../lib/Adapters.hpp"

int main() {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  for (std::pair<std::string, int> i : v) std::cout << i.second << ' ';
  auto b = v | TransformClass([](int i) { return i * 2; });

  for (int i : b) std::cout << i << ' ';
}