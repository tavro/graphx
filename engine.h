#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>

namespace engine {
    class Engine;
    class Sprite;

    /*
     *    Advanced Engine Config
     */
	constexpr uint8_t  mouse_buttons = 5;
	constexpr uint8_t  default_alpha = 0xFF;
	constexpr uint8_t  tab_size_in_spaces = 4;
	constexpr uint32_t default_pixel = (default_alpha << 24);
	constexpr size_t   MAX_VERTECIES = 128;

	enum Code { 
        FAIL = 0, 
        OK = 1, 
        NO_FILE = -1 
    };

    struct Pixel {
        union
		{
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
		Pixel  operator * (const float i) const;
		Pixel  operator / (const float i) const;
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
	Pixel pixel_lerp(const engine::Pixel& p1, const engine::Pixel& p2, float t);

	static const Pixel
		WHITE(255, 255, 255), BLACK(0, 0, 0);

    enum Key {
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD,
		EQUALS, COMMA, MINUS,
		OEM_1, OEM_2, OEM_3, OEM_4, OEM_5, OEM_6, OEM_7, OEM_8,
		CAPS_LOCK, ENUM_END
	};

    namespace Mouse {
		static constexpr int32_t LEFT = 0;
		static constexpr int32_t RIGHT = 1;
		static constexpr int32_t MIDDLE = 2;
	};

    struct ButtonState {
		bool pressed = false;
		bool released = false;
		bool held = false;
	};

    template <class T>
    struct Vector2D {
		T x = 0;
		T y = 0;

		Vector2D() : x(0), y(0) {}
		Vector2D(T _x, T _y) : x(_x), y(_y) {}
		Vector2D(const Vector2D& v) : x(v.x), y(v.y) {}

		Vector2D& operator=(const Vector2D& v) = default;
		
        T mag () const { return T(std::sqrt(x * x + y * y)); }
		T mag2() const { return x * x + y * y; }

		Vector2D  norm() const { 
            T r = 1 / mag(); 
            return Vector2D(x * r, y * r); 
        }

		Vector2D  perp () const { return Vector2D(-y, x); }
		Vector2D  floor() const { return Vector2D(std::floor(x), std::floor(y)); }
		Vector2D  ceil () const { return Vector2D(std::ceil(x), std::ceil(y)); }

		Vector2D  max(const Vector2D& v) const { return Vector2D(std::max(x, v.x), std::max(y, v.y)); }
		Vector2D  min(const Vector2D& v) const { return Vector2D(std::min(x, v.x), std::min(y, v.y)); }

		Vector2D  cart () { return { std::cos(y) * x, std::sin(y) * x }; }
		Vector2D  polar() { return { mag(), std::atan2(y, x) }; }

		Vector2D  clamp(const Vector2D& v1, const Vector2D& v2) const { return this->max(v1)->min(v2); }
		Vector2D  lerp (const Vector2D& v1, const double t) { return this->operator*(T(1.0 - t)) + (v1 * T(t)); }

		T dot  (const Vector2D& rhs) const { return this->x * rhs.x + this->y * rhs.y; }
		T cross(const Vector2D& rhs) const { return this->x * rhs.y - this->y * rhs.x; }

		Vector2D  operator +  (const Vector2D& rhs) const { return Vector2D(this->x + rhs.x, this->y + rhs.y); }
		Vector2D  operator -  (const Vector2D& rhs) const { return Vector2D(this->x - rhs.x, this->y - rhs.y); }

		Vector2D  operator *  (const T& rhs)           const { return Vector2D(this->x * rhs, this->y * rhs); }
		Vector2D  operator *  (const Vector2D& rhs)    const { return Vector2D(this->x * rhs.x, this->y * rhs.y); }
		Vector2D  operator /  (const T& rhs)           const { return Vector2D(this->x / rhs, this->y / rhs); }
		Vector2D  operator /  (const Vector2D& rhs)    const { return Vector2D(this->x / rhs.x, this->y / rhs.y); }

		Vector2D& operator += (const Vector2D& rhs) { 
            this->x += rhs.x; this->y += rhs.y; 
            return *this; 
        }

		Vector2D& operator -= (const Vector2D& rhs) { 
            this->x -= rhs.x; this->y -= rhs.y; 
            return *this; 
        }

		Vector2D& operator *= (const T& rhs) { 
            this->x *= rhs; this->y *= rhs; 
            return *this; 
        }

		Vector2D& operator /= (const T& rhs) { 
            this->x /= rhs; this->y /= rhs; 
            return *this; 
        }

		Vector2D& operator *= (const Vector2D& rhs) { 
            this->x *= rhs.x; this->y *= rhs.y; 
            return *this; 
        }

		Vector2D& operator /= (const Vector2D& rhs) { 
            this->x /= rhs.x; this->y /= rhs.y; 
            return *this; 
        }

		Vector2D  operator +  () const { return { +x, +y }; }
		Vector2D  operator -  () const { return { -x, -y }; }

		bool operator == (const Vector2D& rhs) const { return (this->x == rhs.x && this->y == rhs.y); }
		bool operator != (const Vector2D& rhs) const { return (this->x != rhs.x || this->y != rhs.y); }

		const std::string str() const { return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")"; }

		friend std::ostream& operator << (std::ostream& os, const Vector2D& rhs) { os << rhs.str(); return os; }

		operator Vector2D<int32_t>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
		operator Vector2D<float>  () const { return { static_cast<float>(this->x), static_cast<float>(this->y) }; }
		operator Vector2D<double> () const { return { static_cast<double>(this->x), static_cast<double>(this->y) }; }
	};

    template<class T> inline Vector2D<T> operator * (const float& lhs, const Vector2D<T>& rhs) { 
        return Vector2D<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y)); 
    }

	template<class T> inline Vector2D<T> operator * (const double& lhs, const Vector2D<T>& rhs) { 
        return Vector2D<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y)); 
    }

	template<class T> inline Vector2D<T> operator * (const int& lhs, const Vector2D<T>& rhs) { 
        return Vector2D<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y)); 
    }

	template<class T> inline Vector2D<T> operator / (const float& lhs, const Vector2D<T>& rhs) { 
        return Vector2D<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y)); 
    }

	template<class T> inline Vector2D<T> operator / (const double& lhs, const Vector2D<T>& rhs) { 
        return Vector2D<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y)); 
    }

	template<class T> inline Vector2D<T> operator / (const int& lhs, const Vector2D<T>& rhs) { 
        return Vector2D<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y)); 
    }

	template<class T, class U> inline bool operator < (const Vector2D<T>& lhs, const Vector2D<U>& rhs) { 
        return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x); 
    }

	template<class T, class U> inline bool operator > (const Vector2D<T>& lhs, const Vector2D<U>& rhs) { 
        return lhs.y > rhs.y || (lhs.y == rhs.y && lhs.x > rhs.x); 
    }

	typedef Vector2D<int32_t>  int_vector_2d;
	typedef Vector2D<uint32_t> unsigned_vector_2d;
	typedef Vector2D<float>    float_vector_2d;
	typedef Vector2D<double>   double_vector_2d;

    struct ResourceBuffer : public std::streambuf {
		ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size);
		std::vector<char> memory;
	};

    class ResourcePack : public std::streambuf {
	public:
		ResourcePack();
		~ResourcePack();

		bool add(const std::string& file);
		bool load(const std::string& file, const std::string& key);
		bool save(const std::string& file, const std::string& key);

		ResourceBuffer get_file_buffer(const std::string& file);
		bool loaded();
	private:
		struct ResourceFile { 
            uint32_t size; 
            uint32_t offset; 
        };
		std::map<std::string, ResourceFile> files;
		std::ifstream base_file;
		std::vector<char> scramble(const std::vector<char>& data, const std::string& key);
		std::string makeposix(const std::string& path);
	};

    class ImageLoader {
	public:
		ImageLoader() = default;
		virtual ~ImageLoader() = default;

		virtual engine::Code load_img_resource(engine::Sprite* spr, const std::string& img_file, engine::ResourcePack* pack) = 0;
		virtual engine::Code save_img_resource(engine::Sprite* spr, const std::string& img_file) = 0;
	};

    class Sprite {
	public:
		Sprite();
		Sprite(const std::string& img_file, engine::ResourcePack* pack = nullptr);
		Sprite(int32_t w, int32_t h);
		Sprite(const engine::Sprite&) = delete;
		~Sprite();

	public:
		engine::Code load_from_file(const std::string& img_file, engine::ResourcePack* pack = nullptr);

	public:
		int32_t width = 0;
		int32_t height = 0;

		enum Mode { 
            NORMAL, 
            PERIODIC, 
            CLAMP 
        };

		enum Flip { 
            NONE = 0, 
            HORIZ = 1, 
            VERT = 2 
        };

	public:
		void set_sample_mode(engine::Sprite::Mode mode = engine::Sprite::Mode::NORMAL);

		Pixel get_pixel(int32_t x, int32_t y) const;
		Pixel get_pixel(const engine::int_vector_2d& a) const;

		bool  set_pixel(int32_t x, int32_t y, Pixel p);
		bool  set_pixel(const engine::int_vector_2d& a, Pixel p);

		Pixel sample(float x, float y) const;
		Pixel sample(const engine::float_vector_2d& uv) const;

		Pixel sample_BL(float u, float v) const;
		Pixel sample_BL(const engine::float_vector_2d& uv) const;

		Pixel* get_data();

		engine::Sprite* duplicate();
		engine::Sprite* duplicate(const engine::int_vector_2d& pos, const engine::int_vector_2d& size);

		engine::int_vector_2d size() const;
		std::vector<engine::Pixel> col_data;
		Mode sample_mode = Mode::NORMAL;

		static std::unique_ptr<engine::ImageLoader> loader;
	};

    class Decal {
	public:
		Decal(engine::Sprite* spr, bool filter = false, bool clamp = true);
		Decal(const uint32_t existing_texture_resource, engine::Sprite* spr);
		virtual ~Decal();

		void update();
		void update_sprite();

	public:
		int32_t id = -1;
		engine::Sprite* sprite = nullptr;
		engine::float_vector_2d UV_scale = { 1.0f, 1.0f };
	};

    enum class DecalMode {
		NORMAL,
		ADDITIVE,
		MULTIPLICATIVE,
		STENCIL,
		ILLUMINATE,
		WIREFRAME,
		MODEL3D,
	};

	enum class DecalStructure {
		LINE,
		FAN,
		STRIP,
		LIST
	};

    class Renderable {
	public:
		Renderable() = default;		
		Renderable(Renderable&& r) : sprite(std::move(r.sprite)), decal(std::move(r.decal)) {}		
		Renderable(const Renderable&) = delete;

		engine::Code load(const std::string& file, ResourcePack* pack = nullptr, bool filter = false, bool clamp = true);

		void create(uint32_t width, uint32_t height, bool filter = false, bool clamp = true);

		engine::Decal* Decal() const;
		engine::Sprite* Sprite() const;

	private:
		std::unique_ptr<engine::Sprite> sprite = nullptr;
		std::unique_ptr<engine::Decal> decal = nullptr;
	};

    struct DecalInstance
	{
		engine::Decal* decal = nullptr;

		std::vector<engine::float_vector_2d> pos;
		std::vector<engine::float_vector_2d> uv;
		std::vector<float> w;
		std::vector<engine::Pixel> tint;

		engine::DecalMode mode = engine::DecalMode::NORMAL;
		engine::DecalStructure structure = engine::DecalStructure::FAN;

		uint32_t points = 0;
	};

    struct LayerDesc
	{
		engine::float_vector_2d offset = { 0, 0 };
		engine::float_vector_2d scale  = { 1, 1 };

		bool show   = false;
		bool update = false;

		engine::Renderable draw_target;
		
        uint32_t res_ID = 0;
		std::vector<DecalInstance> decal_instances;
		engine::Pixel tint = engine::WHITE;
		std::function<void()> func_hook = nullptr;
	};

    class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void       prepare_device() = 0;

		virtual engine::Code create_device(std::vector<void*> params, bool fullscreen, bool VSYNC) = 0;
		virtual engine::Code destroy_device() = 0;

		virtual void       display_frame  () = 0;
		virtual void       prepare_drawing() = 0;
		virtual void	   set_decal_mode (const engine::DecalMode& mode) = 0;
		virtual void       draw_layer_quad(const engine::float_vector_2d& offset, const engine::float_vector_2d& scale, const engine::Pixel tint) = 0;
		virtual void       draw_decal     (const engine::DecalInstance& decal) = 0;
		virtual uint32_t   create_texture (const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) = 0;
		virtual void       update_texture (uint32_t id, engine::Sprite* spr) = 0;
		virtual void       read_texture   (uint32_t id, engine::Sprite* spr) = 0;
		virtual uint32_t   delete_texture (const uint32_t id) = 0;
		virtual void       apply_texture  (uint32_t id) = 0;
		virtual void       update_viewport(const engine::int_vector_2d& pos, const engine::int_vector_2d& size) = 0;
		virtual void       clear_buffer   (engine::Pixel p, bool depth) = 0;

		static engine::Engine* engine_ptr;
	};

    class Platform
	{
	public:
		virtual ~Platform() = default;

		virtual engine::Code application_startup() = 0;
		virtual engine::Code application_cleanup() = 0;

		virtual engine::Code thread_startup() = 0;
		virtual engine::Code thread_cleanup() = 0;

		virtual engine::Code create_graphics(bool fullscreen, bool enable_VSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) = 0;
		virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool fullscreen) = 0;
		virtual engine::Code set_window_title(const std::string& s) = 0;

		virtual engine::Code start_system_event_loop() = 0;
		virtual engine::Code handle_system_event    () = 0;

		static engine::Engine* engine_ptr;
	};

	class EngineX;

    static std::unique_ptr<Renderer> renderer;
	static std::unique_ptr<Platform> platform;
	static std::map<size_t, uint8_t> keys;

    class Engine {
	public:
		Engine();
		virtual ~Engine();
    
	public:
		engine::Code construct (int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool fullscreen = false, bool vsync = false, bool cohesion = false);
		engine::Code start();

	public:
		virtual bool on_create();
		virtual bool on_update(float elapsed_time);
		virtual bool on_destroy();

		virtual void on_text_entry_complete(const std::string& stext);
		virtual bool on_console_cmd(const std::string& sCommand);

	public:
		bool is_focused() const;

		ButtonState get_key(Key k) const;
		ButtonState get_mouse(uint32_t b) const;

		int32_t get_mouse_x() const;
		int32_t get_mouse_y() const;
		int32_t get_mouse_wheel_delta() const;
		
		const engine::int_vector_2d& get_window_mouse() const;
		const engine::int_vector_2d& get_mouse_pos() const;

		static const std::map<size_t, uint8_t>& get_keys() { return keys; }

	public:
		int32_t screen_width() const;
		int32_t screen_height() const;
		
        int32_t get_draw_target_width() const;
		int32_t get_draw_target_height() const;

		engine::Sprite* get_draw_target() const;

		void set_screen_size(int w, int h);
        
		void set_draw_target(Sprite* target);
		
        uint32_t get_FPS() const;
		
        float get_elapsed_time() const;
		
        const engine::int_vector_2d& get_window_size() const;
		const engine::int_vector_2d& get_pixel_size() const;
		const engine::int_vector_2d& get_screen_pixel_size() const;
		const engine::int_vector_2d& get_screen_size() const;
		
        const std::vector<std::string>& get_dropped_files() const;
		const engine::int_vector_2d& get_dropped_files_point() const;

	public:
		void set_draw_target (uint8_t layer, bool bDirty = true);
		void enable_layer    (uint8_t layer, bool b);
		void set_layer_offset(uint8_t layer, const engine::float_vector_2d& offset);
		void set_layer_offset(uint8_t layer, float x, float y);
		void set_layer_scale (uint8_t layer, const engine::float_vector_2d& scale);
		void set_layer_scale (uint8_t layer, float x, float y);
		void set_layer_tint  (uint8_t layer, const engine::Pixel& tint);
		void set_layer_custom_render_func(uint8_t layer, std::function<void()> f);

		std::vector<LayerDesc>& get_layers();
		uint32_t create_layer();

		// NORMAL = No transparency
		// MASK   = Transparent if alpha is < 255
		// ALPHA  = Full transparency
		void set_pixel_mode(Pixel::Mode m);
		Pixel::Mode get_pixel_mode();

		void set_pixel_mode(std::function<engine::Pixel(const int x, const int y, const engine::Pixel& src, const engine::Pixel& dest)> pixel_mode);
		void set_pixel_blend(float blend);

	public: 
		virtual bool draw(int32_t x, int32_t y, Pixel p = engine::WHITE);
		bool draw(const engine::int_vector_2d& pos, Pixel p = engine::WHITE);

		void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = engine::WHITE, uint32_t pattern = 0xFFFFFFFF);
		void draw_line(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, Pixel p = engine::WHITE, uint32_t pattern = 0xFFFFFFFF);

		void draw_circle(int32_t x, int32_t y, int32_t radius, Pixel p = engine::WHITE, uint8_t mask = 0xFF);
		void draw_circle(const engine::int_vector_2d& pos, int32_t radius, Pixel p = engine::WHITE, uint8_t mask = 0xFF);
		void fill_circle(int32_t x, int32_t y, int32_t radius, Pixel p = engine::WHITE);
		void fill_circle(const engine::int_vector_2d& pos, int32_t radius, Pixel p = engine::WHITE);
		
		void draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = engine::WHITE);
		void draw_rect(const engine::int_vector_2d& pos, const engine::int_vector_2d& size, Pixel p = engine::WHITE);
		void fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = engine::WHITE);
		void fill_rect(const engine::int_vector_2d& pos, const engine::int_vector_2d& size, Pixel p = engine::WHITE);
		
		void draw_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = engine::WHITE);
		void draw_triangle(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, const engine::int_vector_2d& pos3, Pixel p = engine::WHITE);
		void fill_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = engine::WHITE);
		void fill_triangle(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, const engine::int_vector_2d& pos3, Pixel p = engine::WHITE);
		
		void fill_textured_triangle(const std::vector<engine::float_vector_2d>& points, std::vector<engine::float_vector_2d> tex, std::vector<engine::Pixel> color, engine::Sprite* spr_tex);
		void fill_textured_polygon(const std::vector<engine::float_vector_2d>& points, const std::vector<engine::float_vector_2d>& tex, const std::vector<engine::Pixel>& color, engine::Sprite* spr_tex, engine::DecalStructure structure = engine::DecalStructure::LIST);
		
		void draw_sprite(int32_t x, int32_t y, Sprite* sprite, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
		void draw_sprite(const engine::int_vector_2d& pos, Sprite* sprite, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
		void draw_partial_sprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
		void draw_partial_sprite(const engine::int_vector_2d& pos, Sprite* sprite, const engine::int_vector_2d& source_pos, const engine::int_vector_2d& size, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
		
		void draw_string(int32_t x, int32_t y, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
		void draw_string(const engine::int_vector_2d& pos, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
		
		engine::int_vector_2d get_text_size(const std::string& s);
		
		void draw_string_prop(int32_t x, int32_t y, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
		void draw_string_prop(const engine::int_vector_2d& pos, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
		
		engine::int_vector_2d get_text_size_prop(const std::string& s);

		void set_decal_mode(const engine::DecalMode& mode);
		void set_decal_structure(const engine::DecalStructure& structure);
		
		void draw_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const engine::float_vector_2d& scale = { 1.0f,1.0f }, const engine::Pixel& tint = engine::WHITE);
		
		void draw_partial_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::float_vector_2d& scale = { 1.0f,1.0f }, const engine::Pixel& tint = engine::WHITE);
		void draw_partial_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, engine::Decal* decal, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
		
		void draw_explicit_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::float_vector_2d* uv, const engine::Pixel* col, uint32_t elements = 4);
		
		void draw_warped_decal(engine::Decal* decal, const engine::float_vector_2d(&pos)[4], const engine::Pixel& tint = engine::WHITE);
		void draw_warped_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::Pixel& tint = engine::WHITE);
		void draw_warped_decal(engine::Decal* decal, const std::array<engine::float_vector_2d, 4>& pos, const engine::Pixel& tint = engine::WHITE);
		
		void draw_partial_warped_decal(engine::Decal* decal, const engine::float_vector_2d(&pos)[4], const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
		void draw_partial_warped_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
		void draw_partial_warped_decal(engine::Decal* decal, const std::array<engine::float_vector_2d, 4>& pos, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
		
		void draw_rotated_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const float angle, const engine::float_vector_2d& center = { 0.0f, 0.0f }, const engine::float_vector_2d& scale = { 1.0f,1.0f }, const engine::Pixel& tint = engine::WHITE);
		void draw_partial_rotated_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const float angle, const engine::float_vector_2d& center, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::float_vector_2d& scale = { 1.0f, 1.0f }, const engine::Pixel& tint = engine::WHITE);
		
		void draw_string_decal(const engine::float_vector_2d& pos, const std::string& text, const Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		
		void draw_string_prop_decal(const engine::float_vector_2d& pos, const std::string& text, const Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		
		void draw_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col = engine::WHITE);
		void fill_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col = engine::WHITE);
		
		void gradient_fill_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col_TL, const engine::Pixel col_BL, const engine::Pixel col_BR, const engine::Pixel col_TR);
		
		void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const engine::Pixel tint = engine::WHITE);
		void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<float>& depth, const std::vector<engine::float_vector_2d>& uv, const engine::Pixel tint = engine::WHITE);
		void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const std::vector<engine::Pixel>& tint);
		void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const std::vector<engine::Pixel>& colors, const engine::Pixel tint);

		void draw_line_decal(const engine::float_vector_2d& pos1, const engine::float_vector_2d& pos2, Pixel p = engine::WHITE);
		void draw_rotated_string_decal(const engine::float_vector_2d& pos, const std::string& text, const float angle, const engine::float_vector_2d& center = { 0.0f, 0.0f }, const engine::Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		void draw_rotated_string_prop_decal(const engine::float_vector_2d& pos, const std::string& text, const float angle, const engine::float_vector_2d& center = { 0.0f, 0.0f }, const engine::Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		
		void clear(Pixel p);
		void clear_buffer(Pixel p, bool depth = true);
		
		engine::Sprite* get_font_sprite();

		bool clip_line_to_screen(engine::int_vector_2d& pos1, engine::int_vector_2d& pos2);

		void enable_pixel_transfer(const bool enable = true);

		void console_show(const engine::Key &key_exit, bool suspend_time = true);
		bool is_console_showing() const;
		void console_clear();
		std::stringstream& console_out();
		void console_capture_std_out(const bool capture);

		void text_entry_enable(const bool enable, const std::string& text = "");
		std::string text_entry_get_string() const;
		int32_t text_entry_get_cursor() const;
		bool is_text_entry_enabled() const;

	private:
		void update_text_entry();
		void update_console();

	public:
		std::string app_name;

	private:
		engine::Sprite*         draw_target = nullptr;
		Pixel::Mode	            pixel_mode = Pixel::NORMAL;
		float		            blend_factor = 1.0f;
		engine::int_vector_2d	screen_size = { 256, 240 };
		engine::float_vector_2d	inv_screen_size = { 1.0f / 256.0f, 1.0f / 240.0f };
		engine::int_vector_2d	pixel_size = { 4, 4 };
		engine::int_vector_2d   screen_pixel_size = { 4, 4 };
		engine::int_vector_2d	mouse_pos = { 0, 0 };
		int32_t		            mouse_wheel_delta = 0;
		engine::int_vector_2d	mouse_pos_cache = { 0, 0 };
		engine::int_vector_2d   mouse_window_pos = { 0, 0 };
		int32_t		            mouse_wheel_delta_cache = 0;
		engine::int_vector_2d	window_size = { 0, 0 };
		engine::int_vector_2d	view_pos = { 0, 0 };
		engine::int_vector_2d	view_size = { 0,0 };
		bool		            fullscreen = false;
		engine::float_vector_2d	pixel = { 1.0f, 1.0f };
		bool		            has_input_focus = false;
		bool		            has_mouse_focus = false;
		bool		            enable_VSYNC = false;
		float		            frame_timer = 1.0f;
		float		            last_elapsed = 0.0f;
		int			            frame_count = 0;		
		bool                    suspend_texture_transfer = false;
		Renderable              font_renderable;
		std::vector<LayerDesc>  layers;
		uint8_t		            target_layer = 0;
		uint32_t	            last_FPS = 0;
		bool                    pixel_cohesion = false;
		DecalMode               decal_mode = DecalMode::NORMAL;
		DecalStructure          decal_structure = DecalStructure::FAN;

		std::function<engine::Pixel(const int x, const int y, const engine::Pixel&, const engine::Pixel&)> func_pixel_mode;
		std::chrono::time_point<std::chrono::system_clock> time_point1, time_point2;
		std::vector<engine::int_vector_2d> font_spacing;

		std::vector<std::string> dropped_files;
		std::vector<std::string> dropped_files_cache;

		engine::int_vector_2d dropped_files_point;
		engine::int_vector_2d dropped_files_point_cache;

		bool console_show = false;
		bool console_suspend_time = false;
		
        engine::Key key_console_exit = engine::Key::F1;
		
        std::stringstream console_output;
		std::streambuf* buf_old_cout = nullptr;
		
        engine::int_vector_2d console_size;
		engine::int_vector_2d console_cursor = { 0,0 };
		engine::float_vector_2d console_character_scale = { 1.0f, 2.0f };

		std::vector<std::string> console_lines;
		std::list<std::string> cmd_history;
		std::list<std::string>::iterator cmd_history_it;

		bool text_entry_enable = false;
		std::string text_entry_string = "";
		int32_t text_entry_cursor = 0;
		std::vector<std::tuple<engine::Key, std::string, std::string>> keyboard;

		bool		key_new_state[256] = { 0 };
		bool		key_old_state[256] = { 0 };
		ButtonState	keyboard_state[256] = { 0 };

		bool		mouse_new_state[mouse_buttons] = { 0 };
		bool		mouse_old_state[mouse_buttons] = { 0 };
		ButtonState	mouse_state[mouse_buttons] = { 0 };

		void		EngineThread();

		static std::atomic<bool> atom_active;

	public:
		void engine_update_mouse(int32_t x, int32_t y);
		void engine_update_mouse_wheel(int32_t delta);
		void engine_update_window_size(int32_t x, int32_t y);
		void engine_update_viewport();
		void engine_construct_fontsheet();
		void engine_core_update();
		void engine_prepare_engine();
		void engine_update_mouse_state(int32_t button, bool state);
		void engine_update_key_state(int32_t key, bool state);
		void engine_update_mouse_focus(bool state);
		void engine_update_key_focus(bool state);
		void engine_terminate();
		void engine_drop_files(int32_t x, int32_t y, const std::vector<std::string>& files);
		void engine_reanimate();
		bool engine_is_running();

		// chooses which components to compile
		virtual void engine_configure_system();

	public:
		friend class EngineX;
		void enginex_register(engine::EngineX* enginex);

	private:
		std::vector<engine::EngineX*> extensions;
	};

	class EngineX
	{
		friend class engine::Engine;
	public:
		EngineX(bool hook = false);

	protected:
		virtual void on_before_create();
		virtual void on_after_create ();
		virtual bool on_before_update(float &elapsed_time);
		virtual void on_after_update (float elapsed_time);

	protected:
		static Engine* engine_ptr;
	};
}