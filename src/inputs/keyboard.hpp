#pragma once
#include "button.hpp"
#include <unordered_map>
#include <sol/sol.hpp>

namespace jadeite
{
class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	
	void Update();
	void OnKeyPressed(int key);
	void OnKeyReleased(int key);
	
	const bool IsKeyPressed(int key) const;
	const bool IsKeyJustPressed(int key) const;
	const bool IsKeyJustReleased(int key) const;
	const bool IsAnyKeyPressed() const;
	
	static void CreateLuaBind(sol::state& lua, Keyboard& keyboard);
	
private:
	std::unordered_map<int, Button> m_mapButtons;
};

} // jadeite
