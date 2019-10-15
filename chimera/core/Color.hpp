// #ifndef __CHIMERA_COLOR__HPP
// #define __CHIMERA_COLOR__HPP

// #ifndef WIN32
// #include <cstring>
// #endif

// #include <glm/glm.hpp>

// namespace Chimera {

// class Color {
//   public:
//     explicit Color(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f) {
//         value[0] = _r;
//         value[1] = _g;
//         value[2] = _b;
//         value[3] = _a;
//     }

//     Color(const Color& _c) : value(_c.value) {}

//     void set(const Color _c) { value = _c.value; }

//     void set(float _r, float _g, float _b, float _a) {
//         value[0] = _r;
//         value[1] = _g;
//         value[2] = _b;
//         value[3] = _a;
//     }

//     static const Color ZERO;
//     static const Color BLACK;
//     static const Color WHITE;
//     static const Color RED;
//     static const Color GREEN;
//     static const Color BLUE;

//     bool operator==(const Color& rhs) const { return (value == rhs.value); }
//     bool operator!=(const Color& rhs) const { return !(*this == rhs); }

//     Color operator=(const Color& other) {
//         value = other.value;
//         return *this;
//     }

//     Color operator+(const Color& other) {
//         glm::vec4 vv = value + other.value;
//         return Color(vv[0], vv[1], vv[2], vv[3]);
//     }

//     // Array accessor operator
//     inline float operator[](const size_t i) const { return *(&value[i]); }

//     // Array accessor operator
//     inline float& operator[](const size_t i) { return *(&value[i]); }

//     inline float* ptr() { return &value[0]; }

//     inline const float* ptr() const { return &value[0]; }

//     inline glm::vec4 get() const { return value; }

//     void saturate(void);

//     Color saturateCopy(void) const;

//     glm::vec4 value;
// };
// } // namespace Chimera

// #endif