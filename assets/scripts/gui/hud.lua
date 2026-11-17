-- Hud class
Hud = {}
Hud.__index = Hud

function Hud:Create(params)
	local this = 
	{
		maxHearts = 3,
		maxCoins = 999,
		maxHealth = 6,
		currentCoins = 0,
		hearts = {}
	}
	
	this.currentHealth = this.maxHealth
	
	this.coinImage = UIImage:Create(
		{
			position = vec2(800 - 96 * 2, 0),
			imageInfo = 
			{
				sprite = Sprite("coin", 16, 16, 2, 0, 0, Color(255, 255, 255, 255)),
				scale = vec2(4, 4)
			},
			animationInfo = 
			{
				animation = Animation(7, 4, false, true)
			}
		}
	)
	
	this.coinLabel = Label:Create(
		{
			position = vec2(800 - 60 * 2, 32 * 1.5),
			textInfo = 
			{
				text = TextComponent("pixel32", string.format("%d", this.currentCoins )),
				scale = vec2(2, 2)
			}
		}
	)
	
	this.coinHolder = UIImage:Create(
		{
			position = vec2(800 - 96 * 2, 0),
			imageInfo = 
			{
				sprite = Sprite("ui", 96, 32, 1, 0, 0, Color(255, 255, 255, 255)),
				scale = vec2(2, 2)
			}
		}
	)
	
	this.heartHolder = UIImage:Create(
		{
			position = vec2(0, 0),
			imageInfo = 
			{
				sprite = Sprite("ui", 96, 32, 1, 0, 1, Color(255, 255, 255, 255)),
				scale = vec2(2, 2)
			}
		}
	)
	
	this.heart1 = UIImage:Create(
		{
			position = vec2(28, 16),
			imageInfo = 
			{
				sprite = Sprite("hearts", 36, 32, 2, 0, 0, Color(255, 255, 255, 255)),
				scale = vec2(1, 1)
			}
		}
	)
	
	this.heart2 = UIImage:Create(
		{
			position = vec2(79, 16),
			imageInfo = 
			{
				sprite = Sprite("hearts", 36, 32, 2, 0, 0, Color(255, 255, 255, 255)),
				scale = vec2(1, 1)
			}
		}
	)
	
	this.heart3 = UIImage:Create(
		{
			position = vec2(129, 16),
			imageInfo = 
			{
				sprite = Sprite("hearts", 36, 32, 2, 0, 0, Color(255, 255, 255, 255)),
				scale = vec2(1, 1)
			}
		}
	)
	
	table.insert(this.hearts, this.heart1)
	table.insert(this.hearts, this.heart2)
	table.insert(this.hearts, this.heart3)
	
	setmetatable(this, self)
	 
	return this
end

function Hud:OnCoinsChanged(amount)
	
	self.currentCoins = J2D_clamp(self.currentCoins + amount, 0, 999)
	
	if self.coinLabel then
		self.coinLabel:ChangeText(string.format("%d", self.currentCoins))
	end	
end

function Hud:OnHealthChanged(amount)
	self.currentHealth = J2D_clamp(self.currentHealth + amount, 0, self.maxHealth)
	
	for i = 1, #self.hearts do
		local heart = self.hearts[i]
		local sprite = heart.imageEntity:getComponent(Sprite)
		local heartIndex = i
		
		if self.currentHealth <= 0 then
			sprite.startX = 2
		elseif self.currentHealth >= heartIndex * 2 then
			sprite.startX = 0
		elseif self.currentHealth == heartIndex * 2 - 1 then
			sprite.startX = 1
		elseif self.currentHealth == heartIndex * 2 - 2 then
			sprite.startX = 2
		end
		
		sprite:inspectX()
	end	
end