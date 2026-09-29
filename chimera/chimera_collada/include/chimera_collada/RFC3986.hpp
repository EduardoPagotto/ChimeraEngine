#pragma once
#include <string>

namespace ce {

    enum class RFC3986_SCHEME { FILE = 0, LOCAL = 1, HTTP = 2, INVALID = 3 };

    class RFC3986 {
      public:
        RFC3986() : scheme_(RFC3986_SCHEME::INVALID), path_(""), fragment_("") {}
        RFC3986(const std::string& url);
        RFC3986(const RFC3986& other) : scheme_(other.scheme_), path_(other.path_), fragment_(other.fragment_) {}
        virtual ~RFC3986() = default;
        const RFC3986_SCHEME& setUrl(const std::string& url);
        inline const RFC3986_SCHEME getScheme() const { return scheme_; }
        inline const std::string& getPath() const { return path_; }
        inline const std::string& getFragment() const { return fragment_; }
        inline const bool isInvalid() const { return scheme_ == RFC3986_SCHEME::INVALID; }

      private:
        RFC3986_SCHEME scheme_;
        std::string path_;
        std::string fragment_;
    };
} // namespace ce
