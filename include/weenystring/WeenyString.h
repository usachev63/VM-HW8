#pragma once

namespace weenystring {

class WeenyString {
public:
  WeenyString();
  explicit WeenyString(const char *cstr);
  WeenyString &operator=(const char *cstr);
  explicit WeenyString(const WeenyString &other);
  explicit WeenyString(WeenyString &&other);
  WeenyString &operator=(const WeenyString &other);
  WeenyString &operator=(WeenyString &&other);

  const char *cstr() const;

  void dump() const;

private:
};

} // namespace weenystring
