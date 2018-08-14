#pragma once

#include <map>
#include <vector>
#include <string>

namespace pcre2 {
  class Pattern;
  class MatchContext;

  struct Group {
    const size_t start;
    const size_t end;
    const std::string text;
  };

  struct Match {
    explicit Match(const MatchContext& context);

    const Pattern& pattern;
    const std::string& text;
    const size_t pos;
    const size_t endpos;

    const std::vector<std::shared_ptr<const Group>> groups;
    const std::map<std::string, std::shared_ptr<const Group>> groupmap;
  };
}