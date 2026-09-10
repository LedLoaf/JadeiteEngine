-- Run the Assets script
J2D_RunScript("assets/scripts/defs/assetDefs.lua")
J2D_RunScript("assets/scripts/defs/characterDefs.lua")
J2D_RunScript("assets/scripts/follow_camera.lua")
J2D_RunScript("assets/scripts/utilities.lua")
J2D_RunScript("assets/scripts/defs/maps/level1.lua")
----------------------------------------------------------------
LoadAssets(AssetDefs)

-- Load Level 1 map
LoadLevel(1)
----------------------------------------------------------------------------------------------

-- FPS Text information (FOLDED HERE GREEN ARROW ALT+H)
local fps_text = Entity()
fps_text:addComponent(Transform(vec2( 550, 25), vec2(1, 1), 0.0))
fps_text:addComponent(TextComponent( "pixel32", "FPS: " ))	
local text = fps_text:getComponent(TextComponent)
	
local frameCount = 0
local startTime = os.clock()
local fps = 0

local frameOffset_text = Entity()
frameOffset_text:addComponent(Transform(vec2( 1, 400), vec2(1, 1), 0.0))
frameOffset_text:addComponent(TextComponent( "pixel16", "Press [1-7] Frameoffset: 3" ))	
local frameOffsetComp= frameOffset_text:getComponent(TextComponent)

----------------------------------------------------------------------------------------------

function updateFPS()
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
gPlayer = LoadEntity(CharacterDefs["player"])

function UpdatePlayer(ent)
	local physics = ent:getComponent(PhysicsComp)
	physics:setLinearVelocity(vec2(0, 0))
	
	if Keyboard.pressed(KEY_W) then 
		physics:setLinearVelocity(vec2(0, -10))
	elseif Keyboard.pressed(KEY_S) then 
		physics:setLinearVelocity(vec2(0, 10))
	elseif Keyboard.pressed(KEY_A) then 
		physics:setLinearVelocity(vec2(-10, 0))
	elseif Keyboard.pressed(KEY_D) then 
		physics:setLinearVelocity(vec2(10, 0))
	end
end

gCam = Camera.get()
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

function UpdateCam(cam)
	local speed = 10
	
	if Keyboard.pressed(KEY_W) then
		cam.move(vec2(0, -1 * speed))
	elseif Keyboard.pressed(KEY_S) then
		cam.move(vec2(0,  1 * speed))
	elseif Keyboard.pressed(KEY_A) then
		cam.move(vec2(-1 * speed, 0))
	elseif Keyboard.pressed(KEY_D) then
		cam.move(vec2( 1 * speed, 0))
	end
end

local toggleCollision = false

function ChangeFrameOffset(value)
		local animComp = gPlayer:getComponent(Animation)
		animComp.frameOffset = value
	
		frameOffsetComp.sText = "Press [1-6]\n Frameoffset: "..tostring(value);
end


main = 
{
	update = function()
		--UpdatePlayer(gPlayer)
		
		--UpdateCam(gCam)
		gFollowCam:Update(gPlayer:id())
		
		-- In the update function
		if Keyboard.justReleased(KEY_F1) then
		  bEnabled = not bEnabled
		  J2D_EnableCollision(bEnabled)
		end
		
		if Keyboard.justReleased(KEY_1) then
			ChangeFrameOffset(0)
		elseif Keyboard.justReleased(KEY_2) then
			ChangeFrameOffset(3)
		elseif Keyboard.justReleased(KEY_3) then
			ChangeFrameOffset(6)
		elseif Keyboard.justReleased(KEY_4) then
			ChangeFrameOffset(9)
		elseif Keyboard.justReleased(KEY_5) then
			ChangeFrameOffset(12)
		elseif Keyboard.justReleased(KEY_6) then
			ChangeFrameOffset(15)
		elseif Keyboard.justReleased(KEY_7) then
			ChangeFrameOffset(18)
		end
			
		--updateFPS()

	end
}

