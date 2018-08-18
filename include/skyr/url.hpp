// Copyright 2018 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SKYR_URL_INC
#define SKYR_URL_INC

#include <string>
#include <skyr/string_view.hpp>
#include <skyr/expected.hpp>
#include <skyr/url_record.hpp>
#include <skyr/url_error.hpp>
#include <skyr/url_search_parameters.hpp>
#include <skyr/details/translate.hpp>

#ifdef SKYR_URI_MSVC
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

namespace skyr {
/// `url_parse_error` is thrown when there is an error parsing the URL.
class url_parse_error : public std::runtime_error {
 public:
  /// Constructor
  /// \param error
  explicit url_parse_error(std::error_code error) noexcept
    : runtime_error("URL parse error")
    , error_(std::move(error)) {}

  /// \returns
  std::error_code error() const noexcept {
    return error_;
  }

 private:

  std::error_code error_;

};

/// This class represents a URL.
class url {
 public:

  using string_type = std::string;
  using value_type = string_view::value_type;
  using iterator = string_view::iterator;
  using const_iterator = string_view::const_iterator;
  using reference = string_view::reference;
  using const_reference = string_view::const_reference;
  using size_type = string_view::size_type;
  using difference_type = string_view::difference_type;

  /// Default constructor
  url() = default;

  /// Constructor
  /// \tparam Source
  /// \param input The input string
  /// \throws `url_parse_exception`
  template <class Source>
  explicit url(Source input) {
    initialize(details::translate(input));
  }

  /// Constructor
  /// \tparam Source
  /// \param input The input string
  /// \param base A base URL
  /// \throws `url_parse_exception`
  template <class Source>
  url(Source input, url base) {
    initialize(details::translate(input), base.url_);
  }

  /// Constructor
  /// \param input A URL record
  explicit url(url_record &&input) noexcept;

  ///
  /// \param other
  void swap(url &other) noexcept;

  /// \returns
  std::string href() const;

  /// \param href
  expected<void, std::error_code> set_href(std::string href);

  /// \returns
  std::string to_json() const;

  /// \returns
  std::string protocol() const;

  /// \param protocol
  expected<void, std::error_code> set_protocol(std::string protocol);

  /// \returns
  std::string username() const;

  /// \param username
  expected<void, std::error_code> set_username(std::string username);

  /// \returns
  std::string password() const;

  /// \param password
  expected<void, std::error_code> set_password(std::string password);

  /// \returns
  std::string host() const;

  /// \param host
  expected<void, std::error_code> set_host(std::string host);

  /// \returns
  std::string hostname() const;

  /// \param hostname
  expected<void, std::error_code> set_hostname(std::string hostname);

  /// \returns
  std::string port() const;

  /// \returns
  template <typename intT>
  intT port(typename std::is_integral<intT>::type * = nullptr) const {
    auto p = port();
    const char *port_first = p.data();
    char *port_last = nullptr;
    return static_cast<intT>(std::strtoul(port_first, &port_last, 10));
  }

  /// \param port
  expected<void, std::error_code> set_port(std::string port);

  /// \param port
  expected<void, std::error_code> set_port(std::uint16_t port);

  /// \returns
  std::string pathname() const;

  /// \param pathname
  expected<void, std::error_code> set_pathname(std::string pathname);

  /// \returns
  std::string search() const;

  /// \param search
  expected<void, std::error_code> set_search(std::string search);

  /// \returns
  url_search_parameters &search_parameters();

  /// \returns
  std::string hash() const;

  /// \param hash
  expected<void, std::error_code> set_hash(std::string hash);

  /// \returns
  url_record record() const;

  /// \returns
  bool is_special() const noexcept;

  /// \returns
  bool validation_error() const noexcept;

  /// \returns
  const_iterator begin() const noexcept {
    return view_.begin();
  }

  /// \returns
  const_iterator end() const noexcept {
    return view_.end();
  }

  /// \returns
  string_view view() const noexcept {
    return view_;
  }

  /// \returns
  bool empty() const noexcept {
    return view_.empty();
  }

  /// \returns
  size_type size() const noexcept {
    return view_.size();
  }

  /// \returns
  size_type length() const noexcept {
    return view_.length();
  }

  /// \param other
  /// \returns
  int compare(const url &other) const noexcept {
    return view_.compare(other.view_);
  }

  /// \param scheme
  /// \returns
  static optional<std::uint16_t> default_port(const std::string &scheme) noexcept;

  ///
  void clear();

  ///
  /// \returns
  const char *c_str() const noexcept;

  ///
  /// \returns
  operator string_type() const;

  ///
  /// \returns
  std::string string() const;

  ///
  /// \returns
  std::wstring wstring() const;

  ///
  /// \returns
  std::string u8string() const;

  ///
  /// \returns
  std::u16string u16string() const;

  ///
  /// \returns
  std::u32string u32string() const;

 private:

  void initialize(std::string input, optional<url_record> base = nullopt);
  void update_record(url_record &&record);

  url_record url_;
  string_view view_;
  url_search_parameters parameters_;
};

void swap(url &lhs, url &rhs) noexcept;

/// \exclude
namespace details {
expected<url, std::error_code> make_url(std::string input, optional<url_record> base = nullopt);
}  // details

/// \tparam Source
/// \param input
/// \returns
template <class Source>
expected<url, std::error_code> make_url(Source input) {
  return details::make_url(details::translate(input));
}

/// \tparam Source
/// \param input
/// \param base
/// \returns
template <class Source>
expected<url, std::error_code> make_url(Source input, url base) {
  return details::make_url(details::translate(input), base.record());
}

/// Equality operator
/// \param lhs
/// \param rhs
/// \returns
inline bool operator == (const url &lhs, const url &rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

/// Inequality operator
/// \param lhs
/// \param rhs
/// \returns
inline bool operator != (const url &lhs, const url &rhs) noexcept {
  return !(lhs == rhs);
}

/// Comparison operator
/// \param lhs
/// \param rhs
/// \returns
inline bool operator < (const url &lhs, const url &rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

/// Comparison operator
/// \param lhs
/// \param rhs
/// \returns
inline bool operator > (const url &lhs, const url &rhs) noexcept {
  return rhs < lhs;
}

/// Comparison operator
/// \param lhs
/// \param rhs
/// \returns
inline bool operator <= (const url &lhs, const url &rhs) noexcept {
  return !(lhs > rhs);
}

/// Comparison operator
/// \param lhs
/// \param rhs
/// \returns
inline bool operator >= (const url &lhs, const url &rhs) noexcept {
  return !(lhs < rhs);
}
}  // namespace skyr

#endif  // SKYR_URL_INC
