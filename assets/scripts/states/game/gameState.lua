-- GameState -- State that all gameplay logic should run from
GameState = setmetatable({}, { __index = State })
GameState.__index = GameState

function GameState:Create(params)
	local this = State.Create(self)
	
	this.stackRef = params.stackRef
	
	--local tilemap = LoadTiledMap(CreateLevel1Map())
	--LoadMap(tilemap)
	LoadLevel(2)
	
	local cam = Camera.get()
	this.player = Player:Create({ name = "player", startPos = vec2(16 * 1, 16 * 24)})
	
	this.followCamera = FollowCamera:Create(
	cam,
		{
			scale = 4.0,
			minX = 0.0,
			minY = 0.0,
			maxX = 16 * 40,
			maxY = 16 * 30,
			springback = 0.07
		}
	)

	cam.setPosition(vec2(0, this.player.startPos.y - (cam.getHeight() / cam.getScale()) * 0.5))
	this.player:SetFollowCamera(this.followCamera)
	
	this.hud = Hud:Create()
	
	gEventManager:AddListener( "CoinCollectEvent", function(event) this.hud:OnCoinsChanged(event.amount) end )
	gEventManager:AddListener( "HealthChangeEvent", function(event) this.hud:OnHealthChanged(event.amount) end )
	
	function CreatePickup(pos)
		return Pickup:Create({ name = "coin", amount = 5, spawnPos = pos } )
	end
	
	this.testPickups = {}
	table.insert(this.testPickups, CreatePickup(vec2(16 * 3, 16 * 24)))
	table.insert(this.testPickups, CreatePickup(vec2(16 * 8, 16 * 20)))
	
	return this
end

function GameState:Enter(params)
	print("Entered Game State")
end

function GameState:Exit()
	print("Exited Game State")
end

function GameState:Update(dt)
	self.player:Update()
end
