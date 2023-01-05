constexpr uint8_t  default_alpha = 0xFF;
constexpr uint32_t default_pixel = (default_alpha << 24);

struct Pixel {
    union {
	    uint32_t n = default_pixel;
		struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
	};

	enum Mode { 
        NORMAL, 
        MASK, 
        ALPHA, 
        CUSTOM 
    };

	Pixel();
	Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = default_alpha);
	Pixel(uint32_t p);

	Pixel& operator = (const Pixel& v) = default;
	bool   operator ==(const Pixel& p) const;
	bool   operator !=(const Pixel& p) const;
	Pixel  operator * (const float i)  const;
	Pixel  operator / (const float i)  const;
	Pixel& operator *=(const float i);
	Pixel& operator /=(const float i);
	Pixel  operator + (const Pixel& p) const;
	Pixel  operator - (const Pixel& p) const;
	Pixel& operator +=(const Pixel& p);
	Pixel& operator -=(const Pixel& p);
	Pixel  operator * (const Pixel& p) const;
	Pixel& operator *=(const Pixel& p);

	Pixel  inv() const;
};

Pixel pixel_float(float red, float green, float blue, float alpha = 1.0f);
Pixel pixel_lerp (const engine::Pixel& p1, const engine::Pixel& p2, float t);

static const Pixel WHITE(255, 255, 255);
static const Pixel BLACK(0, 0, 0);