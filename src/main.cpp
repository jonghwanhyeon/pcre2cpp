#include <iostream>
#include <memory>
#include <utility>
#include <string>

#include "pcre2cpp.h"

namespace re = pcre2;

void search(const std::string& pattern, const std::string& text, size_t pos=0, size_t endpos=std::string::npos) {
  const re::Pattern p(pattern);
  const re::Matcher matcher = p.matcher();

  auto context = matcher.find(text);
  if (context == nullptr) {
    std::cout << "no match" << std::endl;
    return;
  }

  size_t index = 1;
  while (context != nullptr) {
    std::cout << "[MATCH " << index << "]" << std::endl;
    auto match = context->to_match();

    const auto& groups = match.groups;
    for (auto iterator = groups.begin(); iterator != groups.end(); ++iterator) {
        std::cout << std::distance(groups.begin(), iterator) << " -> " << (*iterator)->start << ":" << (*iterator)->end << " = " << (*iterator)->text << std::endl;
    }

    for (const auto& pair : match.groupmap) {
        std::cout << pair.first << " -> ";
        std::cout << pair.second->start << ":" << pair.second->end << " = " << pair.second->text << std::endl;
    }

    context = matcher.find(context);

    index += 1;
    std::cout << std::endl;
  }
}

int main(int argc, char *argv[]) {
  search("(?<adverb>(?P<adjective>\\w+)ly)", "He was carefully disguised but captured quickly by police.");
  return 0;
}