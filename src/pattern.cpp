#include "pattern.h"
#include "matcher.h"
#include "utils.h"

#include <utility>
#include <iostream>

namespace pcre2 {
  static pcre2_code* compile(const std::string& pattern, uint32_t flags);
  static std::map<std::string, uint32_t> build_groupindex(const Pattern& pattern);

  Pattern::Pattern(const std::string& pattern, uint32_t flags)
  : pattern{pattern},
    code{compile(this->pattern, flags)},
    flags{query_info<uint32_t>(PCRE2_INFO_ALLOPTIONS) & (~PCRE2_UTF)},
    groups{query_info<uint32_t>(PCRE2_INFO_CAPTURECOUNT)},
    groupindex{build_groupindex(*this)} {
    std::cout << "Pattern::Pattern(const std::string&, uint32_t) -> " << this << " <code=" << code  << ">" << std::endl;
  }

  Pattern::~Pattern() {
    std::cout << "Pattern::~Pattern() -> " << this << " <code=" << code  << ">" << std::endl;

    if (code != nullptr) {
      pcre2_code_free(code);
    }
  }

  Pattern::Pattern(Pattern&& other) noexcept
  : code{std::move(other.code)},
    flags{std::move(other.flags)},
    groups{std::move(other.groups)},
    groupindex{std::move(other.groupindex)},
    pattern{std::move(other.pattern)} {
    other.code = nullptr;
    std::cout << "Pattern::Pattern(Pattern&&)-> " << this << " <code=" << code  << ">" << std::endl;
  }

  const Matcher Pattern::matcher() const {
    return Matcher(*this);
  }

  pcre2_code* compile(const std::string& pattern, uint32_t flags) {
    int error_code;
    PCRE2_SIZE error_offset;

    pcre2_code* code = pcre2_compile(
      reinterpret_cast<PCRE2_SPTR>(pattern.data()),
      pattern.size(),
      flags | PCRE2_UTF,
      &error_code,
      &error_offset,
      NULL
    );

    if (code == NULL) {
      throw std::invalid_argument(utils::build_error_message(error_code, error_offset));
    }

    return code;
  }

  std::map<std::string, uint32_t> build_groupindex(const Pattern& pattern) {
    uint32_t count = pattern.query_info<uint32_t>(PCRE2_INFO_NAMECOUNT);
    PCRE2_SPTR table = pattern.query_info<PCRE2_SPTR>(PCRE2_INFO_NAMETABLE);
    uint32_t size_of_entry = pattern.query_info<uint32_t>(PCRE2_INFO_NAMEENTRYSIZE);

    std::map<std::string, uint32_t> groupindex;
    for (uint32_t i = 0; i < count; ++i) {
      uint32_t index = (table[0] << 8) | table[1];
      groupindex[utils::to_string(table + 2)] = index;

      table += size_of_entry;
    }

    return groupindex;

  }
}