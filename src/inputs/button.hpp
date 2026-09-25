#pragma once

namespace jadeite
{
	/* Button struc to keep track if a button is pressed, just pressed, or released */
	struct Button
	{	
		/* Button states */
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