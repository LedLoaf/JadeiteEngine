-- Run the Assets script
J2D_RunScript("assets/scripts/defs/assetDefs.lua")
J2D_RunScript("assets/scripts/utilities.lua")
----------------------------------------------------------------
LoadAssets(AssetDefs)
----------------------------------------------------------------------------------------------
local gPlayer = Entity()
local transform = gPlayer:addComponent(Transform(vec2(400, 200), vec2(2, 2), 0))
local sprite = gPlayer:addComponent(Sprite("characters", 32, 32, 0, 0, 1, J2D_WHITE))
sprite:generateUVs(736, 128)

local circleCollider = gPlayer:addComponent(CircleCollider( 8, vec2(24, 24)))

local physicsAttr = PhysicsAttributes()
physicsAttr.eType = BodyType.Dynamic
physicsAttr.density = 100.0
physicsAttr.friction = 0.0
physicsAttr.restitution = 0.0
physicsAttr.position = transform.position 
physicsAttr.scale = transform.scale 
physicsAttr.offset = circleCollider.offset
physicsAttr.gravityScale = 0.0
physicsAttr.radius = circleCollider.radius 
physicsAttr.bCircle = true 
physicsAttr.bFixedRotation = true 
physicsAttr.objectData = ObjectData("G Player 1", "", false, false, true, gPlayer:id())



gPlayer:addComponent(PhysicsComp(physicsAttr))
----------------------------------------------------------------------------------------------
local gPlayer2 = Entity()
local transform2 = gPlayer2:addComponent(Transform(vec2(50, 300), vec2(2, 2), 0))
local sprite2 = gPlayer2:addComponent(Sprite("characters", 32, 32, 0, 0, 1, J2D_WHITE))
sprite2:generateUVs(736, 128)

local circleCollider2 = gPlayer2:addComponent(CircleCollider( 8, vec2(24, 24)))

local physicsAttr2 = PhysicsAttributes()
physicsAttr2.eType = BodyType.Dynamic
physicsAttr2.density = 1000.0
physicsAttr2.friction = 0.0
physicsAttr2.restitution = 1.0
physicsAttr2.gravityScale = 0.0
physicsAttr2.position = transform2.position 
physicsAttr2.scale = transform2.scale
physicsAttr2.offset = circleCollider2.offset
physicsAttr2.radius = circleCollider2.radius 
physicsAttr2.bCircle = true 
physicsAttr2.bFixedRotation = true 
physicsAttr2.objectData = ObjectData("G Player 2", "", false, false, true, gPlayer2:id())


gPlayer2:addComponent(PhysicsComp(physicsAttr2))
----------------------------------------------------------------------------------------------
-- Text information
	local fps_text = Entity()
	fps_text:addComponent(Transform(vec2( 550, 25), vec2(1, 1), 0.0))
	fps_text:addComponent(TextComponent( "pixel32", "FPS: " ))	
	local text = fps_text:getComponent(TextComponent)
	
	local showTilesText = Entity()
	showTilesText:addComponent(Transform(vec2( 550, 50), vec2(1, 1), 0.0))
	showTilesText:addComponent(TextComponent( "pixel32", "Show tiles: " ))	
	local hiddenTileText = showTilesText:getComponent(TextComponent)
	
	local fastDrawText = Entity()
	fastDrawText:addComponent(Transform(vec2( 550, 75), vec2(1, 1), 0.0))
	fastDrawText:addComponent(TextComponent( "pixel32", "Fast Draw: " ))
	local hiddenFastDrawText = fastDrawText:getComponent(TextComponent)
	
	local frameCount = 0
	local startTime = os.clock()
	local fps = 0
--------------------------------------------------------------------
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
-----------------------------------------------------------
function UpdateRayCast(ent)
	local physics = ent:getComponent(PhysicsComp)
	local transform = ent:getComponent(Transform)
	
	--[[
	local hitObject = physics:castRay(
		vec2(
			transform.position.x + 32, 
			transform.position.y + 64
		),
		vec2(
			transform.position.x + 200, 
			transform.position.y + 64
		)
	)
	
	DrawLine(
		Line(
			vec2(
				transform.position.x + 32, 
				transform.position.y + 64
			),
			vec2(
				transform.position.x + 200, 
				transform.position.y + 64
			),
			Color(0, 255, 0, 255)
		)
	)
	
	if hitObject then 
		print(hitObject.tag)
	end
	
	--]]
	
	local hitObjects = physics:boxTrace(
		vec2(
				transform.position.x + 32, 
				transform.position.y + 32
			),
		vec2(
			transform.position.x + 200, 
			transform.position.y + 64
		)
	)
	
	DrawRect(
		Rect(
			vec2(
					transform.position.x + 32, 
					transform.position.y + 32
			),
			vec2(168, 32),
			Color(0, 255, 0, 255),
			false
		)
	)
	
	for _, v in ipairs(hitObjects) do 
		print(v.tag)
	end	
end
---------------------------------------------------------------

function MakeSprites(num)
    local color = J2D_WHITE
    local scale = vec2(1, 1)
    for i = 1, num do
        for j = 1, num do
			local entity = Entity()
            local entTransform = entity:addComponent(Transform(vec2((i-1) * 32 * scale.x, (j-1) * 32 * scale.y), scale, 0))
			
			local sprite = entity:addComponent(Sprite("characters", 32, 32, 0, 0, 1, color))
            sprite:generateUVs(736, 128)
			
			local entCircleCollider = entity:addComponent(CircleCollider( 8, vec2((32*scale.x) / 2 - 4, (32*scale.y) /2 - 4)))
						
			local physicsAttr = PhysicsAttributes()
			physicsAttr.eType = BodyType.Dynamic
			physicsAttr.density = 100.0
			physicsAttr.friction = 0.0
			physicsAttr.restitution = 0.0
			physicsAttr.position = entTransform.position 
			physicsAttr.scale = entTransform.scale 
			physicsAttr.offset = entCircleCollider.offset
			physicsAttr.gravityScale = 0.0
			physicsAttr.radius = entCircleCollider.radius 
			physicsAttr.bCircle = true 
			physicsAttr.bFixedRotation = true 
			physicsAttr.objectData = ObjectData("Entity"..tostring((i+j)*num), "", false, false, true, entity:id())
			entity:addComponent(PhysicsComp(physicsAttr))
	   end
    end
    
end

MakeSprites(5)
------------------------------------------------------------------

fastDraw = true
showRaycast = false
showTiles = true

main = 
{
	update = function()
		UpdatePlayer(gPlayer)
		
		if showTiles then 
			if fastDraw then
				for x = 1, 100 do
					for y = 1, 100 do
						DrawRectFast( (x-1)*32, (y-1)*32, 32, 32, 0, 200, 0, 155, true)
					end
				end
			else
				for x = 1, 100 do
					for y = 1, 100 do
						DrawRect(Rect(
									vec2((x-1)*32, (y-1)*32),
									vec2(32, 32),
									Color(200, 0, 0, 155),
									true
									)
								)
					end
				end
			end
		end	
		
	--[[
			local ex = (3 % 100) * 0.2 + math.sin(J2D_GetTicks + i) * 0.5
			local ey = (3 / 100) * 0.2 + math.cos(J2D_GetTicks * 0.5 + i) * 0.5
		
		
		local entity3 = ent[3].getComponent(Transform)
		entity3.x = entity3.x + ex
		entity3.y = entity3.y + ey
		]]--
		
	
		local reg = Registry()
		local ents = reg:getEntities(Transform) -- This returns a runtime_view

		ents:for_each(
		  function(entity)
				
			if entity:id() ~= 1 then
		
				local physics = entity:getComponent(PhysicsComp)
				local transform = entity:getComponent(Transform)
				
				if physics.position.x > 800 then
					physics:setLinearVelocity(vec2(-1, 0))
				elseif physics.position.x < 0 then
					physics:setLinearVelocity(vec2(1 , 0))
				end
				
				if physics.position.y > 600 then
					physics:setLinearVelocity(vec2(0,-1 ))
				elseif physics.position.y < 0 then
					physics:setLinearVelocity(vec2(0, 1 ))
				else
					physics:setLinearVelocity(vec2(1, 1 ))
				end
		
			end
		end
		)
		
		updateFPS()
		
		if Keyboard.justPressed(KEY_SPACE) then 
			showRaycast = not showRaycast
		end		
		
		if Keyboard.justPressed(KEY_E) then
			fastDraw = not fastDraw
		end
		
		if Keyboard.justPressed(KEY_Q) then
			showTiles = not showTiles
		end
		
		if showRaycast then
			UpdateRayCast(gPlayer)
		end
		
		hiddenFastDrawText.sText = "Fast Draw: "..tostring(fastDraw)
		hiddenTileText.sText = "Show Tiles: "..tostring(showTiles)
		
	end
}

