#include "Hotkeys.h"
#include "Actions.h"

std::map<const std::string, unsigned int> hotkeysMap = std::map<const std::string, unsigned int>();
std::map<const unsigned int, Hotkey*> hotkey = std::map<const unsigned int, Hotkey*>();

void InitHotkeysMap()
{
	hotkeysMap["space"] = VK_SPACE;
	hotkeysMap[","] = VK_OEM_COMMA;
	hotkeysMap["."] = VK_OEM_PERIOD;
}

Hotkey::Hotkey(const std::string action_name, const std::string key_name, const std::string alt_str,
			   const std::string control_str, const std::string shift_str, const std::string win_str)
{
	key = GetKeyCode(key_name);
	
	alt = IsKeyUsed(alt_str);
	control = IsKeyUsed(control_str);
	shift = IsKeyUsed(shift_str);
	win = IsKeyUsed(win_str);

	/*id = key + (alt ? 1000 * MOD_ALT : 0) + (control ? 10000 * MOD_CONTROL : 0) + 
		(shift ? 100000 * MOD_SHIFT : 0) + (win ? 1000000 * MOD_WIN : 0);*/

	Action = GetAction(action_name);
}

Hotkey::~Hotkey()
{
	/* nothing to do */
}

void (*Hotkey::GetAction(const std::string action_name))(void)
{
	switch (actionsMap[action_name])
	{
	case eActionPlayPause:
		return &PlayPause;
	case eActionPreviousTrack:
		return &PreviousTrack;
	case eActionNextTrack:
		return &NextTrack;
	default:
		return 0;
	}
}

unsigned int Hotkey::GetKeyCode(const std::string key_name)
{
	return hotkeysMap[key_name];
}

bool Hotkey::IsKeyUsed(const std::string key_str)
{
	return (key_str == std::string("true"));
}

void Hotkey::PerformAction()
{
	if(Action > 0)
		Action();
}