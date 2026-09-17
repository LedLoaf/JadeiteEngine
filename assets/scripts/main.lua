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
fps_text:addComponent(Transform(vec2( 20 * 16, 12 * 16), vec2(1, 1), 0.0))
fps_text:addComponent(TextComponent( "pixel16", "FPS: 0" ))	
local text = fps_text:getComponent(TextComponent)
	
local frameCount = 0
local startTime = os.clock()
local fps = 0

----------------------------------------------------------------------------------------------

function updateFPS(cam)
		local fpsPos = fps_text:getComponent(Transform)
		local camPos = cam.getPosition()
		fpsPos.position.x = camPos.x + 212
		fpsPos.position.y = camPos.y + 16
	
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

-- Player Def
local def = CharacterDefs["player"]
def.startPos = vec2(2 * 16, 15 * 16)
gPlayer = LoadEntity(def)

gCam = Camera.get()
gFollowCam = FollowCamera:Create(
	gCam,
	{
		scale = 3.0,
		minX = 0,
		minY = 0,
		maxX = 16 * 40,
		maxY = 16 * 30,
		springback = 0.05
	}
)

gCam.setPosition(vec2(0, def.startPos.y))

function UpdatePlayer(ent)
	-- Get player components
	local physics = ent:getComponent(PhysicsComp)
	local sprite = ent:getComponent(Sprite)
	local anim = ent:getComponent(Animation)
	-- Component Data
	local velocity = physics:getLinearVelocity()
	local objectData = physics:objectData()
	
	local roundVelX = J2D_round(velocity.x)
	local roundVelY = J2D_round(velocity.y)

	print("Rounded Values: "..roundVelX..", "..roundVelY)
	-- If completely NOT moving
	if roundVelX == 0 and roundVelY == 0 then
		animation.numFrames = 1
	elseif animation.numFrames ~=4 and not objectData.userData.bInAir then
		animation.numFrames = 4
		animation.frameRate = 6
		sprite.startX = 0
		sprite:inspectX()
		animation:reset()
	end
	
	-- Left and Right movement
	if Keyboard.pressed(KEY_A) then 
		if velocity.x > -5 then
			physics:applyForce(vec2(-1500, 0))
		end
	elseif Keyboard.pressed(KEY_D) then 
		if velocity.x < 5 then
			physics:applyForce(vec2(1500, 0))
		end
	else	
		-- If no left or right input
		physics:applyForce(vec2(velocity.x * -1000, 0))
	end
	
	-- Checking if the user can jump again
	if roundVelY ~= 0 then
		-- Handle air stuff (we are in air)
	elseif not objectData.userData.airTimer:isRunning() then
		objectData.userData.airTimer:start()
	elseif objectData.userData.airTimer:elapsedMs() > 250 then
		objectData.userData.bInAir = false
		objectData.userData.airTimer:stop()
	end
	
	-- Jump movement
	if (Keyboard.justPressed(KEY_W) or Keyboard.justPressed(KEY_SPACE)) and not objectData.userData.bInAir then
		objectData.userData.bInAir = true
		objectData.userData.airTimer:stop()
		physics:linearImpulse(vec2(0, -1200))
	end

end


main = 
{
	update = function()
		
		UpdatePlayer(gPlayer)
		
		gFollowCam:Update(gPlayer:id())
		
		-- In the update function
		if Keyboard.justReleased(KEY_F1) then
		  bEnabled = not bEnabled
		  J2D_EnableCollision(bEnabled)
		end
			
	--	updateFPS(gCam)
	end
}

