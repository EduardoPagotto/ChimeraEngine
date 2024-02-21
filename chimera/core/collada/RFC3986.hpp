#pragma once
#include <string>

namespace Chimera {

enum class RFC3986_SCHEME { FILE = 0, LOCAL = 1, HTTP = 2, INVALID = 3 };

class RFC3986 {
  private:
    RFC3986_SCHEME m_scheme;
    std::string m_path;
    std::string m_fragment;

  public:
    RFC3986() : m_scheme(RFC3986_SCHEME::INVALID), m_path(""), m_fragment("") {}

    RFC3986(const std::string& url) { this->setUrl(url); }

    RFC3986(const RFC3986& other) : m_scheme(other.m_scheme), m_path(other.m_path), m_fragment(other.m_fragment) {}

    virtual ~RFC3986() = default;

    inline const RFC3986_SCHEME getScheme() const { return m_scheme; }
    inline const std::string& getPath() const { return m_path; }
    inline const std::string& getFragment() const { return m_fragment; }
    inline const bool isInvalid() const { return m_scheme == RFC3986_SCHEME::INVALID; }

    const RFC3986_SCHEME& setUrl(const std::string& url) { //"file://./assets/models/piso2_mestre.xml#Scene"
        const char* urlFile = "file://";
        size_t urlFileLen = 7;
        std::size_t mark1 = url.rfind('#');
        m_fragment = (mark1 != std::string::npos) ? url.substr(mark1 + 1, std::string::npos) : url;

        if (url.find(urlFile, 0, urlFileLen) != std::string::npos) {
            if (mark1 == std::string::npos) {
                m_scheme = RFC3986_SCHEME::INVALID;
                m_path = url.substr(urlFileLen, std::string::npos);
            } else {
                m_scheme = RFC3986_SCHEME::FILE;
                m_path = url.substr(urlFileLen, mark1 - urlFileLen);
            }
        } else {
            m_scheme = RFC3986_SCHEME::LOCAL;
            m_path = "";
        }

        return m_scheme;
    }
};
} // namespace Chimera