-- Run the Assets script
J2D_RunScript("assets/scripts/defs/assetDefs.lua")
J2D_RunScript("assets/scripts/defs/characterDefs.lua")
J2D_RunScript("assets/scripts/follow_camera.lua")
J2D_RunScript("assets/scripts/player.lua")
J2D_RunScript("assets/scripts/utilities.lua")
J2D_RunScript("assets/scripts/defs/maps/level1.lua")
----------------------------------------------------------------
LoadAssets(AssetDefs)

-- Load Level 1 map
LoadLevel(1)
----------------------------------------------------------------------------------------------
-- Create the player
gPlayer = Player:Create({name = "player",startPos = vec2(2 * 16, 15 * 16)})

-- FPS Text information (FOLDED HERE GREEN ARROW ALT+H)
local fps_text = Entity()
fps_text:addComponent(Transform(vec2( 20 * 16, 12 * 16), vec2(1, 1), 0.0))
fps_text:addComponent(TextComponent( "pixel12", "FPS: 0" ))	
local text = fps_text:getComponent(TextComponent)

-- TODO: Add in text that SHIFT allows peeking
local control_text = Entity()
control_text:addComponent(Transform(vec2( 0 * 16, 10 * 16), vec2(1, 1), 0.0))
control_text:addComponent(TextComponent( "pixel12", "Press F1  for Debug\nE off ladder\nShift+WASD for peek" ))	
local controlString= control_text:getComponent(TextComponent)
	
local frameCount = 0
local startTime = os.clock()
local fps = 0

----------------------------------------------------------------------------------------------
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

gFollowCam = FollowCamera:Create(
	gCam,
	{
		scale = 4.0,
		minX = 0,
		minY = 0,
		maxX = 16 * 40,
		maxY = 16 * 30,
		springback = 0.05
	}
)

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

--*** MAIN PROGRAM LOOP ***
main = 
{
	update = function()

		OverrideFollowCamera(gFollowCam)
		gFollowCam:Update(gPlayer.entity:id())
		gPlayer:Update()
		
		-- In the update function
		if Keyboard.justReleased(KEY_F1) then
		  bEnabled = not bEnabled
		  J2D_EnableCollision(bEnabled)
		end
			
		updateFPS(gCam)
	end
}
--*************************
