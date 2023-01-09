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

namespace engine {
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