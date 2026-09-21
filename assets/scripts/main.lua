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

-- Player Def
local def = CharacterDefs["player"]
def.startPos = vec2(2 * 16, 15 * 16)
gPlayer = LoadEntity(def)

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

gCam.setPosition(vec2(0, def.startPos.y))

function UpdatePlayer(ent)
	-- Get player components
	local physics = ent:getComponent(PhysicsComp)
	local objectData = physics:objectData()
		
	local sprite = ent:getComponent(Sprite)
	local animation = ent:getComponent(Animation)
	local velocity = physics:getLinearVelocity()
	
	-- Climb Ladder
	if objectData.userData.bOnLadder then
		UpdateLadderClimb(ent)
		return
	end
		
		
	local roundVelX = J2D_round(velocity.x)
	local roundVelY = J2D_round(velocity.y)

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
	
	-- Handle sprite flipping
	if velocity.x < 0 then
		sprite.bFlipX = true
	elseif velocity.x > 0 then
		sprite.bFlipX = false
	end
	
	-- Left and Right movement 
	if not Keyboard.pressed(KEY_LSHIFT) then					-- if not using peek
		if Keyboard.pressed(KEY_A) then 
			if velocity.x > -3 then
				physics:applyForce(vec2(-1000, 0))
			end
		elseif Keyboard.pressed(KEY_D) then 
			if velocity.x < 3 then
				physics:applyForce(vec2(1000, 0))
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
		elseif objectData.userData.airTimer:elapsedMs() > 225 then
			objectData.userData.bInAir = false
			objectData.userData.airTimer:stop()
		end
	end
	
	-- Jumped movement
	if not Keyboard.pressed(KEY_LSHIFT) then				-- if not using peek
		if (Keyboard.justPressed(KEY_W) or Keyboard.justPressed(KEY_SPACE)) and not objectData.userData.bInAir then
			objectData.userData.bInAir = true
			objectData.userData.airTimer:stop()
			physics:linearImpulse(vec2(0, -1200))
		end
	end
end

-- Update the player contacts
function UpdatePlayerContacts(ent)
		local physics = ent:getComponent(PhysicsComp)
		local objectData = physics:objectData()
		
		if objectData.userData.bOnLadder and #objectData.contactEntities == 0 then
			objectData.userData.bOnLadder = false
			physics:setGravityScale(0.5)
			physics:setLinearVelocity(vec2(0,0))
		end
		
		-- Check if one of the contacts is a ladder
		for _, v in pairs(objectData.contactEntities) do
			if v.group == "ladder" and not objectData.userData.bOnLadder and Keyboard.justReleased(KEY_SPACE) then
				physics:setGravityScale(0.0)
				physics:setLinearVelocity(vec2(0,0))
				-- We know we are on the ladder
				objectData.userData.bOnLadder = true
				objectData.userData.bInAir = false
				objectData.userData.airTimer:stop()
				return
			end
		end
end

-- Climbing the ladder
function UpdateLadderClimb(ent)

	local physics = ent:getComponent(PhysicsComp)
	local objectData = physics:objectData()
	local sprite = ent:getComponent(Sprite)
	local animation = ent:getComponent(Animation)
	local velocity = physics:getLinearVelocity()
	
	if sprite.startX ~= 18 then
		sprite.startX = 18
		sprite:inspectX()
	end
	
	if J2D_round(velocity.x) == 0 and J2D_round(velocity.y) == 0 then
		animation.numFrames = 1
	elseif animation.numFrames ~= 4 then
		animation.numFrames = 4
		animation:reset()
	end
	
	if Keyboard.pressed(KEY_W) then
		if velocity.y > -5 then
			physics:applyForce(vec2(0, -500))
		end
	elseif Keyboard.pressed(KEY_S) then
		if velocity.y < 5 then
			physics:applyForce(vec2(0, 500))
		end
	else
		physics:applyForce(vec2(velocity.x * -500, velocity.y * -500))
	end
	
	if objectData.userData.bOnLadder and Keyboard.justReleased(KEY_E) then
		objectData.userData.bOnLadder = false
		physics:setGravityScale(0.5)
	end
	
end

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
		UpdatePlayerContacts(gPlayer)
		UpdatePlayer(gPlayer)
		
		OverrideFollowCamera(gFollowCam)
		gFollowCam:Update(gPlayer:id())
		
		-- In the update function
		if Keyboard.justReleased(KEY_F1) then
		  bEnabled = not bEnabled
		  J2D_EnableCollision(bEnabled)
		end
			
		updateFPS(gCam)
	end
}
--*************************
