#ifndef BTN_DEF
#define BTN_DEF

struct ButtonState {
	bool pressed = false;
	bool released = false;
	bool held = false;
};

#endif