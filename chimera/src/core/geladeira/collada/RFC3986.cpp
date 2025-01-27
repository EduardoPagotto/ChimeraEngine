#include "chimera/core/collada/RFC3986.hpp"

namespace Chimera {
RFC3986::RFC3986(const std::string& url) { this->setUrl(url); }

const RFC3986_SCHEME& RFC3986::setUrl(const std::string& url) { //"file://./assets/models/piso2_mestre.xml#Scene"
    const char* urlFile = "file://";
    size_t urlFileLen = 7;
    std::size_t mark1 = url.rfind('#');
    fragment = (mark1 != std::string::npos) ? url.substr(mark1 + 1, std::string::npos) : url;

    if (url.find(urlFile, 0, urlFileLen) != std::string::npos) {
        if (mark1 == std::string::npos) {
            scheme = RFC3986_SCHEME::INVALID;
            path = url.substr(urlFileLen, std::string::npos);
        } else {
            scheme = RFC3986_SCHEME::FILE;
            path = url.substr(urlFileLen, mark1 - urlFileLen);
        }
    } else {
        scheme = RFC3986_SCHEME::LOCAL;
        path = "";
    }

    return scheme;
}

} // namespace Chimera