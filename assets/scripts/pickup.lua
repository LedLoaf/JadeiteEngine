-- Pickup Base Class

Pickup = {}
Pickup.__index = Pickup

function Pickup:Create(params)

	local this = 
	{
		name 			= params.name, 						-- used to look up the definition from our PickupDefs
		type 			= nil,								-- nil because it's derived from the definition
		amount 			= params.amount, 					-- amount applied when collected
		spawnPos 		= params.spawnPos or vec2(0, 0),	-- the spawn position of the coin
		pickupSound 	= nil,								-- sound that plays on pickup			
		pickupAnimation = nil,								-- the animation to play on pickup
		bCollected 		= false,							-- if it has already been collected
		bPickedUp 		= false,							-- a simple flag on if picked up or not
		entity 			= nil								-- direct access to the components and such
	}	

	local def 		= PickupDefs[this.name]
	def.startPos 	= this.spawnPos
	
	assert(def, "Pickup definition is not valid or does not exist.")
	
	-- Sets the type
	this.type 	= def.type
	-- Retrieves and loads the entity
	this.entity = LoadEntity(def)
	
	-- Check if there is a pickup animation
	if def.pickupAnimation then
		this.pickupAnimation = def.pickupAnimation
	end
	
	-- Check if there is a sound
	if def.pickupSound then
		this.pickupSound = def.pickupSound
	end
	
	setmetatable(this, self)
	
	-- When a collision happens we need to be able to access the data
	local physics = this.entity:getComponent(PhysicsComp)
	if physics then
		local objectData = physics:objectData()
		objectData.userData = this
		-- Disables physical collision
		objectData:setOnPreSolve(function(objData) return false end )
		physics:setObjectData(objectData)
	end
	
	return this
end

function Pickup:ApplyPickupBaseOnType(collectEnt)
	if self.type == PickupType.Health then
		-- TODO: Increase Health on the collecting entity
	elseif self.type == PickupType.Ammo then
		-- TODO: Increase Ammo on the collecting entity
	elseif self.type == PickupType.Coin then
		print("Picked up " .. self.amount .. " coins")
	end
end

function Pickup:Collect(collectingEnt)
	-- Leave if already collected
	if self.bCollected then
		return
	end
	
	-- Override current animation with pickup animation
	if self.pickupAnimation then
		local animation 	= self.entity:getComponent(Animation)
		animation.numFrames = self.pickupAnimation.numFrames
		animation.frameRate = self.pickupAnimation.frameRate
		animation.bVertical = self.pickupAnimation.bVertical
		animation.bLooped 	= self.pickupAnimation.bLooped
		
		local sprite 		= self.entity:getComponent(Sprite)
		sprite.startX 		= self.pickupAnimation.frameOffset or 0
		
		animation:reset()
		print("Pickup animation changed");	-- debug purpose
	end
	
	self:ApplyPickupBaseOnType(collectingEnt)
	
	-- If there is a pickup sound play it
	if self.pickupSound then 
		SoundPlayer.play(self.pickupSound)
	end
	
	-- Flag the pickup as collected
	self.bCollected = true
end

-- Coroute that waits for the pickup animation to finish before destroying the entity. 
-- If it doesn't have the new pickup animation then it will automatically destroy it and the coroutine will be finished
function Pickup:UpdateDestroy()
	-- Retrieve animation components
	local animation = self.entity:getComponent(Animation)
	-- Wait until animation reaches final frame (not looped animation)
	if self.pickupAnimation then
		while(animation.currentFrame ~= animation.numFrames - 1) do
			-- If it's already destroyed ELSE WHERE just exit loop because it has been destroyed
			if self.bPickedup then
				break
			end
			
			print("Picking up item")
			-- Yields the execution of this function to the next frame
			coroutine.yield()	
		end
	end
	
	-- Ensure we aren't coming back in the function to destroy entity again and eventually the class will be garbage collecting
	if not self.bPickedUp then
		self.entity:destroy()
		self.entity = nil
		self.bPickedUp = true
		print("Item has been picked up successfully")
	end
end