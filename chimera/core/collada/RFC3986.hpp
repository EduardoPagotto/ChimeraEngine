#pragma once
#include <string>

namespace Chimera {

enum class RFC3986_SCHEME { FILE = 0, LOCAL = 1, HTTP = 2, INVALID = 3 };

class RFC3986 {
  public:
    RFC3986() : scheme(RFC3986_SCHEME::INVALID), path(""), fragment("") {}
    RFC3986(const std::string& url);
    RFC3986(const RFC3986& other) : scheme(other.scheme), path(other.path), fragment(other.fragment) {}
    virtual ~RFC3986() = default;
    const RFC3986_SCHEME& setUrl(const std::string& url);
    inline const RFC3986_SCHEME getScheme() const { return scheme; }
    inline const std::string& getPath() const { return path; }
    inline const std::string& getFragment() const { return fragment; }
    inline const bool isInvalid() const { return scheme == RFC3986_SCHEME::INVALID; }

  private:
    RFC3986_SCHEME scheme;
    std::string path;
    std::string fragment;
};
} // namespace Chimera