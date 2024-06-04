#include "../lib/Adapters.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <set>
#include <map>

TEST(TransformTest, NotMapContainer1) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto b = v | TransformClass([](int i) { return i * 2; });
  std::vector<int> res = {2, 4, 6, 8, 10};

  ASSERT_EQ(b, res);
}

TEST(TransformTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | TransformClass([](int i) { return i * 2; });
  std::vector<int> res = {2, 4, 6, 8, 10};

  ASSERT_EQ(b, res);
}

TEST(TransformTest, MapContainer) {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  auto b = v | TransformClass([](int i) { return i * 2; });
  std::vector<int> res = {10, 0, 6, 2, 4};

  ASSERT_EQ(b, res);
}

TEST(FilterTest, NotMapContainer1) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto b = v | FilterClass([](int i) { return i % 2 == 0; });
  std::vector<int> res = {2, 4};

  ASSERT_EQ(b, res);
}

TEST(FilterTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | FilterClass([](int i) { return i % 2 == 0; });
  std::vector<int> res = {2, 4};

  ASSERT_EQ(b, res);
}

TEST(FilterTest, MapContainer) {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  auto b = v | FilterClass([](int i) { return i % 2 == 0; });
  std::vector<int> res = {0, 2};

  ASSERT_EQ(b, res);
}

TEST(TakeTest, NotMapContainer1) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto b = v | TakeClass(3);
  std::vector<int> res = {1, 2, 3};

  ASSERT_EQ(b, res);
}

TEST(TakeTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | TakeClass(3);
  std::vector<int> res = {1, 2, 3};

  ASSERT_EQ(b, res);
}

TEST(TakeTest, MapContainer) {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  auto b = v | TakeClass(3);
  std::vector<int> res = {5, 0, 3};

  ASSERT_EQ(b, res);
}

TEST(DropTest, NotMapContainer1) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto b = v | DropClass(3);
  std::vector<int> res = {4, 5};

  ASSERT_EQ(b, res);
}

TEST(DropTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | DropClass(3);
  std::vector<int> res = {4, 5};

  ASSERT_EQ(b, res);
}

TEST(DropTest, MapContainer) {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  auto b = v | DropClass(3);
  std::vector<int> res = {1, 2};

  ASSERT_EQ(b, res);
}

TEST(ReverseTest, NotMapContainer1) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto b = v | ReverseClass();
  std::vector<int> res = {5, 4, 3, 2, 1};

  ASSERT_EQ(b, res);
}

TEST(ReverseTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | ReverseClass();
  std::vector<int> res = {5, 4, 3, 2, 1};

  ASSERT_EQ(b, res);
}

TEST(KeyTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | KeyClass();
  std::vector<int> res = {1, 2, 3, 4, 5};

  ASSERT_EQ(b, res);
}

TEST(KeyTest, MapContainer) {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  auto b = v | KeyClass();
  std::vector<std::string> res = {"fifth", "first", "fourth", "second", "third"};

  ASSERT_EQ(b, res);
}

TEST(ValueTest, NotMapContainer2) {
  std::set<int> v = {1, 2, 3, 4, 5};
  auto b = v | ValueClass();
  std::vector<int> res = {1, 2, 3, 4, 5};

  ASSERT_EQ(b, res);
}

TEST(ValueTest, MapContainer) {
  std::map<std::string, int> v = {{"first", 0}, {"second", 1}, {"third", 2}, {"fourth", 3}, {"fifth", 5}};
  auto b = v | ValueClass();
  std::vector<int> res = {5, 0, 3, 1, 2};

  ASSERT_EQ(b, res);
}