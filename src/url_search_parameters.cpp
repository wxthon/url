// Copyright 2017-18 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include "skyr/url_search_parameters.hpp"

namespace skyr {
url_search_parameters::url_search_parameters() {}

url_search_parameters::url_search_parameters(
    const std::vector<value_type> &parameters)
  : parameters_(parameters) {
  update();
}

url_search_parameters::url_search_parameters(
    string_view query) {
  initialize(query);
  update();
}

url_search_parameters::url_search_parameters(
    url_record &url)
  : url_(url) {
  if (url_.value().get().query) {
    initialize(url_.value().get().query.value());
  }
}

void url_search_parameters::append(
    const string_type &name,
    const string_type &value) {
  parameters_.emplace_back(name, value);
  update();
}

void url_search_parameters::remove(
    const string_type &name) {
  auto first = std::begin(parameters_), last = std::end(parameters_);
  auto it = std::remove_if(
      first, last,
      [&name](const value_type &parameter) -> bool {
        return name.compare(parameter.first) == 0;
      });
  parameters_.erase(it, last);
  update();
}

optional<url_search_parameters::string_type> url_search_parameters::get(
    const string_type &name) const noexcept {
  auto first = std::begin(parameters_), last = std::end(parameters_);
  auto it = std::find_if(
      first, last,
      [&name](const value_type &parameter) -> bool {
        return name.compare(parameter.first) == 0;
      });
  if (it == last) {
    return nullopt;
  }

  return it->second;
}

std::vector<url_search_parameters::string_type> url_search_parameters::get_all(
    const string_type &name) const {
  std::vector<string_type> result;
  for (const auto &param : parameters_) {
    if (param.first == name) {
      result.push_back(param.second);
    }
  }
  return result;
}

bool url_search_parameters::contains(const string_type &name) const noexcept {
  auto first = std::begin(parameters_), last = std::end(parameters_);
  auto it = std::find_if(first, last,
      [&name](const value_type &parameter) -> bool {
    return name.compare(parameter.first) == 0;
  });
  return it != last;
}

void url_search_parameters::set(
    const string_type &name,
    const string_type &value) {
  auto first = std::begin(parameters_), last = std::end(parameters_);
  auto it = std::find_if(
      first, last,
      [&name](const value_type &parameter) -> bool {
        return name.compare(parameter.first) == 0;
      });
  if (it != last) {
    it->second = value;
    ++it;
    it = std::remove_if(
        it, last,
        [&name](const value_type &parameter) -> bool {
          return name.compare(parameter.first) == 0;
        });
    parameters_.erase(it, last);
  }
  else {
    parameters_.emplace_back(name, value);
  }

  update();
}

void url_search_parameters::clear() noexcept {
  parameters_.clear();
}

void url_search_parameters::sort() {
  auto first = std::begin(parameters_), last = std::end(parameters_);
  std::sort(
      first, last,
      [](const value_type &lhs, const value_type &rhs) -> bool {
        return lhs.first < rhs.first;
      });
  update();
}

bool url_search_parameters::empty() const noexcept {
  return parameters_.empty();
}

url_search_parameters::size_type url_search_parameters::size() const noexcept {
  return parameters_.size();
}

url_search_parameters::const_iterator url_search_parameters::begin() const noexcept {
  return parameters_.begin();
}

url_search_parameters::const_iterator url_search_parameters::end() const noexcept {
  return parameters_.end();
}

url_search_parameters::string_type url_search_parameters::to_string() const {
  auto result = string_type{};

  auto first = std::begin(parameters_), last = std::end(parameters_);
  auto it = first;
  while (it != last) {
    result.append(it->first);
    result.append("=");
    result.append(it->second);

    ++it;
    if (it != last) {
      result.append("&");
    }
  }

  return result;
}

void url_search_parameters::initialize(std::string_view query) {
  auto first = std::begin(query), last = std::end(query);
  auto it = first;
  while (it != last) {
    auto sep_it = std::find_if(
        it, last, [](char c) -> bool { return c == '&' || c == ';'; });
    auto eq_it = std::find_if(
        it, sep_it, [](char c) -> bool { return c == '='; });

    auto name = string_type(it, eq_it);
    if (eq_it != sep_it) {
      ++eq_it;  // skip '=' symbol
    }
    auto value = string_type(eq_it, sep_it);

    parameters_.emplace_back(name, value);

    it = sep_it;
    if (*it == '&' || *it == ';') {
      ++it;
    }
  }
}

void url_search_parameters::update() {
  if (url_) {
    auto query = to_string();
    if (!query.empty()) {
      url_.value().get().query = query;
    } else {
      url_.value().get().query = nullopt;
    }
  }
}
}  // namespace skyr
