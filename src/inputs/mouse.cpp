#include "mouse.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace jadeite
{
	/* Default Constructor setting the x/y, mouse wheel x/y, and if moving */
	Mouse::Mouse()
		: m_mapButtons{
			{ MOUSE_LEFT, Button{} },
			{ MOUSE_MIDDLE, Button{} },
			{ MOUSE_RIGHT, Button{} }
		}
		, m_X{ 0 }
		, m_Y{ 0 }
		, m_WheelX{ 0 }
		, m_WheelY{ 0 }
		, m_bMoving{ false }
	{
	}

	/* Resets the mouse buttons and sets the mouse wheel x/y to 0 and moving to false */
	void Mouse::Update()
	{
		for (auto& [_, btn] : m_mapButtons)
		{
			btn.Reset();
		} 
		
		m_WheelX = 0;
		m_WheelY = 0;
		m_bMoving = false;
	}

	/* If the specific mouse button was pressed */
	void Mouse::OnBtnPressed(int btn)
	{
		if ( btn == MOUSE_UNKNOWN )
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

	/* If the specific mouse button was released */
	void Mouse::OnBtnReleased(int btn)
	{
		if ( btn == MOUSE_UNKNOWN )
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

	/* Check if the specific mouse button is currently held down */
	const bool Mouse::IsBtnPressed( int btn ) const
	{
		if ( btn == MOUSE_UNKNOWN )
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

	/* Checks if the mouse button was JUST pressed THIS frame */
	const bool Mouse::IsBtnJustPressed( int btn ) const
	{
		if ( btn == MOUSE_UNKNOWN )
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

	/* Checks if the mouse button was JUST released THIS frame */
	const bool Mouse::IsBtnJustReleased( int btn ) const
	{
		if ( btn == MOUSE_UNKNOWN )
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
	
	// TODO: Change to a vector2
	/* Get the mouse position as a tuple */
	const std::tuple<int, int> Mouse::GetMouseScreenPosition()
	{
		SDL_GetMouseState(&m_X, &m_Y);
		return std::make_tuple(m_X, m_Y);
	}

	/* The bindings for lua to access */
	void Mouse::CreateLuaBind(sol::state& lua, Mouse& mouse)
	{
		// Mouse Buttons Lua Binding
		lua.set("LEFT_BTN", MOUSE_LEFT);
		lua.set("MIDDLE_BTN", MOUSE_MIDDLE);
		lua.set("RIGHT_BTN", MOUSE_RIGHT);
		
		// Mouse Lua Binding
		lua.new_usertype<Mouse>(
			"Mouse",
			sol::no_constructor,
			"justPressed", [&](int btn) { return mouse.IsBtnJustPressed(btn); },
			"justReleased", [&](int btn) { return mouse.IsBtnJustReleased(btn); },
			"pressed", [&](int btn) { return mouse.IsBtnPressed(btn); },
			"screenPosition", [&] 
			{
				auto [x, y] = mouse.GetMouseScreenPosition();
				return glm::vec2{ x, y };
			},
			"wheelX", [&] { return mouse.GetMouseWheelX(); },
			"wheelY", [&] { return mouse.GetMouseWheelY(); }
		);
	}
} // jadeite::Mouse
