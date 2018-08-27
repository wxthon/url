// Copyright 2017-18 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SKYR_URL_SEARCH_PARAMETERS_INC
#define SKYR_URL_SEARCH_PARAMETERS_INC

#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <skyr/optional.hpp>
#include <skyr/url_record.hpp>

namespace skyr {
/// Allows iterating through search parameters.
class url_search_parameters {

 public:

  ///
  using string_type = std::string;

  ///
  using string_view = std::string_view;

  ///
  using value_type = std::pair<string_type, string_type>;

  ///
  using const_iterator = std::vector<value_type>::const_iterator;

  ///
  using size_type = std::vector<value_type>::size_type;

  /// Constructor.
  url_search_parameters();

  /// Constructor.
  url_search_parameters(std::initializer_list<value_type> parameters)
    : url_search_parameters{std::vector<value_type>{parameters}} {}

  /// Constructor.
  /// \param parameters
  explicit url_search_parameters(const std::vector<value_type> &parameters);

  /// Constructor.
  /// \param query The search string.
  explicit url_search_parameters(string_view query);

  /// Constructor.
  /// \param url The URL record.
  explicit url_search_parameters(url_record &url);

  /// Constructor.
  /// \param

  /// Appends a name-value pair to the search string.
  /// \param name The parameter name.
  /// \param value The parameter value.
  void append(const string_type &name, const string_type &value);

  /// Removes a parameter from the search string.
  /// \param name The name of the parameter to remove.
  void remove(const string_type &name);

  /// \param name
  /// \returns
  optional<string_type> get(const string_type &name) const noexcept;
  
  /// \param name
  /// \returns
  std::vector<string_type> get_all(const string_type &name) const;

  /// \param name
  /// \returns `true` if the value is in the search parameters, `false` otherwise.
  bool contains(const string_type &name) const noexcept;

  /// \param name
  /// \param value
  void set(const string_type &name, const string_type &value);

  ///
  void clear() noexcept;

  ///
  void sort();

  ///
  bool empty() const noexcept;

  ///
  size_type size() const noexcept;

  /// \returns
  const_iterator begin() const noexcept;

  /// \returns
  const_iterator end() const noexcept;

  /// \returns
  string_type to_string() const;

 private:
  void initialize(std::string_view query);
  void update();

  std::vector<value_type> parameters_;
  optional<std::reference_wrapper<url_record>> url_;
};
}  // namespace skyr

#endif  // SKYR_URL_SEARCH_PARAMETERS_INC
