#pragma once

#include <memory>
#include <map>
#include <string>
#include <pcre2.h>

namespace pcre2 {
  class Matcher;

  class Pattern {
  public:
    explicit Pattern(const std::string& pattern, uint32_t flags = 0);
    virtual ~Pattern();

    Pattern(const Pattern& other) = delete;
    Pattern& operator=(const Pattern& other) = delete;

    Pattern(Pattern&& other) noexcept;
    Pattern& operator=(Pattern&& other) = delete;

    const Matcher matcher() const;

    template <typename T>
    inline T query_info(uint32_t what) const { T where; pcre2_pattern_info(code, what, &where); return where; }

    inline operator pcre2_code*() const { return code; }

  public:
    const std::string pattern;

  private:
    pcre2_code* code;

  public:
    const uint32_t flags;
    const uint32_t groups;
    const std::map<std::string, uint32_t> groupindex;
  };
}