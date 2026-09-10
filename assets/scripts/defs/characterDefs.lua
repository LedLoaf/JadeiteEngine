CharacterDefs = 
{
    player =
    {
        components = 
        {
            sprite = 
            {
                texture = "characters",
                width = 32,
                height = 32,
                layer = 4,
                startX = 0,
                startY = 18,
            },
            animation =
            {
                numFrames = 5,
                frameRate = 6,
                bVertical = false,
                bLooped = true,
            },
            circleCollider =
            {
                radius = 8,
                offset = vec2(8,12)
            },
            physics = 
            {
                type = BodyType.Dynamic,
                density = 100,
                friction = 1.0,
                restitution = 0.0,
                radius = 8,
                bCircle = true,
                bFixedRotation = true,
                bIsSensor = false,
				gravityScale = 1.0,
                objectData = 
                {   
                    tag = "player",
                    group = "",
                    bIsFriendly = true
                }
            }
        }, 
        startPos = vec2(50,0)
    } 
} 