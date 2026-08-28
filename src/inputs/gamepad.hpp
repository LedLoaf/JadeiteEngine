#pragma once 
#include "button.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

namespace jadeite
{

struct ControllerDestroyer
{
	void operator()(SDL_GameController* pController) const;
};

using Controller = std::shared_ptr<SDL_GameController>;

inline Controller MakeSharedController(SDL_GameController* pController)
{
	return std::shared_ptr<SDL_GameController>(pController, ControllerDestroyer{});
}

class Gamepad
{
public:
	Gamepad();
	Gamepad(Controller pController);
	~Gamepad();
	
	void Update();
	void OnBtnPressed(int btn);
	void OnBtnReleased(int btn);
	
	const bool IsBtnPressed( int btn ) const;
	const bool IsBtnJustPressed( int btn ) const;
	const bool IsBtnJustReleased( int btn ) const;
	
	const bool IsGamepadPresent() const;
	
	const Sint16 GetAxisPosition(Uint8 axis);
	void SetAxisPositionValue(Uint8 axis, Sint16 value);
	
	void SetController(Controller pController);
	void RemoveController();
	
	inline void SetJoystickHatValue(Uint8 value) { m_JoystickHatValue = value; }
	inline const Uint8 GetJoystickHatValue() const { return m_JoystickHatValue; }
	inline const std::string& GetName() const { return m_sName; }
	
	static void CreateLuaBind(sol::state& lua, Gamepad& gamepad);
	
private:
	Controller m_pController;
	std::unordered_map<int, Button> m_mapButtons;
	std::unordered_map<Uint8, Sint16> m_mapAxisValues;
	SDL_JoystickID m_InstanceID;
	Uint8 m_JoystickHatValue;
	std::string m_sName;
};

} // jadeite

