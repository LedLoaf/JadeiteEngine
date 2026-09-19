-- Character Defs

PlayerData = {}
PlayerData.__index = PlayerData

function PlayerData:Create()
    local this =
    {
        bOnLadder = false,
        bInAir = false,
        bOnGround = false,
        airTimer = Timer()
    }
    setmetatable(this, self)
    return this
end

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
                startY = 1,
            },
            animation =
            {
                numFrames = 4,
                frameRate = 6,
                bVertical = false,
                bLooped = true,
            },
            circleCollider =
            {
                radius = 8,
                offset = vec2(8,16)
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
        startPos = vec2(50,0),
        userData = PlayerData:Create()
    } 
} 