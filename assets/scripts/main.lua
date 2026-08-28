-- Run the Assets script
J2D_RunScript("assets/scripts/defs/assetDefs.lua")
J2D_RunScript("assets/scripts/defs/characterDefs.lua")
J2D_RunScript("assets/scripts/utilities.lua")
J2D_RunScript("assets/scripts/defs/maps/level2.lua")
----------------------------------------------------------------
LoadAssets(AssetDefs)

local tilemap = LoadTiledMap(CreateLevel1Map())
LoadMap(tilemap)
----------------------------------------------------------------------------------------------

-- FPS Text information (FOLDED HERE GREEN ARROW ALT+H)
local fps_text = Entity()
fps_text:addComponent(Transform(vec2( 550, 25), vec2(1, 1), 0.0))
fps_text:addComponent(TextComponent( "pixel32", "FPS: " ))	
local text = fps_text:getComponent(TextComponent)
	
local frameCount = 0
local startTime = os.clock()
local fps = 0
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
gCam.setScale(2)

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



--Camera.get().setPosition(transform.position.x,transform.position.y)
main = 
{
	update = function()
		--UpdatePlayer(gPlayer)
		
		UpdateCam(gCam)
		
		updateFPS()
		
		
		
	end
}

