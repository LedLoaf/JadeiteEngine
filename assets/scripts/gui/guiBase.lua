-- GuiBase Class
GuiBase = {}

function GuiBase:Create(params)
	local this = 
	{
		type = params.type or UIObjectType.None,
		
		baseColor = params.baseColor or Color(255, 255, 255, 255),
		hoveredColor = params.hoveredColor or Color(255, 255, 255, 255),
		activeColor = params.activeColor or Color(255, 255, 255, 255),
		bHovered = false,
		bActive = false,
		
		textInfo = params.textInfo or nil,
		imageInfo = params.imageInfo or nil,
		
		onPressed = params.onPressed or nil,
		
		bHidden = false,
		imageEntity = nil,
		textEntity = nil,
	}
	
	if this.imageInfo then
		assert(this.imageInfo.sprite, 
			"All gui using images must have a sprite components as part of the imageInfo")
			
		this.imageEntity = Entity()
		this.imageEntity:addComponent(UIComponent(this.type))
		this.imageEntity:addComponent(
			Transform(
				(params.position or vec2(0, 0)) + (this.imageInfo.offset or vec2(0, 0)),
				this.imageInfo.scale or vec2(1, 1),
				this.imageInfo.rotation or 0.0
			)
		)
		
		local imageSprite = this.imageEntity:addComponent(this.imageInfo.sprite)
		local texture = AssetManager.getTexture(imageSprite.sTexture)
		assert(texture, "failed to get texture [" ..imageSprite.sTexture .."]")
		imageSprite:generateUVs(texture.width, texture.height)
	end
	
	if this.textInfo then
		assert(this.textInfo.text, 
			"All gui using text must have a text component as part of the textInfo")
			
		this.textEntity = Entity()
		this.textEntity:addComponent(UIComponent(this.type))
		this.textEntity:addComponent(
			Transform(
				(params.position or vec2(0, 0)) + (this.textInfo.offset or vec2(0, 0)),
				this.textInfo.scale or vec2(1, 1),
				this.textInfo.rotation or 0.0
			)
		)
		
		this.textEntity:addComponent(this.textInfo.text)
	end
	
	setmetatable(this, self)
	self.__index = self
	return this
end

function GuiBase:IsHovered()

	if self.imageEntity then
		print("Gots an image entity")
		local transform = self.imageEntity:getComponent(Transform)
		local sprite = self.imageEntity:getComponent(Sprite)
		
		local mousePos = Mouse.screenPosition()
		local width = sprite.width * transform.scale.x
		local height = sprite.height * transform.scale.y
		
		return mousePos.x > transform.position.x and
				mousePos.x < transform.position.x + width and
				mousePos.y > transform.position.y and
				mousePos.y < transform.position.y + height
	end
	
	-- Text Hovering -- TODO:
	if self.textEntity then 
		-- TODO: Handle text Hovering
	end 
	
	return false
end

function GuiBase:SetHovered(hovered)
	if self.imageEntity then 
		local sprite = self.imageEntity:getComponent(Sprite)
		self.bHovered = hovered
		if self.bHovered then
			sprite.color = self.hoveredColor
		else
			sprite.color = self.baseColor
		end
	end
	
	-- TODO: Do we want to change text color as well?
end

function GuiBase:Update()
	self:SetHovered(self:IsHovered())
	self:OnPressed()
end

function GuiBase:OnPressed()
	if not self.onPressed then 
		return 
	end
	
	if self.bHovered and Mouse.justReleased(LEFT_BTN) then 
		self.onPressed()
	end
end

function GuiBase:Hide()
	if self.imageEntity then 
		self.imageEntity:getComponent(Sprite).bHidden = true
	end
	
	if self.textEntity then 
		self.textEntity:getComponent(TextComponent).bHidden = true
	end
end

function GuiBase:Show()
	if self.imageEntity then 
		self.imageEntity:getComponent(Sprite).bHidden = false
	end
	
	if self.textEntity then 
		self.textEntity:getComponent(TextComponent).bHidden = false
	end
end

-- =====================================================
-- Label Class 
-- =====================================================
Label = setmetatable({}, { __index = GuiBase })
Label.__index = Label

function Label:Create(params)
	local this = GuiBase:Create(
		{
			type = UIObjectType.Label,
			position = params.position or vec2(0, 0),
			textInfo = params.textInfo,
			
			-- State Variables 
			baseColor = params.baseColor or Color(255, 255, 255, 255),
			hoveredColor = params.hoveredColor or Color(255, 255, 255, 255),
			activeColor = params.activeColor or Color(255, 255, 255, 255),
			onPressed = params.onPressed or nil,
		}
	)
	
	setmetatable(this, self)
	return this
end

function Label:ChangeText(text)
	assert(type(text) == "string", "ChangeText must be a valid string.")
	local textComp = self.textEntity:getComponent(TextComponent)
	textComp.sText = text
end

-- =====================================================
-- UIImage Class 
-- =====================================================
UIImage = setmetatable({}, { __index = GuiBase })
UIImage.__index = UIImage

function UIImage:Create(params)
	local this = GuiBase:Create(
		{
			type = UIObjectType.Image,
			position = params.position or vec2(0, 0),
			imageInfo = params.imageInfo,
			
			-- State Variables 
			baseColor = params.baseColor or Color(255, 255, 255, 255),
			hoveredColor = params.hoveredColor or Color(255, 255, 255, 255),
			activeColor = params.activeColor or Color(255, 255, 255, 255),
			onPressed = params.onPressed or nil,
		}
	)
	
	this.animationInfo = params.animationInfo or nil
	
	if this.animationInfo then
		this.imageEntity:addComponent(this.animationInfo.animation)
	end
	
	setmetatable(this, self)
	return this
end

-- =====================================================
-- Button Class 
-- =====================================================
Button = setmetatable({}, { __index = GuiBase })
Button.__index = Button

function Button:Create(params)
	local this = GuiBase:Create(
		{
			type = UIObjectType.Button,
			position = params.position or vec2(0, 0),
			imageInfo = params.imageInfo,
			textInfo = params.textInfo or nil,
			
			-- State Variables 
			baseColor = params.baseColor or Color(255, 255, 255, 255),
			hoveredColor = params.hoveredColor or Color(255, 255, 255, 255),
			activeColor = params.activeColor or Color(255, 255, 255, 255),
			onPressed = params.onPressed or function() print("Please add action to button") end,
		}
	)
	
	this.animationInfo = params.animationInfo or nil
	
	if this.animationInfo then
		this.imageEntity:addComponent(this.animationInfo.animation)
	end
	
	if params.bgImage then
		local imageTransform = this.imageEntity:getComponent(Transform)
		this.bgImageEntity = Entity()
		this.bgImageEntity:addComponent(
			Transform(
				imageTransform.position + params.bgImage.offset or vec2(0, 0),
				params.bgImage.scale or imageTransform.scale,
				0.0
			)
		)
		
		local bgSprite = this.bgImageEntity:addComponent(params.bgImage.sprite)
		local texture = AssetManager.getTexture(bgSprite.sTexture)
		bgSprite:generateUVs(texture.width, texture.height)
	end
	
	setmetatable(this, self)
	return this
end
