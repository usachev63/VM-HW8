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
  WeenyString() noexcept;
  ~WeenyString() noexcept;
  explicit WeenyString(const char *cstr);
  WeenyString(const WeenyString &other) noexcept;
  WeenyString(WeenyString &&other) noexcept;
  WeenyString &operator=(const char *cstr) noexcept;
  WeenyString &operator=(const WeenyString &other) noexcept;
  WeenyString &operator=(WeenyString &&other) noexcept;
  void swap(WeenyString &other) noexcept;

  bool operator==(const char *cstr) const noexcept;
  bool operator==(const WeenyString &other) const noexcept;
  bool operator!=(const char *cstr) const noexcept;
  bool operator!=(const WeenyString &other) const noexcept;
  bool operator<(const char *cstr) const noexcept;
  bool operator<(const WeenyString &other) const noexcept;
  bool operator<=(const char *cstr) const noexcept;
  bool operator<=(const WeenyString &other) const noexcept;
  bool operator>(const char *cstr) const noexcept;
  bool operator>(const WeenyString &other) const noexcept;
  bool operator>=(const char *cstr) const noexcept;
  bool operator>=(const WeenyString &other) const noexcept;

  /// \return non-null
  const char *cstr() const noexcept;
  bool isUniqueRef() const noexcept;

  void dump() const;

#ifdef WEENY_TEST
  static inline int allocs = 0;
  static inline int frees = 0;
#endif

private:
  void initFrom(const char *cstr);
  void initFrom(const WeenyString &other);
  void initFrom(WeenyString &&other);
  void reset();
  void setEmpty();

private:
  mutable uintptr_t fixnum;

private:
  static const char emptyString[];
};

inline void swap(WeenyString &lhs, WeenyString &rhs) { lhs.swap(rhs); }

static_assert(std::is_swappable_v<WeenyString>);

// impl

inline WeenyString::WeenyString() noexcept { setEmpty(); }

inline WeenyString::~WeenyString() noexcept { reset(); }

inline WeenyString::WeenyString(const char *cstr) { initFrom(cstr); }

inline WeenyString::WeenyString(const WeenyString &other) noexcept {
  initFrom(other);
}

inline WeenyString::WeenyString(WeenyString &&other) noexcept {
  initFrom(std::move(other));
}

inline WeenyString &WeenyString::operator=(const char *cstr) noexcept {
  reset();
  initFrom(cstr);
  return *this;
}

inline WeenyString &WeenyString::operator=(const WeenyString &other) noexcept {
  if (this == &other)
    return *this;
  reset();
  initFrom(other);
  return *this;
}

inline WeenyString &WeenyString::operator=(WeenyString &&other) noexcept {
  if (this == &other)
    return *this;
  reset();
  initFrom(std::move(other));
  return *this;
}

inline void WeenyString::swap(WeenyString &other) noexcept {
  std::swap(fixnum, other.fixnum);
}

inline bool WeenyString::operator==(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr ? cstr : "") == 0;
}
inline bool WeenyString::operator==(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) == 0;
}
inline bool WeenyString::operator!=(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr ? cstr : "") != 0;
}
inline bool WeenyString::operator!=(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) != 0;
}
inline bool WeenyString::operator<(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr ? cstr : "") < 0;
}
inline bool WeenyString::operator<(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) < 0;
}
inline bool WeenyString::operator<=(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr ? cstr : "") <= 0;
}
inline bool WeenyString::operator<=(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) <= 0;
}
inline bool WeenyString::operator>(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr ? cstr : "") > 0;
}
inline bool WeenyString::operator>(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) > 0;
}
inline bool WeenyString::operator>=(const char *cstr) const noexcept {
  return strcmp(this->cstr(), cstr ? cstr : "") >= 0;
}
inline bool WeenyString::operator>=(const WeenyString &other) const noexcept {
  return strcmp(this->cstr(), other.cstr()) >= 0;
}

// internal

inline const char *WeenyString::cstr() const noexcept {
  return reinterpret_cast<const char *>(fixnum & ~1);
}

inline bool WeenyString::isUniqueRef() const noexcept { return ~fixnum & 1; }

inline void WeenyString::initFrom(const char *cstr) {
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

inline void WeenyString::initFrom(const WeenyString &other) {
#ifdef WEENY_DEBUG_LOG
  std::cerr << "WeenyString initFrom copied " << other.cstr() << std::endl;
#endif
  if (other.isUniqueRef())
    other.fixnum |= 1;
  this->fixnum = other.fixnum;
}

inline void WeenyString::initFrom(WeenyString &&other) {
#ifdef WEENY_DEBUG_LOG
  std::cerr << "WeenyString initFrom moved " << other.cstr() << std::endl;
#endif
  this->fixnum = other.fixnum;
  other.setEmpty();
}

inline void WeenyString::reset() {
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

inline void WeenyString::setEmpty() {
  fixnum = reinterpret_cast<uintptr_t>(emptyString) | 1;
}

} // namespace weenystring
