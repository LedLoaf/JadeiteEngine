-- Player Class
Player = {}
Player.__index = Player

-- Create a player object
function Player:Create(params)
	params = params or {}
	
	local this =
	{
		name = params.name,
		entity = nil,
		debounce = Timer(),
		startPos = params.startPos or vec2(0, 0)	
	}
	
	local def = CharacterDefs[this.name]
	def.startPos = this.startPos
	this.entity = LoadEntity(def)
	assert(this.entity,"Failed to create entity: ["..this.name.."] does not exist.")
	
	setmetatable(this, self)
	return this
end

-- Player main update function
function Player:Update()
	self:UpdatePlayerContacts()
	self:UpdatePlayer()
	
end

function Player:UpdatePlayer()
	-- Get player components
	local physics 		= self.entity:getComponent(PhysicsComp)		
	local sprite 		= self.entity:getComponent(Sprite)
	local animation 	= self.entity:getComponent(Animation)
	local objectData 	= physics:objectData()
	local velocity 		= physics:getLinearVelocity()
	
	-- Climb Ladder
	if objectData.userData.bOnLadder then
		self:UpdateLadderClimb(ent)
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
function Player:UpdatePlayerContacts()
		local physics = self.entity:getComponent(PhysicsComp)
		local objectData = physics:objectData()
		
		if objectData.userData.bOnLadder and #objectData.contactEntities == 0 then
			objectData.userData.bOnLadder = false
			physics:setGravityScale(0.5)
			physics:setLinearVelocity(vec2(0,0))
		end
		
		-- Check if one of the contacts is a ladder
		for _, v in pairs(objectData.contactEntities) do
			if v.group == "ladder" and not objectData.userData.bOnLadder and Keyboard.justReleased(KEY_SPACE) then
				-- Turn off the gravity scale and velocity while on the ladder
				physics:setGravityScale(0.0)
				physics:setLinearVelocity(vec2(0,0))
				-- Center the player on the ladder
				self:CenterPlayer(Entity(v.entityID))
				-- We know we are on the ladder
				objectData.userData.bOnLadder = true
				objectData.userData.bInAir = false
				objectData.userData.airTimer:stop()
				return
			end
		end
end

-- Climbing the ladder
function Player:UpdateLadderClimb()

	local physics 	 = self.entity:getComponent(PhysicsComp)
	local sprite 	 = self.entity:getComponent(Sprite)
	local animation	 = self.entity:getComponent(Animation)
	local objectData = physics:objectData()
	local velocity	 = physics:getLinearVelocity()
	
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

-- Currently used to center the player on the ladder
function Player:CenterPlayer(otherEnt)
	local transform	 = self.entity:getComponent(Transform)
	local physics 	 = self.entity:getComponent(PhysicsComp)
	
	local otherTransform = otherEnt:getComponent(Transform)
	local boxCollider = otherEnt:getComponent(BoxCollider)
	
	if boxCollider then
		local circle = self.entity:getComponent(CircleCollider)
		physics:setTransform(
			vec2(
				otherTransform.position.x + (boxCollider.width / 2) + circle.offset.x - circle.radius + 1,
				transform.position.y + circle.offset.y + circle.radius
			)
		)
		
		return
	end
	-- TODO: Handle possible circle colliders
end