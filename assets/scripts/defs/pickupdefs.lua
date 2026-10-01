-- Pick-up definitions

-- Type of pick-up Enum
PickupType =
{
	Health 	= 1,
	Ammo 	= 2,
	Coin 	= 3,
	-- TODO: Add more types as needed
}

-- Table to hold all pickup definitions
PickupDefs = 
{
	coin = 
	{
		type = PickupType.Coin,
		pickupSound = "coin_pickup",
		components = 
        {
			sprite = 
			{
				texture = "coin",
				width 	= 16,
				height 	= 16,
				layer 	= 4,
				startX 	= 0,
				startY 	= 0,
			},
			animation =
			{
				numFrames 	= 7,
				frameRate 	= 14,
				bVertical 	= false,
				bLooped		= true,
			},
			boxCollider =
			{
				width 	= 16,
				height 	= 16,
				offset 	= vec2(0, 0),
				color 	= Color(177, 0, 177, 133)
			},
			physics = 
			{
				type = BodyType.Static,				-- static so it doesn't move
				density 		= 100,
				friction 		= 0.0,
				restitution 	= 0.0,
				radius 			= 0,
				boxSize 		= vec2(16, 16),
				bCircle 		= false,
				bBoxShape 		= true,
				bFixedRotation 	= true,
				bIsSensor 		= false,
				objectData = 
				{   
					tag = "",
					group = "pickup",
					bIsFriendly = true
				}
			}
		}, 
		pickupAnimation = 
		{
			numFrames 	= 4,
			frameRate 	= 10,
			frameOffset = 7,
			bVertical 	= false,
			bLooped 	= false
		}
	} 
}