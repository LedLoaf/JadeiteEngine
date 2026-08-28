#pragma once
#include "button.hpp"
#include <unordered_map> 
#include <sol/sol.hpp>

namespace jadeite
{
constexpr int MOUSE_UNKNOWN = -1;
constexpr int MOUSE_LEFT = 1;
constexpr int MOUSE_MIDDLE = 2;
constexpr int MOUSE_RIGHT = 3;

class Mouse
{
public:
	Mouse();
	~Mouse();
	
	void Update();
	void OnBtnPressed(int btn);
	void OnBtnReleased(int btn);
	
	const bool IsBtnPressed( int btn ) const;
	const bool IsBtnJustPressed( int btn ) const;
	const bool IsBtnJustReleased( int btn ) const;
	
	const std::tuple<int, int> GetMouseScreenPosition();
	
	inline void SetMouseWheelValues(int wheelX, int wheelY) { m_WheelX = wheelX; m_WheelY = wheelY; }
	inline void SetMouseMoving(bool bMoving) { m_bMoving = bMoving; }
	
	inline const int GetMouseWheelX() const { return m_WheelX; }
	inline const int GetMouseWheelY() const { return m_WheelY; }
	inline const bool IsMouseMoving() const { return m_bMoving; }
	
	static void CreateLuaBind(sol::state& lua, Mouse& mouse);
	
private:
	std::unordered_map<int, Button> m_mapButtons;
	int m_X;
	int m_Y;
	int m_WheelX;
	int m_WheelY;
	bool m_bMoving;
};

} // jadeite
