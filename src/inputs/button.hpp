#pragma once

namespace jadeite
{
struct Button
{
	bool bIsPressed{ false };
	bool bJustPressed{ false };
	bool bJustReleased{ false };
	
	inline void Update(bool bPressed)
	{
		bJustPressed = !bIsPressed && bPressed;
		bJustReleased = bIsPressed && !bPressed;
		
		bIsPressed = bPressed;
	}
	
	inline void Reset()
	{
		bJustPressed = false;
		bJustReleased = false;
	}
};

} // jadeite