#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace weenystring {

class WeenyString {
public:
  inline WeenyString() noexcept;
  inline ~WeenyString() noexcept;
  inline explicit WeenyString(const char *cstr);
  inline WeenyString(const WeenyString &other) noexcept;
  inline WeenyString(WeenyString &&other) noexcept;
  inline WeenyString &operator=(const char *cstr) noexcept;
  inline WeenyString &operator=(const WeenyString &other) noexcept;
  inline WeenyString &operator=(WeenyString &&other) noexcept;
  inline void swap(WeenyString &other) noexcept;

  inline bool operator==(const char *cstr) const noexcept;
  inline bool operator==(const WeenyString &other) const noexcept;
  inline bool operator!=(const char *cstr) const noexcept;
  inline bool operator!=(const WeenyString &other) const noexcept;
  inline bool operator<(const char *cstr) const noexcept;
  inline bool operator<(const WeenyString &other) const noexcept;
  inline bool operator<=(const char *cstr) const noexcept;
  inline bool operator<=(const WeenyString &other) const noexcept;
  inline bool operator>(const char *cstr) const noexcept;
  inline bool operator>(const WeenyString &other) const noexcept;
  inline bool operator>=(const char *cstr) const noexcept;
  inline bool operator>=(const WeenyString &other) const noexcept;

  inline const char *cstr() const noexcept;
  inline bool isUniqueRef() const noexcept;

  void dump() const;

#ifdef WEENY_TEST
  static inline int allocs = 0;
  static inline int frees = 0;
#endif

private:
  inline void initFrom(const char *cstr);
  inline void initFrom(const WeenyString &other);
  inline void initFrom(WeenyString &&other);
  inline void reset();
  inline void setEmpty();

private:
  mutable uintptr_t fixnum;

private:
  static const char emptyString[];
};

inline void swap(WeenyString &lhs, WeenyString &rhs) { lhs.swap(rhs); }

static_assert(std::is_swappable_v<WeenyString>);

// impl

WeenyString::WeenyString() noexcept { setEmpty(); }

WeenyString::~WeenyString() noexcept { reset(); }

WeenyString::WeenyString(const char *cstr) { initFrom(cstr); }

WeenyString::WeenyString(const WeenyString &other) noexcept { initFrom(other); }

WeenyString::WeenyString(WeenyString &&other) noexcept {
  initFrom(std::move(other));
}

WeenyString &WeenyString::operator=(const char *cstr) noexcept {
  reset();
  initFrom(cstr);
  return *this;
}

WeenyString &WeenyString::operator=(const WeenyString &other) noexcept {
  if (this == &other)
    return *this;
  reset();
  initFrom(other);
  return *this;
}

WeenyString &WeenyString::operator=(WeenyString &&other) noexcept {
  if (this == &other)
    return *this;
  reset();
  initFrom(std::move(other));
  return *this;
}

void WeenyString::swap(WeenyString &other) noexcept {
  std::swap(fixnum, other.fixnum);
}

bool WeenyString::operator==(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr) == 0;
}
bool WeenyString::operator==(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) == 0;
}
bool WeenyString::operator!=(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr) != 0;
}
bool WeenyString::operator!=(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) != 0;
}
bool WeenyString::operator<(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr) < 0;
}
bool WeenyString::operator<(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) < 0;
}
bool WeenyString::operator<=(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr) <= 0;
}
bool WeenyString::operator<=(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) <= 0;
}
bool WeenyString::operator>(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr) > 0;
}
bool WeenyString::operator>(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) > 0;
}
bool WeenyString::operator>=(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr) >= 0;
}
bool WeenyString::operator>=(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) >= 0;
}

// internal

const char *WeenyString::cstr() const noexcept {
  return reinterpret_cast<const char *>(fixnum & ~1);
}

bool WeenyString::isUniqueRef() const noexcept { return ~fixnum & 1; }

void WeenyString::initFrom(const char *cstr) {
  if (cstr == nullptr) {
    setEmpty();
    return;
  }
  size_t len = strlen(cstr);
  char *allocated = reinterpret_cast<char *>(malloc(len + 1));
  if (allocated == nullptr)
    throw std::runtime_error("allocation fail");
#ifdef WEENY_DEBUG_LOG
  std::cerr << "WeenyString alloc at " << (void *)allocated << " for " << cstr
            << std::endl;
#endif
#ifdef WEENY_TEST
  ++allocs;
#endif
  strcpy(allocated, cstr);
  fixnum = reinterpret_cast<uintptr_t>(allocated);
}

void WeenyString::initFrom(const WeenyString &other) {
#ifdef WEENY_DEBUG_LOG
  std::cerr << "WeenyString initFrom copied " << other.cstr() << std::endl;
#endif
  if (other.isUniqueRef())
    other.fixnum |= 1;
  this->fixnum = other.fixnum;
}

void WeenyString::initFrom(WeenyString &&other) {
#ifdef WEENY_DEBUG_LOG
  std::cerr << "WeenyString initFrom moved " << other.cstr() << std::endl;
#endif
  this->fixnum = other.fixnum;
  other.setEmpty();
}

void WeenyString::reset() {
  if (isUniqueRef()) {
#ifdef WEENY_DEBUG_LOG
    std::cerr << "WeenyString free at addr " << (void *)cstr() << " string "
              << cstr() << std::endl;
#endif
#ifdef WEENY_TEST
    ++frees;
#endif
    free((void *)cstr());
  }
  setEmpty();
}

void WeenyString::setEmpty() {
  fixnum = reinterpret_cast<uintptr_t>(emptyString) | 1;
}

} // namespace weenystring
