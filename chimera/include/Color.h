#ifndef COLOR_H_
#define COLOR_H_

#include <cstring>

namespace Chimera {

class Color {
public:
	explicit Color(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f) : r(_r) , g(_g) , b(_b) , a(_a) {}

	Color(const Color &_c) : r(_c.r), g(_c.g), b(_c.b), a(_c.a) {}

	void set(const Color _c) { 
		r = _c.r;
		g = _c.g;
		b = _c.b;
		a = _c.a; 
	}

	void set(float _r, float _g, float _b, float _a) { 
		r = _r;
		g = _g;
		b = _b;
		a = _a; 
	}

    static const Color ZERO;
    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;

    bool operator==(const Color& rhs) const;
    bool operator!=(const Color& rhs) const;

	/// Array accessor operator
	inline float operator [] ( const size_t i ) const {
		return *(&r+i);
	}

	/// Array accessor operator
	inline float& operator [] ( const size_t i ) {
		return *(&r+i);
	}	

	inline float* ptr()	{
		return &r;
	}
	
	inline const float* ptr() const	{
		return &r;
	}

	void saturate(void);

	Color saturateCopy(void) const {
        Color ret = *this;
        ret.saturate();
        return ret;
	}

union {
        float value[4];
        struct { float r,g,b,a; };
      };
};

//struct Material {
//    Color   diffuse;        /* Diffuse color RGBA */
//    Color   ambient;        /* Ambient color RGB */
//    Color   specular;       /* Specular 'shininess' */
//    Color   emissive;       /* Emissive color RGB */
//	float  shininess;
//};

}

#endif