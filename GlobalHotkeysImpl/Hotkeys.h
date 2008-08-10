#ifndef HOTKEYS_H
#define HOTKEYS_H

#include <windows.h>

#include <string>
#include <map>

extern std::map<const std::string, unsigned int> hotkeysMap;

void InitHotkeysMap();

class Hotkey
{
public:
	explicit Hotkey() : Action(0), key(0), alt(false), control(false), shift(false), win(false) { };
	explicit Hotkey(const std::string action_name, const std::string key_name, const std::string alt_str, 
		const std::string control_str, const std::string shift_str, const std::string win_str);
	~Hotkey();

	inline unsigned int GetKeyCode() { return key; };

	inline bool GetAlt() { return alt; };
	inline bool GetControl() { return control; };
	inline bool GetShift() { return shift; };
	inline bool GetWin() { return win; };

	void PerformAction();	

private:
	// int id;
	unsigned int key;
	bool alt;
	bool control;
	bool shift;
	bool win;

	void (*Action)(void);

	void (*GetAction(const std::string action_name))(void);
	unsigned int GetKeyCode(const std::string key_name);
	bool IsKeyUsed(const std::string key_str);
};

extern std::map<const unsigned int, Hotkey*> hotkey;

#endif /* HOTKEYS_H */