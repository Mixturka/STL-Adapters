# STL-Adapters

## Overview

Adapters library that simplifies algorithms for STL containers. Library provides an elegant and simple way to apply multiple algorithms one by one to same container.

## Adapter list

- ```transform```: changes container elements according to provided function
- ```filter```: filtering container by attribute, attribute passed as argument
- ```take```: takes first N elements of container
- ```drop```: skips first N elements of container
- ```reverse```: reverses container
- ```keys```: returns keys for associative containers
- ```values```: returns values for associative containers

## Usage example

```cpp
std::vector<int> v = {1, 2, 3, 4, 5, 6};

for(int i : v | filter([](int i){ return i % 2; }) | transform([](int i){ return i * i; }))
     std::cout << i << " ";
```
