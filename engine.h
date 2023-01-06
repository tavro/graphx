#ifndef ENGINE_DEF
#define ENGINE_DEF

#pragma region std_includes
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
#pragma endregion

#define ENGINE_VERSION 001

// COMPILER CONFIGURATION
#pragma region compiler_config
#define USE_EXPERIMENTAL_FS
#if defined(_WIN32)
	#if _MSC_VER >= 1920 && _MSVC_LANG >= 201703L
		#undef USE_EXPERIMENTAL_FS
	#endif
#endif
#if defined(__linux__) || defined(__MINGW32__) || defined(__EMSCRIPTEN__) || defined(__FreeBSD__) || defined(__APPLE__)
	#if __cplusplus >= 201703L
		#undef USE_EXPERIMENTAL_FS
	#endif
#endif

#if !defined(ENGINE_KEYBOARD_UK)
	#define ENGINE_KEYBOARD_UK
#endif


#if defined(USE_EXPERIMENTAL_FS) || defined(FORCE_EXPERIMENTAL_FS)
	// C++14
	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#include <experimental/filesystem>
	namespace _gfs = std::experimental::filesystem::v1;
#else
	// C++17
	#include <filesystem>
	namespace _gfs = std::filesystem;
#endif

#if defined(UNICODE) || defined(_UNICODE)
	#define engineT(s) L##s
#else
	#define engineT(s) s
#endif

#define UNUSED(x) (void)(x)

// PLATFORM SELECTION
#if defined(ENGINE_PGE_HEADLESS)
	#define ENGINE_PLATFORM_HEADLESS
	#define ENGINE_GFX_HEADLESS
	#if !defined(ENGINE_IMAGE_STB) && !defined(ENGINE_IMAGE_GDI) && !defined(ENGINE_IMAGE_LIBPNG)
		#define ENGINE_IMAGE_HEADLESS
	#endif
#endif

#if !defined(ENGINE_PLATFORM_WINAPI) && !defined(ENGINE_PLATFORM_X11) && !defined(ENGINE_PLATFORM_GLUT) && !defined(ENGINE_PLATFORM_EMSCRIPTEN) && !defined(ENGINE_PLATFORM_HEADLESS)
	#if !defined(ENGINE_PLATFORM_CUSTOM_EX)
		#if defined(_WIN32)
			#define ENGINE_PLATFORM_WINAPI
		#endif
		#if defined(__linux__) || defined(__FreeBSD__)
			#define ENGINE_PLATFORM_X11
		#endif
		#if defined(__APPLE__)
			#define GL_SILENCE_DEPRECATION
			#define ENGINE_PLATFORM_GLUT
		#endif
		#if defined(__EMSCRIPTEN__)
			#define ENGINE_PLATFORM_EMSCRIPTEN
		#endif
	#endif
#endif

#if defined(ENGINE_PLATFORM_GLUT) || defined(ENGINE_PLATFORM_EMSCRIPTEN)
	#define PGE_USE_CUSTOM_START
#endif

#if !defined(ENGINE_GFX_OPENGL10) && !defined(ENGINE_GFX_OPENGL33) && !defined(ENGINE_GFX_DIRECTX10) && !defined(ENGINE_GFX_HEADLESS)
	#if !defined(ENGINE_GFX_CUSTOM_EX)
		#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
			#define ENGINE_GFX_OPENGL33
		#else
			#define ENGINE_GFX_OPENGL10
		#endif
	#endif
#endif

#if !defined(ENGINE_IMAGE_STB) && !defined(ENGINE_IMAGE_GDI) && !defined(ENGINE_IMAGE_LIBPNG) && !defined(ENGINE_IMAGE_HEADLESS)
	#if !defined(ENGINE_IMAGE_CUSTOM_EX)
		#if defined(_WIN32)
			#define	ENGINE_IMAGE_GDI
		#endif
		#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
			#define	ENGINE_IMAGE_LIBPNG
		#endif
	#endif
#endif

// DEPENDENCIES
#if !defined(ENGINE_PGE_HEADLESS)
#if defined(ENGINE_PLATFORM_WINAPI)	
	#define _WINSOCKAPI_
		#if !defined(VC_EXTRALEAN)
		#define VC_EXTRALEAN
	#endif
	#if !defined(NOMINMAX)
		#define NOMINMAX
	#endif

	#if !defined(_WIN32_WINNT)
		#ifdef HAVE_MSMF
			#define _WIN32_WINNT 0x0600
		#else
			#define _WIN32_WINNT 0x0500
		#endif
	#endif

	#include <windows.h>
	#undef _WINSOCKAPI_
#endif

#if defined(ENGINE_PLATFORM_X11)
	namespace X11
	{
		#include <X11/X.h>
		#include <X11/Xlib.h>
	}
#endif

#if defined(ENGINE_PLATFORM_GLUT)
	#if defined(__linux__)
		#include <GL/glut.h>
		#include <GL/freeglut_ext.h>
	#endif
	#if defined(__APPLE__)
		#include <GLUT/glut.h>
	#include <objc/message.h>
	#include <objc/NSObjCRuntime.h>
	#endif
#endif
#endif

#if defined(ENGINE_PGE_HEADLESS)
#if defined max
#undef max
#endif
#if defined min
#undef min
#endif
#endif
#pragma endregion

#pragma region engine_declaration
namespace engine {
    class Engine;
    class Sprite;

    // ADVANCED ENGINE CONFIG
	constexpr uint8_t  mouse_buttons = 5;
	constexpr uint8_t  default_alpha = 0xFF;
	constexpr uint8_t  tab_size_in_spaces = 4;
	constexpr uint32_t default_pixel = (default_alpha << 24);
	constexpr size_t   ENGINE_MAX_VERTS = 128;

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

#if !defined(ENGINE_IGNORE_VEC2D)
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
#endif

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

		static engine::Engine* ptr_engine;
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

		static engine::Engine* ptr_engine;
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
		virtual bool on_console_command(const std::string& sCommand);

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

#ifdef ENGINE_ENABLE_EXPERIMENTAL
		void LW3D_View(const std::array<float, 16>& m);
		void LW3D_World(const std::array<float, 16>& m);
		void LW3D_Projection(const std::array<float, 16>& m);
		
		void LW3D_DrawTriangles(engine::Decal* decal, const std::vector<std::array<float,3>>& pos, const std::vector<engine::float_vector_2d>& tex, const std::vector<engine::Pixel>& col);

		void LW3D_ModelTranslate(const float x, const float y, const float z);
		
		void LW3D_SetCameraAtTarget(const float fEyeX, const float fEyeY, const float fEyeZ,
			const float fTargetX, const float fTargetY, const float fTargetZ,
			const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);
		void LW3D_SetCameraAlongDirection(const float fEyeX, const float fEyeY, const float fEyeZ,
			const float fDirX, const float fDirY, const float fDirZ,
			const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);

		void LW3D_EnableDepthTest(const bool bEnableDepth);
		void LW3D_EnableBackfaceCulling(const bool bEnableCull);
#endif

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

		bool show_console = false;
		bool console_suspend_time = false;
		
        engine::Key key_console_exit = engine::Key::F1;
		
        std::stringstream console_output;
		std::streambuf* buf_old_cout = nullptr;
		
        engine::int_vector_2d console_size;
		engine::int_vector_2d console_cursor = { 0,0 };
		engine::float_vector_2d console_character_scale = { 1.0f, 2.0f };

		std::vector<std::string> console_lines;
		std::list<std::string> command_history;
		std::list<std::string>::iterator command_history_it;

		bool enable_text_entry = false;
		std::string text_entry_string = "";
		int32_t text_entry_cursor = 0;
		std::vector<std::tuple<engine::Key, std::string, std::string>> keyboard;

		bool		key_new_state[256] = { 0 };
		bool		key_old_state[256] = { 0 };
		ButtonState	keyboard_state[256] = { 0 };

		bool		mouse_new_state[mouse_buttons] = { 0 };
		bool		mouse_old_state[mouse_buttons] = { 0 };
		ButtonState	mouse_state[mouse_buttons] = { 0 };

		void		engine_thread();

		static std::atomic<bool> atom_active;

	public:
		void engine_update_mouse(int32_t x, int32_t y);
		void engine_update_mouse_wheel(int32_t delta);
		void engine_update_window_size(int32_t x, int32_t y);
		void engine_update_viewport();
		void engine_construct_fontsheet();
		void engine_core_update();
		void engine_prepare();
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
		static Engine* engine;
	};
}

#pragma endregion

#pragma region opengl33_iface

#if defined(ENGINE_GFX_OPENGL33)

	#if defined(ENGINE_PLATFORM_WINAPI)
		#include <gl/GL.h>
		#define CALLSTYLE __stdcall
	#endif

	#if defined(__linux__) || defined(__FreeBSD__)
		#include <GL/gl.h>
	#endif

	#if defined(ENGINE_PLATFORM_X11)
		namespace X11
		{#include <GL/glx.h>}
		#define CALLSTYLE 
	#endif

	#if defined(__APPLE__)
		#define GL_SILENCE_DEPRECATION
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
	#endif

	#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
		#define GL_GLEXT_PROTOTYPES
		#include <GLES2/gl2ext.h>
		#include <emscripten/emscripten.h>
		#define CALLSTYLE
		#define GL_CLAMP GL_CLAMP_TO_EDGE
	#endif

namespace engine {
	typedef char GLchar;
	typedef ptrdiff_t GLsizeiptr;

	typedef GLuint CALLSTYLE locCreateShader_t(GLenum type);
	typedef GLuint CALLSTYLE locCreateProgram_t(void);
	typedef void CALLSTYLE locDeleteShader_t(GLuint shader);
	typedef void CALLSTYLE locCompileShader_t(GLuint shader);
	typedef void CALLSTYLE locLinkProgram_t(GLuint program);
	typedef void CALLSTYLE locDeleteProgram_t(GLuint program);
	typedef void CALLSTYLE locAttachShader_t(GLuint program, GLuint shader);
	typedef void CALLSTYLE locBindBuffer_t(GLenum target, GLuint buffer);
	typedef void CALLSTYLE locBufferData_t(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void CALLSTYLE locGenBuffers_t(GLsizei n, GLuint* buffers);
	typedef void CALLSTYLE locVertexAttribPointer_t(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void CALLSTYLE locEnableVertexAttribArray_t(GLuint index);
	typedef void CALLSTYLE locUseProgram_t(GLuint program);
	typedef void CALLSTYLE locBindVertexArray_t(GLuint array);
	typedef void CALLSTYLE locGenVertexArrays_t(GLsizei n, GLuint* arrays);
	typedef void CALLSTYLE locGetShaderInfoLog_t(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
	typedef GLint CALLSTYLE locGetUniformLocation_t(GLuint program, const GLchar* name);
	typedef void CALLSTYLE locUniform1f_t(GLint location, GLfloat v0);
	typedef void CALLSTYLE locUniform1i_t(GLint location, GLint v0);
	typedef void CALLSTYLE locUniform2fv_t(GLint location, GLsizei count, const GLfloat* value);
	typedef void CALLSTYLE locActiveTexture_t(GLenum texture);
	typedef void CALLSTYLE locGenFrameBuffers_t(GLsizei n, GLuint* ids);
	typedef void CALLSTYLE locBindFrameBuffer_t(GLenum target, GLuint fb);
	typedef GLenum CALLSTYLE locCheckFrameBufferStatus_t(GLenum target);
	typedef void CALLSTYLE locDeleteFrameBuffers_t(GLsizei n, const GLuint* fbs);
	typedef void CALLSTYLE locFrameBufferTexture2D_t(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void CALLSTYLE locDrawBuffers_t(GLsizei n, const GLenum* bufs);
	typedef void CALLSTYLE locBlendFuncSeparate_t(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

#if defined(ENGINE_PLATFORM_WINAPI)
	typedef void __stdcall locSwapInterval_t(GLsizei n);
#endif

#if defined(ENGINE_PLATFORM_X11)
	typedef int(locSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
#endif

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
	typedef EGLBoolean(locSwapInterval_t)(EGLDisplay display, EGLint interval);
#else
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
#endif

}
#endif
#pragma endregion

#endif // END ENGINE

#ifdef APPLICATION_DEF
#undef APPLICATION_DEF

#pragma region engine_implementation
namespace engine {
	Pixel::Pixel() { 
        r = 0; 
        g = 0; 
        b = 0; 
        a = default_alpha; 
    }

	Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) { 
        n = red | (green << 8) | (blue << 16) | (alpha << 24); 
    } 

	Pixel::Pixel(uint32_t p) { n = p; }

	bool Pixel::operator==(const Pixel& p) const { return n == p.n; }

	bool Pixel::operator!=(const Pixel& p) const { return n != p.n; }

	Pixel  Pixel::operator * (const float i) const {
		float fR = std::min(255.0f, std::max(0.0f, float(r) * i));
		float fG = std::min(255.0f, std::max(0.0f, float(g) * i));
		float fB = std::min(255.0f, std::max(0.0f, float(b) * i));
		return Pixel(uint8_t(fR), uint8_t(fG), uint8_t(fB), a);
	}

	Pixel  Pixel::operator / (const float i) const {
		float fR = std::min(255.0f, std::max(0.0f, float(r) / i));
		float fG = std::min(255.0f, std::max(0.0f, float(g) / i));
		float fB = std::min(255.0f, std::max(0.0f, float(b) / i));
		return Pixel(uint8_t(fR), uint8_t(fG), uint8_t(fB), a);
	}

	Pixel& Pixel::operator *=(const float i) {
		this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * i)));
		this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * i)));
		this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * i)));
		return *this;
	}

	Pixel& Pixel::operator /=(const float i) {
		this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) / i)));
		this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) / i)));
		this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) / i)));
		return *this;
	}

	Pixel  Pixel::operator + (const Pixel& p) const {
		uint8_t nR = uint8_t(std::min(255, std::max(0, int(r) + int(p.r))));
		uint8_t nG = uint8_t(std::min(255, std::max(0, int(g) + int(p.g))));
		uint8_t nB = uint8_t(std::min(255, std::max(0, int(b) + int(p.b))));
		return Pixel(nR, nG, nB, a);
	}

	Pixel  Pixel::operator - (const Pixel& p) const {
		uint8_t nR = uint8_t(std::min(255, std::max(0, int(r) - int(p.r))));
		uint8_t nG = uint8_t(std::min(255, std::max(0, int(g) - int(p.g))));
		uint8_t nB = uint8_t(std::min(255, std::max(0, int(b) - int(p.b))));
		return Pixel(nR, nG, nB, a);
	}

	Pixel& Pixel::operator += (const Pixel& p) {
		this->r = uint8_t(std::min(255, std::max(0, int(r) + int(p.r))));
		this->g = uint8_t(std::min(255, std::max(0, int(g) + int(p.g))));
		this->b = uint8_t(std::min(255, std::max(0, int(b) + int(p.b))));
		return *this;
	}

	Pixel& Pixel::operator -= (const Pixel& p) {
		this->r = uint8_t(std::min(255, std::max(0, int(r) - int(p.r))));
		this->g = uint8_t(std::min(255, std::max(0, int(g) - int(p.g))));
		this->b = uint8_t(std::min(255, std::max(0, int(b) - int(p.b))));
		return *this;
	}

	Pixel Pixel::operator * (const Pixel& p) const {
		uint8_t nR = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * float(p.r) / 255.0f)));
		uint8_t nG = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * float(p.g) / 255.0f)));
		uint8_t nB = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * float(p.b) / 255.0f)));
		uint8_t nA = uint8_t(std::min(255.0f, std::max(0.0f, float(a) * float(p.a) / 255.0f)));
		return Pixel(nR, nG, nB, nA);
	}

	Pixel& Pixel::operator *=(const Pixel& p) {
		this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * float(p.r) / 255.0f)));
		this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * float(p.g) / 255.0f)));
		this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * float(p.b) / 255.0f)));
		this->a = uint8_t(std::min(255.0f, std::max(0.0f, float(a) * float(p.a) / 255.0f)));
		return *this;
	}

	Pixel Pixel::inv() const {
		uint8_t nR = uint8_t(std::min(255, std::max(0, 255 - int(r))));
		uint8_t nG = uint8_t(std::min(255, std::max(0, 255 - int(g))));
		uint8_t nB = uint8_t(std::min(255, std::max(0, 255 - int(b))));
		return Pixel(nR, nG, nB, a);
	}

	Pixel pixel_float(float red, float green, float blue, float alpha) { 
        return Pixel(uint8_t(red * 255.0f), uint8_t(green * 255.0f), uint8_t(blue * 255.0f), uint8_t(alpha * 255.0f)); 
    }

	Pixel pixel_lerp(const engine::Pixel& p1, const engine::Pixel& p2, float t) { 
        return (p2 * t) + p1 * (1.0f - t); 
    }

	Sprite::Sprite() { 
        width = 0; 
        height = 0; 
    }

	Sprite::Sprite(const std::string& img_file, engine::ResourcePack* pack) { 
        load_from_file(img_file, pack); 
    }

	Sprite::Sprite(int32_t w, int32_t h) {
		width = w;		
        height = h;
		col_data.resize(width * height);
		col_data.resize(width * height, default_pixel);
	}

	Sprite::~Sprite() { col_data.clear(); }

	void Sprite::set_sample_mode(engine::Sprite::Mode mode) { sample_mode = mode; }

	Pixel Sprite::get_pixel(const engine::int_vector_2d& a) const { return get_pixel(a.x, a.y); }
	Pixel Sprite::get_pixel(int32_t x, int32_t y) const {
		if (sample_mode == engine::Sprite::Mode::NORMAL) {
			if (x >= 0 && x < width && y >= 0 && y < height)
				return col_data[y * width + x];
			else
				return Pixel(0, 0, 0, 0);
		}
		else {
			if (sample_mode == engine::Sprite::Mode::PERIODIC)
				return col_data[abs(y % height) * width + abs(x % width)];
			else
				return col_data[std::max(0, std::min(y, height-1)) * width + std::max(0, std::min(x, width-1))];
		}
	}

	bool Sprite::set_pixel(const engine::int_vector_2d& a, Pixel p) { return set_pixel(a.x, a.y, p); }
	bool Sprite::set_pixel(int32_t x, int32_t y, Pixel p)
	{
		if (x >= 0 && x < width && y >= 0 && y < height) {
			col_data[y * width + x] = p;
			return true;
		}
		else {
			return false;
        }
	}

	Pixel Sprite::sample(float x, float y) const {
		int32_t sx = std::min((int32_t)((x * (float)width)), width - 1);
		int32_t sy = std::min((int32_t)((y * (float)height)), height - 1);
		return get_pixel(sx, sy);
	}
	Pixel Sprite::sample(const engine::float_vector_2d& uv) const { return sample(uv.x, uv.y); }

	Pixel Sprite::sample_BL(float u, float v) const {
		u = u * width - 0.5f;
		v = v * height - 0.5f;

		int x = (int)floor(u);
		int y = (int)floor(v);

		float u_ratio = u - x;
		float v_ratio = v - y;

		float u_opposite = 1 - u_ratio;
		float v_opposite = 1 - v_ratio;

		engine::Pixel p1 = get_pixel(std::max(x, 0), std::max(y, 0));
		engine::Pixel p2 = get_pixel(std::min(x + 1, (int)width - 1), std::max(y, 0));
		engine::Pixel p3 = get_pixel(std::max(x, 0), std::min(y + 1, (int)height - 1));
		engine::Pixel p4 = get_pixel(std::min(x + 1, (int)width - 1), std::min(y + 1, (int)height - 1));

		return engine::Pixel(
			(uint8_t)((p1.r * u_opposite + p2.r * u_ratio) * v_opposite + (p3.r * u_opposite + p4.r * u_ratio) * v_ratio),
			(uint8_t)((p1.g * u_opposite + p2.g * u_ratio) * v_opposite + (p3.g * u_opposite + p4.g * u_ratio) * v_ratio),
			(uint8_t)((p1.b * u_opposite + p2.b * u_ratio) * v_opposite + (p3.b * u_opposite + p4.b * u_ratio) * v_ratio));
	}

	Pixel Sprite::sample_BL(const engine::float_vector_2d& uv) const { return sample_BL(uv.x, uv.y); }

	Pixel* Sprite::get_data() { return col_data.data(); }

	engine::Code Sprite::load_from_file(const std::string& img_file, engine::ResourcePack* pack) {
		UNUSED(pack);
		return loader->load_img_resource(this, img_file, pack);
	}

	engine::Sprite* Sprite::duplicate() {
		engine::Sprite* spr = new engine::Sprite(width, height);
		std::memcpy(spr->get_data(), get_data(), width * height * sizeof(engine::Pixel));
		spr->sample_mode = sample_mode;
		return spr;
	}

	engine::Sprite* Sprite::duplicate(const engine::int_vector_2d& pos, const engine::int_vector_2d& size) {
		engine::Sprite* spr = new engine::Sprite(size.x, size.y);
		for (int y = 0; y < size.y; y++)
			for (int x = 0; x < size.x; x++)
				spr->set_pixel(x, y, get_pixel(pos.x + x, pos.y + y));
		return spr;
	}

	engine::int_vector_2d engine::Sprite::size() const { return { width, height }; }

	Decal::Decal(engine::Sprite* spr, bool filter, bool clamp) {
		id = -1;
		if (spr == nullptr) return;
		sprite = spr;
		id = renderer->create_texture(sprite->width, sprite->height, filter, clamp);
		update();
	}

	Decal::Decal(const uint32_t existing_texture_resource, engine::Sprite* spr) {
		if (spr == nullptr) return;
		id = existing_texture_resource;
	}

	void Decal::update() {
		if (sprite == nullptr) return;
		UV_scale = { 1.0f / float(sprite->width), 1.0f / float(sprite->height) };
		renderer->apply_texture(id);
		renderer->update_texture(id, sprite);
	}

	void Decal::update_sprite() {
		if (sprite == nullptr) return;
		renderer->apply_texture(id);
		renderer->read_texture(id, sprite);
	}

	Decal::~Decal() {
		if (id != -1) {
			renderer->delete_texture(id);
			id = -1;
		}
	}

	void Renderable::create(uint32_t width, uint32_t height, bool filter, bool clamp) {
		sprite = std::make_unique<engine::Sprite>(width, height);
		decal = std::make_unique<engine::Decal>(sprite.get(), filter, clamp);
	}

	engine::Code Renderable::load(const std::string& file, ResourcePack* pack, bool filter, bool clamp) {
		sprite = std::make_unique<engine::Sprite>();
		if (sprite->load_from_file(file, pack) == engine::Code::OK) {
			decal = std::make_unique<engine::Decal>(sprite.get(), filter, clamp);
			return engine::Code::OK;
		}
		else {
			sprite.release();
			sprite = nullptr;
			return engine::Code::NO_FILE;
		}
	}

	engine::Decal* Renderable::Decal  () const { return decal.get(); }
	engine::Sprite* Renderable::Sprite() const { return sprite.get(); }

	ResourceBuffer::ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size) {
		memory.resize(size);
		ifs.seekg(offset); 
        ifs.read(memory.data(), memory.size());
		setg(memory.data(), memory.data(), memory.data() + size);
	}

	ResourcePack::ResourcePack () { }
	ResourcePack::~ResourcePack() { base_file.close(); }

	bool ResourcePack::add(const std::string& input_file) {
		const std::string file = makeposix(input_file);

		if (_gfs::exists(file)) {
			ResourceFile e;
			e.size = (uint32_t)_gfs::file_size(file);
			e.offset = 0;
			files[file] = e;
			return true;
		}
		return false;
	}

	bool ResourcePack::load(const std::string& file, const std::string& key) {
		base_file.open(file, std::ifstream::binary);
		if (!base_file.is_open()) return false;

		uint32_t index_size = 0;
		base_file.read((char*)&index_size, sizeof(uint32_t));

		std::vector<char> buffer(index_size);
		for (uint32_t j = 0; j < index_size; j++)
			buffer[j] = base_file.get();

		std::vector<char> decoded = scramble(buffer, key);
		size_t pos = 0;
		auto read = [&decoded, &pos](char* dst, size_t size) {
			memcpy((void*)dst, (const void*)(decoded.data() + pos), size);
			pos += size;
		};

		auto get = [&read]() -> int { char c; read(&c, 1); return c; };

		uint32_t map_entries = 0;
		read((char*)&map_entries, sizeof(uint32_t));
		for (uint32_t i = 0; i < map_entries; i++) {
			uint32_t file_path_size = 0;
			read((char*)&file_path_size, sizeof(uint32_t));

			std::string file_name(file_path_size, ' ');
			for (uint32_t j = 0; j < file_path_size; j++)
				file_name[j] = get();

			ResourceFile e;
			read((char*)&e.size, sizeof(uint32_t));
			read((char*)&e.offset, sizeof(uint32_t));
			files[file_name] = e;
		}

		// don't close base file! stream pointer is provided when file requested
		return true;
	}

	bool ResourcePack::save(const std::string& file, const std::string& key)
	{
		std::ofstream ofs(file, std::ofstream::binary);
		if (!ofs.is_open()) return false;

		uint32_t index_size = 0;
		ofs.write((char*)&index_size, sizeof(uint32_t));
		uint32_t map_size = uint32_t(files.size());
		ofs.write((char*)&map_size, sizeof(uint32_t));

		for (auto& e : files) {
			size_t path_size = e.first.size();
			ofs.write((char*)&path_size, sizeof(uint32_t));
			ofs.write(e.first.c_str(), path_size);

			ofs.write((char*)&e.second.size, sizeof(uint32_t));
			ofs.write((char*)&e.second.offset, sizeof(uint32_t));
		}

		std::streampos offset = ofs.tellp();
		index_size = (uint32_t)offset;
		for (auto& e : files) {
			e.second.offset = (uint32_t)offset;

			std::vector<uint8_t> buffer(e.second.size);
			std::ifstream i(e.first, std::ifstream::binary);
			i.read((char*)buffer.data(), e.second.size);
			i.close();

			ofs.write((char*)buffer.data(), e.second.size);
			offset += e.second.size;
		}

		std::vector<char> stream;
		auto write = [&stream](const char* data, size_t size) {
			size_t size_now = stream.size();
			stream.resize(size_now + size);
			memcpy(stream.data() + size_now, data, size);
		};

		write((char*)&map_size, sizeof(uint32_t));
		for (auto& e : files) {
			size_t path_size = e.first.size();
			write((char*)&path_size, sizeof(uint32_t));
			write(e.first.c_str(), path_size);

			write((char*)&e.second.size, sizeof(uint32_t));
			write((char*)&e.second.offset, sizeof(uint32_t));
		}

		std::vector<char> index_string = scramble(stream, key);
		uint32_t index_string_len = uint32_t(index_string.size());
		ofs.seekp(0, std::ios::beg);
		ofs.write((char*)&index_string_len, sizeof(uint32_t));
		ofs.write(index_string.data(), index_string_len);
		ofs.close();

		return true;
	}

	ResourceBuffer ResourcePack::get_file_buffer(const std::string& file) { 
        return ResourceBuffer(base_file, files[file].offset, files[file].size); 
    }

	bool ResourcePack::loaded() { return base_file.is_open(); }

	std::vector<char> ResourcePack::scramble(const std::vector<char>& data, const std::string& key) {
		if (key.empty()) return data;
		std::vector<char> o;
		size_t c = 0;
		for (auto s : data)	o.push_back(s ^ key[(c++) % key.size()]);
		return o;
	};

	std::string ResourcePack::makeposix(const std::string& path) {
		std::string o;
		for (auto s : path) o += std::string(1, s == '\\' ? '/' : s);
		return o;
	};

	Engine::Engine() {
		app_name = "Undefined";
		engine::EngineX::engine = this;
		engine_configure_system();
	}
	Engine::~Engine() {}

	engine::Code Engine::construct(int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool full_screen, bool vsync, bool cohesion) {
		pixel_cohesion = cohesion;
		screen_size = { screen_w, screen_h };
		inv_screen_size = { 1.0f / float(screen_w), 1.0f / float(screen_h) };
		pixel_size = { pixel_w, pixel_h };
		window_size = screen_size * pixel_size;
		fullscreen = full_screen;
		enable_VSYNC = vsync;
		pixel = 2.0f / screen_size;

		if (pixel_size.x <= 0 || pixel_size.y <= 0 || screen_size.x <= 0 || screen_size.y <= 0)
			return engine::FAIL;
		
        return engine::OK;
	}

	void Engine::set_screen_size(int w, int h) {
		screen_size = { w, h };
		inv_screen_size = { 1.0f / float(w), 1.0f / float(h) };
		
        for (auto& layer : layers) {
			layer.draw_target.create(screen_size.x, screen_size.y);
			layer.update = true;
		}

		set_draw_target(nullptr);
		renderer->clear_buffer(engine::BLACK, true);
		renderer->display_frame();
		renderer->clear_buffer(engine::BLACK, true);
		renderer->update_viewport(view_pos, view_size);
	}

#if !defined(ENGINE_USE_CUSTOM_START)
	engine::Code Engine::start() {
		if (platform->application_startup() != engine::OK) return engine::FAIL;

		if (platform->create_window_pane({ 30,30 }, window_size, fullscreen) != engine::OK) return engine::FAIL;
		engine_update_window_size(window_size.x, window_size.y);

		atom_active = true;
		std::thread t = std::thread(&Engine::engine_thread, this);

		platform->start_system_event_loop();

		t.join();

		if (platform->application_cleanup() != engine::OK) return engine::FAIL;

		return engine::OK;
	}
#endif

	void Engine::set_draw_target(Sprite* target) {
		if (target) {
			draw_target = target;
		}
		else {
			target_layer = 0;
			draw_target = layers[0].draw_target.Sprite();
		}
	}

	void Engine::set_draw_target(uint8_t layer, bool dirty) {
		if (layer < layers.size()) {
			draw_target = layers[layer].draw_target.Sprite();
			layers[layer].update = dirty;
			target_layer = layer;
		}
	}

	void Engine::enable_layer(uint8_t layer, bool b) { 
        if (layer < layers.size()) layers[layer].show = b; 
    }

	void Engine::set_layer_offset(uint8_t layer, const engine::float_vector_2d& offset) { 
        set_layer_offset(layer, offset.x, offset.y); 
    }

	void Engine::set_layer_offset(uint8_t layer, float x, float y) { 
        if (layer < layers.size()) layers[layer].offset = { x, y }; 
    }

	void Engine::set_layer_scale(uint8_t layer, const engine::float_vector_2d& scale) { 
        set_layer_scale(layer, scale.x, scale.y); 
    }

	void Engine::set_layer_scale(uint8_t layer, float x, float y) { 
        if (layer < layers.size()) layers[layer].scale = { x, y }; 
    }

	void Engine::set_layer_tint(uint8_t layer, const engine::Pixel& tint) { 
        if (layer < layers.size()) layers[layer].tint = tint; 
    }

	void Engine::set_layer_custom_render_func(uint8_t layer, std::function<void()> f) { 
        if (layer < layers.size()) layers[layer].func_hook = f; 
    }

	std::vector<LayerDesc>& Engine::get_layers() { return layers; }

	uint32_t Engine::create_layer() {
		LayerDesc ld;
		ld.draw_target.create(screen_size.x, screen_size.y);
		layers.push_back(std::move(ld));
		return uint32_t(layers.size()) - 1;
	}

	Sprite* Engine::get_draw_target() const { return draw_target; }

	int32_t Engine::get_draw_target_width() const {
		if (draw_target)
			return draw_target->width;
		else
			return 0;
	}

	int32_t Engine::get_draw_target_height() const {
		if (draw_target)
			return draw_target->height;
		else
			return 0;
	}

	uint32_t Engine::get_FPS() const { return last_FPS; }

	bool Engine::is_focused() const { return has_input_focus; }

	ButtonState Engine::get_key(Key k) const { return keyboard_state[k]; }
	ButtonState Engine::get_mouse(uint32_t b) const { return mouse_state[b]; }

	int32_t Engine::get_mouse_x() const { return mouse_pos.x; }
	int32_t Engine::get_mouse_y() const { return mouse_pos.y; }

	const engine::int_vector_2d& Engine::get_mouse_pos() const { return mouse_pos; }

	int32_t Engine::get_mouse_wheel_delta() const { return mouse_wheel_delta; }

	int32_t Engine::screen_width () const { return screen_size.x; }
	int32_t Engine::screen_height() const { return screen_size.y; }

	float Engine::get_elapsed_time() const { return last_elapsed; }

	const engine::int_vector_2d& Engine::get_window_size() const { return window_size; }
	const engine::int_vector_2d& Engine::get_pixel_size () const { return pixel_size; }

	const engine::int_vector_2d& Engine::get_screen_pixel_size() const { return screen_pixel_size; }
	const engine::int_vector_2d& Engine::get_screen_size() const { return screen_size; }

	const engine::int_vector_2d& Engine::get_window_mouse() const { return mouse_window_pos; }

	bool Engine::draw(const engine::int_vector_2d& pos, Pixel p) { return draw(pos.x, pos.y, p); }

	bool Engine::draw(int32_t x, int32_t y, Pixel p) {
		if (!draw_target) return false;

		if (pixel_mode == Pixel::NORMAL)
			return draw_target->set_pixel(x, y, p);

		if (pixel_mode == Pixel::MASK)
			if (p.a == 255)
				return draw_target->set_pixel(x, y, p);

		if (pixel_mode == Pixel::ALPHA) {
			Pixel d = draw_target->get_pixel(x, y);
			
            float a = (float)(p.a / 255.0f) * blend_factor;
			float c = 1.0f - a;
			float r = a * (float)p.r + c * (float)d.r;
			float g = a * (float)p.g + c * (float)d.g;
			float b = a * (float)p.b + c * (float)d.b;
			
            return draw_target->set_pixel(x, y, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b));
		}

		if (pixel_mode == Pixel::CUSTOM)
			return draw_target->set_pixel(x, y, func_pixel_mode(x, y, p, draw_target->get_pixel(x, y)));

		return false;
	}

	void Engine::draw_line(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, Pixel p, uint32_t pattern) { 
        draw_line(pos1.x, pos1.y, pos2.x, pos2.y, p, pattern); 
    }

	void Engine::draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p, uint32_t pattern) {
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;

		auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };

		engine::int_vector_2d p1(x1, y1), p2(x2, y2);

		x1 = p1.x; y1 = p1.y;
		x2 = p2.x; y2 = p2.y;

		if (dx == 0) { // vertical
			if (y2 < y1) std::swap(y1, y2);
			for (y = y1; y <= y2; y++) if (rol()) draw(x1, y, p);
			return;
		}

		if (dy == 0) { // horizontal
			if (x2 < x1) std::swap(x1, x2);
			for (x = x1; x <= x2; x++) if (rol()) draw(x, y1, p);
			return;
		}

		dx1 = abs(dx); 
        dy1 = abs(dy);
		
        px = 2 * dy1 - dx1;	
        py = 2 * dx1 - dy1;
		
        if (dy1 <= dx1) {
			if (dx >= 0) {
				x = x1; 
                y = y1; 
                xe = x2;
			}
			else {
				x = x2; 
                y = y2; 
                xe = x1;
			}

			if (rol()) draw(x, y, p);

			for (i = 0; x < xe; i++) {
				x = x + 1;
				if (px < 0) {
					px = px + 2 * dy1;
                }
				else {
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				if (rol()) draw(x, y, p);
			}
		}
		else {
			if (dy >= 0) {
				x = x1; 
                y = y1; 
                ye = y2;
			}
			else {
				x = x2; 
                y = y2; 
                ye = y1;
			}

			if (rol()) draw(x, y, p);

			for (i = 0; y < ye; i++) {
				y = y + 1;
				if (py <= 0) {
					py = py + 2 * dx1;
                }
				else {
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				if (rol()) draw(x, y, p);
			}
		}
	}

	void Engine::draw_circle(const engine::int_vector_2d& pos, int32_t radius, Pixel p, uint8_t mask) { 
        draw_circle(pos.x, pos.y, radius, p, mask); 
    }

	void Engine::draw_circle(int32_t x, int32_t y, int32_t radius, Pixel p, uint8_t mask) {
		if (radius < 0 || x < -radius || y < -radius || x - get_draw_target_width() > radius || y - get_draw_target_height() > radius)
			return;

		if (radius > 0) {
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			while (y0 >= x0) { // 1/8 of circle
				if (mask & 0x01) draw(x + x0, y - y0, p); // upper right right
				if (mask & 0x04) draw(x + y0, y + x0, p); // lower lower right
				if (mask & 0x10) draw(x - x0, y + y0, p); // lower left left
				if (mask & 0x40) draw(x - y0, y - x0, p); // upper upper left

				if (x0 != 0 && x0 != y0) {
					if (mask & 0x02) draw(x + y0, y - x0, p); // upper upper right
					if (mask & 0x08) draw(x + x0, y + y0, p); // lower right right
					if (mask & 0x20) draw(x - y0, y + x0, p); // lower lower left
					if (mask & 0x80) draw(x - x0, y - y0, p); // upper left left
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		else {
			draw(x, y, p);
        }
	}

	void Engine::fill_circle(const engine::int_vector_2d& pos, int32_t radius, Pixel p) { 
        fill_circle(pos.x, pos.y, radius, p); 
    }

	void Engine::fill_circle(int32_t x, int32_t y, int32_t radius, Pixel p) {
		if (radius < 0 || x < -radius || y < -radius || x - get_draw_target_width() > radius || y - get_draw_target_height() > radius)
			return;

		if (radius > 0) {
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			auto drawline = [&](int sx, int ex, int y) {
				for (int x = sx; x <= ex; x++)
					draw(x, y, p);
			};

			while (y0 >= x0) {
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0) {
					d += 4 * x0++ + 6;
                }
				else {
					if (x0 != y0) {
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else {
			draw(x, y, p);
        }
	}

	void Engine::draw_rect(const engine::int_vector_2d& pos, const engine::int_vector_2d& size, Pixel p) { 
        draw_rect(pos.x, pos.y, size.x, size.y, p); 
    }

	void Engine::draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p) {
		draw_line(x, y, x + w, y, p);
		draw_line(x + w, y, x + w, y + h, p);
		draw_line(x + w, y + h, x, y + h, p);
		draw_line(x, y + h, x, y, p);
	}

	void Engine::clear(Pixel p) {
		int pixels = get_draw_target_width() * get_draw_target_height();
		Pixel* m = get_draw_target()->get_data();
		for (int i = 0; i < pixels; i++) m[i] = p;
	}

	void Engine::clear_buffer(Pixel p, bool depth) { 
        renderer->clear_buffer(p, depth);
    }

	engine::Sprite* Engine::get_font_sprite() { 
        return font_renderable.Sprite(); 
    }

	bool Engine::clip_line_to_screen(engine::int_vector_2d& in_p1, engine::int_vector_2d& in_p2) {
		// ARTICLE: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

		static constexpr int SEG_I = 0b0000, SEG_L = 0b0001, SEG_R = 0b0010, SEG_B = 0b0100, SEG_T = 0b1000;
		auto Segment = [&screen_size = screen_size](const engine::int_vector_2d& v) {
			int i = SEG_I;
			if (v.x < 0) i |= SEG_L; else if (v.x > screen_size.x) i |= SEG_R;
			if (v.y < 0) i |= SEG_B; else if (v.y > screen_size.y) i |= SEG_T;
			return i;
		};

		int s1 = Segment(in_p1), s2 = Segment(in_p2);

		while (true) {
			if (!(s1 | s2))	  return true;
			else if (s1 & s2) return false;
			else {
				int s3 = s2 > s1 ? s2 : s1;
				engine::int_vector_2d n;
				if (s3 & SEG_T)  { 
                    n.x = in_p1.x + (in_p2.x - in_p1.x) * (screen_size.y - in_p1.y) / (in_p2.y - in_p1.y); 
                    n.y = screen_size.y; 
                }
				else if (s3 & SEG_B) { 
                    n.x = in_p1.x + (in_p2.x - in_p1.x) * (0 - in_p1.y) / (in_p2.y - in_p1.y); 
                    n.y = 0; 
                }
				else if (s3 & SEG_R) { 
                    n.x = screen_size.x; 
                    n.y = in_p1.y + (in_p2.y - in_p1.y) * (screen_size.x - in_p1.x) / (in_p2.x - in_p1.x); 
                }
				else if (s3 & SEG_L) { 
                    n.x = 0; 
                    n.y = in_p1.y + (in_p2.y - in_p1.y) * (0 - in_p1.x) / (in_p2.x - in_p1.x); 
                }

				if (s3 == s1) { 
                    in_p1 = n; 
                    s1 = Segment(in_p1); 
                }
				else {
                    in_p2 = n; 
                    s2 = Segment(in_p2); 
                }
			}
		}
		return true;
	}

	void Engine::enable_pixel_transfer(const bool enable) {
		suspend_texture_transfer = !enable;
	}

	void Engine::fill_rect(const engine::int_vector_2d& pos, const engine::int_vector_2d& size, Pixel p) { 
        fill_rect(pos.x, pos.y, size.x, size.y, p); 
    }

	void Engine::fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p) {
		int32_t x2 = x + w;
		int32_t y2 = y + h;

		if (x < 0) x = 0;
		if (x >= (int32_t)get_draw_target_width()) x = (int32_t)get_draw_target_width();
		if (y < 0) y = 0;
		if (y >= (int32_t)get_draw_target_height()) y = (int32_t)get_draw_target_height();

		if (x2 < 0) x2 = 0;
		if (x2 >= (int32_t)get_draw_target_width()) x2 = (int32_t)get_draw_target_width();
		if (y2 < 0) y2 = 0;
		if (y2 >= (int32_t)get_draw_target_height()) y2 = (int32_t)get_draw_target_height();

		for (int i = x; i < x2; i++)
			for (int j = y; j < y2; j++)
				draw(i, j, p);
	}

	void Engine::draw_triangle(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, const engine::int_vector_2d& pos3, Pixel p) { 
        draw_triangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p); 
    }

	void Engine::draw_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p) {
		draw_line(x1, y1, x2, y2, p);
		draw_line(x2, y2, x3, y3, p);
		draw_line(x3, y3, x1, y1, p);
	}

	void Engine::fill_triangle(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, const engine::int_vector_2d& pos3, Pixel p) { 
        fill_triangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p); 
    }

    // found online
	void Engine::fill_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p) {
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) draw(i, ny, p); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;

		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;
		dx1 = (int)(x2 - x1);

		if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;

		dy1 = (int)(y2 - y1);
		dx2 = (int)(x3 - x1);

		if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;

		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = (int)(dx2 >> 1);

		if (y1 == y2) goto next;
		
        e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { 
                minx = t1x; 
                maxx = t2x; 
            }
			else { 
                minx = t2x; 
                maxx = t1x; 
            }

			while (i < dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
		next1:
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;

			drawline(minx, maxx, y);
			
            if (!changed1) t1x += signx1;
			t1x += t1xp;
			
            if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			
            if (y == y2) break;
		}
	next:
		dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;

		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { 
                minx = t1x; 
                maxx = t2x; 
            }
			else { 
                minx = t2x; 
                maxx = t1x; 
            }
			
            while (i < dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }
					else          goto next3;
				}

				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i < dx1) i++;
			}
		next3:
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;

			drawline(minx, maxx, y);
			
            if (!changed1) t1x += signx1;
			t1x += t1xp;
			
            if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			
            if (y > y3) return;
		}
	}

	void Engine::fill_textured_triangle(const std::vector<engine::float_vector_2d>& points, std::vector<engine::float_vector_2d> tex, std::vector<engine::Pixel> color, engine::Sprite* spr_tex) {
		engine::int_vector_2d p1 = points[0];
		engine::int_vector_2d p2 = points[1];
		engine::int_vector_2d p3 = points[2];

		if (p2.y < p1.y) {
            std::swap(p1.y, p2.y); 
            std::swap(p1.x, p2.x); 
            std::swap(tex[0].x, tex[1].x); 
            std::swap(tex[0].y, tex[1].y); 
            std::swap(color[0], color[1]);
        }

		if (p3.y < p1.y) {
            std::swap(p1.y, p3.y); 
            std::swap(p1.x, p3.x); 
            std::swap(tex[0].x, tex[2].x); 
            std::swap(tex[0].y, tex[2].y); 
            std::swap(color[0], color[2]);
        }

		if (p3.y < p2.y) {
            std::swap(p2.y, p3.y); 
            std::swap(p2.x, p3.x); 
            std::swap(tex[1].x, tex[2].x); 
            std::swap(tex[1].y, tex[2].y); 
            std::swap(color[1], color[2]);
        }

		engine::int_vector_2d pos1 = p2 - p1;
		engine::float_vector_2d tex1 = tex[1] - tex[0];
		int dcr1 = color[1].r - color[0].r;
		int dcg1 = color[1].g - color[0].g;
		int dcb1 = color[1].b - color[0].b;
		int dca1 = color[1].a - color[0].a;

		engine::int_vector_2d pos2 = p3 - p1;
		engine::float_vector_2d tex2 = tex[2] - tex[0];
		int dcr2 = color[2].r - color[0].r;
		int dcg2 = color[2].g - color[0].g;
		int dcb2 = color[2].b - color[0].b;
		int dca2 = color[2].a - color[0].a;

		float dax_step = 0, dbx_step = 0, dcr1_step = 0, dcr2_step = 0,	dcg1_step = 0, dcg2_step = 0, dcb1_step = 0, dcb2_step = 0,	dca1_step = 0, dca2_step = 0;
		engine::float_vector_2d tex_1_step, tex_2_step;

		if (pos1.y) {
			dax_step = pos1.x / (float)abs(pos1.y);
			tex_1_step = tex1 / (float)abs(pos1.y);
			dcr1_step = dcr1 / (float)abs(pos1.y);
			dcg1_step = dcg1 / (float)abs(pos1.y);
			dcb1_step = dcb1 / (float)abs(pos1.y);
			dca1_step = dca1 / (float)abs(pos1.y);
		}

		if (pos2.y) {
			dbx_step = pos2.x / (float)abs(pos2.y);
			tex_2_step = tex2 / (float)abs(pos2.y);
			dcr2_step = dcr2 / (float)abs(pos2.y);
			dcg2_step = dcg2 / (float)abs(pos2.y);
			dcb2_step = dcb2 / (float)abs(pos2.y);
			dca2_step = dca2 / (float)abs(pos2.y);
		}

		engine::int_vector_2d start;
		engine::int_vector_2d end;
		int start_idx;

		for (int pass = 0; pass < 2; pass++) {
			if (pass == 0) {
				start = p1; 
                end = p2;
                start_idx = 0;
			}
			else {
				pos1 = p3 - p2;
				tex1 = tex[2] - tex[1];
				dcr1 = color[2].r - color[1].r;
				dcg1 = color[2].g - color[1].g;
				dcb1 = color[2].b - color[1].b;
				dca1 = color[2].a - color[1].a;
				dcr1_step = 0; 
                dcg1_step = 0; 
                dcb1_step = 0; 
                dca1_step = 0;

				if (pos2.y) dbx_step = pos2.x / (float)abs(pos2.y);
				if (pos1.y) {
					dax_step = pos1.x / (float)abs(pos1.y);
					tex_1_step = tex1 / (float)abs(pos1.y);
					dcr1_step = dcr1 / (float)abs(pos1.y);
					dcg1_step = dcg1 / (float)abs(pos1.y);
					dcb1_step = dcb1 / (float)abs(pos1.y);
					dca1_step = dca1 / (float)abs(pos1.y);
				}

				start = p2;
                end = p3;
                start_idx = 1;
			}

			if (pos1.y) {
				for (int i = start.y; i <= end.y; i++) {
					int ax = int(start.x + (float)(i - start.y) * dax_step);
					int bx = int(p1.x + (float)(i - p1.y) * dbx_step);

					engine::float_vector_2d tex_s(tex[start_idx].x + (float)(i - start.y) * tex_1_step.x, tex[start_idx].y + (float)(i - start.y) * tex_1_step.y);
					engine::float_vector_2d tex_e(tex[0].x + (float)(i - p1.y) * tex_2_step.x, tex[0].y + (float)(i - p1.y) * tex_2_step.y);

					engine::Pixel col_s(color[start_idx].r + uint8_t((float)(i - start.y) * dcr1_step), color[start_idx].g + uint8_t((float)(i - start.y) * dcg1_step),
						color[start_idx].b + uint8_t((float)(i - start.y) * dcb1_step), color[start_idx].a + uint8_t((float)(i - start.y) * dca1_step));

					engine::Pixel col_e(color[0].r + uint8_t((float)(i - p1.y) * dcr2_step), color[0].g + uint8_t((float)(i - p1.y) * dcg2_step),
						color[0].b + uint8_t((float)(i - p1.y) * dcb2_step), color[0].a + uint8_t((float)(i - p1.y) * dca2_step));

					if (ax > bx) { 
                        std::swap(ax, bx); 
                        std::swap(tex_s, tex_e); 
                        std::swap(col_s, col_e); 
                    }

					float tstep = 1.0f / ((float)(bx - ax));
					float t = 0.0f;

					for (int j = ax; j < bx; j++) {
						engine::Pixel pixel = pixel_lerp(col_s, col_e, t);
						if (spr_tex != nullptr) pixel *= spr_tex->sample(tex_s.lerp(tex_e, t));
						draw(j, i, pixel);
						t += tstep;
					}
				}
			}
		}			
	}

	void Engine::fill_textured_polygon(const std::vector<engine::float_vector_2d>& points, const std::vector<engine::float_vector_2d>& tex, const std::vector<engine::Pixel>& color, engine::Sprite* spr_tex, engine::DecalStructure structure) {
		if (structure == engine::DecalStructure::LINE)
			return;

		if (points.size() < 3 || tex.size() < 3 || color.size() < 3)
			return;

		if (structure == engine::DecalStructure::LIST) {
			for (int tri = 0; tri < points.size() / 3; tri++) {
				std::vector<engine::float_vector_2d> p = { points[tri * 3 + 0], points[tri * 3 + 1], points[tri * 3 + 2] };
				std::vector<engine::float_vector_2d> t = { tex[tri * 3 + 0], tex[tri * 3 + 1], tex[tri * 3 + 2] };
				std::vector<engine::Pixel> c = { color[tri * 3 + 0], color[tri * 3 + 1], color[tri * 3 + 2] };
				fill_textured_triangle(p, t, c, spr_tex);
			}
			return;
		}

		if (structure == engine::DecalStructure::STRIP) {
			for (int tri = 2; tri < points.size(); tri++) {
				std::vector<engine::float_vector_2d> p = { points[tri - 2], points[tri-1], points[tri] };
				std::vector<engine::float_vector_2d> t = { tex[tri - 2], tex[tri - 1], tex[tri] };
				std::vector<engine::Pixel> c = { color[tri - 2], color[tri - 1], color[tri] };
				fill_textured_triangle(p, t, c, spr_tex);
			}
			return;
		}

		if (structure == engine::DecalStructure::FAN) {
			for (int tri = 2; tri < points.size(); tri++) {
				std::vector<engine::float_vector_2d> p = { points[0], points[tri - 1], points[tri] };
				std::vector<engine::float_vector_2d> t = { tex[0], tex[tri - 1], tex[tri] };
				std::vector<engine::Pixel> c = { color[0], color[tri - 1], color[tri] };
				fill_textured_triangle(p, t, c, spr_tex);
			}
			return;
		}
	}

	void Engine::draw_sprite(const engine::int_vector_2d& pos, Sprite* sprite, uint32_t scale, uint8_t flip) { 
        draw_sprite(pos.x, pos.y, sprite, scale, flip); 
    }

	void Engine::draw_sprite(int32_t x, int32_t y, Sprite* sprite, uint32_t scale, uint8_t flip) {
		if (sprite == nullptr)
			return;

		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;
		
        if (flip & engine::Sprite::Flip::HORIZ) { 
            fxs = sprite->width - 1; 
            fxm = -1; 
        }

		if (flip & engine::Sprite::Flip::VERT) { 
            fys = sprite->height - 1; 
            fym = -1; 
        }

		if (scale > 1) {
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm) {
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							draw(x + (i * scale) + is, y + (j * scale) + js, sprite->get_pixel(fx, fy));
			}
		}
		else {
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm) {
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					draw(x + i, y + j, sprite->get_pixel(fx, fy));
			}
		}
	}

	void Engine::draw_partial_sprite(const engine::int_vector_2d& pos, Sprite* sprite, const engine::int_vector_2d& sourcepos, const engine::int_vector_2d& size, uint32_t scale, uint8_t flip) { 
        draw_partial_sprite(pos.x, pos.y, sprite, sourcepos.x, sourcepos.y, size.x, size.y, scale, flip); 
    }

	void Engine::draw_partial_sprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale, uint8_t flip) {
		if (sprite == nullptr)
			return;

		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;

		if (flip & engine::Sprite::Flip::HORIZ) { 
            fxs = w - 1; 
            fxm = -1; 
        }

		if (flip & engine::Sprite::Flip::VERT) { 
            fys = h - 1; 
            fym = -1; 
        }

		if (scale > 1) {
			fx = fxs;
			for (int32_t i = 0; i < w; i++, fx += fxm) {
				fy = fys;
				for (int32_t j = 0; j < h; j++, fy += fym)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							draw(x + (i * scale) + is, y + (j * scale) + js, sprite->get_pixel(fx + ox, fy + oy));
			}
		}
		else {
			fx = fxs;
			for (int32_t i = 0; i < w; i++, fx += fxm) {
				fy = fys;
				for (int32_t j = 0; j < h; j++, fy += fym)
					draw(x + i, y + j, sprite->get_pixel(fx + ox, fy + oy));
			}
		}
	}

	void Engine::set_decal_mode(const engine::DecalMode& mode) { 
        decal_mode = mode; 
    }

	void Engine::set_decal_structure(const engine::DecalStructure& structure) { 
        decal_structure = structure; 
    }

	void Engine::draw_partial_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::float_vector_2d& scale, const engine::Pixel& tint) {
		engine::float_vector_2d screen_space_pos = {
			  (pos.x * inv_screen_size.x) * 2.0f - 1.0f,
			-((pos.y * inv_screen_size.y) * 2.0f - 1.0f)
		};

		engine::float_vector_2d screen_space_dim = {
			  ((pos.x + source_size.x * scale.x) * inv_screen_size.x) * 2.0f - 1.0f,
			-(((pos.y + source_size.y * scale.y) * inv_screen_size.y) * 2.0f - 1.0f)
		};

		engine::float_vector_2d window = engine::float_vector_2d(view_size);
		engine::float_vector_2d quantised_pos = ((screen_space_pos * window) + engine::float_vector_2d(0.5f, 0.5f)).floor() / window;
		engine::float_vector_2d quantised_dim = ((screen_space_dim * window) + engine::float_vector_2d(0.5f, -0.5f)).ceil() / window;

		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.pos = {  
            { quantised_pos.x, quantised_pos.y }, 
            { quantised_pos.x, quantised_dim.y }, 
            { quantised_dim.x, quantised_dim.y }, 
            { quantised_dim.x, quantised_pos.y } 
        };

		engine::float_vector_2d uvtl = (source_pos + engine::float_vector_2d(0.0001f, 0.0001f)) * decal->UV_scale;
		engine::float_vector_2d uvbr = (source_pos + source_size - engine::float_vector_2d(0.0001f, 0.0001f)) * decal->UV_scale;

		di.uv = { 
            { uvtl.x, uvtl.y }, 
            { uvtl.x, uvbr.y }, 
            { uvbr.x, uvbr.y }, 
            { uvbr.x, uvtl.y } 
        };
		di.w = { 1,1,1,1 };
		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_partial_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, engine::Decal* decal, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint) {
		engine::float_vector_2d screen_space_pos = {
			(pos.x * inv_screen_size.x) * 2.0f - 1.0f,
			((pos.y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f
		};

		engine::float_vector_2d screen_space_dim = {
			screen_space_pos.x + (2.0f * size.x * inv_screen_size.x),
			screen_space_pos.y - (2.0f * size.y * inv_screen_size.y)
		};

		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.pos = { 
            { screen_space_pos.x, screen_space_pos.y }, 
            { screen_space_pos.x, screen_space_dim.y }, 
            { screen_space_dim.x, screen_space_dim.y }, 
            { screen_space_dim.x, screen_space_pos.y } 
        };

		engine::float_vector_2d uvtl = (source_pos) * decal->UV_scale;
		engine::float_vector_2d uvbr = uvtl + ((source_size) * decal->UV_scale);

		di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.w = { 1,1,1,1 };
		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const engine::float_vector_2d& scale, const engine::Pixel& tint) {
		engine::float_vector_2d screen_space_pos = {
			(pos.x * inv_screen_size.x) * 2.0f - 1.0f,
			((pos.y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f
		};

		engine::float_vector_2d screen_space_dim = {
			screen_space_pos.x + (2.0f * (float(decal->sprite->width) * inv_screen_size.x)) * scale.x,
			screen_space_pos.y - (2.0f * (float(decal->sprite->height) * inv_screen_size.y)) * scale.y
		};

		DecalInstance di;
		di.decal = decal;
		di.points = 4;
		di.tint = { tint, tint, tint, tint };
		di.pos = { 
            { screen_space_pos.x, screen_space_pos.y }, 
            { screen_space_pos.x, screen_space_dim.y }, 
            { screen_space_dim.x, screen_space_dim.y }, 
            { screen_space_dim.x, screen_space_pos.y } 
        };
		di.uv = { 
            { 0.0f, 0.0f}, 
            {0.0f, 1.0f}, 
            {1.0f, 1.0f}, 
            {1.0f, 0.0f} 
        };
		di.w = { 1, 1, 1, 1 };
		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_explicit_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::float_vector_2d* uv, const engine::Pixel* col, uint32_t elements) {
		DecalInstance di;
		di.decal = decal;
		di.pos.resize(elements);
		di.uv.resize(elements);
		di.w.resize(elements);
		di.tint.resize(elements);
		di.points = elements;

		for (uint32_t i = 0; i < elements; i++) {
			di.pos[i] = { (pos[i].x * inv_screen_size.x) * 2.0f - 1.0f, ((pos[i].y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = col[i];
			di.w[i] = 1.0f;
		}

		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const engine::Pixel tint) {
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);

		for (uint32_t i = 0; i < di.points; i++) {
			di.pos[i] = { (pos[i].x * inv_screen_size.x) * 2.0f - 1.0f, ((pos[i].y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = tint;
			di.w[i] = 1.0f;
		}

		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const std::vector<engine::Pixel> &tint) {
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);

		for (uint32_t i = 0; i < di.points; i++) {
			di.pos[i] = { (pos[i].x * inv_screen_size.x) * 2.0f - 1.0f, ((pos[i].y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = tint[i];
			di.w[i] = 1.0f;
		}

		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const std::vector<engine::Pixel>& colors, const engine::Pixel tint) {
		std::vector<engine::Pixel> new_colors(colors.size(), engine::WHITE);
		std::transform(colors.begin(), colors.end(), new_colors.begin(), [&tint](const engine::Pixel pin) { return pin * tint; });
		draw_polygon_decal(decal, pos, uv, new_colors);
	}

	void Engine::draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<float>& depth, const std::vector<engine::float_vector_2d>& uv, const engine::Pixel tint) {
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);

		for (uint32_t i = 0; i < di.points; i++) {
			di.pos[i] = { (pos[i].x * inv_screen_size.x) * 2.0f - 1.0f, ((pos[i].y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = tint;
			di.w[i] = 1.0f;
		}

		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

#ifdef ENGINE_ENABLE_EXPERIMENTAL
	void Engine::LW3D_DrawTriangles(engine::Decal* decal, const std::vector<std::array<float, 3>>& pos, const std::vector<engine::float_vector_2d>& tex, const std::vector<engine::Pixel>& col) {
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);

		for (uint32_t i = 0; i < di.points; i++) {
			di.pos[i] = { pos[i][0], pos[i][1] };
			di.w[i] = pos[i][2];
			di.uv[i] = tex[i];
			di.tint[i] = col[i];			
		}

		di.mode = DecalMode::MODEL3D;
		layers[target_layer].decal_instances.push_back(di);
	}
#endif

	void Engine::draw_line_decal(const engine::float_vector_2d& pos1, const engine::float_vector_2d& pos2, Pixel p) {
		auto m = decal_mode;
		decal_mode = engine::DecalMode::WIREFRAME;
		draw_polygon_decal(nullptr, { pos1, pos2 }, { {0, 0}, {0,0} }, p);
		decal_mode = m;
	}

	void Engine::draw_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col) {
		auto m = decal_mode;
		set_decal_mode(engine::DecalMode::WIREFRAME);
		engine::float_vector_2d new_size = size;
		std::array<engine::float_vector_2d, 4> points = { { {pos}, {pos.x, pos.y + new_size.y}, {pos + new_size}, {pos.x + new_size.x, pos.y} } };
		std::array<engine::float_vector_2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
		std::array<engine::Pixel, 4> cols = { {col, col, col, col} };
		draw_explicit_decal(nullptr, points.data(), uvs.data(), cols.data(), 4);
		set_decal_mode(m);
	}

	void Engine::fill_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col) {
		engine::float_vector_2d new_size = size;
		std::array<engine::float_vector_2d, 4> points = { { {pos}, {pos.x, pos.y + new_size.y}, {pos + new_size}, {pos.x + new_size.x, pos.y} } };
		std::array<engine::float_vector_2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
		std::array<engine::Pixel, 4> cols = { {col, col, col, col} };
		draw_explicit_decal(nullptr, points.data(), uvs.data(), cols.data(), 4);
	}

	void Engine::gradient_fill_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel colTL, const engine::Pixel colBL, const engine::Pixel colBR, const engine::Pixel colTR) {
		std::array<engine::float_vector_2d, 4> points = { { {pos}, {pos.x, pos.y + size.y}, {pos + size}, {pos.x + size.x, pos.y} } };
		std::array<engine::float_vector_2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
		std::array<engine::Pixel, 4> cols = { {colTL, colBL, colBR, colTR} };
		draw_explicit_decal(nullptr, points.data(), uvs.data(), cols.data(), 4);
	}

	void Engine::draw_rotated_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const float angle, const engine::float_vector_2d& center, const engine::float_vector_2d& scale, const engine::Pixel& tint) {
		DecalInstance di;
		di.decal = decal;
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		di.w = { 1, 1, 1, 1 };
		di.tint = { tint, tint, tint, tint };
		di.points = 4;
		di.pos[0] = (engine::float_vector_2d(0.0f, 0.0f) - center) * scale;
		di.pos[1] = (engine::float_vector_2d(0.0f, float(decal->sprite->height)) - center) * scale;
		di.pos[2] = (engine::float_vector_2d(float(decal->sprite->width), float(decal->sprite->height)) - center) * scale;
		di.pos[3] = (engine::float_vector_2d(float(decal->sprite->width), 0.0f) - center) * scale;
		float c = cos(angle), s = sin(angle);

		for (int i = 0; i < 4; i++) {
			di.pos[i] = pos + engine::float_vector_2d(di.pos[i].x * c - di.pos[i].y * s, di.pos[i].x * s + di.pos[i].y * c);
			di.pos[i] = di.pos[i] * inv_screen_size * 2.0f - engine::float_vector_2d(1.0f, 1.0f);
			di.pos[i].y *= -1.0f;
			di.w[i] = 1;
		}

		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_partial_rotated_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const float angle, const engine::float_vector_2d& center, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::float_vector_2d& scale, const engine::Pixel& tint) {
		DecalInstance di;
		di.decal = decal;
		di.points = 4;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.pos[0] = (engine::float_vector_2d(0.0f, 0.0f) - center) * scale;
		di.pos[1] = (engine::float_vector_2d(0.0f, source_size.y) - center) * scale;
		di.pos[2] = (engine::float_vector_2d(source_size.x, source_size.y) - center) * scale;
		di.pos[3] = (engine::float_vector_2d(source_size.x, 0.0f) - center) * scale;
		float c = cos(angle), s = sin(angle);

		for (int i = 0; i < 4; i++) {
			di.pos[i] = pos + engine::float_vector_2d(di.pos[i].x * c - di.pos[i].y * s, di.pos[i].x * s + di.pos[i].y * c);
			di.pos[i] = di.pos[i] * inv_screen_size * 2.0f - engine::float_vector_2d(1.0f, 1.0f);
			di.pos[i].y *= -1.0f;
		}

		engine::float_vector_2d uvtl = source_pos * decal->UV_scale;
		engine::float_vector_2d uvbr = uvtl + (source_size * decal->UV_scale);
		
        di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.mode = decal_mode;
		di.structure = decal_structure;
		layers[target_layer].decal_instances.push_back(di);
	}

	void Engine::draw_partial_warped_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint) {
		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		engine::float_vector_2d center;
		float rd = ((pos[2].x - pos[0].x) * (pos[3].y - pos[1].y) - (pos[3].x - pos[1].x) * (pos[2].y - pos[0].y));
		
        if (rd != 0) {
			engine::float_vector_2d uvtl = source_pos * decal->UV_scale;
			engine::float_vector_2d uvbr = uvtl + (source_size * decal->UV_scale);
			di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };

			rd = 1.0f / rd;
			float rn = ((pos[3].x - pos[1].x) * (pos[0].y - pos[1].y) - (pos[3].y - pos[1].y) * (pos[0].x - pos[1].x)) * rd;
			float sn = ((pos[2].x - pos[0].x) * (pos[0].y - pos[1].y) - (pos[2].y - pos[0].y) * (pos[0].x - pos[1].x)) * rd;
			if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)) center = pos[0] + rn * (pos[2] - pos[0]);
			float d[4];	for (int i = 0; i < 4; i++)	d[i] = (pos[i] - center).mag();
			
            for (int i = 0; i < 4; i++) {
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				di.uv[i] *= q; di.w[i] *= q;
				di.pos[i] = { (pos[i].x * inv_screen_size.x) * 2.0f - 1.0f, ((pos[i].y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f };
			}

			di.mode = decal_mode;
			di.structure = decal_structure;
			layers[target_layer].decal_instances.push_back(di);
		}
	}

	void Engine::draw_warped_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::Pixel& tint) {
		// ARTICLE: http://www.reedbeta.com/blog/quadrilateral-interpolation-part-1/
        
		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		engine::float_vector_2d center;
		float rd = ((pos[2].x - pos[0].x) * (pos[3].y - pos[1].y) - (pos[3].x - pos[1].x) * (pos[2].y - pos[0].y));
		
        if (rd != 0) {
			rd = 1.0f / rd;
			float rn = ((pos[3].x - pos[1].x) * (pos[0].y - pos[1].y) - (pos[3].y - pos[1].y) * (pos[0].x - pos[1].x)) * rd;
			float sn = ((pos[2].x - pos[0].x) * (pos[0].y - pos[1].y) - (pos[2].y - pos[0].y) * (pos[0].x - pos[1].x)) * rd;
			if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)) center = pos[0] + rn * (pos[2] - pos[0]);
			float d[4];	for (int i = 0; i < 4; i++)	d[i] = (pos[i] - center).mag();
			
            for (int i = 0; i < 4; i++) {
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				di.uv[i] *= q; di.w[i] *= q;
				di.pos[i] = { (pos[i].x * inv_screen_size.x) * 2.0f - 1.0f, ((pos[i].y * inv_screen_size.y) * 2.0f - 1.0f) * -1.0f };
			}

			di.mode = decal_mode;
			di.structure = decal_structure;
			layers[target_layer].decal_instances.push_back(di);
		}
	}

	void Engine::draw_warped_decal(engine::Decal* decal, const std::array<engine::float_vector_2d, 4>& pos, const engine::Pixel& tint) { 
        draw_warped_decal(decal, pos.data(), tint); 
    }

	void Engine::draw_warped_decal(engine::Decal* decal, const engine::float_vector_2d(&pos)[4], const engine::Pixel& tint) { 
        draw_warped_decal(decal, &pos[0], tint); 
    }

	void Engine::draw_partial_warped_decal(engine::Decal* decal, const std::array<engine::float_vector_2d, 4>& pos, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint) { 
        draw_partial_warped_decal(decal, pos.data(), source_pos, source_size, tint); 
    }

	void Engine::draw_partial_warped_decal(engine::Decal* decal, const engine::float_vector_2d(&pos)[4], const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint) { 
        draw_partial_warped_decal(decal, &pos[0], source_pos, source_size, tint); 
    }

	void Engine::draw_string_decal(const engine::float_vector_2d& pos, const std::string& text, const Pixel col, const engine::float_vector_2d& scale) {
		engine::float_vector_2d spos = { 0.0f, 0.0f };
		for (auto c : text) {
			if (c == '\n') {
				spos.x = 0; 
                spos.y += 8.0f * scale.y;
			}
			else if (c == '\t') {
				spos.x += 8.0f * float(tab_size_in_spaces) * scale.x;
			}
			else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				
                draw_partial_decal(pos + spos, font_renderable.Decal(), {float(ox) * 8.0f, float(oy) * 8.0f}, {8.0f, 8.0f}, scale, col);
				spos.x += 8.0f * scale.x;
			}
		}
	}

	void Engine::draw_string_prop_decal(const engine::float_vector_2d& pos, const std::string& text, const Pixel col, const engine::float_vector_2d& scale) {
		engine::float_vector_2d spos = { 0.0f, 0.0f };
		for (auto c : text) {
			if (c == '\n') {
				spos.x = 0; 
                spos.y += 8.0f * scale.y;
			}
			else if (c == '\t') {
				spos.x += 8.0f * float(tab_size_in_spaces) * scale.x;
			}
			else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				draw_partial_decal(pos + spos, font_renderable.Decal(), { float(ox) * 8.0f + float(font_spacing[c - 32].x), float(oy) * 8.0f }, { float(font_spacing[c - 32].y), 8.0f }, scale, col);
				spos.x += float(font_spacing[c - 32].y) * scale.x;
			}
		}
	}

	void Engine::draw_rotated_string_decal(const engine::float_vector_2d& pos, const std::string& text, const float angle, const engine::float_vector_2d& center, const Pixel col, const engine::float_vector_2d& scale) {
		engine::float_vector_2d spos = center;
		for (auto c : text) {
			if (c == '\n') {
				spos.x = center.x; spos.y -= 8.0f;
			}
			else if (c == '\t') {
				spos.x += 8.0f * float(tab_size_in_spaces) * scale.x;
			}
			else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				draw_partial_rotated_decal(pos, font_renderable.Decal(), angle, spos, { float(ox) * 8.0f, float(oy) * 8.0f }, { 8.0f, 8.0f }, scale, col);
				spos.x -= 8.0f;
			}
		}
	}

	void Engine::draw_rotated_string_prop_decal(const engine::float_vector_2d& pos, const std::string& text, const float angle, const engine::float_vector_2d& center, const Pixel col, const engine::float_vector_2d& scale) {
		engine::float_vector_2d spos = center;
		for (auto c : text) {
			if (c == '\n') {
				spos.x = center.x; spos.y -= 8.0f;
			}
			else if (c == '\t') {
				spos.x += 8.0f * float(tab_size_in_spaces) * scale.x;
			}
			else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				draw_partial_rotated_decal(pos, font_renderable.Decal(), angle, spos, { float(ox) * 8.0f + float(font_spacing[c - 32].x), float(oy) * 8.0f }, { float(font_spacing[c - 32].y), 8.0f }, scale, col);
				spos.x -= float(font_spacing[c - 32].y);
			}
		}
	}

	engine::int_vector_2d Engine::get_text_size(const std::string& s)
	{
		engine::int_vector_2d size = { 0,1 };
		engine::int_vector_2d pos = { 0,1 };
		for (auto c : s) {
			if (c == '\n') { 
                pos.y++; 
                pos.x = 0; 
            }
			else if (c == '\t') { pos.x += tab_size_in_spaces; }
			else pos.x++;
			size.x = std::max(size.x, pos.x);
			size.y = std::max(size.y, pos.y);
		}
		return size * 8;
	}

	void Engine::draw_string(const engine::int_vector_2d& pos, const std::string& text, Pixel col, uint32_t scale) { 
        draw_string(pos.x, pos.y, text, col, scale); 
    }

	void Engine::draw_string(int32_t x, int32_t y, const std::string& text, Pixel col, uint32_t scale) {
		int32_t sx = 0;
		int32_t sy = 0;
		Pixel::Mode m = pixel_mode;

		if (m != Pixel::CUSTOM) {
			if (col.a != 255)		set_pixel_mode(Pixel::ALPHA);
			else					set_pixel_mode(Pixel::MASK);
		}

		for (auto c : text) {
			if (c == '\n') {
				sx = 0; sy += 8 * scale;
			}
			else if (c == '\t') {
				sx += 8 * tab_size_in_spaces * scale;
			}
			else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1) {
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (font_renderable.Sprite()->get_pixel(i + ox * 8, j + oy * 8).r > 0)
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
				}
				else {
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (font_renderable.Sprite()->get_pixel(i + ox * 8, j + oy * 8).r > 0)
								draw(x + sx + i, y + sy + j, col);
				}
				sx += 8 * scale;
			}
		}
		set_pixel_mode(m);
	}

	engine::int_vector_2d Engine::get_text_size_prop(const std::string& s) {
		engine::int_vector_2d size = { 0,1 };
		engine::int_vector_2d pos = { 0,1 };
		for (auto c : s) {
			if (c == '\n') { 
                pos.y += 1;
                pos.x = 0; 
            }
			else if (c == '\t') { pos.x += tab_size_in_spaces * 8; }
			else pos.x += font_spacing[c - 32].y;
			size.x = std::max(size.x, pos.x);
			size.y = std::max(size.y, pos.y);
		}

		size.y *= 8;
		return size;
	}

	void Engine::draw_string_prop(const engine::int_vector_2d& pos, const std::string& text, Pixel col, uint32_t scale) { 
        draw_string_prop(pos.x, pos.y, text, col, scale); 
    }

	void Engine::draw_string_prop(int32_t x, int32_t y, const std::string& text, Pixel col, uint32_t scale) {
		int32_t sx = 0;
		int32_t sy = 0;
		Pixel::Mode m = pixel_mode;

		if (m != Pixel::CUSTOM) {
			if (col.a != 255)		set_pixel_mode(Pixel::ALPHA);
			else					set_pixel_mode(Pixel::MASK);
		}

		for (auto c : text) {
			if (c == '\n') {
				sx = 0; sy += 8 * scale;
			}
			else if (c == '\t') {
				sx += 8 * tab_size_in_spaces * scale;
			}
			else {
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1) {
					for (int32_t i = 0; i < font_spacing[c - 32].y; i++)
						for (int32_t j = 0; j < 8; j++)
							if (font_renderable.Sprite()->get_pixel(i + ox * 8 + font_spacing[c - 32].x, j + oy * 8).r > 0)
								for (int32_t is = 0; is < int(scale); is++)
									for (int32_t js = 0; js < int(scale); js++)
										draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
				}
				else {
					for (int32_t i = 0; i < font_spacing[c - 32].y; i++)
						for (int32_t j = 0; j < 8; j++)
							if (font_renderable.Sprite()->get_pixel(i + ox * 8 + font_spacing[c - 32].x, j + oy * 8).r > 0)
								draw(x + sx + i, y + sy + j, col);
				}
				sx += font_spacing[c - 32].y * scale;
			}
		}
		set_pixel_mode(m);
	}

	void Engine::set_pixel_mode(Pixel::Mode m) { 
        pixel_mode = m; 
    }

	Pixel::Mode Engine::get_pixel_mode() { 
        return pixel_mode; 
    }

	void Engine::set_pixel_mode(std::function<engine::Pixel(const int x, const int y, const engine::Pixel&, const engine::Pixel&)> pixelmode) {
		func_pixel_mode = pixelmode;
		pixel_mode = Pixel::Mode::CUSTOM;
	}

	void Engine::set_pixel_blend(float blend) {
		blend_factor = blend;
		if (blend_factor < 0.0f) blend_factor = 0.0f;
		if (blend_factor > 1.0f) blend_factor = 1.0f;
	}

	std::stringstream& Engine::console_out() { 
        return console_output; 
    }

	bool Engine::is_console_showing() const { 
        return show_console; 
    }

	void Engine::console_show(const engine::Key& key_exit, bool suspend_time) {
		if (show_console)
			return;

		show_console = true;		
		console_suspend_time = suspend_time;
		text_entry_enable(true);
		key_console_exit = key_exit;
		keyboard_state[key_console_exit].held = false;
		keyboard_state[key_console_exit].pressed = false;
		keyboard_state[key_console_exit].released = true;
	}
	
	void Engine::console_clear() { 
        console_lines.clear(); 
    }

	void Engine::console_capture_std_out(const bool capture) {
		if(capture)
			buf_old_cout = std::cout.rdbuf(console_output.rdbuf());
		else
			std::cout.rdbuf(buf_old_cout);
	}

	void Engine::update_console() {
		if (get_key(key_console_exit).pressed) {
			text_entry_enable(false);
			console_suspend_time = false;
			show_console = false;
			return;
		}

		console_character_scale = engine::float_vector_2d(1.0f, 2.0f) / (engine::float_vector_2d(view_size) * inv_screen_size);
		console_size = (view_size / engine::int_vector_2d(8, 16)) - engine::int_vector_2d(2, 4);

		if (console_size.y != console_lines.size()) {
			console_cursor = { 0,0 };
			console_lines.clear();
			console_lines.resize(console_size.y);
		}

		auto TypeCharacter = [&](const char c) {
			if (c >= 32 && c < 127) {
				console_lines[console_cursor.y].append(1, c);
				console_cursor.x++;
			}

			if( c == '\n' || console_cursor.x >= console_size.x) {
				console_cursor.y++; 
                console_cursor.x = 0;				
			}			

			if (console_cursor.y >= console_size.y) {
				console_cursor.y = console_size.y - 1;
				for (size_t i = 1; i < console_size.y; i++)
					console_lines[i - 1] = console_lines[i];
				console_lines[console_cursor.y].clear();
			}
		};

		while (console_output.rdbuf()->sgetc() != -1) {
			char c = console_output.rdbuf()->sbumpc();
			TypeCharacter(c);
		}

		gradient_fill_rect_decal({ 0,0 }, engine::float_vector_2d(screen_size), engine::pixel_float(0, 0, 0.5f, 0.5f), engine::pixel_float(0, 0, 0.25f, 0.5f), engine::pixel_float(0, 0, 0.25f, 0.5f), engine::pixel_float(0, 0, 0.25f, 0.5f));
		
        set_decal_mode(engine::DecalMode::NORMAL);
		for (int32_t line = 0; line < console_size.y; line++)
			draw_string_decal(engine::float_vector_2d( 1, 1 + float(line) ) * console_character_scale * 8.0f, console_lines[line], engine::WHITE, console_character_scale);

		fill_rect_decal(engine::float_vector_2d(1 + float((text_entry_get_cursor() + 1)), 1 + float((console_size.y - 1))) * console_character_scale * 8.0f, engine::float_vector_2d(8, 8) * console_character_scale, engine::BLACK); // TODO: Add DARK_CYAN
		draw_string_decal(engine::float_vector_2d(1, 1 + float((console_size.y - 1))) * console_character_scale * 8.0f, std::string(">") + text_entry_get_string(), engine::WHITE, console_character_scale); // TODO: Add YELLOW
	}

	const std::vector<std::string>& Engine::get_dropped_files() const { 
        return dropped_files;
    }

	const engine::int_vector_2d& Engine::get_dropped_files_point() const { 
        return dropped_files_point; 
    }

	void Engine::text_entry_enable(const bool enable, const std::string& text) {
		if (enable) {
			text_entry_cursor = int32_t(text.size());
			text_entry_string = text;
			enable_text_entry = true;
		}
		else {
			enable_text_entry = false;
		}
	}

	std::string Engine::text_entry_get_string() const { 
        return text_entry_string; 
    }

	int32_t Engine::text_entry_get_cursor() const { 
        return text_entry_cursor; 
    }

	bool Engine::is_text_entry_enabled() const { 
        return enable_text_entry; 
    }

	void Engine::update_text_entry() {
		for (const auto& key : keyboard)
			if (get_key(std::get<0>(key)).pressed) {
				text_entry_string.insert(text_entry_cursor, get_key(engine::Key::SHIFT).held ? std::get<2>(key) : std::get<1>(key));
				text_entry_cursor++;
			}

		if (get_key(engine::Key::LEFT).pressed)
			text_entry_cursor = std::max(0, text_entry_cursor - 1);
		if (get_key(engine::Key::RIGHT).pressed)
			text_entry_cursor = std::min(int32_t(text_entry_string.size()), text_entry_cursor + 1);
		
        if (get_key(engine::Key::BACK).pressed && text_entry_cursor > 0) {
			text_entry_string.erase(text_entry_cursor-1, 1);
			text_entry_cursor = std::max(0, text_entry_cursor - 1);
		}

		if (get_key(engine::Key::DEL).pressed && text_entry_cursor < text_entry_string.size())
			text_entry_string.erase(text_entry_cursor, 1);	

		if (get_key(engine::Key::UP).pressed) {
			if (!command_history.empty()) {
				if (command_history_it != command_history.begin())
					command_history_it--;

				text_entry_cursor = int32_t(command_history_it->size());
				text_entry_string = *command_history_it;
			}
		}

		if (get_key(engine::Key::DOWN).pressed) {	
			if (!command_history.empty()) {
				if (command_history_it != command_history.end()) {
					command_history_it++;
					if (command_history_it != command_history.end()) {
						text_entry_cursor = int32_t(command_history_it->size());
						text_entry_string = *command_history_it;
					}
					else {
						text_entry_cursor = 0;
						text_entry_string = "";
					}
				}
			}
		}

		if (get_key(engine::Key::ENTER).pressed) {
			if (show_console) {
				std::cout << ">" + text_entry_string + "\n";
				if (on_console_command(text_entry_string)) {
					command_history.push_back(text_entry_string);
					command_history_it = command_history.end();
				}
				text_entry_string.clear();
				text_entry_cursor = 0;
			}
			else {
				on_text_entry_complete(text_entry_string);
				text_entry_enable(false);
			}
		}
	}

	bool Engine::on_create() { 
        return false;
    }

	bool Engine::on_update(float elapsed_time) { 
        UNUSED(elapsed_time); 
        return false;
    }

	bool Engine::on_destroy() { 
        return true; 
    }

	void Engine::on_text_entry_complete(const std::string& text) { UNUSED(text); }
	bool Engine::on_console_command(const std::string& command) { 
        UNUSED(command); 
        return false; 
    }
	
	void Engine::engine_update_viewport() {
		int32_t ww = screen_size.x * pixel_size.x;
		int32_t wh = screen_size.y * pixel_size.y;
		float wasp = (float)ww / (float)wh;

		if (pixel_cohesion) {
			screen_pixel_size = (window_size / screen_size);
			view_size = (window_size / screen_size) * screen_size;
		}
		else {
			view_size.x = (int32_t)window_size.x;
			view_size.y = (int32_t)((float)view_size.x / wasp);

			if (view_size.y > window_size.y) {
				view_size.y = window_size.y;
				view_size.x = (int32_t)((float)view_size.y * wasp);
			}
		}

		view_pos = (window_size - view_size) / 2;
	}

	void Engine::engine_update_window_size(int32_t x, int32_t y) {
		window_size = { x, y };
		engine_update_viewport();
	}

	void Engine::engine_update_mouse_wheel(int32_t delta) { 
        mouse_wheel_delta_cache += delta; 
    }

	void Engine::engine_update_mouse(int32_t x, int32_t y) {
		has_mouse_focus = true;
		mouse_window_pos = { x, y };
		
        x -= view_pos.x;
		y -= view_pos.y;
		
        mouse_pos_cache.x = (int32_t)(((float)x / (float)(window_size.x - (view_pos.x * 2)) * (float)screen_size.x));
		mouse_pos_cache.y = (int32_t)(((float)y / (float)(window_size.y - (view_pos.y * 2)) * (float)screen_size.y));

		if (mouse_pos_cache.x >= (int32_t)screen_size.x)	mouse_pos_cache.x = screen_size.x - 1;
		if (mouse_pos_cache.y >= (int32_t)screen_size.y)	mouse_pos_cache.y = screen_size.y - 1;
		if (mouse_pos_cache.x < 0) mouse_pos_cache.x = 0;
		if (mouse_pos_cache.y < 0) mouse_pos_cache.y = 0;
	}

	void Engine::engine_update_mouse_state(int32_t button, bool state) { 
        mouse_new_state[button] = state; 
    }

	void Engine::engine_update_key_state(int32_t key, bool state) { 
        key_new_state[key] = state; 
    }

	void Engine::engine_update_mouse_focus(bool state) { 
        has_mouse_focus = state; 
    }

	void Engine::engine_update_key_focus(bool state) { 
        has_input_focus = state; 
    }

	void Engine::engine_drop_files(int32_t x, int32_t y, const std::vector<std::string>& files) { 
		x -= view_pos.x;
		y -= view_pos.y;

		dropped_files_point_cache.x = (int32_t)(((float)x / (float)(window_size.x - (view_pos.x * 2)) * (float)screen_size.x));
		dropped_files_point_cache.y = (int32_t)(((float)y / (float)(window_size.y - (view_pos.y * 2)) * (float)screen_size.y));

		if (dropped_files_point_cache.x >= (int32_t)screen_size.x)	dropped_files_point_cache.x = screen_size.x - 1;
		if (dropped_files_point_cache.y >= (int32_t)screen_size.y)	dropped_files_point_cache.y = screen_size.y - 1;
		if (dropped_files_point_cache.x < 0) dropped_files_point_cache.x = 0;
		if (dropped_files_point_cache.y < 0) dropped_files_point_cache.y = 0;

		dropped_files_cache = files; 
	}

	void Engine::engine_reanimate() { 
        atom_active = true; 
    }

	bool Engine::engine_is_running() { 
        return atom_active; 
    }

	void Engine::engine_terminate() { 
        atom_active = false; 
    }

	void Engine::engine_thread() {
		if (platform->thread_startup() == engine::FAIL)	return;

		engine_prepare();

		for (auto& ext : extensions) ext->on_before_create();
		if (!on_create()) atom_active = false;
		for (auto& ext : extensions) ext->on_after_create();

		while (atom_active) {
			while (atom_active) { engine_core_update(); }

			if (!on_destroy()) {
				atom_active = true;
			}
		}

		platform->thread_cleanup();
	}

	void Engine::engine_prepare() {
		if (platform->create_graphics(fullscreen, enable_VSYNC, view_pos, view_size) == engine::FAIL) return;

		engine_construct_fontsheet();

		create_layer();
		layers[0].update = true;
		layers[0].show = true;
		set_draw_target(nullptr);

		time_point1 = std::chrono::system_clock::now();
		time_point2 = std::chrono::system_clock::now();
	}

	void Engine::engine_core_update() {
		time_point2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = time_point2 - time_point1;
		time_point1 = time_point2;

		float elapsed_time = elapsedTime.count();
		last_elapsed = elapsed_time;

		if (console_suspend_time)
			elapsed_time = 0.0f;

		platform->handle_system_event();

		auto ScanHardware = [&](ButtonState* keys, bool* state_old, bool* state_new, uint32_t key_count) {
			for (uint32_t i = 0; i < key_count; i++) {
				keys[i].pressed = false;
				keys[i].released = false;
				if (state_new[i] != state_old[i]) {
					if (state_new[i]) {
						keys[i].pressed = !keys[i].held;
						keys[i].held = true;
					}
					else {
						keys[i].released = true;
						keys[i].held = false;
					}
				}
				state_old[i] = state_new[i];
			}
		};

		ScanHardware(keyboard_state, key_old_state, key_new_state, 256);
		ScanHardware(mouse_state, mouse_old_state, mouse_new_state, mouse_buttons);

		mouse_pos = mouse_pos_cache;
		mouse_wheel_delta = mouse_wheel_delta_cache;
		mouse_wheel_delta_cache = 0;

		dropped_files = dropped_files_cache;
		dropped_files_point = dropped_files_point_cache;
		dropped_files_cache.clear();

		if (enable_text_entry) {
			update_text_entry();
		}

		bool extension_block_frame = false;		
		for (auto& ext : extensions) extension_block_frame |= ext->on_before_update(elapsed_time);
		if (!extension_block_frame) {
			if (!on_update(elapsed_time)) atom_active = false;
		}

		for (auto& ext : extensions) ext->on_after_update(elapsed_time);

		if (show_console) {
			set_draw_target((uint8_t)0);
			update_console();
		}

		renderer->update_viewport(view_pos, view_size);
		renderer->clear_buffer(engine::BLACK, true);

		layers[0].update = true;
		layers[0].show = true;
		set_decal_mode(DecalMode::NORMAL);
		renderer->prepare_drawing();

		for (auto layer = layers.rbegin(); layer != layers.rend(); ++layer) {
			if (layer->show) {
				if (layer->func_hook == nullptr) {
					renderer->apply_texture(layer->draw_target.Decal()->id);
					if (!suspend_texture_transfer && layer->update) {
						layer->draw_target.Decal()->update();
						layer->update = false;
					}

					renderer->draw_layer_quad(layer->offset, layer->scale, layer->tint);

					for (auto& decal : layer->decal_instances)
						renderer->draw_decal(decal);
					layer->decal_instances.clear();
				}
				else {
					layer->func_hook();
				}
			}
		}

		renderer->display_frame();

		frame_timer += elapsed_time;
		frame_count++;
		if (frame_timer >= 1.0f) {
			last_FPS = frame_count;
			frame_timer -= 1.0f;
			std::string title = "isakhorvath.me - engine - " + app_name + " - FPS: " + std::to_string(frame_count);
			platform->set_window_title(title);
			frame_count = 0;
		}
	}

	void Engine::engine_construct_fontsheet()
	{
		std::string data;
		data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
		data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
		data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
		data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
		data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
		data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
		data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
		data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
		data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
		data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
		data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
		data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
		data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
		data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
		data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
		data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		font_renderable.create(128, 48);

		int px = 0, py = 0;
		for (size_t b = 0; b < 1024; b += 4) {
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++) {
				int k = r & (1 << i) ? 255 : 0;
				font_renderable.Sprite()->set_pixel(px, py, engine::Pixel(k, k, k, k));
				if (++py == 48) { 
                    px++; 
                    py = 0; 
                }
			}
		}

		font_renderable.Decal()->update();

		constexpr std::array<uint8_t, 96> spacing = { {
			0x03,0x25,0x16,0x08,0x07,0x08,0x08,0x04,0x15,0x15,0x08,0x07,0x15,0x07,0x24,0x08,
			0x08,0x17,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x24,0x15,0x06,0x07,0x16,0x17,
			0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x17,0x08,0x08,0x17,0x08,0x08,0x08,
			0x08,0x08,0x08,0x08,0x17,0x08,0x08,0x08,0x08,0x17,0x08,0x15,0x08,0x15,0x08,0x08,
			0x24,0x18,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x33,0x17,0x17,0x33,0x18,0x17,0x17,
			0x17,0x17,0x17,0x17,0x07,0x17,0x17,0x18,0x18,0x17,0x17,0x07,0x33,0x07,0x08,0x00, } };

		for (auto c : spacing) font_spacing.push_back({ c >> 4, c & 15 });

#ifdef ENGINE_KEYBOARD_UK
		keyboard =
		{
			{engine::Key::A, "a", "A"}, {engine::Key::B, "b", "B"}, {engine::Key::C, "c", "C"}, {engine::Key::D, "d", "D"}, {engine::Key::E, "e", "E"},
			{engine::Key::F, "f", "F"}, {engine::Key::G, "g", "G"}, {engine::Key::H, "h", "H"}, {engine::Key::I, "i", "I"}, {engine::Key::J, "j", "J"},
			{engine::Key::K, "k", "K"}, {engine::Key::L, "l", "L"}, {engine::Key::M, "m", "M"}, {engine::Key::N, "n", "N"}, {engine::Key::O, "o", "O"},
			{engine::Key::P, "p", "P"}, {engine::Key::Q, "q", "Q"}, {engine::Key::R, "r", "R"}, {engine::Key::S, "s", "S"}, {engine::Key::T, "t", "T"},
			{engine::Key::U, "u", "U"}, {engine::Key::V, "v", "V"}, {engine::Key::W, "w", "W"}, {engine::Key::X, "x", "X"}, {engine::Key::Y, "y", "Y"},
			{engine::Key::Z, "z", "Z"},

			{engine::Key::K0, "0", ")"}, {engine::Key::K1, "1", "!"}, {engine::Key::K2, "2", "\""}, {engine::Key::K3, "3", "#"},	{engine::Key::K4, "4", "$"},
			{engine::Key::K5, "5", "%"}, {engine::Key::K6, "6", "^"}, {engine::Key::K7, "7", "&"}, {engine::Key::K8, "8", "*"},	{engine::Key::K9, "9", "("},

			{engine::Key::NP0, "0", "0"}, {engine::Key::NP1, "1", "1"}, {engine::Key::NP2, "2", "2"}, {engine::Key::NP3, "3", "3"},	{engine::Key::NP4, "4", "4"},
			{engine::Key::NP5, "5", "5"}, {engine::Key::NP6, "6", "6"}, {engine::Key::NP7, "7", "7"}, {engine::Key::NP8, "8", "8"},	{engine::Key::NP9, "9", "9"},
			{engine::Key::NP_MUL, "*", "*"}, {engine::Key::NP_DIV, "/", "/"}, {engine::Key::NP_ADD, "+", "+"}, {engine::Key::NP_SUB, "-", "-"},	{engine::Key::NP_DECIMAL, ".", "."},

			{engine::Key::PERIOD, ".", ">"}, {engine::Key::EQUALS, "=", "+"}, {engine::Key::COMMA, ",", "<"}, {engine::Key::MINUS, "-", "_"}, {engine::Key::SPACE, " ", " "},

			{engine::Key::OEM_1, ";", ":"}, {engine::Key::OEM_2, "/", "?"}, {engine::Key::OEM_3, "\'", "@"}, {engine::Key::OEM_4, "[", "{"},
			{engine::Key::OEM_5, "\\", "|"}, {engine::Key::OEM_6, "]", "}"}, {engine::Key::OEM_7, "#", "~"}, 
		};
#endif
	}

	void Engine::enginex_register(engine::EngineX* engine_x) {
		if (std::find(extensions.begin(), extensions.end(), engine_x) == extensions.end())
			extensions.push_back(engine_x);			
	}

	EngineX::EngineX(bool hook) { if(hook) engine->enginex_register(this); }
	void EngineX::on_before_create() {}
	void EngineX::on_after_create()	{}
	bool EngineX::on_before_update(float& elapsed_time) { return false; }
	void EngineX::on_after_update(float elapsed_time) {}

	std::atomic<bool> Engine::atom_active{ false };
	engine::Engine* engine::EngineX::engine = nullptr;
	engine::Engine* engine::Platform::ptr_engine = nullptr;
	engine::Engine* engine::Renderer::ptr_engine = nullptr;
	std::unique_ptr<ImageLoader> engine::Sprite::loader = nullptr;
};
#pragma endregion 

#pragma region platform_headless
namespace engine {
#if defined(ENGINE_GFX_HEADLESS)
	class Renderer_Headless : public engine::Renderer {
	public:
		virtual void         prepare_device() {};
		virtual engine::Code create_device(std::vector<void*> params, bool fullscreen, bool VSYNC) { return engine::Code::OK; }
		virtual engine::Code destroy_device() { return engine::Code::OK; }
		virtual void         display_frame() {}
		virtual void         prepare_drawing() {}
		virtual void	     set_decal_mode(const engine::DecalMode& mode) {}
		virtual void         draw_layer_quad(const engine::float_vector_2d& offset, const engine::float_vector_2d& scale, const engine::Pixel tint) {}
		virtual void         draw_decal(const engine::DecalInstance& decal) {}
		virtual uint32_t     create_texture(const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) {return 1;};
		virtual void         update_texture(uint32_t id, engine::Sprite* spr) {}
		virtual void         read_texture(uint32_t id, engine::Sprite* spr) {}
		virtual uint32_t     delete_texture(const uint32_t id) {return 1;}
		virtual void         apply_texture(uint32_t id) {}
		virtual void         update_viewport(const engine::int_vector_2d& pos, const engine::int_vector_2d& size) {}
		virtual void         clear_buffer(engine::Pixel p, bool depth) {}
	};
#endif

#if defined(ENGINE_PLATFORM_HEADLESS)
	class Platform_Headless : public engine::Platform {
	public:
		virtual engine::Code application_startup() { return engine::Code::OK; }
		virtual engine::Code application_cleanup() { return engine::Code::OK; }
		virtual engine::Code thread_startup() { return engine::Code::OK; }
		virtual engine::Code thread_cleanup() { return engine::Code::OK; }
		virtual engine::Code create_graphics(bool fullscreen, bool enable_VSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) { return engine::Code::OK; }
		virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool fullscreen) { return engine::Code::OK; }
		virtual engine::Code set_window_title(const std::string& s) { return engine::Code::OK; }
		virtual engine::Code start_system_event_loop() { return engine::Code::OK; }
		virtual engine::Code handle_system_event() { return engine::Code::OK; }
	};
#endif
}
#pragma endregion

#pragma region image_stb

#if defined(ENGINE_IMAGE_STB)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace engine {
	class ImageLoader_STB : public engine::ImageLoader {
	public:
		ImageLoader_STB() : ImageLoader() {}

		engine::Code load_img_resource(engine::Sprite* spr, const std::string& img_file, engine::ResourcePack* pack) override {
			UNUSED(pack);
			spr->col_data.clear();
			stbi_uc* bytes = nullptr;
			int w = 0, h = 0, cmp = 0;
			if (pack != nullptr) {
				ResourceBuffer rb = pack->get_file_buffer(img_file);
				bytes = stbi_load_from_memory((unsigned char*)rb.memory.data(), rb.memory.size(), &w, &h, &cmp, 4);
			}
			else {
				if (!_gfs::exists(img_file)) return engine::Code::NO_FILE;
				bytes = stbi_load(img_file.c_str(), &w, &h, &cmp, 4);
			}

			if (!bytes) return engine::Code::FAIL;
			spr->width = w; spr->height = h;
			spr->col_data.resize(spr->width * spr->height);
			std::memcpy(spr->col_data.data(), bytes, spr->width * spr->height * 4);
			delete[] bytes;
			return engine::Code::OK;
		}

		engine::Code save_img_resource(engine::Sprite* spr, const std::string& img_file) override {
			return engine::Code::OK;
		}
	};
}
#endif
#pragma endregion

#if !defined(ENGINE_PGE_HEADLESS)

#pragma region renderer_ogl10
#if defined(ENGINE_GFX_OPENGL10)

#if defined(ENGINE_PLATFORM_WINAPI)
	#include <dwmapi.h>
	#include <GL/gl.h>
	#if !defined(__MINGW32__)
		#pragma comment(lib, "Dwmapi.lib")
	#endif
	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t* wglSwapInterval = nullptr;
	typedef HDC glDeviceContext_t;
	typedef HGLRC glRenderContext_t;
#endif

#if defined(__linux__) || defined(__FreeBSD__)
	#include <GL/gl.h>
#endif

#if defined(ENGINE_PLATFORM_X11)
	namespace X11 {
		#include <GL/glx.h>
	}
	typedef int(glSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
	static glSwapInterval_t* glSwapIntervalEXT;
	typedef X11::GLXContext glDeviceContext_t;
	typedef X11::GLXContext glRenderContext_t;
#endif

#if defined(__APPLE__)
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#endif

namespace engine {
	class Renderer_OGL10 : public engine::Renderer
	{
	private:
#if defined(ENGINE_PLATFORM_GLUT)
		bool _fullscreen = false;
#else
		glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
#endif

		bool sync = false;
		engine::DecalMode decal_mode = engine::DecalMode(-1);
		engine::DecalStructure decal_structure = engine::DecalStructure(-1);
#if defined(ENGINE_PLATFORM_X11)
		X11::Display* engine_Display = nullptr;
		X11::Window* engine_Window = nullptr;
		X11::XVisualInfo* engine_VisualInfo = nullptr;
#endif

	public:
		void prepare_device() override {
#if defined(ENGINE_PLATFORM_GLUT)
			// workaround: glutInit has to be called with arguments
			int argc = 0;
			char* argv[1] = { (char*)"" };
			glutInit(&argc, argv);
			glutInitWindowPosition(0, 0);
			glutInitWindowSize(512, 512);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
			glutCreateWindow("isakhorvath.me - Engine");
			glEnable(GL_TEXTURE_2D);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
		}

		engine::Code create_device(std::vector<void*> params, bool fullscreen, bool VSYNC) override {
#if defined(ENGINE_PLATFORM_WINAPI)
			glDeviceContext = GetDC((HWND)(params[0]));
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			int pf = 0;
			if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return engine::FAIL;
			SetPixelFormat(glDeviceContext, pf, &pfd);

			if (!(glRenderContext = wglCreateContext(glDeviceContext))) return engine::FAIL;
			wglMakeCurrent(glDeviceContext, glRenderContext);

			wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapInterval && !VSYNC) wglSwapInterval(0);
			sync = VSYNC;
#endif

#if defined(ENGINE_PLATFORM_X11)
			using namespace X11;
			engine_Display = (X11::Display*)(params[0]);
			engine_Window = (X11::Window*)(params[1]);
			engine_VisualInfo = (X11::XVisualInfo*)(params[2]);

			glDeviceContext = glXCreateContext(engine_Display, engine_VisualInfo, nullptr, GL_TRUE);
			glXMakeCurrent(engine_Display, *engine_Window, glDeviceContext);

			XWindowAttributes gwa;
			XGetWindowAttributes(engine_Display, *engine_Window, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);

			glSwapIntervalEXT = nullptr;
			glSwapIntervalEXT = (glSwapInterval_t*)glXGetProcAddress((unsigned char*)"glXSwapIntervalEXT");

			if (glSwapIntervalEXT == nullptr && !VSYNC) {
				printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
				printf("      Don't worry though, things will still work, it's just the\n");
				printf("      frame rate will be capped to your monitors refresh rate.\n");
			}

			if (glSwapIntervalEXT != nullptr && !VSYNC)
				glSwapIntervalEXT(engine_Display, *engine_Window, 0);
#endif		

#if defined(ENGINE_PLATFORM_GLUT)
			_fullscreen = fullscreen;
			if (!VSYNC) {
#if defined(__APPLE__)
				GLint sync = 0;
				CGLContextObj ctx = CGLGetCurrentContext();
				if (ctx) CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
			}
#else
			glEnable(GL_TEXTURE_2D);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
			return engine::Code::OK;
		}

		engine::Code destroy_device() override {
#if defined(ENGINE_PLATFORM_WINAPI)
			wglDeleteContext(glRenderContext);
#endif

#if defined(ENGINE_PLATFORM_X11)
			glXMakeCurrent(engine_Display, None, NULL);
			glXDestroyContext(engine_Display, glDeviceContext);
#endif

#if defined(ENGINE_PLATFORM_GLUT)
			glutDestroyWindow(glutGetWindow());
#endif
			return engine::Code::OK;
		}

		void display_frame() override {
#if defined(ENGINE_PLATFORM_WINAPI)
			SwapBuffers(glDeviceContext);
			if (sync) DwmFlush();
#endif	

#if defined(ENGINE_PLATFORM_X11)
			X11::glXSwapBuffers(engine_Display, *engine_Window);
#endif		

#if defined(ENGINE_PLATFORM_GLUT)
			glutSwapBuffers();
#endif
		}

		void prepare_drawing() override {
			glEnable(GL_BLEND);
			decal_mode = DecalMode::NORMAL;
			decal_structure = DecalStructure::FAN;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void set_decal_mode(const engine::DecalMode& mode) {
			if (mode != decal_mode) {
				switch (mode) {
				case engine::DecalMode::NORMAL:
				case engine::DecalMode::MODEL3D:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case engine::DecalMode::ADDITIVE:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					break;
				case engine::DecalMode::MULTIPLICATIVE:
					glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case engine::DecalMode::STENCIL:
					glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
					break;
				case engine::DecalMode::ILLUMINATE:
					glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
					break;
				case engine::DecalMode::WIREFRAME:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				}

				decal_mode = mode;
			}
		}

		void draw_layer_quad(const engine::float_vector_2d& offset, const engine::float_vector_2d& scale, const engine::Pixel tint) override {
			glBegin(GL_QUADS);
			glColor4ub(tint.r, tint.g, tint.b, tint.a);
			glTexCoord2f(0.0f * scale.x + offset.x, 1.0f * scale.y + offset.y);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f * scale.x + offset.x, 0.0f * scale.y + offset.y);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(1.0f * scale.x + offset.x, 0.0f * scale.y + offset.y);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(1.0f * scale.x + offset.x, 1.0f * scale.y + offset.y);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glEnd();
		}

		void draw_decal(const engine::DecalInstance& decal) override {
			set_decal_mode(decal.mode);

			if (decal.decal == nullptr)
				glBindTexture(GL_TEXTURE_2D, 0);
			else
				glBindTexture(GL_TEXTURE_2D, decal.decal->id);
			
			if (decal_mode == DecalMode::MODEL3D) {
#ifdef ENGINE_ENABLE_EXPERIMENTAL
				glMatrixMode(GL_PROJECTION); glPushMatrix();
				glMatrixMode(GL_MODELVIEW);  glPushMatrix();

				glEnable(GL_DEPTH_TEST);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 1, 1000);

				#pragma comment (lib, "winmm.lib")

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0, -40, -200);
				glRotatef(float(clock()) * 0.1f, 1, 0, 0);
				glRotatef(float(clock()) * 0.1f * 2, 0, 1, 0);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_TRIANGLES);
				
				for (uint32_t n = 0; n < decal.points; n++) {
					glColor4ub(decal.tint[n].r, decal.tint[n].g, decal.tint[n].b, decal.tint[n].a);
					glTexCoord2f(decal.uv[n].x, decal.uv[n].y);
					glVertex3f(decal.pos[n].x, decal.pos[n].y, decal.w[n]);
				}

				glEnd();

				glMatrixMode(GL_PROJECTION); glPopMatrix();
				glMatrixMode(GL_MODELVIEW);  glPopMatrix();
				glDisable(GL_DEPTH_TEST);
#endif
			}
			else {
				if (decal_mode == DecalMode::WIREFRAME)
					glBegin(GL_LINE_LOOP);
				else {
					if(decal.structure == engine::DecalStructure::FAN)
						glBegin(GL_TRIANGLE_FAN);
					else if(decal.structure == engine::DecalStructure::STRIP)
						glBegin(GL_TRIANGLE_STRIP);
					else if(decal.structure == engine::DecalStructure::LIST)
						glBegin(GL_TRIANGLES);
				}

				for (uint32_t n = 0; n < decal.points; n++) {
					glColor4ub(decal.tint[n].r, decal.tint[n].g, decal.tint[n].b, decal.tint[n].a);
					glTexCoord4f(decal.uv[n].x, decal.uv[n].y, 0.0f, decal.w[n]);
					glVertex2f(decal.pos[n].x, decal.pos[n].y);
				}
				glEnd();
			}

		}

		uint32_t create_texture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override {
			UNUSED(width);
			UNUSED(height);
			uint32_t id = 0;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			if (filtered) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}

			if (clamp) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			return id;
		}

		uint32_t delete_texture(const uint32_t id) override {
			glDeleteTextures(1, &id);
			return id;
		}

		void update_texture(uint32_t id, engine::Sprite* spr) override {
			UNUSED(id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->get_data());
		}

		void read_texture(uint32_t id, engine::Sprite* spr) override {
			glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->get_data());
		}

		void apply_texture(uint32_t id) override {
			glBindTexture(GL_TEXTURE_2D, id);
		}

		void clear_buffer(engine::Pixel p, bool depth) override {
			glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			if (depth) glClear(GL_DEPTH_BUFFER_BIT);
		}

		void update_viewport(const engine::int_vector_2d& pos, const engine::int_vector_2d& size) override {
			glViewport(pos.x, pos.y, size.x, size.y);
		}
	};
}
#endif

#pragma endregion

#pragma region renderer_ogl33
#if defined(ENGINE_GFX_OPENGL33)

#if defined(ENGINE_PLATFORM_WINAPI)
	#include <dwmapi.h>
	#if !defined(__MINGW32__)
		#pragma comment(lib, "Dwmapi.lib")
	#endif		
	typedef HDC glDeviceContext_t;
	typedef HGLRC glRenderContext_t;
	#define OGL_LOAD(t, n) (t*)wglGetProcAddress(#n)
#endif

#if defined(ENGINE_PLATFORM_X11)
	typedef X11::GLXContext glDeviceContext_t;
	typedef X11::GLXContext glRenderContext_t;
	#define OGL_LOAD(t, n) (t*)glXGetProcAddress((unsigned char*)#n);
#endif

namespace engine {
	class Renderer_OGL33 : public engine::Renderer {
	private:
#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
		EGLDisplay engine_Display;
		EGLConfig engine_Config;
		EGLContext engine_Context;
		EGLSurface engine_Surface;
#endif

#if defined(ENGINE_PLATFORM_GLUT)
		bool _fullscreen = false;
#else
	#if !defined(ENGINE_PLATFORM_EMSCRIPTEN)
		glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
	#endif
#endif
		bool sync = false;
		engine::DecalMode decal_mode = engine::DecalMode(-1);
#if defined(ENGINE_PLATFORM_X11)
		X11::Display* engine_Display = nullptr;
		X11::Window* engine_Window = nullptr;
		X11::XVisualInfo* engine_VisualInfo = nullptr;
#endif

	private:
		locCreateShader_t* locCreateShader = nullptr;
		locShaderSource_t* locShaderSource = nullptr;
		locCompileShader_t* locCompileShader = nullptr;
		locDeleteShader_t* locDeleteShader = nullptr;
		locCreateProgram_t* locCreateProgram = nullptr;
		locDeleteProgram_t* locDeleteProgram = nullptr;
		locLinkProgram_t* locLinkProgram = nullptr;
		locAttachShader_t* locAttachShader = nullptr;
		locBindBuffer_t* locBindBuffer = nullptr;
		locBufferData_t* locBufferData = nullptr;
		locGenBuffers_t* locGenBuffers = nullptr;
		locVertexAttribPointer_t* locVertexAttribPointer = nullptr;
		locEnableVertexAttribArray_t* locEnableVertexAttribArray = nullptr;
		locUseProgram_t* locUseProgram = nullptr;
		locBindVertexArray_t* locBindVertexArray = nullptr;
		locGenVertexArrays_t* locGenVertexArrays = nullptr;
		locSwapInterval_t* locSwapInterval = nullptr;
		locGetShaderInfoLog_t* locGetShaderInfoLog = nullptr;

		uint32_t m_nFS = 0;
		uint32_t m_nVS = 0;
		uint32_t m_nQuadShader = 0;
		uint32_t m_vbQuad = 0;
		uint32_t m_vaQuad = 0;

		struct locVertex {
			float pos[3];
			engine::float_vector_2d tex;
			engine::Pixel col;
		};

		locVertex pVertexMem[ENGINE_MAX_VERTS];

		engine::Renderable rendBlankQuad;

	public:
		void prepare_device() override {
#if defined(ENGINE_PLATFORM_GLUT)
			int argc = 0;
			char* argv[1] = { (char*)"" };
			glutInit(&argc, argv);
			glutInitWindowPosition(0, 0);
			glutInitWindowSize(512, 512);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
			glutCreateWindow("isakhorvath.me - Engine");
			glEnable(GL_TEXTURE_2D);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
		}

		engine::Code create_device(std::vector<void*> params, bool fullscreen, bool VSYNC) override {
#if defined(ENGINE_PLATFORM_WINAPI)
			glDeviceContext = GetDC((HWND)(params[0]));
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			int pf = 0;
			if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return engine::FAIL;
			SetPixelFormat(glDeviceContext, pf, &pfd);

			if (!(glRenderContext = wglCreateContext(glDeviceContext))) return engine::FAIL;
			wglMakeCurrent(glDeviceContext, glRenderContext);

			locSwapInterval = OGL_LOAD(locSwapInterval_t, wglSwapIntervalEXT);
			if (locSwapInterval && !VSYNC) locSwapInterval(0);
			sync = VSYNC;
#endif

#if defined(ENGINE_PLATFORM_X11)
			using namespace X11;

			engine_Display = (X11::Display*)(params[0]);
			engine_Window = (X11::Window*)(params[1]);
			engine_VisualInfo = (X11::XVisualInfo*)(params[2]);

			glDeviceContext = glXCreateContext(engine_Display, engine_VisualInfo, nullptr, GL_TRUE);
			glXMakeCurrent(engine_Display, *engine_Window, glDeviceContext);

			XWindowAttributes gwa;
			XGetWindowAttributes(engine_Display, *engine_Window, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);

			locSwapInterval = OGL_LOAD(locSwapInterval_t, glXSwapIntervalEXT);

			if (locSwapInterval == nullptr && !VSYNC) {
				printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
				printf("      Don't worry though, things will still work, it's just the\n");
				printf("      frame rate will be capped to your monitors refresh rate.\n");
			}

			if (locSwapInterval != nullptr && !VSYNC)
				locSwapInterval(engine_Display, *engine_Window, 0);
#endif		

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
			EGLint const attribute_list[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
			EGLint const context_config[] = { EGL_CONTEXT_CLIENT_VERSION , 2, EGL_NONE };
			EGLint num_config;

			engine_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			eglInitialize(engine_Display, nullptr, nullptr);
			eglChooseConfig(engine_Display, attribute_list, &engine_Config, 1, &num_config);

			engine_Context = eglCreateContext(engine_Display, engine_Config, EGL_NO_CONTEXT, context_config);
			engine_Surface = eglCreateWindowSurface(engine_Display, engine_Config, NULL, nullptr);
			eglMakeCurrent(engine_Display, engine_Surface, engine_Surface, engine_Context);
			locSwapInterval = &eglSwapInterval;
			locSwapInterval(engine_Display, VSYNC ? 1 : 0);
#endif

#if defined(ENGINE_PLATFORM_GLUT)
			_fullscreen = fullscreen;
			if (!VSYNC) {
#if defined(__APPLE__)
				GLint sync = 0;
				CGLContextObj ctx = CGLGetCurrentContext();
				if (ctx) CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
			}
#else
	#if !defined(ENGINE_PLATFORM_EMSCRIPTEN)
			glEnable(GL_TEXTURE_2D);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	#endif
#endif
			locCreateShader = OGL_LOAD(locCreateShader_t, glCreateShader);
			locCompileShader = OGL_LOAD(locCompileShader_t, glCompileShader);
			locShaderSource = OGL_LOAD(locShaderSource_t, glShaderSource);
			locDeleteShader = OGL_LOAD(locDeleteShader_t, glDeleteShader);
			locCreateProgram = OGL_LOAD(locCreateProgram_t, glCreateProgram);
			locDeleteProgram = OGL_LOAD(locDeleteProgram_t, glDeleteProgram);
			locLinkProgram = OGL_LOAD(locLinkProgram_t, glLinkProgram);
			locAttachShader = OGL_LOAD(locAttachShader_t, glAttachShader);
			locBindBuffer = OGL_LOAD(locBindBuffer_t, glBindBuffer);
			locBufferData = OGL_LOAD(locBufferData_t, glBufferData);
			locGenBuffers = OGL_LOAD(locGenBuffers_t, glGenBuffers);
			locVertexAttribPointer = OGL_LOAD(locVertexAttribPointer_t, glVertexAttribPointer);
			locEnableVertexAttribArray = OGL_LOAD(locEnableVertexAttribArray_t, glEnableVertexAttribArray);
			locUseProgram = OGL_LOAD(locUseProgram_t, glUseProgram);
			locGetShaderInfoLog = OGL_LOAD(locGetShaderInfoLog_t, glGetShaderInfoLog);
#if !defined(ENGINE_PLATFORM_EMSCRIPTEN)
			locBindVertexArray = OGL_LOAD(locBindVertexArray_t, glBindVertexArray);
			locGenVertexArrays = OGL_LOAD(locGenVertexArrays_t, glGenVertexArrays);
#else
			locBindVertexArray = glBindVertexArrayOES;
			locGenVertexArrays = glGenVertexArraysOES;
#endif

			m_nFS = locCreateShader(0x8B30);
			const GLchar* strFS =
#if defined(__arm__) || defined(ENGINE_PLATFORM_EMSCRIPTEN)
				"#version 300 es\n"
				"precision mediump float;"
#else
				"#version 330 core\n"
#endif
				"out vec4 pixel;\n""in vec2 oTex;\n"
				"in vec4 oCol;\n""uniform sampler2D sprTex;\n""void main(){pixel = texture(sprTex, oTex) * oCol;}";
			locShaderSource(m_nFS, 1, &strFS, NULL);
			locCompileShader(m_nFS);

			m_nVS = locCreateShader(0x8B31);
			const GLchar* strVS =
#if defined(__arm__) || defined(ENGINE_PLATFORM_EMSCRIPTEN)
				"#version 300 es\n"
				"precision mediump float;"
#else
				"#version 330 core\n"
#endif
				"layout(location = 0) in vec3 aPos;\n""layout(location = 1) in vec2 aTex;\n"
				"layout(location = 2) in vec4 aCol;\n""out vec2 oTex;\n""out vec4 oCol;\n"
				"void main(){ float p = 1.0 / aPos.z; gl_Position = p * vec4(aPos.x, aPos.y, 0.0, 1.0); oTex = p * aTex; oCol = aCol;}";
			locShaderSource(m_nVS, 1, &strVS, NULL);
			locCompileShader(m_nVS);

			m_nQuadShader = locCreateProgram();
			locAttachShader(m_nQuadShader, m_nFS);
			locAttachShader(m_nQuadShader, m_nVS);
			locLinkProgram(m_nQuadShader);

			locGenBuffers(1, &m_vbQuad);
			locGenVertexArrays(1, &m_vaQuad);
			locBindVertexArray(m_vaQuad);
			locBindBuffer(0x8892, m_vbQuad);

			locVertex verts[ENGINE_MAX_VERTS];
			locBufferData(0x8892, sizeof(locVertex) * ENGINE_MAX_VERTS, verts, 0x88E0);
			locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(locVertex), 0); locEnableVertexAttribArray(0);
			locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(locVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
			locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(locVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
			locBindBuffer(0x8892, 0);
			locBindVertexArray(0);

			rendBlankQuad.create(1, 1);
			rendBlankQuad.Sprite()->get_data()[0] = engine::WHITE;
			rendBlankQuad.Decal()->update();
			return engine::Code::OK;
		}

		engine::Code destroy_device() override {
#if defined(ENGINE_PLATFORM_WINAPI)
			wglDeleteContext(glRenderContext);
#endif

#if defined(ENGINE_PLATFORM_X11)
			glXMakeCurrent(engine_Display, None, NULL);
			glXDestroyContext(engine_Display, glDeviceContext);
#endif

#if defined(ENGINE_PLATFORM_GLUT)
			glutDestroyWindow(glutGetWindow());
#endif

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
			eglMakeCurrent(engine_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(engine_Display, engine_Context);
			eglDestroySurface(engine_Display, engine_Surface);
			eglTerminate(engine_Display);
			engine_Display = EGL_NO_DISPLAY;
			engine_Surface = EGL_NO_SURFACE;
			engine_Context = EGL_NO_CONTEXT;
#endif
			return engine::Code::OK;
		}

		void display_frame() override {
#if defined(ENGINE_PLATFORM_WINAPI)
			SwapBuffers(glDeviceContext);
			if (sync) DwmFlush();
#endif	

#if defined(ENGINE_PLATFORM_X11)
			X11::glXSwapBuffers(engine_Display, *engine_Window);
#endif		

#if defined(ENGINE_PLATFORM_GLUT)
			glutSwapBuffers();
#endif

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
			eglSwapBuffers(engine_Display, engine_Surface);
#endif
		}

		void prepare_drawing() override {
			glEnable(GL_BLEND);
			decal_mode = DecalMode::NORMAL;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			locUseProgram(m_nQuadShader);
			locBindVertexArray(m_vaQuad);

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
			locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(locVertex), 0); locEnableVertexAttribArray(0);
			locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(locVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
			locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(locVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
#endif
		}

		void set_decal_mode(const engine::DecalMode& mode) override {
			if (mode != decal_mode) {
				switch (mode) {
				case engine::DecalMode::NORMAL: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
				case engine::DecalMode::ADDITIVE: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
				case engine::DecalMode::MULTIPLICATIVE: glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);	break;
				case engine::DecalMode::STENCIL: glBlendFunc(GL_ZERO, GL_SRC_ALPHA); break;
				case engine::DecalMode::ILLUMINATE: glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);	break;
				case engine::DecalMode::WIREFRAME: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
				}

				decal_mode = mode;
			}
		}

		void draw_layer_quad(const engine::float_vector_2d& offset, const engine::float_vector_2d& scale, const engine::Pixel tint) override {
			locBindBuffer(0x8892, m_vbQuad);
			locVertex verts[4] = {
				{{-1.0f, -1.0f, 1.0}, {0.0f * scale.x + offset.x, 1.0f * scale.y + offset.y}, tint},
				{{+1.0f, -1.0f, 1.0}, {1.0f * scale.x + offset.x, 1.0f * scale.y + offset.y}, tint},
				{{-1.0f, +1.0f, 1.0}, {0.0f * scale.x + offset.x, 0.0f * scale.y + offset.y}, tint},
				{{+1.0f, +1.0f, 1.0}, {1.0f * scale.x + offset.x, 0.0f * scale.y + offset.y}, tint},
			};

			locBufferData(0x8892, sizeof(locVertex) * 4, verts, 0x88E0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		void draw_decal(const engine::DecalInstance& decal) override
		{
			set_decal_mode(decal.mode);
			if (decal.decal == nullptr)
				glBindTexture(GL_TEXTURE_2D, rendBlankQuad.Decal()->id);
			else
				glBindTexture(GL_TEXTURE_2D, decal.decal->id);

			locBindBuffer(0x8892, m_vbQuad);

			for (uint32_t i = 0; i < decal.points; i++)
				pVertexMem[i] = { { decal.pos[i].x, decal.pos[i].y, decal.w[i] }, { decal.uv[i].x, decal.uv[i].y }, decal.tint[i] };

			locBufferData(0x8892, sizeof(locVertex) * decal.points, pVertexMem, 0x88E0);

			if (decal_mode == DecalMode::WIREFRAME)
				glDrawArrays(GL_LINE_LOOP, 0, decal.points);
			else {
				if (decal.structure == engine::DecalStructure::FAN)
					glDrawArrays(GL_TRIANGLE_FAN, 0, decal.points);
				else if (decal.structure == engine::DecalStructure::STRIP)
					glDrawArrays(GL_TRIANGLE_STRIP, 0, decal.points);
				else if (decal.structure == engine::DecalStructure::LIST)
					glDrawArrays(GL_TRIANGLES, 0, decal.points);
			}
		}

		uint32_t create_texture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override {
			UNUSED(width);
			UNUSED(height);
			uint32_t id = 0;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			if (filtered) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}

			if (clamp) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
#if !defined(ENGINE_PLATFORM_EMSCRIPTEN)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif
			return id;
		}

		uint32_t delete_texture(const uint32_t id) override {
			glDeleteTextures(1, &id);
			return id;
		}

		void update_texture(uint32_t id, engine::Sprite* spr) override {
			UNUSED(id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->get_data());
		}

		void read_texture(uint32_t id, engine::Sprite* spr) override {
			glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->get_data());
		}

		void apply_texture(uint32_t id) override {
			glBindTexture(GL_TEXTURE_2D, id);
		}

		void clear_buffer(engine::Pixel p, bool depth) override {
			glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			if (depth) glClear(GL_DEPTH_BUFFER_BIT);
		}

		void update_viewport(const engine::int_vector_2d& pos, const engine::int_vector_2d& size) override {
			glViewport(pos.x, pos.y, size.x, size.y);
		}
	};
}
#endif

#pragma endregion

#pragma region image_gdi

#if defined(ENGINE_IMAGE_GDI)

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)
#include <objidl.h>
#include <gdiplus.h>
#if defined(__MINGW32__)
	#include <gdiplus/gdiplusinit.h>
#else
	#include <gdiplusinit.h>
#endif
#include <shlwapi.h>
#undef min
#undef max

#if !defined(__MINGW32__)
	#pragma comment(lib, "gdiplus.lib")
	#pragma comment(lib, "Shlwapi.lib")
#endif

namespace engine {
	static class GDIPlusStartup {
	public:
		GDIPlusStartup() {	
			Gdiplus::GdiplusStartupInput startupInput;
			GdiplusStartup(&token, &startupInput, NULL);
		}

		ULONG_PTR	token;
		
		~GDIPlusStartup() {
			Gdiplus::GdiplusShutdown(token);
		}
	} gdistartup;

	class ImageLoader_GDIPlus : public engine::ImageLoader {
	private:
		std::wstring ConvertS2W(std::string s) {
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}

	public:
		ImageLoader_GDIPlus() : ImageLoader()
		{}

		engine::Code load_image_resource(engine::Sprite* spr, const std::string& img_file, engine::ResourcePack* pack) override {
			spr->col_data.clear();

			UNUSED(pack);
			Gdiplus::Bitmap* bmp = nullptr;
			if (pack != nullptr) {
				ResourceBuffer rb = pack->get_file_buffer(img_file);
				bmp = Gdiplus::Bitmap::FromStream(SHCreateMemStream((BYTE*)rb.memory.data(), UINT(rb.memory.size())));
			}
			else {
				if (!_gfs::exists(img_file)) return engine::Code::NO_FILE;

				bmp = Gdiplus::Bitmap::FromFile(ConvertS2W(img_file).c_str());
			}

			if (bmp->GetLastStatus() != Gdiplus::Ok) return engine::Code::FAIL;
			spr->width = bmp->GetWidth();
			spr->height = bmp->GetHeight();

			spr->col_data.resize(spr->width * spr->height);

			for (int y = 0; y < spr->height; y++)
				for (int x = 0; x < spr->width; x++)
				{
					Gdiplus::Color c;
					bmp->GetPixel(x, y, &c);
					spr->set_pixel(x, y, engine::Pixel(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha()));
				}
			delete bmp;
			return engine::Code::OK;
		}

		engine::Code save_img_resource(engine::Sprite* spr, const std::string& img_file) override {
			return engine::Code::OK;
		}
	};
}
#endif

#pragma endregion

#pragma region image_libpng

#if defined(ENGINE_IMAGE_LIBPNG)
#include <png.h>
namespace engine {
	void png_read_stream(png_structp png_ptr, png_bytep data, png_size_t length) {
		png_voidp a = png_get_io_ptr(png_ptr);
		((std::istream*)a)->read((char*)data, length);
	}

	class ImageLoader_LibPNG : public engine::ImageLoader {
	public:
		ImageLoader_LibPNG() : ImageLoader() {}

		engine::Code load_img_resource(engine::Sprite* spr, const std::string& img_file, engine::ResourcePack* pack) override {
			UNUSED(pack);

			spr->col_data.clear();

			// ARTICLE: http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/
			png_structp png;
			png_infop info;

			auto loadPNG = [&]() {
				png_read_info(png, info);
				png_byte color_type;
				png_byte bit_depth;
				png_bytep* row_pointers;
				spr->width = png_get_image_width(png, info);
				spr->height = png_get_image_height(png, info);
				color_type = png_get_color_type(png, info);
				bit_depth = png_get_bit_depth(png, info);
				if (bit_depth == 16) png_set_strip_16(png);
				if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
				if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
				if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
				if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
					png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
				if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
					png_set_gray_to_rgb(png);
				png_read_update_info(png, info);
				row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * spr->height);

				for (int y = 0; y < spr->height; y++) {
					row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
				}
				png_read_image(png, row_pointers);

				spr->col_data.resize(spr->width * spr->height);

				for (int y = 0; y < spr->height; y++) {
					png_bytep row = row_pointers[y];
					for (int x = 0; x < spr->width; x++) {
						png_bytep px = &(row[x * 4]);
						spr->set_pixel(x, y, Pixel(px[0], px[1], px[2], px[3]));
					}
				}

				for (int y = 0; y < spr->height; y++)
					free(row_pointers[y]);
				free(row_pointers);

				png_destroy_read_struct(&png, &info, nullptr);
			};

			png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png) goto fail_load;

			info = png_create_info_struct(png);
			if (!info) goto fail_load;

			if (setjmp(png_jmpbuf(png))) goto fail_load;

			if (pack == nullptr) {
				FILE* f = fopen(img_file.c_str(), "rb");
				if (!f) return engine::Code::NO_FILE;
				png_init_io(png, f);
				loadPNG();
				fclose(f);
			}
			else {
				ResourceBuffer rb = pack->get_file_buffer(img_file);
				std::istream is(&rb);
				png_set_read_fn(png, (png_voidp)&is, png_read_stream);
				loadPNG();
			}

			return engine::Code::OK;

		fail_load:
			spr->width = 0;
			spr->height = 0;
			spr->col_data.clear();
			return engine::Code::FAIL;
		}

		engine::Code save_img_resource(engine::Sprite* spr, const std::string& img_file) override {
			return engine::Code::OK;
		}
	};
}
#endif

#pragma endregion

#pragma region platform_windows

#if defined(ENGINE_PLATFORM_WINAPI)

#if defined(_WIN32) && !defined(__MINGW32__)
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "opengl32.lib")
#endif

namespace engine {
	class Platform_Windows : public engine::Platform {
	private:
		HWND engine_hWnd = nullptr;
		std::wstring wsAppName;

		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}

	public:
		virtual engine::Code application_startup() override { return engine::Code::OK; }
		virtual engine::Code application_cleanup() override { return engine::Code::OK; }
		virtual engine::Code thread_startup() override { return engine::Code::OK; }

		virtual engine::Code thread_cleanup() override {
			renderer->destroy_device();
			PostMessage(engine_hWnd, WM_DESTROY, 0, 0);
			return engine::OK;
		}

		virtual engine::Code create_graphics(bool fullscreen, bool enableVSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) override {
			if (renderer->create_device({ engine_hWnd }, fullscreen, enableVSYNC) == engine::Code::OK) {
				renderer->UpdateViewport(vViewPos, vViewSize);
				return engine::Code::OK;
			}
			else {
				return engine::Code::FAIL;
			}
		}

		virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool fullscreen) override {
			WNDCLASS wc;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.lpfnWndProc = engine_WindowEvent;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.lpszMenuName = nullptr;
			wc.hbrBackground = nullptr;
			wc.lpszClassName = engineT("ENGINE");
			RegisterClass(&wc);

			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

			engine::int_vector_2d vTopLeft = window_pos;

			if (fullscreen) {
				dwExStyle = 0;
				dwStyle = WS_VISIBLE | WS_POPUP;
				HMONITOR hmon = MonitorFromWindow(engine_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				if (!GetMonitorInfo(hmon, &mi)) return engine::Code::FAIL;
				window_size = { mi.rcMonitor.right, mi.rcMonitor.bottom };
				vTopLeft.x = 0;
				vTopLeft.y = 0;
			}

			RECT rWndRect = { 0, 0, window_size.x, window_size.y };
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			int width = rWndRect.right - rWndRect.left;
			int height = rWndRect.bottom - rWndRect.top;

			engine_hWnd = CreateWindowEx(dwExStyle, engineT("ENGINE"), engineT(""), dwStyle,
				vTopLeft.x, vTopLeft.y, width, height, NULL, NULL, GetModuleHandle(nullptr), this);

			DragAcceptFiles(engine_hWnd, true);

			keys[0x00] = Key::NONE;
			keys[0x41] = Key::A; keys[0x42] = Key::B; keys[0x43] = Key::C; keys[0x44] = Key::D; keys[0x45] = Key::E;
			keys[0x46] = Key::F; keys[0x47] = Key::G; keys[0x48] = Key::H; keys[0x49] = Key::I; keys[0x4A] = Key::J;
			keys[0x4B] = Key::K; keys[0x4C] = Key::L; keys[0x4D] = Key::M; keys[0x4E] = Key::N; keys[0x4F] = Key::O;
			keys[0x50] = Key::P; keys[0x51] = Key::Q; keys[0x52] = Key::R; keys[0x53] = Key::S; keys[0x54] = Key::T;
			keys[0x55] = Key::U; keys[0x56] = Key::V; keys[0x57] = Key::W; keys[0x58] = Key::X; keys[0x59] = Key::Y;
			keys[0x5A] = Key::Z;

			keys[VK_F1] = Key::F1; keys[VK_F2] = Key::F2; keys[VK_F3] = Key::F3; keys[VK_F4] = Key::F4;
			keys[VK_F5] = Key::F5; keys[VK_F6] = Key::F6; keys[VK_F7] = Key::F7; keys[VK_F8] = Key::F8;
			keys[VK_F9] = Key::F9; keys[VK_F10] = Key::F10; keys[VK_F11] = Key::F11; keys[VK_F12] = Key::F12;

			keys[VK_DOWN] = Key::DOWN; keys[VK_LEFT] = Key::LEFT; keys[VK_RIGHT] = Key::RIGHT; keys[VK_UP] = Key::UP;
			
			keys[VK_BACK] = Key::BACK; keys[VK_ESCAPE] = Key::ESCAPE; keys[VK_RETURN] = Key::ENTER; keys[VK_PAUSE] = Key::PAUSE;
			keys[VK_SCROLL] = Key::SCROLL; keys[VK_TAB] = Key::TAB; keys[VK_DELETE] = Key::DEL; keys[VK_HOME] = Key::HOME;
			keys[VK_END] = Key::END; keys[VK_PRIOR] = Key::PGUP; keys[VK_NEXT] = Key::PGDN; keys[VK_INSERT] = Key::INS;
			keys[VK_SHIFT] = Key::SHIFT; keys[VK_CONTROL] = Key::CTRL;
			keys[VK_SPACE] = Key::SPACE;

			keys[0x30] = Key::K0; keys[0x31] = Key::K1; keys[0x32] = Key::K2; keys[0x33] = Key::K3; keys[0x34] = Key::K4;
			keys[0x35] = Key::K5; keys[0x36] = Key::K6; keys[0x37] = Key::K7; keys[0x38] = Key::K8; keys[0x39] = Key::K9;

			keys[VK_NUMPAD0] = Key::NP0; keys[VK_NUMPAD1] = Key::NP1; keys[VK_NUMPAD2] = Key::NP2; keys[VK_NUMPAD3] = Key::NP3; keys[VK_NUMPAD4] = Key::NP4;
			keys[VK_NUMPAD5] = Key::NP5; keys[VK_NUMPAD6] = Key::NP6; keys[VK_NUMPAD7] = Key::NP7; keys[VK_NUMPAD8] = Key::NP8; keys[VK_NUMPAD9] = Key::NP9;
			keys[VK_MULTIPLY] = Key::NP_MUL; keys[VK_ADD] = Key::NP_ADD; keys[VK_DIVIDE] = Key::NP_DIV; keys[VK_SUBTRACT] = Key::NP_SUB; keys[VK_DECIMAL] = Key::NP_DECIMAL;

			keys[VK_OEM_1] = Key::OEM_1;
			keys[VK_OEM_2] = Key::OEM_2;
			keys[VK_OEM_3] = Key::OEM_3;
			keys[VK_OEM_4] = Key::OEM_4;
			keys[VK_OEM_5] = Key::OEM_5;
			keys[VK_OEM_6] = Key::OEM_6;
			keys[VK_OEM_7] = Key::OEM_7;
			keys[VK_OEM_8] = Key::OEM_8;
			keys[VK_OEM_PLUS] = Key::EQUALS;
			keys[VK_OEM_COMMA] = Key::COMMA;
			keys[VK_OEM_MINUS] = Key::MINUS;
			keys[VK_OEM_PERIOD] = Key::PERIOD;
			keys[VK_CAPITAL] = Key::CAPS_LOCK;
			return engine::OK;
		}

		virtual engine::Code set_window_title(const std::string& s) override {
#ifdef UNICODE
			SetWindowText(engine_hWnd, ConvertS2W(s).c_str());
#else
			SetWindowText(engine_hWnd, s.c_str());
#endif
			return engine::OK;
		}

		virtual engine::Code start_system_event_loop() override {
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return engine::OK;
		}

		virtual engine::Code handle_system_event() override { return engine::Code::FAIL; }

		static LRESULT CALLBACK engine_window_event(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
			case WM_MOUSEMOVE: {
				uint16_t x = lParam & 0xFFFF; uint16_t y = (lParam >> 16) & 0xFFFF;
				int16_t ix = *(int16_t*)&x;   int16_t iy = *(int16_t*)&y;
				ptr_engine->engine_update_mouse(ix, iy);
				return 0;
			}
			case WM_SIZE:       ptr_engine->engine_update_window_size(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);	return 0;
			case WM_MOUSEWHEEL:	ptr_engine->engine_update_mouse_wheel(GET_WHEEL_DELTA_WPARAM(wParam));           return 0;
			case WM_MOUSELEAVE: ptr_engine->engine_update_mouse_focus(false);                                    return 0;
			case WM_SETFOCUS:	ptr_engine->engine_update_key_focus(true);                                       return 0;
			case WM_KILLFOCUS:	ptr_engine->engine_update_key_focus(false);                                      return 0;
			case WM_KEYDOWN:	ptr_engine->engine_update_key_state(keys[wParam], true);                      return 0;
			case WM_KEYUP:		ptr_engine->engine_update_key_state(keys[wParam], false);                     return 0;
			case WM_SYSKEYDOWN: ptr_engine->engine_update_key_state(keys[wParam], true);						return 0;
			case WM_SYSKEYUP:	ptr_engine->engine_update_key_state(keys[wParam], false);						return 0;
			case WM_LBUTTONDOWN:ptr_engine->engine_update_mouse_state(0, true);                                  return 0;
			case WM_LBUTTONUP:	ptr_engine->engine_update_mouse_state(0, false);                                 return 0;
			case WM_RBUTTONDOWN:ptr_engine->engine_update_mouse_state(1, true);                                  return 0;
			case WM_RBUTTONUP:	ptr_engine->engine_update_mouse_state(1, false);                                 return 0;
			case WM_MBUTTONDOWN:ptr_engine->engine_update_mouse_state(2, true);                                  return 0;
			case WM_MBUTTONUP:	ptr_engine->engine_update_mouse_state(2, false);                                 return 0;
			case WM_DROPFILES:
			{
				HDROP drop = (HDROP)wParam;
				
				uint32_t nFiles = DragQueryFile(drop, 0xFFFFFFFF, nullptr, 0);
				std::vector<std::string> vFiles;
				for (uint32_t i = 0; i < nFiles; i++) {
					TCHAR dfbuffer[256]{};
					uint32_t len = DragQueryFile(drop, i, nullptr, 0);
					DragQueryFile(drop, i, dfbuffer, 256);
#ifdef UNICODE
	#ifdef __MINGW32__
					char* buffer = new char[len + 1];
					wcstombs(buffer, dfbuffer, len);
					buffer[len] = '\0';
	#else
					int count = WideCharToMultiByte(CP_UTF8, 0, dfbuffer, -1, NULL, 0, NULL, NULL);
					char* buffer = new char[count];
					WideCharToMultiByte(CP_UTF8, 0, dfbuffer, -1, buffer, count, NULL, NULL);
	#endif				
					vFiles.push_back(std::string(buffer));
					delete[] buffer;
#else
					vFiles.push_back(std::string(dfbuffer));
#endif
				}

				POINT p; DragQueryPoint(drop, &p);
				ptr_engine->engine_drop_files(p.x, p.y, vFiles);
				DragFinish(drop);
				return 0;
			} 
			break;
			case WM_CLOSE:		ptr_engine->engine_terminate();                                                return 0;
			case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd);								return 0;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	};
}
#endif
#pragma endregion 

#pragma region platform_linux

#if defined(ENGINE_PLATFORM_X11)
namespace engine {
	class Platform_Linux : public engine::Platform {
	private:
		X11::Display* engine_Display = nullptr;
		X11::Window					 engine_WindowRoot;
		X11::Window					 engine_Window;
		X11::XVisualInfo* engine_VisualInfo;
		X11::Colormap                engine_ColourMap;
		X11::XSetWindowAttributes    engine_SetWindowAttribs;

	public:
		virtual engine::Code application_startup() override {
			return engine::Code::OK;
		}

		virtual engine::Code application_cleanup() override {
			XDestroyWindow(engine_Display, engine_Window);
			return engine::Code::OK;
		}

		virtual engine::Code thread_startup() override {
			return engine::Code::OK;
		}

		virtual engine::Code thread_cleanup() override {
			renderer->destroy_device();
			return engine::OK;
		}

		virtual engine::Code create_graphics(bool fullscreen, bool enableVSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) override {
			if (renderer->create_device({ engine_Display, &engine_Window, engine_VisualInfo }, fullscreen, enableVSYNC) == engine::Code::OK) {
				renderer->update_viewport(view_pos, view_size);
				return engine::Code::OK;
			}
			else {
				return engine::Code::FAIL;
            }
		}

		virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool fullscreen) override {
			using namespace X11;
			XInitThreads();

			engine_Display = XOpenDisplay(NULL);
			engine_WindowRoot = DefaultRootWindow(engine_Display);

			GLint engine_GLAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			engine_VisualInfo = glXChooseVisual(engine_Display, 0, engine_GLAttribs);
			engine_ColourMap = XCreateColormap(engine_Display, engine_WindowRoot, engine_VisualInfo->visual, AllocNone);
			engine_SetWindowAttribs.colormap = engine_ColourMap;

			engine_SetWindowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | StructureNotifyMask;

			engine_Window = XCreateWindow(engine_Display, engine_WindowRoot, window_pos.x, window_pos.y,
				window_size.x, window_size.y,
				0, engine_VisualInfo->depth, InputOutput, engine_VisualInfo->visual,
				CWColormap | CWEventMask, &engine_SetWindowAttribs);

			Atom wmDelete = XInternAtom(engine_Display, "WM_DELETE_WINDOW", true);
			XSetWMProtocols(engine_Display, engine_Window, &wmDelete, 1);

			XMapWindow(engine_Display, engine_Window);
			XStoreName(engine_Display, engine_Window, "isakhorvath.me - Engine");

			if (fullscreen) {
				Atom wm_state;
				Atom a_fullscreen;
				wm_state = XInternAtom(engine_Display, "_NET_WM_STATE", False);
				a_fullscreen = XInternAtom(engine_Display, "_NET_WM_STATE_FULLSCREEN", False);
				XEvent xev{ 0 };
				xev.type = ClientMessage;
				xev.xclient.window = engine_Window;
				xev.xclient.message_type = wm_state;
				xev.xclient.format = 32;
				xev.xclient.data.l[0] = (fullscreen ? 1 : 0);   
				xev.xclient.data.l[1] = a_fullscreen;             
				xev.xclient.data.l[2] = 0;                      
				xev.xclient.data.l[3] = 0;                      
				XMapWindow(engine_Display, engine_Window);
				XSendEvent(engine_Display, DefaultRootWindow(engine_Display), False,
					SubstructureRedirectMask | SubstructureNotifyMask, &xev);
				XFlush(engine_Display);
				XWindowAttributes gwa;
				XGetWindowAttributes(engine_Display, engine_Window, &gwa);
				window_size.x = gwa.width;
				window_size.y = gwa.height;
			}

			keys[0x00] = Key::NONE;
			keys[0x61] = Key::A; keys[0x62] = Key::B; keys[0x63] = Key::C; keys[0x64] = Key::D; keys[0x65] = Key::E;
			keys[0x66] = Key::F; keys[0x67] = Key::G; keys[0x68] = Key::H; keys[0x69] = Key::I; keys[0x6A] = Key::J;
			keys[0x6B] = Key::K; keys[0x6C] = Key::L; keys[0x6D] = Key::M; keys[0x6E] = Key::N; keys[0x6F] = Key::O;
			keys[0x70] = Key::P; keys[0x71] = Key::Q; keys[0x72] = Key::R; keys[0x73] = Key::S; keys[0x74] = Key::T;
			keys[0x75] = Key::U; keys[0x76] = Key::V; keys[0x77] = Key::W; keys[0x78] = Key::X; keys[0x79] = Key::Y;
			keys[0x7A] = Key::Z;

			keys[XK_F1] = Key::F1; keys[XK_F2] = Key::F2; keys[XK_F3] = Key::F3; keys[XK_F4] = Key::F4;
			keys[XK_F5] = Key::F5; keys[XK_F6] = Key::F6; keys[XK_F7] = Key::F7; keys[XK_F8] = Key::F8;
			keys[XK_F9] = Key::F9; keys[XK_F10] = Key::F10; keys[XK_F11] = Key::F11; keys[XK_F12] = Key::F12;

			keys[XK_Down] = Key::DOWN; keys[XK_Left] = Key::LEFT; keys[XK_Right] = Key::RIGHT; keys[XK_Up] = Key::UP;
			keys[XK_KP_Enter] = Key::ENTER; keys[XK_Return] = Key::ENTER;

			keys[XK_BackSpace] = Key::BACK; keys[XK_Escape] = Key::ESCAPE; keys[XK_Linefeed] = Key::ENTER;	keys[XK_Pause] = Key::PAUSE;
			keys[XK_Scroll_Lock] = Key::SCROLL; keys[XK_Tab] = Key::TAB; keys[XK_Delete] = Key::DEL; keys[XK_Home] = Key::HOME;
			keys[XK_End] = Key::END; keys[XK_Page_Up] = Key::PGUP; keys[XK_Page_Down] = Key::PGDN;	keys[XK_Insert] = Key::INS;
			keys[XK_Shift_L] = Key::SHIFT; keys[XK_Shift_R] = Key::SHIFT; keys[XK_Control_L] = Key::CTRL; keys[XK_Control_R] = Key::CTRL;
			keys[XK_space] = Key::SPACE; keys[XK_period] = Key::PERIOD;

			keys[XK_0] = Key::K0; keys[XK_1] = Key::K1; keys[XK_2] = Key::K2; keys[XK_3] = Key::K3; keys[XK_4] = Key::K4;
			keys[XK_5] = Key::K5; keys[XK_6] = Key::K6; keys[XK_7] = Key::K7; keys[XK_8] = Key::K8; keys[XK_9] = Key::K9;

			keys[XK_KP_0] = Key::NP0; keys[XK_KP_1] = Key::NP1; keys[XK_KP_2] = Key::NP2; keys[XK_KP_3] = Key::NP3; keys[XK_KP_4] = Key::NP4;
			keys[XK_KP_5] = Key::NP5; keys[XK_KP_6] = Key::NP6; keys[XK_KP_7] = Key::NP7; keys[XK_KP_8] = Key::NP8; keys[XK_KP_9] = Key::NP9;
			keys[XK_KP_Multiply] = Key::NP_MUL; keys[XK_KP_Add] = Key::NP_ADD; keys[XK_KP_Divide] = Key::NP_DIV; keys[XK_KP_Subtract] = Key::NP_SUB; keys[XK_KP_Decimal] = Key::NP_DECIMAL;

			keys[XK_semicolon] = Key::OEM_1;
			keys[XK_slash] = Key::OEM_2;
			keys[XK_asciitilde] = Key::OEM_3;
			keys[XK_bracketleft] = Key::OEM_4;
			keys[XK_backslash] = Key::OEM_5;
			keys[XK_bracketright] = Key::OEM_6;
			keys[XK_apostrophe] = Key::OEM_7;
			keys[XK_numbersign] = Key::OEM_8;
			keys[XK_equal] = Key::EQUALS;
			keys[XK_comma] = Key::COMMA;
			keys[XK_minus] = Key::MINUS;		

			keys[XK_Caps_Lock] = Key::CAPS_LOCK;

			return engine::OK;
		}

		virtual engine::Code set_window_title(const std::string& s) override {
			X11::XStoreName(engine_Display, engine_Window, s.c_str());
			return engine::OK;
		}

		virtual engine::Code start_system_event_loop() override {
			return engine::OK;
		}

		virtual engine::Code handle_system_event() override {
			using namespace X11;
			XEvent xev;
			while (XPending(engine_Display)) {
				XNextEvent(engine_Display, &xev);
				if (xev.type == Expose) {
					XWindowAttributes gwa;
					XGetWindowAttributes(engine_Display, engine_Window, &gwa);
					ptr_engine->engine_update_window_size(gwa.width, gwa.height);
				}
				else if (xev.type == ConfigureNotify) {
					XConfigureEvent xce = xev.xconfigure;
					ptr_engine->engine_update_window_size(xce.width, xce.height);
				}
				else if (xev.type == KeyPress) {
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					ptr_engine->engine_update_key_state(keys[sym], true);
					XKeyEvent* e = (XKeyEvent*)&xev;
					XLookupString(e, NULL, 0, &sym, NULL);
					ptr_engine->engine_update_key_state(keys[sym], true);
				}
				else if (xev.type == KeyRelease) {
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					ptr_engine->engine_update_key_state(keys[sym], false);
					XKeyEvent* e = (XKeyEvent*)&xev;
					XLookupString(e, NULL, 0, &sym, NULL);
					ptr_engine->engine_update_key_state(keys[sym], false);
				}
				else if (xev.type == ButtonPress) {
					switch (xev.xbutton.button) {
                        case 1:	ptr_engine->engine_update_mouse_state(0, true); break;
                        case 2:	ptr_engine->engine_update_mouse_state(2, true); break;
                        case 3:	ptr_engine->engine_update_mouse_state(1, true); break;
                        case 4:	ptr_engine->engine_update_mouse_wheel(120); break;
                        case 5:	ptr_engine->engine_update_mouse_wheel(-120); break;
                        default: break;
					}
				}
				else if (xev.type == ButtonRelease) {
					switch (xev.xbutton.button) {
                        case 1:	ptr_engine->engine_update_mouse_state(0, false); break;
                        case 2:	ptr_engine->engine_update_mouse_state(2, false); break;
                        case 3:	ptr_engine->engine_update_mouse_state(1, false); break;
                        default: break;
					}
				}
				else if (xev.type == MotionNotify) {
					ptr_engine->engine_update_mouse(xev.xmotion.x, xev.xmotion.y);
				}
				else if (xev.type == FocusIn) {
					ptr_engine->engine_update_key_focus(true);
				}
				else if (xev.type == FocusOut) {
					ptr_engine->engine_update_key_focus(false);
				}
				else if (xev.type == ClientMessage) {
					ptr_engine->engine_terminate();
				}
			}
			return engine::OK;
		}
	};
}
#endif
#pragma endregion

#pragma region platform_glut

#if defined(ENGINE_PLATFORM_GLUT)
namespace engine {

	class Platform_GLUT : public engine::Platform {
	public:
		static std::atomic<bool>* active_ref;

		virtual engine::Code application_startup() override {
			return engine::Code::OK;
		}

		virtual engine::Code application_cleanup() override {
			return engine::Code::OK;
		}

		virtual engine::Code thread_startup() override {
			return engine::Code::OK;
		}

		virtual engine::Code thread_cleanup() override {
			renderer->destroy_device();
			return engine::OK;
		}

		virtual engine::Code create_graphics(bool fullscreen, bool enableVSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) override {
			if (renderer->create_device({}, fullscreen, enableVSYNC) == engine::Code::OK) {
				renderer->update_viewport(view_pos, view_size);
				return engine::Code::OK;
			}
			else {
				return engine::Code::FAIL;
            }
		}

		static void ExitMainLoop() {
			if (!ptr_engine->on_destroy()) {
				*active_ref = true;
				return;
			}
			platform->thread_cleanup();
			platform->application_cleanup();
			exit(0);
		}

#if defined(__APPLE__)
		static void scrollWheelUpdate(id selff, SEL _sel, id theEvent) {
			static const SEL deltaYSel = sel_registerName("deltaY");

#if defined(__aarch64__)
			double deltaY = ((double (*)(id, SEL))objc_msgSend)(theEvent, deltaYSel);
#else
			double deltaY = ((double (*)(id, SEL))objc_msgSend_fpret)(theEvent, deltaYSel);
#endif

			for (int i = 0; i < abs(deltaY); i++) {
				if (deltaY > 0) {
					ptr_engine->engine_update_mouse_wheel(-1);
				}
				else if (deltaY < 0) {
					ptr_engine->engine_update_mouse_wheel(1);
				}
			}
		}
#endif
		static void thread_funct() {
#if defined(__APPLE__)
			static bool hasEnabledCocoa = false;
			if (!hasEnabledCocoa) {
				Class NSApplicationClass = objc_getClass("NSApplication");

				SEL sharedApplicationSel = sel_registerName("sharedApplication");
				id NSApp = ((id(*)(Class, SEL))objc_msgSend)(NSApplicationClass, sharedApplicationSel);
				SEL mainWindowSel = sel_registerName("mainWindow");
				id window = ((id(*)(id, SEL))objc_msgSend)(NSApp, mainWindowSel);

				SEL setStyleMaskSel = sel_registerName("setStyleMask:");
				((void (*)(id, SEL, NSUInteger))objc_msgSend)(window, setStyleMaskSel, 7);

				hasEnabledCocoa = true;
			}
#endif
			if (!*active_ref) {
				ExitMainLoop();
				return;
			}
			glutPostRedisplay();
		}

		static void draw_funct() {
			ptr_engine->engine_core_update();
		}

		virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool full_screen) override
		{
#if defined(__APPLE__)
			Class GLUTViewClass = objc_getClass("GLUTView");

			SEL scrollWheelSel = sel_registerName("scrollWheel:");
			bool resultAddMethod = class_addMethod(GLUTViewClass, scrollWheelSel, (IMP)scrollWheelUpdate, "v@:@");
			assert(resultAddMethod);
#endif
			renderer->prepare_device();

			if (fullscreen) {
				window_size.x = glutGet(GLUT_SCREEN_WIDTH);
				window_size.y = glutGet(GLUT_SCREEN_HEIGHT);
				glutFullScreen();
			}
			else {
				if (window_size.x > glutGet(GLUT_SCREEN_WIDTH) || window_size.y > glutGet(GLUT_SCREEN_HEIGHT)) {
					perror("ERROR: The specified window dimensions do not fit on your screen\n");
					return engine::FAIL;
				}
				glutReshapeWindow(window_size.x, window_size.y - 1);
			}

			keys[0x00] = Key::NONE;
			keys['A'] = Key::A; keys['B'] = Key::B; keys['C'] = Key::C; keys['D'] = Key::D; keys['E'] = Key::E;
			keys['F'] = Key::F; keys['G'] = Key::G; keys['H'] = Key::H; keys['I'] = Key::I; keys['J'] = Key::J;
			keys['K'] = Key::K; keys['L'] = Key::L; keys['M'] = Key::M; keys['N'] = Key::N; keys['O'] = Key::O;
			keys['P'] = Key::P; keys['Q'] = Key::Q; keys['R'] = Key::R; keys['S'] = Key::S; keys['T'] = Key::T;
			keys['U'] = Key::U; keys['V'] = Key::V; keys['W'] = Key::W; keys['X'] = Key::X; keys['Y'] = Key::Y;
			keys['Z'] = Key::Z;

			keys[GLUT_KEY_F1] = Key::F1; keys[GLUT_KEY_F2] = Key::F2; keys[GLUT_KEY_F3] = Key::F3; keys[GLUT_KEY_F4] = Key::F4;
			keys[GLUT_KEY_F5] = Key::F5; keys[GLUT_KEY_F6] = Key::F6; keys[GLUT_KEY_F7] = Key::F7; keys[GLUT_KEY_F8] = Key::F8;
			keys[GLUT_KEY_F9] = Key::F9; keys[GLUT_KEY_F10] = Key::F10; keys[GLUT_KEY_F11] = Key::F11; keys[GLUT_KEY_F12] = Key::F12;

			keys[GLUT_KEY_DOWN] = Key::DOWN; keys[GLUT_KEY_LEFT] = Key::LEFT; keys[GLUT_KEY_RIGHT] = Key::RIGHT; keys[GLUT_KEY_UP] = Key::UP;
			keys[13] = Key::ENTER;

			keys[127] = Key::BACK; keys[27] = Key::ESCAPE;
			keys[9] = Key::TAB;  keys[GLUT_KEY_HOME] = Key::HOME;
			keys[GLUT_KEY_END] = Key::END; keys[GLUT_KEY_PAGE_UP] = Key::PGUP; keys[GLUT_KEY_PAGE_DOWN] = Key::PGDN;    keys[GLUT_KEY_INSERT] = Key::INS;
			keys[32] = Key::SPACE; keys[46] = Key::PERIOD;

			keys[48] = Key::K0; keys[49] = Key::K1; keys[50] = Key::K2; keys[51] = Key::K3; keys[52] = Key::K4;
			keys[53] = Key::K5; keys[54] = Key::K6; keys[55] = Key::K7; keys[56] = Key::K8; keys[57] = Key::K9;

			glutKeyboardFunc([](unsigned char key, int x, int y) -> void {
				switch (glutGetModifiers()) {
				case 0:
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				case GLUT_ACTIVE_SHIFT:
					ptr_engine->engine_update_key_state(Key::SHIFT, true);
					break;
				case GLUT_ACTIVE_CTRL:
					if ('a' <= key && key <= 'z') key -= 32;
					ptr_engine->engine_update_key_state(Key::CTRL, true);
					break;
				case GLUT_ACTIVE_ALT:
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				}

				if (keys[key])
					ptr_engine->engine_update_key_state(keys[key], true);
				});

			glutKeyboardUpFunc([](unsigned char key, int x, int y) -> void {
				switch (glutGetModifiers()) {
				case 0:
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				case GLUT_ACTIVE_SHIFT:
					ptr_engine->engine_update_key_state(Key::SHIFT, false);
					break;
				case GLUT_ACTIVE_CTRL:
					if ('a' <= key && key <= 'z') key -= 32;
					ptr_engine->engine_update_key_state(Key::CTRL, false);
					break;
				case GLUT_ACTIVE_ALT:
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				}

				if (keys[key])
					ptr_engine->engine_update_key_state(keys[key], false);
				});

			glutSpecialFunc([](int key, int x, int y) -> void {
				if (keys[key])
					ptr_engine->engine_update_key_state(keys[key], true);
				});

			glutSpecialUpFunc([](int key, int x, int y) -> void {
				if (keys[key])
					ptr_engine->engine_update_key_state(keys[key], false);
				});

			glutMouseFunc([](int button, int state, int x, int y) -> void {
				switch (button) {
				case GLUT_LEFT_BUTTON:
					if (state == GLUT_UP) ptr_engine->engine_update_mouse_state(0, false);
					else if (state == GLUT_DOWN) ptr_engine->engine_update_mouse_state(0, true);
					break;
				case GLUT_MIDDLE_BUTTON:
					if (state == GLUT_UP) ptr_engine->engine_update_mouse_state(2, false);
					else if (state == GLUT_DOWN) ptr_engine->engine_update_mouse_state(2, true);
					break;
				case GLUT_RIGHT_BUTTON:
					if (state == GLUT_UP) ptr_engine->engine_update_mouse_state(1, false);
					else if (state == GLUT_DOWN) ptr_engine->engine_update_mouse_state(1, true);
					break;
				}
				});

			auto mouseMoveCall = [](int x, int y) -> void {
				ptr_engine->engine_update_mouse(x, y);
			};

			glutMotionFunc(mouseMoveCall);
			glutPassiveMotionFunc(mouseMoveCall);

			glutEntryFunc([](int state) -> void {
				if (state == GLUT_ENTERED) ptr_engine->engine_update_key_focus(true);
				else if (state == GLUT_LEFT) ptr_engine->engine_update_key_focus(false);
				});

			glutDisplayFunc(DrawFunct);
			glutIdleFunc(ThreadFunct);

			return engine::OK;
		}

		virtual engine::Code set_window_title(const std::string& s) override
		{
			glutSetWindowTitle(s.c_str());
			return engine::OK;
		}

		virtual engine::Code start_system_event_loop() override {
			glutMainLoop();
			return engine::OK;
		}

		virtual engine::Code handle_system_event() override
		{
			return engine::OK;
		}
	};

	std::atomic<bool>* Platform_GLUT::active_ref{ nullptr };

	engine::Code Engine::start()
	{
		if (platform->application_startup() != engine::OK) return engine::FAIL;

		if (platform->create_window_pane({ 30,30 }, window_size, fullscreen) != engine::OK) return engine::FAIL;
		engine_update_window_size(window_size.x, window_size.y);

		if (platform->thread_startup() == engine::FAIL)  return engine::FAIL;
		engine_prepare();
		if (!on_create()) return engine::FAIL;
		Platform_GLUT::active_ref = &atom_active;
		glutWMCloseFunc(Platform_GLUT::ExitMainLoop);
		atom_active = true;
		platform->start_system_event_loop();

		if (platform->application_cleanup() != engine::OK) return engine::FAIL;

		return engine::OK;
	}
}

#endif

#pragma endregion 

#pragma region platform_emscripten

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)

#include <emscripten/html5.h>
#include <emscripten/key_codes.h>

extern "C" {
	EMSCRIPTEN_KEEPALIVE inline int engine_on_page_unload() { 
        engine::platform->application_cleanup(); 
        return 0; 
    }
}

namespace engine
{
	class Platform_Emscripten : public engine::Platform {
	public:

		virtual engine::Code application_startup() override { return engine::Code::OK; }

		virtual engine::Code application_cleanup() override { 
            thread_cleanup(); 
            return engine::Code::OK; 
        }

		virtual engine::Code thread_startup() override { return engine::Code::OK; }

		virtual engine::Code thread_cleanup() override { 
            renderer->destroy_device(); 
            return engine::OK; 
        }

		virtual engine::Code create_graphics(bool fullscreen, bool enableVSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) override {
			if (renderer->create_device({}, fullscreen, enableVSYNC) == engine::Code::OK) {
				renderer->update_viewport(view_pos, view_size);
				return engine::Code::OK;
			}
			else {
				return engine::Code::FAIL;
            }
		}

		virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool fullscreen) override {
			emscripten_set_canvas_element_size("#canvas", window_size.x, window_size.y);

			keys[DOM_PK_UNKNOWN] = Key::NONE;
			keys[DOM_PK_A] = Key::A; keys[DOM_PK_B] = Key::B; keys[DOM_PK_C] = Key::C; keys[DOM_PK_D] = Key::D;
			keys[DOM_PK_E] = Key::E; keys[DOM_PK_F] = Key::F; keys[DOM_PK_G] = Key::G; keys[DOM_PK_H] = Key::H;
			keys[DOM_PK_I] = Key::I; keys[DOM_PK_J] = Key::J; keys[DOM_PK_K] = Key::K; keys[DOM_PK_L] = Key::L;
			keys[DOM_PK_M] = Key::M; keys[DOM_PK_N] = Key::N; keys[DOM_PK_O] = Key::O; keys[DOM_PK_P] = Key::P;
			keys[DOM_PK_Q] = Key::Q; keys[DOM_PK_R] = Key::R; keys[DOM_PK_S] = Key::S; keys[DOM_PK_T] = Key::T;
			keys[DOM_PK_U] = Key::U; keys[DOM_PK_V] = Key::V; keys[DOM_PK_W] = Key::W; keys[DOM_PK_X] = Key::X;
			keys[DOM_PK_Y] = Key::Y; keys[DOM_PK_Z] = Key::Z;
			keys[DOM_PK_0] = Key::K0; keys[DOM_PK_1] = Key::K1; keys[DOM_PK_2] = Key::K2;
			keys[DOM_PK_3] = Key::K3; keys[DOM_PK_4] = Key::K4; keys[DOM_PK_5] = Key::K5;
			keys[DOM_PK_6] = Key::K6; keys[DOM_PK_7] = Key::K7; keys[DOM_PK_8] = Key::K8;
			keys[DOM_PK_9] = Key::K9;
			keys[DOM_PK_F1] = Key::F1; keys[DOM_PK_F2] = Key::F2; keys[DOM_PK_F3] = Key::F3; keys[DOM_PK_F4] = Key::F4;
			keys[DOM_PK_F5] = Key::F5; keys[DOM_PK_F6] = Key::F6; keys[DOM_PK_F7] = Key::F7; keys[DOM_PK_F8] = Key::F8;
			keys[DOM_PK_F9] = Key::F9; keys[DOM_PK_F10] = Key::F10; keys[DOM_PK_F11] = Key::F11; keys[DOM_PK_F12] = Key::F12;
			keys[DOM_PK_ARROW_UP] = Key::UP; keys[DOM_PK_ARROW_DOWN] = Key::DOWN;
			keys[DOM_PK_ARROW_LEFT] = Key::LEFT; keys[DOM_PK_ARROW_RIGHT] = Key::RIGHT;
			keys[DOM_PK_SPACE] = Key::SPACE; keys[DOM_PK_TAB] = Key::TAB;
			keys[DOM_PK_SHIFT_LEFT] = Key::SHIFT; keys[DOM_PK_SHIFT_RIGHT] = Key::SHIFT;
			keys[DOM_PK_CONTROL_LEFT] = Key::CTRL; keys[DOM_PK_CONTROL_RIGHT] = Key::CTRL;
			keys[DOM_PK_INSERT] = Key::INS; keys[DOM_PK_DELETE] = Key::DEL; keys[DOM_PK_HOME] = Key::HOME;
			keys[DOM_PK_END] = Key::END; keys[DOM_PK_PAGE_UP] = Key::PGUP; keys[DOM_PK_PAGE_DOWN] = Key::PGDN;
			keys[DOM_PK_BACKSPACE] = Key::BACK; keys[DOM_PK_ESCAPE] = Key::ESCAPE;
			keys[DOM_PK_ENTER] = Key::ENTER; keys[DOM_PK_NUMPAD_EQUAL] = Key::EQUALS;
			keys[DOM_PK_NUMPAD_ENTER] = Key::ENTER; keys[DOM_PK_PAUSE] = Key::PAUSE;
			keys[DOM_PK_SCROLL_LOCK] = Key::SCROLL;
			keys[DOM_PK_NUMPAD_0] = Key::NP0; keys[DOM_PK_NUMPAD_1] = Key::NP1; keys[DOM_PK_NUMPAD_2] = Key::NP2;
			keys[DOM_PK_NUMPAD_3] = Key::NP3; keys[DOM_PK_NUMPAD_4] = Key::NP4; keys[DOM_PK_NUMPAD_5] = Key::NP5;
			keys[DOM_PK_NUMPAD_6] = Key::NP6; keys[DOM_PK_NUMPAD_7] = Key::NP7; keys[DOM_PK_NUMPAD_8] = Key::NP8;
			keys[DOM_PK_NUMPAD_9] = Key::NP9;
			keys[DOM_PK_NUMPAD_MULTIPLY] = Key::NP_MUL; keys[DOM_PK_NUMPAD_DIVIDE] = Key::NP_DIV;
			keys[DOM_PK_NUMPAD_ADD] = Key::NP_ADD; keys[DOM_PK_NUMPAD_SUBTRACT] = Key::NP_SUB;
			keys[DOM_PK_NUMPAD_DECIMAL] = Key::NP_DECIMAL;
			keys[DOM_PK_PERIOD] = Key::PERIOD; keys[DOM_PK_EQUAL] = Key::EQUALS;
			keys[DOM_PK_COMMA] = Key::COMMA; keys[DOM_PK_MINUS] = Key::MINUS;
			keys[DOM_PK_CAPS_LOCK] = Key::CAPS_LOCK;
			keys[DOM_PK_SEMICOLON] = Key::OEM_1;	keys[DOM_PK_SLASH] = Key::OEM_2; keys[DOM_PK_BACKQUOTE] = Key::OEM_3;
			keys[DOM_PK_BRACKET_LEFT] = Key::OEM_4; keys[DOM_PK_BACKSLASH] = Key::OEM_5; keys[DOM_PK_BRACKET_RIGHT] = Key::OEM_6;
			keys[DOM_PK_QUOTE] = Key::OEM_7; keys[DOM_PK_BACKSLASH] = Key::OEM_8;

			emscripten_set_keydown_callback("#canvas", 0, 1, keyboard_callback);
			emscripten_set_keyup_callback("#canvas", 0, 1, keyboard_callback);

			emscripten_set_wheel_callback("#canvas", 0, 1, wheel_callback);
			emscripten_set_mousedown_callback("#canvas", 0, 1, mouse_callback);
			emscripten_set_mouseup_callback("#canvas", 0, 1, mouse_callback);
			emscripten_set_mousemove_callback("#canvas", 0, 1, mouse_callback);

			emscripten_set_touchstart_callback("#canvas", 0, 1, touch_callback);
			emscripten_set_touchmove_callback("#canvas", 0, 1, touch_callback);
			emscripten_set_touchend_callback("#canvas", 0, 1, touch_callback);

			emscripten_set_blur_callback("#canvas", 0, 1, focus_callback);
			emscripten_set_focus_callback("#canvas", 0, 1, focus_callback);
			
#pragma warning disable format
			EM_ASM( window.onunload = Module._engine_on_page_unload; );

			EM_ASM({

			Module.engine_AspectRatio = $0 / $1;

			Module.engine_AssumeDefaultShells = (document.querySelectorAll('.emscripten').length >= 3) ? true : false;

			var engine_ResizeHandler = function() {
				let isFullscreen = (document.fullscreenElement != null);

				let width  = (isFullscreen || !Module.engine_AssumeDefaultShells) ? window.innerWidth  : Module.canvas.parentNode.clientWidth;
				let height = (isFullscreen || !Module.engine_AssumeDefaultShells) ? window.innerHeight : Module.canvas.parentNode.clientHeight;

				let viewWidth  = width;
				let viewHeight = width / Module.engine_AspectRatio;

				if(viewHeight > height) {
					viewWidth  = height * Module.engine_AspectRatio;
					viewHeight = height;
				}
    
				viewWidth  = parseInt(viewWidth);
				viewHeight = parseInt(viewHeight);

				setTimeout(function() {
					if(Module.engine_AssumeDefaultShells)
						Module.canvas.parentNode.setAttribute('style', 'width: 100%; height: 70vh; margin-left: auto; margin-right: auto;');
				
					Module.canvas.setAttribute('width', viewWidth);
					Module.canvas.setAttribute('height', viewHeight);
					Module.canvas.setAttribute('style', `width: ${viewWidth}px; height: ${viewHeight}px;`);
					
					Module._engine_update_window_size(viewWidth, viewHeight);
					
					Module.canvas.focus();
				}, 200);
			};
			
			var engine_Init = function() {
				if(Module.engine_AspectRatio === undefined) {
					setTimeout(function() { Module.engine_Init(); }, 50);
					return;
				}
					
				let resizeObserver = new ResizeObserver(function(entries) {
					Module.engine_ResizeHandler();
				}).observe(Module.canvas.parentNode);

				let mutationObserver = new MutationObserver(function(mutationsList, observer) {
					setTimeout(function() { Module.engine_ResizeHandler(); },  200);
				}).observe(Module.canvas.parentNode, { attributes: false, childList: true, subtree: false });

				window.addEventListener('fullscreenchange', function(e)
				{
					setTimeout(function() { Module.engine_ResizeHandler();},  200);
				});
			};
			
			Module.engine_ResizeHandler = (Module.engine_ResizeHandler != undefined) ? Module.engine_ResizeHandler : engine_ResizeHandler;
			Module.engine_Init          = (Module.engine_Init          != undefined) ? Module.engine_Init : engine_Init;

			Module.engine_Init();

			}, window_size.x, window_size.y);
#pragma warning restore format
			return engine::Code::OK;
		}

		void update_window_size(int width, int height) {
			ptr_engine->engine_update_window_size(width, height);
		}

		static EM_BOOL focus_callback(int event_type, const EmscriptenFocusEvent* focus_event, void* user_data) 
		{
			if (event_type == EMSCRIPTEN_EVENT_BLUR) {
				ptr_engine->eninge_update_key_focus(false);
				ptr_engine->engine_update_mouse_focus(false);
			}
			else if (event_type == EMSCRIPTEN_EVENT_FOCUS) {
				ptr_engine->eninge_update_key_focus(true);
				ptr_engine->engine_update_mouse_focus(true);
			}

			return 0;
		}

		static EM_BOOL keyboard_callback(int event_type, const EmscriptenKeyboardEvent* e, void* user_data)
		{
			if (event_type == EMSCRIPTEN_EVENT_KEYDOWN)
				ptr_engine->engine_update_key_state(keys[emscripten_compute_dom_pk_code(e->code)], true);

			if (event_type == EMSCRIPTEN_EVENT_KEYUP)
				ptr_engine->engine_update_key_state(keys[emscripten_compute_dom_pk_code(e->code)], false);

			return EM_TRUE;
		}

		static EM_BOOL wheel_callback(int event_type, const EmscriptenWheelEvent* e, void* user_data)
		{
			if (event_type == EMSCRIPTEN_EVENT_WHEEL)
				ptr_engine->engine_update_mouse_wheel(-1 * e->deltaY);

			return EM_TRUE;
		}

		static EM_BOOL touch_callback(int event_type, const EmscriptenTouchEvent* e, void* user_data) {
			if (event_type == EMSCRIPTEN_EVENT_TOUCHMOVE) {
				ptr_engine->engine_update_mouse(e->touches->targetX, e->touches->targetY);
			}

			if (event_type == EMSCRIPTEN_EVENT_TOUCHSTART) {
				ptr_engine->engine_update_mouse(e->touches->targetX, e->touches->targetY);
				ptr_engine->engine_update_mouse_state(0, true);
			}

			if (event_type == EMSCRIPTEN_EVENT_TOUCHEND) {
				ptr_engine->engine_update_mouse_state(0, false);
			}

			return EM_TRUE;
		}

		static EM_BOOL mouse_callback(int event_type, const EmscriptenMouseEvent* e, void* user_data) {
			if (event_type == EMSCRIPTEN_EVENT_MOUSEMOVE)
				ptr_engine->engine_update_mouse(e->targetX, e->targetY);

			if (e->button == 0) { // left
				if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptr_engine->engine_update_mouse_state(0, true);
				else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP)
					ptr_engine->engine_update_mouse_state(0, false);
			}

			if (e->button == 2) { // right
				if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptr_engine->engine_update_mouse_state(1, true);
				else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP)
					ptr_engine->engine_update_mouse_state(1, false);
			}

			if (e->button == 1) { // middle
				if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptr_engine->engine_update_mouse_state(2, true);
				else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP)
					ptr_engine->engine_update_mouse_state(2, false);

				return EM_TRUE;
			}

			return EM_FALSE;
		}

		virtual engine::Code set_window_title(const std::string& s) override { 
            emscripten_set_window_title(s.c_str()); 
            return engine::OK; 
        }

		virtual engine::Code start_system_event_loop() override { return engine::OK; }
		virtual engine::Code handle_system_event    () override { return engine::OK; }

		static void MainLoop() {
			engine::Platform::ptr_engine->engine_core_update();
			if (!ptr_engine->engine_is_running()) {
				if (ptr_engine->on_destroy()) {
					emscripten_cancel_main_loop();
					platform->application_cleanup();
				}
				else {
					ptr_engine->engine_reanimate();
				}
			}
		}
	};

	engine::Code Engine::start() {
		if (platform->application_startup() != engine::OK) return engine::FAIL;

		if (platform->create_window_pane({ 30,30 }, window_size, fullscreen) != engine::OK) return engine::FAIL;
		engine_update_window_size(window_size.x, window_size.y);

		if (platform->thread_startup() == engine::FAIL)	return engine::FAIL;

		engine_prepare();

		atom_active = true;

		for (auto& ext : extensions) ext->on_before_create();
		if (!on_create()) atom_active = false;
		for (auto& ext : extensions) ext->on_after_create();

		platform->start_system_event_loop();

		emscripten_set_main_loop(&Platform_Emscripten::MainLoop, 0, 1);

		if (platform->application_cleanup() != engine::OK) return engine::FAIL;
		return engine::OK;
	}
}

extern "C" {
	EMSCRIPTEN_KEEPALIVE inline void engine_update_window_size(int width, int height) {
		emscripten_set_canvas_element_size("#canvas", width, height);
		((engine::Platform_Emscripten*)engine::platform.get())->update_window_size(width, height);
	}
}

#endif

#pragma endregion

#endif // headless

#pragma region engine_config
namespace engine {
	void Engine::engine_configure_system()
	{
		engine::Sprite::loader = nullptr;

#if defined(ENGINE_IMAGE_GDI)
		engine::Sprite::loader = std::make_unique<engine::ImageLoader_GDIPlus>();
#endif

#if defined(ENGINE_IMAGE_LIBPNG)
		engine::Sprite::loader = std::make_unique<engine::ImageLoader_LibPNG>();
#endif

#if defined(ENGINE_IMAGE_STB)
		engine::Sprite::loader = std::make_unique<engine::ImageLoader_STB>();
#endif

#if defined(ENGINE_IMAGE_CUSTOM_EX)
		engine::Sprite::loader = std::make_unique<ENGINE_IMAGE_CUSTOM_EX>();
#endif

#if defined(ENGINE_PLATFORM_HEADLESS)
		platform = std::make_unique<engine::Platform_Headless>();
#endif

#if defined(ENGINE_PLATFORM_WINAPI)
		platform = std::make_unique<engine::Platform_Windows>();
#endif

#if defined(ENGINE_PLATFORM_X11)
		platform = std::make_unique<engine::Platform_Linux>();
#endif

#if defined(ENGINE_PLATFORM_GLUT)
		platform = std::make_unique<engine::Platform_GLUT>();
#endif

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
		platform = std::make_unique<engine::Platform_Emscripten>();
#endif

#if defined(ENGINE_PLATFORM_CUSTOM_EX)
		platform = std::make_unique<ENGINE_PLATFORM_CUSTOM_EX>();
#endif

#if defined(ENGINE_GFX_HEADLESS)
		renderer = std::make_unique<engine::Renderer_Headless>();
#endif

#if defined(ENGINE_GFX_OPENGL10)
		renderer = std::make_unique<engine::Renderer_OGL10>();
#endif

#if defined(ENGINE_GFX_OPENGL33)
		renderer = std::make_unique<engine::Renderer_OGL33>();
#endif

#if defined(ENGINE_GFX_OPENGLES2)
		renderer = std::make_unique<engine::Renderer_OGLES2>();
#endif

#if defined(ENGINE_GFX_DIRECTX10)
		renderer = std::make_unique<engine::Renderer_DX10>();
#endif

#if defined(ENGINE_GFX_DIRECTX11)
		renderer = std::make_unique<engine::Renderer_DX11>();
#endif

#if defined(ENGINE_GFX_CUSTOM_EX)
		renderer = std::make_unique<ENGINE_RENDERER_CUSTOM_EX>();
#endif
		platform->ptr_engine = this;
		renderer->ptr_engine = this;
	}
}
#pragma endregion

#endif // END APPLICATION