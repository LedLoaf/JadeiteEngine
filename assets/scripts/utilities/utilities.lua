--[[ Utilities Functions: 
	ShallowClone, 	DeepClone, 
	GetRandomColor, 
	TileObject, 	Tileset, 	TileMap,
	LoadTileMap, 	LoadMap, 	LoadLevel,
	LoadEntity,
	PrintError, PrintWarning, Print, PrintColorText helper
--]]

--==================================================================================================
-- Color map for PrintText (Regular Colors, Bright Colors, Backgrounds, Styles) Use 'dim' to get darker colors
local textColors = {
    reset   	= "\27[0m",
	-- Regular Colors
    red     		= "\27[31m",
    green   		= "\27[32m",
    yellow  		= "\27[33m",
    blue    		= "\27[34m",
    magenta 		= "\27[35m",
    cyan    		= "\27[36m",
    white   		= "\27[37m",
    gray    		= "\27[90m",
	-- Bright colors
	brightRed     	= "\27[91m",
    brightGreen   	= "\27[92m",
    brightYellow  	= "\27[93m",
    brightBlue    	= "\27[94m",
	bgMagenta   	= "\27[45m",
    brightCyan    	= "\27[95m",
    brightWhite   	= "\27[97m",
	-- Backgrounds
	bgRed     		= "\27[41m",
    bgGreen   		= "\27[42m",
    bgYellow  		= "\27[43m",
    bgBlue    		= "\27[44m",
    bgCyan    		= "\27[46m",
    bgWhite   		= "\27[47m",
    bgBlack   		= "\27[40m",
	-- Styles
	bold       		= "\27[1m",
	dim        		= "\27[2m",
	italic  		= "\27[3m",
    underline  		= "\27[4m",
}

-- Style map for PrintText's style argument
local textStyles = 
{
    bold       	= "\27[1m",
	dim        	= "\27[2m",
	italic  	= "\27[3m",
    underline  	= "\27[4m",
}

--[[ 	
	Print colored text
	- PrintText("Message", color, style, background)
	- Regular Colors (red, green, blue, yellow, blue, magenta, cyan, white, gray)
	- Adding bright before the color will give you the bright version (brightRed, brightGreen, etc.)
	- Style  (bold, underline, italic)
	- background (bgRed, bgGreen, bgYellow, bgBlue, bgCyan, bgWhite, bgBlack)
	e.g. PrintText("Colored Text", "red") or PrintText("Colored Text", "green", "bold", "bgRed")
	
	- You can also within the same text set the color, style, and set the background with %{color} %{style} %{background}
	e.g. PrintText("%{red}Colored Text") or PrintText("%{bold}%{red}Bold Text") 
--]]
function PrintColorText(msg, color, style, bg)
    local prefix = ""
	-- Set style
    if style and textStyles[style] then
        prefix = prefix .. textStyles[style]
    end
	-- Set background
	if bg and textColors[bg] then
        prefix = prefix .. textColors[bg]
    end
	
    if color and textColors[color] then
        prefix = prefix .. textColors[color]
    end
	
	-- Resolve any %{tag} inside the string
    local out = msg:gsub("%%{([%w_]+)}", function(name)
        return textColors[name] or ""
    end)
	
    print(prefix .. out .. textColors.reset)   
end

--==================================================================================================
-- Prints [LUA] ERROR: and the message
function PrintError(msg)
	PrintColorText("%{cyan}[LUA]%{reset} %{bgRed}%{bold}%{magenta}ERROR:%{reset}          %{red}" .. msg)
end

-- Prints [LUA] WARNING: and the message
function PrintWarning(msg)
	PrintColorText("%{cyan}[LUA]%{reset} %{bold}%{yellow}WARNING:          %{reset}%{cyan}" .. msg)
end

-- Prints [LUA] LOG: and the message
function Print(msg)
	PrintColorText("%{cyan}[LUA]%{reset} %{bold}%{green}LOG:          " .. msg)
end

--==================================================================================================

-- Retrieve a random color (Red, Green, Blue, Yellow, Magenta)
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

--==================================================================================================

-- Creates a new table with the same keys and values, but not a deep copy.
-- "Shallow" means: if a value is itself a table (a nested table), the clone only copies the reference to that nested table; not the nested table's contents. 
-- So modifying a nested table through the clone will also modify it in the original
-- NOTE: If you need nested tables to be independent too, you'd need a deep clone (recursive version).
function ShallowClone(tbl)
	local clone = {}
	for k, v in pairs(tbl) do 
		clone[k] = v 			-- copies each key and value into the new table
	end 
	
	return clone
end

--==================================================================================================

-- Creates a deep copy of a table, recursively cloning all nested values.
-- The resulting table is fully independent of the original.
-- NOTE: will not handle circular references
function DeepClone(tbl)
	local clone = {}
	for k, v in pairs(tbl) do 
		if type(v) == "table" then
		--  if it is a table, it recursively calls DeepClone on it, creating a brand-new copy of that nested table (and its nested tables, and so on).
			clone[k] = DeepClone(v)
		else
			clone[k] = v 	-- if it's a primitive (number, string, boolean, etc.), just copy it directly (same as shallow).
		end
	end 
	
	setmetatable(clone, getmetatable(tbl))	--  copies the original table's metatable onto the clone, so any custom behavior ( __index, __newindex, etc.) is preserved.
	return clone
end

--==================================================================================================

--  A factory object that creates tile instances from a parameter table. Uses the metatable pattern for OOP in Lua.
TileObject = {}
TileObject.__index = TileObject

function TileObject:Create(params)
	params = params or {}
	
	local this = 
	{
		name 		= params.name,
		type 		= params.type,
		shape 		= params.shape,
		offset 		= vec2(params.x, params.y) 	or vec2(0,0),
		width 		= params.width 				or 16,
		height 		= params.height 			or 16,
		rotation 	= params.rotations 			or 0
	}
	setmetatable(this,self)
	return this
end

--==================================================================================================

-- Tile Class: Represents a single tile type
Tile = {}
Tile.__index = Tile

function Tile:Create(params)
	params = params or {}
	
	local this = 
	{
		id 			= params.id,
		tileObjects = params.tileObjects or {}
		
	}
	
	setmetatable(this,self)
	return this
end

--==================================================================================================

-- Tileset Class: A collection of tiles sharing the same size and texture atlas
Tileset = {}
Tileset.__index = Tileset

function Tileset:Create(params)
	params = params or {}
	
	local this = 
	{
		name 		= params.name,
		firstGid 	= params.firstgid,
		tileWidth 	= params.tilewidth,
		tileHeight 	= params.tileheight,
		columns 	= params.columns,
		sTexture 	= params.image,
		imageWidth 	= params.imageWidth,
		imageHeight = params.imageHeight,
		tileCount 	= params.tilecount,
		tiles 		= params.tiles or {},
		lastGid 	= -1
	}
	
	this.lastGid = this.firstGid + this.tileCount - 1
	setmetatable(this,self)
	return this
end

function Tileset:ContainsID(id)
	return id >= self.firstGid and id <= self.lastGid
end

function Tileset:GetTileStartXY(id)
	assert(self:ContainsID(id), "Tile ID[" .. id .. "] does not exist in tileset [" .. self.name .."]")
	
	local actualID 	= id - self.firstGid
	local startX 	= math.floor(actualID % self.columns)
	local startY 	= math.floor(actualID / self.columns)
	
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

--==================================================================================================

-- TileMap Class: A 2D grid of tile references that defines a level's layout
Tilemap = {}
Tilemap.__index = Tilemap

function Tilemap:Create(params)
	params = params or {}
	
	local this = 
	{
		name 		= params.name,
		width 		= params.width,
		height 		= params.height,
		tilesets 	= params.tilesets,
		layers 		= params.layers,
		tileWidth 	= params.tilewidth,
		tileHeight 	= params.tileheight
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

--==================================================================================================

-- Parses a Tiled-exported Lua file and returns a TileMap
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
							name 		= w.name,
							type 		= w.type,
							shape 		= w.shape,
							x 			= w.x,
							y 			= w.y,
							width 		= w.width,
							height 		= w.height,
							rotation 	= w.rotation,
						}
					)
					
					table.insert(objects, object)
				end
			end
			
			local tile = Tile:Create(
				{
					id 			= j.id,
					tileObjects = objects
				}
			)
			
			table.insert(mapTiles, tile)
		end
		
		-- Creates a tileset
		local tileset = Tileset:Create(
			{
				name 		= v.name,
				firstgid 	= v.firstgid,
				tilewidth	= v.tilewidth,
				tileheight 	= v.tileheight,
				columns 	= v.columns,
				image 		= v.image,
				imageWidth 	= v.imagewidth,
				imageHeight = v.imageheight,
				tilecount 	= v.tilecount,
				tiles 		= mapTiles				
			}
		)
		
		table.insert(mapTilesets, tileset)
	end
	
	-- Creates the tilemap
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

--==================================================================================================

-- Reads Tiled custom properties (Object Data) from tile objects and applies them to the map
function AddTileObjectDataProps(physAttr, type, tile)

	if type == "pass-through" then
		physAttr.objectData = ObjectData(
			{
				group 		= type,
				bTrigger 	= true,
				bCollider 	= true,
				entityID 	= tile:id()
			}
		)
		physAttr.objectData:setOnPreSolve(
			function(objectData)
				if objectData.tag == "player" then
				
					local player 	= Entity(objectData.entityID)
					local userData 	= objectData.userData
					local physics 	= player:getComponent(PhysicsComp)
					
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
		
		--print("Created pass-through physics object")
		
	elseif type == "ladder" then
		physAttr.bIsSensor 	= true
		physAttr.objectData = ObjectData(
				{
					group 		= type,
					bTrigger 	= true,
					entityID 	= tile:id()
				}
			)
			--print("Created Ladder physics object")
	end
	--TODO HANDLE OTHER TYPES AS NEEDED
end


--==================================================================================================

-- Loads a map file, builds the TileMap, and registers it with the current level
function LoadMap(map)
	local numTiles = 0 
	
	for k, v in ipairs(map.layers) do 
		local rows 	= v.height - 1 
		local cols 	= v.width 
		local layer = k - 1
		
		for row = 0, rows do 
			for col = 1, cols do 
				local id = v.data[row * cols + col]
				
				if id == 0 then 
					goto continue
				end
				
				local tileset = map:GetTilesetTileID(id) 
				assert(tileset, "Tileset does not exist with ID: " .. id)
				
				-- Retrieve the transform component of the tile entity
				local tileEnt = Entity()
				local transform = tileEnt:addComponent(
					Transform(
						vec2((col - 1) * tileset.tileWidth, row * tileset.tileHeight),
						vec2(1, 1),
						0
					)
				)
				
				local objectData = tileset:GetObjectsFromID(id)
				
				-- If the objectData is present
				if objectData then 
				-- Add a box collider, if there is a rect object data
					if objectData.shape == "rectangle" then
						if objectData.type == "pass-through" then	
							tileEnt:addComponent(
								BoxCollider(
									objectData.width,
									objectData.height,
									objectData.offset,
									Color(255, 255, 255, 133)
								)
							)
						else
							-- Add a box collider to the tile entity
							tileEnt:addComponent(
								BoxCollider(
									objectData.width,
									objectData.height,
									objectData.offset,
									Color(255, 0, 0, 133)
								)
							)
						end
						
							local physAttr = PhysicsAttributes(
								{
									eType 		= BodyType.Static,
									density 	= 1000,
									friction 	= 0,
									restitution = 0,
									position 	= {
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
				
				-- Get the starting x and y
				local startX, startY = tileset:GetTileStartXY(id)
				
				-- Add a sprite component to the tile entity
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
	
	--print("Num Tiles: "..numTiles)
end

--==================================================================================================

-- Spawns game entities (enemies, pickups, triggers) from Tiled object layers
function LoadEntity(def)
	assert(def, "Entity Def is not valid")
	
	local newEntity = Entity()
	
	-- Add the transform component to the entity
	local transform = newEntity:addComponent(
		Transform(
			def.startPos 	or vec2(0, 0),
			def.scale 		or vec2(1, 1),
			def.rotation 	or 0.0
		)
	)
	
	if def.components then
		-- Add a sprite component to the entity
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
			
			-- Retrieve the texture for the entity
			local texture = AssetManager.getTexture(sprite.sTexture)
			assert(texture, "Failed to generated UVs. ["..sprite.sTexture.."] is not valid")
			-- Set up the UVs of the texture
			sprite:generateUVs(texture.width, texture.height)
		end
		
		-- Add a Animation component
		if def.components.animation then
			newEntity:addComponent(
				Animation(
					def.components.animation.numFrames 	or 1,
					def.components.animation.frameRate 	or 1,
					def.components.animation.bVertical 	or false,
					def.components.animation.bLooped 	or false	
				)
			)
		end
		
		-- Add a BoxCollider component
		if def.components.boxCollider then
			newEntity:addComponent(
				BoxCollider(
					def.components.boxCollider.width,
					def.components.boxCollider.height,
					def.components.boxCollider.offset or vec2(0, 0),
					def.components.boxCollider.color
				)
			)
		end
		
		-- Add a CircleCollider component
		if def.components.circleCollider then
			newEntity:addComponent(
				CircleCollider(
					def.components.circleCollider.radius,
					def.components.circleCollider.offset or vec2(0, 0),
					def.components.circleCollider.color
				)
			)
		end
		
		-- Add a physics component
		if def.components.physics then
			local physAttr 			= def.components.physics
			-- Set the physic attributes up for the physics component
			local newPhysicsAttr 	= PhysicsAttributes()		
			
			newPhysicsAttr.eType 		= physAttr.type 		or BodyType.Static
			newPhysicsAttr.density 		= physAttr.density 		or 100
			newPhysicsAttr.friction 	= physAttr.friction 	or 0
			newPhysicsAttr.restitution 	= physAttr.restitution 	or 0
			newPhysicsAttr.position 	= transform.position 	or vec2(0, 0)
			newPhysicsAttr.scale 		= transform.scale 		or vec2(1, 1)
			newPhysicsAttr.radius 		= physAttr.radius 		or 0.0
			newPhysicsAttr.gravityScale = physAttr.gravityScale or 1
			newPhysicsAttr.damping 		= physAttr.damping 		or 0
			
			newPhysicsAttr.bIsSensor 		= physAttr.bIsSensor 		== nil and false or physAttr.bIsSensor
			newPhysicsAttr.bCircle 			= physAttr.bCircle 			== nil and false or physAttr.bCircle
			newPhysicsAttr.bFixedRotation 	= physAttr.bFixedRotation 	== nil and false or physAttr.bFixedRotation
			
			-- This fixed the issue with the coins not always responding to being collided with
			if physAttr.boxSize then
				newPhysicsAttr.boxSize = physAttr.boxSize
			end
			
			if physAttr.radius > 0 then
				newPhysicsAttr.radius = physAttr.radius
			end
			
			if physAttr.objectData then
				newPhysicsAttr.objectData = 
					ObjectData(
						physAttr.objectData.tag 		or "",
						physAttr.objectData.group 		or "",
						physAttr.objectData.bCollider 	or false,
						physAttr.objectData.bTrigger 	or false,
						physAttr.objectData.bFriendly 	or false,
						newEntity:id()
					)
					
				if def.userData then
					newPhysicsAttr.objectData.userData = def.userData
					--print("Added custom user data for ID("..newEntity:id()..")")
				end
			end
			-- Finally add the physics component
			newEntity:addComponent(PhysicsComp(newPhysicsAttr))
		end
	end
	
	return newEntity
end

--==================================================================================================
local LevelHandler = require("defs.levelDefs")

-- Loads the levels located in levelDefs.lua
function LoadLevel(lvl)
    local levelDef = LevelHandler:GetLevelDef(lvl)
	
    if not levelDef then
		PrintError("Failed to load level. Level is invalid or does not exist.")
        return
    end
    
    local levelMap = require(levelDef)
    if not levelMap then
		PrintError("Failed to load level. Level definition is not a valid lua path.")
        return
    end
	
    local tiledMap = LoadTiledMap(levelMap)
    LoadMap(tiledMap)
end
--==================================================================================================


-- Make an enum READ-ONLY helper function
local Enum = {}

function Enum.ReadOnly(name, data)
    return setmetatable({}, {
        __index = data,
        __newindex = function(_, key)
            error("ERROR Cannot modify enum '" .. name .. "': " .. key .. " is read-only", 2)
        end,
    })
end

-- return Enum

-- An example of making an Read-Only enum in lua using this function. Currently skipped for simplicity
--[[
local Enum = require("utilities.utilities")		-- for read-only enum

-- Type of pickup Enum
local PickupType = Enum.ReadOnly("PickupType", 
{
	Health 	= 1,
	Ammo 	= 2,
	Coin 	= 3,
	-- TODO: Add more types as needed
})
--]]
--==================================================================================================
