#include "chimera_collada/RFC3986.hpp"

namespace ce {
    RFC3986::RFC3986(const std::string& url) { this->set_url(url); }

    const RFC3986_SCHEME& RFC3986::set_url(const std::string& url) { //"file://./assets/models/piso2_mestre.xml#Scene"
        const char* url_file = "file://";
        size_t url_file_len = 7;
        std::size_t mark1 = url.rfind('#');
        fragment_ = (mark1 != std::string::npos) ? url.substr(mark1 + 1, std::string::npos) : url;

        if (url.find(url_file, 0, url_file_len) != std::string::npos) {
            if (mark1 == std::string::npos) {
                scheme_ = RFC3986_SCHEME::INVALID;
                path_ = url.substr(url_file_len, std::string::npos);
            } else {
                scheme_ = RFC3986_SCHEME::FILE;
                path_ = url.substr(url_file_len, mark1 - url_file_len);
            }
        } else {
            scheme_ = RFC3986_SCHEME::LOCAL;
            path_ = "";
        }

        return scheme_;
    }

} // namespace ce
