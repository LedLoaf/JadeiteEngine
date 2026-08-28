#include "gamepad.hpp"
#include "gp_buttons.hpp"
#include <iostream>

namespace jadeite
{
Gamepad::Gamepad()
	: Gamepad(nullptr)
{
}

Gamepad::Gamepad(Controller pController)
	: m_pController{ std::move(pController) }
	, m_mapButtons{
		{ GP_BTN_A, Button{} },
		{ GP_BTN_B, Button{} },
		{ GP_BTN_X, Button{} },
		{ GP_BTN_Y, Button{} },
		{ GP_BTN_BACK, Button{} },
		{ GP_BTN_GUIDE, Button{} },
		{ GP_BTN_START, Button{} },
		{ GP_BTN_LSTICK, Button{} },
		{ GP_BTN_RSTICK, Button{} },
		{ GP_BTN_LSHOULDER, Button{} },
		{ GP_BTN_RSHOULDER, Button{} },
		{ GP_BTN_DPAD_UP, Button{} },
		{ GP_BTN_DPAD_DOWN, Button{} },
		{ GP_BTN_DPAD_LEFT, Button{} },
		{ GP_BTN_DPAD_RIGHT, Button{} }
	}
	, m_mapAxisValues{ 
		{ 0, 0 }, 
		{ 1, 0 }, 
		{ 2, 0 }, 
		{ 3, 0 }, 
		{ 4, 0 }, 
		{ 5, 0 }
	}
	, m_InstanceID{ -1 }
	, m_JoystickHatValue{ HAT_CENTERED }
	, m_sName{ "" }	
{
	if (m_pController)
	{
		SDL_Joystick* pJoystick = SDL_GameControllerGetJoystick(m_pController.get());
		if (!pJoystick)
		{
			throw("Joystick was invalid.");
		}
		
		m_InstanceID = SDL_JoystickInstanceID(pJoystick);
		m_sName = std::string{ SDL_JoystickName(pJoystick) };
	}
}

Gamepad::~Gamepad() = default;

void Gamepad::Update()
{
	if (!m_pController)
	{
		return;
	}
	
	for (auto& [_, btn] : m_mapButtons)
	{
		btn.Reset();
	} 
}

void Gamepad::OnBtnPressed(int btn)
{
	if (!m_pController)
	{
		return;
	}
	
	if ( btn == GP_BTN_UNKNOWN )
	{
		return;
	}
	
	auto btnItr = m_mapButtons.find(btn);
	if (btnItr == m_mapButtons.end())
	{
		return;
	}
	
	btnItr->second.Update( true );
}

void Gamepad::OnBtnReleased(int btn)
{
	if (!m_pController)
	{
		return;
	}
	
	if ( btn == GP_BTN_UNKNOWN )
	{
		return;
	}
	
	auto btnItr = m_mapButtons.find(btn);
	if (btnItr == m_mapButtons.end())
	{
		return;
	}
	
	btnItr->second.Update( false );
}

const bool Gamepad::IsBtnPressed( int btn ) const
{
	if (!m_pController)
	{
		return false;
	}
	
	if ( btn == GP_BTN_UNKNOWN )
	{
		return false;
	}
	
	auto btnItr = m_mapButtons.find(btn);
	if (btnItr == m_mapButtons.end())
	{
		return false;
	}
	
	return btnItr->second.bIsPressed;
}

const bool Gamepad::IsBtnJustPressed( int btn ) const
{
	if (!m_pController)
	{
		return false;
	}
	
	if ( btn == GP_BTN_UNKNOWN )
	{
		return false;
	}
	
	auto btnItr = m_mapButtons.find(btn);
	if (btnItr == m_mapButtons.end())
	{
		return false;
	}
	
	return btnItr->second.bJustPressed;
}

const bool Gamepad::IsBtnJustReleased( int btn ) const
{
	if (!m_pController)
	{
		return false;
	}
	
	if ( btn == GP_BTN_UNKNOWN )
	{
		return false;
	}
	
	auto btnItr = m_mapButtons.find(btn);
	if (btnItr == m_mapButtons.end())
	{
		return false;
	}
	
	return btnItr->second.bJustReleased;
}

const bool Gamepad::IsGamepadPresent() const
{
	return m_pController != nullptr;
}

const Sint16 Gamepad::GetAxisPosition(Uint8 axis)
{
	if (!m_pController)
	{
		return 0;
	}
	
	auto axisItr = m_mapAxisValues.find(axis);
	if (axisItr == m_mapAxisValues.end())
	{
		return 0;
	}
	
	return axisItr->second;
}

void Gamepad::SetAxisPositionValue(Uint8 axis, Sint16 value)
{
	if (!m_pController)
	{
		return;
	}
	
	auto axisItr = m_mapAxisValues.find(axis);
	if (axisItr == m_mapAxisValues.end())
	{
		return;
	}
	
	axisItr->second = value;
}

void Gamepad::SetController(Controller pController)
{
	if (!pController)
	{
		std::cerr << "Failed to set controller. Controller invalid.\n";
		return;
	}
	
	m_pController = std::move(pController);
	
	SDL_Joystick* pJoystick = SDL_GameControllerGetJoystick(m_pController.get());
	if (!pJoystick)
	{
		throw("Joystick was invalid.");
	}
	
	m_InstanceID = SDL_JoystickInstanceID(pJoystick);
	m_sName = std::string{ SDL_JoystickName(pJoystick) };
	
	std::cout << "Added Controller Successfully.\n";
}

void Gamepad::RemoveController()
{
	if (!m_pController)
	{
		std::cerr << "Controller is already removed.\n";
		return;
	}
	
	Update();
	
	m_pController.reset();
	m_InstanceID = -1;
	m_sName = "";
	std::cout << "Removed Controller Successfully.\n";
}

void Gamepad::CreateLuaBind(sol::state& lua, Gamepad& gamepad)
{
	lua.set("GP_BTN_A", GP_BTN_A);
	lua.set("GP_BTN_B", GP_BTN_B);
	lua.set("GP_BTN_X", GP_BTN_X);
	lua.set("GP_BTN_Y", GP_BTN_Y);
	
	lua.set("GP_BTN_BACK", GP_BTN_BACK);
	lua.set("GP_BTN_GUIDE", GP_BTN_GUIDE);
	lua.set("GP_BTN_START", GP_BTN_START);
	
	lua.set("GP_LSTICK", GP_BTN_LSTICK);
	lua.set("GP_RSTICK", GP_BTN_RSTICK);
	lua.set("GP_LSHOULDER", GP_BTN_LSHOULDER);
	lua.set("GP_RSHOULDER", GP_BTN_RSHOULDER);
	
	lua.set("DPAD_UP", GP_BTN_DPAD_UP);
	lua.set("DPAD_DOWN", GP_BTN_DPAD_DOWN);
	lua.set("DPAD_LEFT", GP_BTN_DPAD_LEFT);
	lua.set("DPAD_RIGHT", GP_BTN_DPAD_RIGHT);
	
	lua.set("AXIS_X1", 0);
	lua.set("AXIS_Y1", 1);
	lua.set("AXIS_X2", 2);
	lua.set("AXIS_Y2", 3);
	
	lua.set("AXIS_Z1", 4);
	lua.set("AXIS_Z1", 5);
	
	lua.new_usertype<Gamepad>(
		"Gamepad",
		sol::no_constructor,
		"justPressed", [&](int btn){ return gamepad.IsBtnJustPressed(btn); },
		"justReleased", [&](int btn){ return gamepad.IsBtnJustReleased(btn); },
		"pressed", [&](int btn){ return gamepad.IsBtnPressed(btn); },
		"getAxisPosition", [&](int axis) { return gamepad.GetAxisPosition(axis); },
		"getHatValue", [&](int axis) { return gamepad.GetJoystickHatValue(); },
		"isGamepadPresent", [&]{ return gamepad.IsGamepadPresent(); }
	);
}

void ControllerDestroyer::operator()(SDL_GameController* pController) const
{
	SDL_GameControllerClose(pController);
	pController = nullptr;
	std::cout << "Removed Controller from SDL.\n";
}


} // jadeite
