#pragma once

#include <memory>
#include <string>
#include <pcre2.h>

namespace pcre2 {
  class Pattern;
  class MatchContext;

  class Matcher {
  public:
    explicit Matcher(const Pattern& pattern);

    std::shared_ptr<MatchContext> find(const std::string& text, size_t pos = 0, size_t endpos = std::string::npos, uint32_t flags = 0) const;
    std::shared_ptr<MatchContext> find(std::shared_ptr<MatchContext> context) const;

  private:
    const Pattern& pattern;
  };
}