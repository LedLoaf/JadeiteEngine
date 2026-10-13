-- Run the Asset scripts
J2D_RunScript("assets/scripts/defs/assetDefs.lua")
J2D_RunScript("assets/scripts/defs/characterDefs.lua")
J2D_RunScript("assets/scripts/defs/pickupdefs.lua")
J2D_RunScript("assets/scripts/defs/maps/level1.lua")

J2D_RunScript("assets/scripts/follow_camera.lua")
J2D_RunScript("assets/scripts/pickup.lua")
J2D_RunScript("assets/scripts/player.lua")

J2D_RunScript("assets/scripts/utilities/coroutineScheduler.lua")
J2D_RunScript("assets/scripts/utilities/utilities.lua")

Print("Running main script...")

----------------------------------------------------------------
LoadAssets(AssetDefs)

-- Load Level 1 map
LoadLevel(1)
----------------------------------------------------------------------------------------------
-- Create the player
gPlayer = Player:Create( {name = "player", startPos = vec2(2 * 16, 15 * 16) } )


-- TODO: TEXT IS NOT WORKING CORRECTLY. IT'S NOT SCALING NOR IS IT IN THE RIGHT POSITION 
-- FPS Text information (FOLDED HERE GREEN ARROW ALT+H)
local fps_text = Entity()
fps_text:addComponent(Transform( vec2( 20 * 16, 12 * 16), vec2( 1,  1), 0.0) )
fps_text:addComponent(TextComponent( "pixel32", "FPS: 0" ))	
fps_text:addComponent(UIComponent(UIObjectType.Label))
local text = fps_text:getComponent(TextComponent)

-- Explaination text of the example
local control_text = Entity()
control_text:addComponent(Transform( vec2( 0 * 16, 10 * 16), vec2(1, 1), 0.0) )
control_text:addComponent(TextComponent( "pixel32", "Press F1  for Debug\nE off ladder\nShift+WASD for peek" ))	
control_text:addComponent(UIComponent(UIObjectType.Label))
local controlString= control_text:getComponent(TextComponent)

-- UI TEST
local uiEntity = Entity()
uiEntity:addComponent(Transform(vec2(50, 50), vec2(2, 2), 0))
local sprite = uiEntity:addComponent(Sprite("characters", 32, 32, 1, 0, 0, Color(255, 255, 255, 255)))
sprite:generateUVs(736, 128)
uiEntity:addComponent(UIComponent(UIObjectType.Image))

-- FPS variables
local frameCount = 0
local startTime = os.clock()
local fps = 0

----------------------------------------------------------------------------------------------
-- Runs the current FPS in the corner and updates the position based on the camera
function updateFPS(cam)
		local fpsPos = fps_text:getComponent(Transform)
		local camPos = cam.getPosition()
		fpsPos.position.x = camPos.x + 150
		fpsPos.position.y = camPos.y + 16
		
		local controlPos = control_text:getComponent(Transform)
		controlPos.position.x = camPos.x + 10
		controlPos.position.y = camPos.y + 16
	
		frameCount = frameCount + 1
		local currentTime = os.clock()
		local elapsedTime = currentTime - startTime

		-- Calculate FPS every 1 second
		if elapsedTime >= 1.0 then
			fps = math.floor(frameCount / elapsedTime)
			
			-- Reset counters
			frameCount = 0
			startTime = currentTime
			
			-- Output FPS (replace with your render logic)
			text.sText="FPS: " .. tostring(fps)
		end
	end
------------------------------------------------------------------

-- Camera stuff
gCam = Camera.get()
gCam.setPosition(vec2(0,  gPlayer.startPos.y - (gCam.getHeight() / gCam.getScale()) * 0.5))

-- Create the global follow camera
gFollowCam = FollowCamera:Create(
	gCam,
	{
		scale 		= 4.0,
		minX 		= 0,
		minY 		= 0,
		maxX 		= 16 * 40,
		maxY 		= 16 * 30,
		springback 	= 0.05
	}
)

-- Allows the ability to peak in-game
function OverrideFollowCamera(followCamera)

	if Keyboard.pressed(KEY_LSHIFT) then
		if Keyboard.pressed(KEY_W) then
			followCamera:SetOverrideDistance(vec2(0, -75))
		elseif Keyboard.pressed(KEY_S) then
			followCamera:SetOverrideDistance(vec2(0, 75))
		elseif Keyboard.pressed(KEY_A) then
			followCamera:SetOverrideDistance(vec2(-75, 0))
		elseif Keyboard.pressed(KEY_D) then
			followCamera:SetOverrideDistance(vec2(75, 0))
		else
			followCamera:SetOverrideDistance(vec2(0, 0))
		end
	end
end

-- Helper function to create pickups
function CreatePickup(pos)
	Pickup:Create( { name = "coin", amount = 5, spawnPos = pos, } )
end

-- Adding in pickups
CreatePickup( vec2(56, 314))
CreatePickup( vec2(132, 269))
CreatePickup( vec2(243, 250))
CreatePickup( vec2(336, 220))
CreatePickup( vec2(434, 265))
CreatePickup( vec2(560, 365))
CreatePickup( vec2(575, 365))
CreatePickup( vec2(511, 74))
CreatePickup( vec2(624, 151))


--*** MAIN PROGRAM LOOP ***
main = 
{
	-- Main update function
	update = function()
	
		-- Program Updates
		OverrideFollowCamera(gFollowCam)
		gPlayer:Update()
		gFollowCam:Update(gPlayer.entity:id())
		gScheduler:Update()
		-- [F1] Toggles showing the collision boxes
		if Keyboard.justReleased(KEY_F1) then
		  bEnabled = not bEnabled
		  J2D_EnableCollision(bEnabled)
		end
		
		-- Update FPS tracker
		updateFPS(gCam)
	end 
	-- Main update function end
	
}
--*************************