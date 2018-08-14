#include "matcher.h"
#include "pattern.h"
#include "matchcontext.h"

#include <iostream>

namespace pcre2 {
  Matcher::Matcher(const Pattern& pattern)
  : pattern{pattern} {
    std::cout << "Matcher::Matcher(const Pattern&) -> " << this << "<pattern=" << &pattern << ">" << std::endl;
  }

  std::shared_ptr<MatchContext> Matcher::find(const std::string& text, size_t pos, size_t endpos, uint32_t flags) const {
    if (endpos > text.size()) endpos = text.size();
    if (pos > endpos) {
      return nullptr;
    }

    std::shared_ptr<MatchContext> context(new MatchContext(pattern, text, pos, endpos, flags));
    int error = pcre2_match(
      context->pattern,
      context->subject,
      context->length,
      0,
      context->flags,
      context->match_data,
      NULL
    );

    if (error < 0) {
      if (error == PCRE2_ERROR_NOMATCH) return nullptr;

      throw std::invalid_argument(utils::build_error_message(error));
    }

    context->update_offset();
    return context;
  }

  std::shared_ptr<MatchContext> Matcher::find(std::shared_ptr<MatchContext> context) const {
    do {
      uint32_t flags = context->flags;

      if (context->was_empty_match()) {
        if (context->is_at_end_of_subject()) {
          return nullptr;
        }

        flags |= PCRE2_NOTEMPTY_ATSTART;
        flags |= PCRE2_ANCHORED;
      }

      int error = pcre2_match(
        context->pattern,
        context->subject,
        context->length,
        context->offset,
        flags,
        context->match_data,
        NULL
      );

      if (error < 0) {
        if (error == PCRE2_ERROR_NOMATCH) {
          if (flags == 0) return nullptr;

          context->advance_offset();
          continue;
        }

        throw std::invalid_argument(utils::build_error_message(error));
      }
    } while (false);

    context->update_offset();
    return context;
  }
}