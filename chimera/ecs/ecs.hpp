#pragma once
#include <string>

namespace ce {

    /// @brief Tag usser in Registry
    struct TagInfo {
        std::string name{""};
        std::string id{""};
    };

    //    struct TagComponent {
    //        std::string tag = "";
    //        std::string id = "";
    //        uint32_t serial = 0;
    //        TagComponent() = default;
    //    };

} // namespace ce
