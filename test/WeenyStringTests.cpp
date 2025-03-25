#define WEENY_DEBUG_LOG
#define WEENY_TEST

#include "weenystring/WeenyString.h"
#include "gtest/gtest.h"

using namespace weenystring;

TEST(WeenyString, EmptyConstructor) {
  WeenyString s;
  EXPECT_NE(nullptr, s.cstr());
  EXPECT_STREQ(s.cstr(), "");
  EXPECT_STREQ(s.cstr(), WeenyString().cstr());
}

TEST(WeenyString, EmptyConstructor_AllocsFrees) {
  int oldAllocs = WeenyString::allocs;
  int oldFrees = WeenyString::frees;
  {
    WeenyString s1;
    WeenyString();
  }
  int allocs = WeenyString::allocs - oldAllocs;
  int frees = WeenyString::frees - oldFrees;
  EXPECT_EQ(0, allocs);
  EXPECT_EQ(0, frees);
}

TEST(WeenyString, CStrConstructor) {
  WeenyString s("abc");
  EXPECT_NE(nullptr, s.cstr());
  EXPECT_STREQ(s.cstr(), "abc");
  EXPECT_STREQ(WeenyString("abc").cstr(), "abc");
  EXPECT_STREQ(WeenyString("").cstr(), "");
}

TEST(WeenyString, CStrConstructor_AllocsFrees) {
  int oldAllocs = WeenyString::allocs;
  int oldFrees = WeenyString::frees;
  {
    WeenyString s1("abc");
  }
  int allocs = WeenyString::allocs - oldAllocs;
  int frees = WeenyString::frees - oldFrees;
  EXPECT_EQ(1, allocs);
  EXPECT_EQ(1, frees);
}

TEST(WeenyString, CopyConstructor) {
  WeenyString s1("abc");
  WeenyString s2(s1);
  EXPECT_STREQ(s2.cstr(), "abc");
  EXPECT_STREQ(WeenyString(s1).cstr(), "abc");
}

TEST(WeenyString, CopyConstructor_Allocs) {
  int oldAllocs = WeenyString::allocs;
  {
    WeenyString s1("abc");
    WeenyString s2(s1);
  }
  int allocs = WeenyString::allocs - oldAllocs;
  EXPECT_EQ(1, allocs);
}

TEST(WeenyString, MoveConstructor) {
  WeenyString s1("abc");
  WeenyString s2(std::move(s1));
  EXPECT_STREQ(s1.cstr(), "");
  EXPECT_STREQ(s2.cstr(), "abc");
  WeenyString s3(WeenyString("abc"));
  EXPECT_STREQ(s3.cstr(), "abc");
}

TEST(WeenyString, MoveConstructor_AllocsFrees) {
  int oldAllocs = WeenyString::allocs;
  int oldFrees = WeenyString::frees;
  {
    WeenyString s1("abc");
    WeenyString s2(std::move(s1));
  }
  int allocs = WeenyString::allocs - oldAllocs;
  int frees = WeenyString::frees - oldFrees;
  EXPECT_EQ(1, allocs);
  EXPECT_EQ(1, frees);
}

TEST(WeenyString, MoveConstructor_AllocsFrees_TempExpr) {
  int oldAllocs = WeenyString::allocs;
  int oldFrees = WeenyString::frees;
  {
    WeenyString s1(WeenyString(WeenyString("abc")));
  }
  int allocs = WeenyString::allocs - oldAllocs;
  int frees = WeenyString::frees - oldFrees;
  EXPECT_EQ(1, allocs);
  EXPECT_EQ(1, frees);
}

TEST(WeenyString, AssignmentToCStr) {
  WeenyString s;
  EXPECT_STREQ(s.cstr(), "");
  s = "4";
  EXPECT_STREQ(s.cstr(), "4");
  s = "134";
  EXPECT_STREQ(s.cstr(), "134");
  EXPECT_STREQ((s = "1337").cstr(), "1337");
}

TEST(WeenyString, CopyAssignment) {
  WeenyString s, s1("1337");
  EXPECT_STREQ(s.cstr(), "");
  EXPECT_STREQ(s1.cstr(), "1337");
  s = s1;
  EXPECT_STREQ(s.cstr(), "1337");
  EXPECT_STREQ(s1.cstr(), "1337");
  WeenyString s2("228");
  s = s2;
  EXPECT_STREQ(s.cstr(), "228");
}

TEST(WeenyString, CopyAssignment_Self) {
  WeenyString s1("1337");
  s1 = s1;
  EXPECT_STREQ(s1.cstr(), "1337");
}

TEST(WeenyString, CopyAssignment_Self_AllocsFrees) {
  int oldAllocs = WeenyString::allocs;
  int oldFrees = WeenyString::frees;
  {
    WeenyString s("abc");
    s = s;
  }
  int allocs = WeenyString::allocs - oldAllocs;
  int frees = WeenyString::frees - oldFrees;
  EXPECT_EQ(1, allocs);
  EXPECT_EQ(1, frees);
}

TEST(WeenyString, MoveAssignment) {
  WeenyString s("123"), s1("456");
  s = std::move(s1);
  EXPECT_STREQ(s.cstr(), "456");
  EXPECT_STREQ(s1.cstr(), "");
  s = WeenyString("1337");
  EXPECT_STREQ(s.cstr(), "1337");
}

TEST(WeenyString, MoveAssignment_Self) {
  WeenyString s("123");
  s = std::move(s);
  EXPECT_STREQ(s.cstr(), "123");
}

TEST(WeenyString, MoveAssignment_Self_AllocsFrees) {
  int oldAllocs = WeenyString::allocs;
  int oldFrees = WeenyString::frees;
  {
    WeenyString s("abc");
    s = std::move(s);
  }
  int allocs = WeenyString::allocs - oldAllocs;
  int frees = WeenyString::frees - oldFrees;
  EXPECT_EQ(1, allocs);
  EXPECT_EQ(1, frees);
}

TEST(WeenyString, EqualsToCStr) {
  EXPECT_TRUE(WeenyString() == "");
  EXPECT_FALSE(WeenyString() == "1");
  EXPECT_TRUE(WeenyString("") == "");
  EXPECT_FALSE(WeenyString("") == "x");
  EXPECT_TRUE(WeenyString("abc") == "abc");
  EXPECT_FALSE(WeenyString("abc") == "abcd");
  EXPECT_FALSE(WeenyString("abcd") == "");
  WeenyString s1("1234");
  EXPECT_TRUE(s1 == "1234");
}

TEST(WeenyString, CompareToCStr_Nullptr) {
  EXPECT_TRUE(WeenyString() == (const char *)nullptr);
  EXPECT_TRUE(WeenyString("") == (const char *)nullptr);
  EXPECT_FALSE(WeenyString("a") == (const char *)nullptr);
  EXPECT_TRUE(WeenyString("a") != (const char *)nullptr);
  EXPECT_FALSE(WeenyString() != (const char *)nullptr);
  EXPECT_FALSE(WeenyString() < (const char *)nullptr);
  EXPECT_FALSE(WeenyString("xyz") < (const char *)nullptr);
  EXPECT_TRUE(WeenyString() <= (const char *)nullptr);
  EXPECT_FALSE(WeenyString("a") <= (const char *)nullptr);
  EXPECT_FALSE(WeenyString() > (const char *)nullptr);
  EXPECT_TRUE(WeenyString("true") > (const char *)nullptr);
  EXPECT_TRUE(WeenyString() >= (const char *)nullptr);
  EXPECT_TRUE(WeenyString("xx") >= (const char *)nullptr);
}

TEST(WeenyString, EqualsToWeenyString) {
  WeenyString empty;
  EXPECT_TRUE(empty == empty);
  EXPECT_TRUE(empty == WeenyString());
  EXPECT_TRUE(empty == WeenyString(""));
  EXPECT_TRUE(WeenyString() == WeenyString());
  EXPECT_TRUE(WeenyString("") == WeenyString());
  EXPECT_TRUE(WeenyString() == WeenyString(""));
  EXPECT_FALSE(empty == WeenyString("abc"));
  WeenyString abc("abc");
  EXPECT_TRUE(abc == abc);
  EXPECT_TRUE(abc == WeenyString("abc"));
  EXPECT_FALSE(abc == WeenyString("cde"));
  EXPECT_TRUE(WeenyString("abc") == WeenyString("abc"));
}

TEST(WeenyString, CStrConstructor_Nullptr) {
  WeenyString s((const char *)nullptr);
  EXPECT_TRUE(s == "");
}

TEST(WeenyString, VectorGrow) {
  std::vector<WeenyString> array;
  array.emplace_back("abc");
  for (int i = 0; i < 16; ++i)
    array.emplace_back("xxx");
  ASSERT_TRUE(array[0].isUniqueRef());
}

void bubbleSort(std::vector<WeenyString> &array) {
  for (ssize_t len = array.size(); len > 0; --len) {
    std::cerr << "bubbleSort for len=" << len << std::endl;
    for (ssize_t i = 0; i + 1 < len; ++i) {
      if (array[i] > array[i + 1])
        std::swap(array[i], array[i + 1]);
    }
  }
}

TEST(WeenyString, BubbleSort_Basic) {
  std::vector<WeenyString> array;
  array.emplace_back();
  array.emplace_back("ba");
  array.emplace_back("b");
  array.emplace_back("z");
  ASSERT_TRUE(array[2].isUniqueRef());
  WeenyString anotherB("b");
  array.push_back(anotherB);
  array.push_back(anotherB);
  bubbleSort(array);
  EXPECT_EQ(array[0], "");
  EXPECT_EQ(array[1], "b");
  EXPECT_TRUE(array[1].isUniqueRef());
  EXPECT_EQ(array[2], "b");
  EXPECT_FALSE(array[2].isUniqueRef());
  EXPECT_EQ(array[3], "b");
  EXPECT_FALSE(array[3].isUniqueRef());
  EXPECT_EQ(array[4], "ba");
  EXPECT_EQ(array[5], "z");

  array[1].dump();
  array[2].dump();
  array[4].dump();
}
