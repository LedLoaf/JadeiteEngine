-- Utilities

function GetRandomColor()
	local val = math.random(5)
	if val == 1 then 
		return J2D_RED 
	elseif val == 2 then 
		return J2D_GREEN
	elseif val == 3 then 
		return J2D_BLUE
	elseif val == 4 then 
		return J2D_YELLOW
	elseif val == 5 then 
		return J2D_MAGENTA
	end
	
	return J2D_GREEN
end

function ShallowClone(tbl)
	local clone = {}
	for k, v in pairs(tbl) do 
		clone[k] = v 
	end 
	
	return clone
end

function DeepClone(tbl)
	local clone = {}
	for k, v in pairs(tbl) do 
		if type(v) == "table" then
			clone[k] = DeepClone(v)
		else
			clone[k] = v 
		end
	end 
	
	setmetatable(clone, getmetatable(tbl))
	return clone
end

-- ===========================================
-- Tile Utilities
-- ==========================================
TileObject = {}
TileObject.__index = TileObject

function TileObject:Create(params)
	params = params or {}
	
	local this = 
	{
		name = params.name,
		type = params.type,
		shape = params.shape,
		offset = vec2(params.x, params.y) or vec2(0,0),
		width = params.width or 16,
		height = params.height or 16,
		rotation = params.rotations or 0
	}
	setmetatable(this,self)
	return this
end
------------------------------------------------------------
Tile = {}
Tile.__index = Tile

function Tile:Create(params)
	params = params or {}
	
	local this = 
	{
		id = params.id,
		tileObjects = params.tileObjects or {}
		
	}
	
	setmetatable(this,self)
	return this
end
------------------------------------------------------------
Tileset = {}
Tileset.__index = Tileset

function Tileset:Create(params)
	params = params or {}
	
	local this = 
	{
		name = params.name,
		firstGid = params.firstgid,
		tileWidth = params.tilewidth,
		tileHeight = params.tileheight,
		columns = params.columns,
		sTexture = params.image,
		imageWidth = params.imageWidth,
		imageHeight = params.imageHeight,
		tileCount = params.tilecount,
		tiles = params.tiles or {},
		lastGid = -1
	}
	
	this.lastGid = this.firstGid + this.tileCount - 1
	setmetatable(this,self)
	return this
end

function Tileset:ContainsID(id)
	return id >= self.firstGid and id <= self.lastGid
end

function Tileset:GetTileStartXY(id)
	assert(self:ContainsID(id), "Tile ID["..id.."] does not exist in tileset ["..self.name.."]")
	
	local actualID = id - self.firstGid
	local startX = math.floor(actualID % self.columns)
	local startY = math.floor(actualID / self.columns)
	
	return startX, startY
end

function Tileset:HasTiles()
	return #self.tiles > 0
end

function Tileset:GetObjectsFromID(id)
	local actualID = id - self.firstGid
	for k, v in ipairs(self.tiles) do
		if v.id == actualID then
			return v.tileObjects[1]
		end
	end
	
	return nil
end	
------------------------------------------------------------
Tilemap = {}
Tilemap.__index = Tilemap

function Tilemap:Create(params)
	params = params or {}
	
	local this = 
	{
		name = params.name,
		width = params.width,
		height = params.height,
		tilesets = params.tilesets,
		layers = params.layers,
		tileWidth = params.tilewidth,
		tileHeight = params.tileheight
	}
	
	setmetatable(this,self)
	return this
end

function Tilemap:GetTilesetTileID(id)
	for k, v in ipairs(self.tilesets) do
		if v:ContainsID(id) then
			return v
		end
	end
	
	return nil
end
------------------------------------------------------------
function LoadTiledMap(map)
	local mapTilesets = {}
	
	for k, v in ipairs(map.tilesets) do
		local mapTiles = {}
		
		for r, j in ipairs(v.tiles) do 
			local objects = {}
			if j.objectGroup and j.objectGroup.objects then 
				-- Create the TileObjects
				for z, w in ipairs(j.objectGroup.objects) do 
					local object = TileObject:Create(
						{
							name = w.name,
							type = w.type,
							shape = w.shape,
							x = w.x,
							y = w.y,
							width = w.width,
							height = w.height,
							rotation = w.rotation,
						}
					)
					
					table.insert(objects, object)
				end
			end
			
			local tile = Tile:Create(
				{
					id = j.id,
					tileObjects = objects
				}
			)
			
			table.insert(mapTiles, tile)
		end
		
		local tileset = Tileset:Create(
			{
				name = v.name,
				firstgid = v.firstgid,
				tilewidth = v.tilewidth,
				tileheight = v.tileheight,
				columns = v.columns,
				image = v.image,
				imageWidth = v.imagewidth,
				imageHeight = v.imageheight,
				tilecount = v.tilecount,
				tiles = mapTiles				
			}
		)
		
		table.insert(mapTilesets, tileset)
	end
	
	local tilemap = Tilemap:Create(
		{
			name = "tilemap",
			width = map.width,
			height = map.height,
			tilesets = mapTilesets,
			tilewidth = map.tilewidth,
			tileheight = map.tileheight,
			layers = map.layers
		}
	)
	
	return tilemap
end

-- Handle all tile classes types
function AddTileObjectDataProps(physAttr, type, tile)
	if type == "passthrough" then
		physAttr.objectData = ObjectData(
			{
				group = type,
				bTrigger = true,
				bCollider = true,
				entityID = tile:id()
			}
		)
		physAttr.objectData:setOnPreSolve(
			function(objectData)
				if objectData.tag == "player" then
					local player = Entity(objectData.entityID)
					local userData = objectData.userData
					local physics = player:getComponent(PhysicsComp)
					if physics then
						local velocity = physics:getLinearVelocity()
						if velocity.y < 0 or userData.bOnLadder then
							return false
						end
					end
				end
				
				return true
			end
		)
		
		print("Created passthrough physics object")
	elseif type == "ladder" then
		physAttr.bIsSensor = true
		physAttr.objectData = ObjectData(
				{
					group = type,
					bTrigger = true,
					entityID = tile:id()
				}
			)
	end
	--TODO HANDLE OTHER TYPES AS NEEDED
end


------------------------------------------------------------
function LoadMap(map)
	local numTiles = 0 
	
	for k, v in ipairs(map.layers) do 
		local rows = v.height - 1 
		local cols = v.width 
		local layer = k - 1
		
		for row = 0, rows do 
			for col = 1, cols do 
				local id = v.data[row * cols + col]
				
				if id == 0 then 
					goto continue
				end
				
				local tileset = map:GetTilesetTileID(id) 
				assert(tileset, "Tileset does not exist with ID: " ..id)
				
				local tileEnt = Entity()
				local transform = tileEnt:addComponent(
					Transform(
						vec2((col - 1) * tileset.tileWidth, row * tileset.tileHeight),
						vec2(1, 1),
						0
					)
				)
				
				local objectData = tileset:GetObjectsFromID(id)
				if objectData then 
				-- Add a box collider, if there is a rect object data
					if objectData.shape == "rectangle" then
						tileEnt:addComponent(
							BoxCollider(
								objectData.width,
								objectData.height,
								objectData.offset
							)
						)
						
							local physAttr = PhysicsAttributes(
								{
									eType = BodyType.Static,
									density = 1000,
									friction = 0,
									restitution = 0,
									position = {
										x = transform.position.x + objectData.offset.x,
										y = transform.position.y + objectData.offset.y,
									},
									boxSize = {x = objectData.width, y = objectData.height }
								}
							)
							
							AddTileObjectDataProps(physAttr, objectData.type, tileEnt)
							
							tileEnt:addComponent(PhysicsComp(physAttr))
					end
				end
				
				local startX, startY = tileset:GetTileStartXY(id)
				
				local sprite = tileEnt:addComponent(
					Sprite(
						tileset.name,
						tileset.tileWidth,
						tileset.tileHeight,
						layer,
						startX,
						startY,
						Color(255, 255, 255, 255)
					)
				)
				
				sprite:generateUVs(tileset.imageWidth, tileset.imageHeight)
				
				numTiles = numTiles + 1
				::continue::
			end
			
		end
	end
	
	print("Num Tiles: "..numTiles)
end

-- ==========================================
-- Character Utilities
-- ==========================================

function LoadEntity(def)
	assert(def, "Entity Def is not valid")
	
	local newEntity = Entity()
	
	local transform = newEntity:addComponent(
		Transform(
			def.startPos or vec2(0,0),
			def.scale or vec2(1,1),
			def.rotation or 0.0
		)
	)
	
	if def.components then
		if def.components.sprite then
			local sprite = newEntity:addComponent(
				Sprite(
						def.components.sprite.texture,
						def.components.sprite.width,
						def.components.sprite.height,
						def.components.sprite.layer,
						def.components.sprite.startX,
						def.components.sprite.startY,
						def.components.sprite.color or J2D_WHITE
					)
			)
			
			local texture = AssetManager.getTexture(sprite.sTexture)
			assert(texture, "Failed to generated UVs. ["..sprite.sTexture.."] is not valid")
			
			sprite:generateUVs(texture.width, texture.height)
		end
		
		if def.components.animation then
			newEntity:addComponent(
				Animation(
					def.components.animation.numFrames or 1,
					def.components.animation.frameRate or 1,
					def.components.animation.frameOffset or 0,
					def.components.animation.bVertical or false,
					def.components.animation.bLooped or false	
				)
			)
		end
		
		if def.components.boxCollider then
			newEntity:addComponent(
				BoxCollider(
					def.components.boxCollider.width,
					def.components.boxCollider.height,
					def.components.boxCollider.offset or vec2(0,0)
				)
			)
		end
		
		if def.components.circleCollider then
			newEntity:addComponent(
				CircleCollider(
					def.components.circleCollider.radius,
					def.components.circleCollider.offset or vec2(0,0)
				)
			)
		end
		
		if def.components.physics then
			local physAttr = def.components.physics
			local newPhysicsAttr = PhysicsAttributes()
			newPhysicsAttr.eType = physAttr.type or BodyType.Static
			newPhysicsAttr.density = physAttr.density or 100
			newPhysicsAttr.friction = physAttr.friction or 0
			newPhysicsAttr.restitution = physAttr.restitution or 0
			newPhysicsAttr.position = transform.position or vec2(0,0)
			newPhysicsAttr.scale = transform.scale or vec2(1,1)
			newPhysicsAttr.radius = physAttr.radius or 0.0
			newPhysicsAttr.gravityScale = physAttr.gravityScale or 1
			
			newPhysicsAttr.bIsSensor = physAttr.bIsSensor == nil and false or physAttr.bIsSensor
			newPhysicsAttr.bCircle = physAttr.bCircle == nil and false or physAttr.bCircle
			newPhysicsAttr.bFixedRotation = physAttr.bFixedRotation == nil and false or physAttr.bFixedRotation
			
			if physAttr.objectData then
				newPhysicsAttr.objectData = 
					ObjectData(
						physAttr.objectData.tag or "",
						physAttr.objectData.group or "",
						physAttr.objectData.bCollider or false,
						physAttr.objectData.bTrigger or false,
						physAttr.objectData.bFriendly or false,
						newEntity:id()
					)
			end
			
			newEntity:addComponent(PhysicsComp(newPhysicsAttr))
		end
	end
	
	return newEntity
end

function LoadLevel(num)
	local level = {}
	
	if num == 1 then
		level = dofile("assets/scripts/defs/maps/level1.lua")
	elseif num == 2 then
		level = dofile("assets/scripts/defs/maps/level2.lua")
	end
	
	assert(level,"Level-"..num.." could not be found...")
	
	local tiledMap =  LoadTiledMap(level)
	LoadMap(tiledMap)
end
