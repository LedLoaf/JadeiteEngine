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
		assert(texture, "failed to get texture [" .. imageSprite.sTexture .. "]")
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
	
	if self.textEntity then 
		-- TODO: Handle text Hovering
	end 
	
	return false
end

function GuiBase:SetHovered(hovered)
	if self.imageEntity then 
		local sprite = self.imageEntity:getComponent(Sprite)
		self.bHovered = hovered
		if not self.bActive then
			if self.bHovered then
				sprite.color = self.hoveredColor
			else
				sprite.color = self.baseColor
			end
		end
	end
end

function GuiBase:SetActive(active)
	if self.imageEntity then
		local sprite = self.imageEntity:getComponent(Sprite)
		self.bActive = active
		if self.bActive then
			sprite.color = self.activeColor
		elseif self.bHovered then
			sprite.color = self.hoveredColor
		else
			sprite.color = self.baseColor
		end
	end
end

function GuiBase:Update()
    local hovered = self:IsHovered()
    
    if hovered and Mouse.justPressed(LEFT_BTN) then
        self.bActive = true
    elseif self.bActive and Mouse.justReleased(LEFT_BTN) then
        self.bActive = false
    end
    
    self:SetActive(self.bActive)
    self:SetHovered(hovered)
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
			baseColor = params.baseColor or Color(255, 255, 255, 255),
			hoveredColor = params.hoveredColor or Color(255, 255, 255, 255),
			activeColor = params.activeColor or Color(255, 255, 255, 255),
			onPressed = params.onPressed or function() print("Please add action to button") end,
		}
	)
	
	-- Rect params
	this.rectPos = params.position or vec2(0, 0)
	this.rectSize = params.size or vec2(100, 40)
	this.rectColor = params.rectColor or Color(40, 40, 60, 220)
	this.rectHoveredColor = params.rectHoveredColor or this.rectColor
	this.rectActiveColor = params.rectActiveColor or this.rectColor
	this.borderColor = params.borderColor or Color(80, 85, 110, 200)
	this.bDrawRect = params.bDrawRect ~= false
	
	--Text params
	this.textHoveredColor = params.textHoveredColor or Color(255, 255, 255, 255)
	this.textActiveColor = params.textActiveColor or Color(255, 255, 255, 255)
	this.textBaseColor = params.textBaseColor or Color(255, 255, 255, 255)   
	
	-- Auto-fill sprite to rect size if no explicit scale given
	if this.imageEntity and not (params.imageInfo and params.imageInfo.scale) then
		local transform = this.imageEntity:getComponent(Transform)
		local sprite = this.imageEntity:getComponent(Sprite)
		transform.scale = vec2(this.rectSize.x / sprite.width, this.rectSize.y / sprite.height)
		transform.position = this.rectPos
	end
	
	this.animationInfo = params.animationInfo or nil
	if this.animationInfo and this.imageEntity then
		this.imageEntity:addComponent(this.animationInfo.animation)
	end
	
	setmetatable(this, self)
	return this
end

function Button:IsHovered()
    local mousePos = Mouse.screenPosition()
    return mousePos.x > self.rectPos.x and
           mousePos.x < self.rectPos.x + self.rectSize.x and
           mousePos.y > self.rectPos.y and
           mousePos.y < self.rectPos.y + self.rectSize.y
end

function Button:Update()
    local hovered = self:IsHovered()
    
    if hovered and Mouse.justPressed(LEFT_BTN) then
        self.bActive = true
    elseif self.bActive and not Mouse.pressed(LEFT_BTN) then
        self.bActive = false
    end
    
    if self.bDrawRect then
        local rectCol = self.rectColor
        if self.bActive then
            rectCol = self.rectActiveColor
        elseif hovered then
            rectCol = self.rectHoveredColor
        end
        
        -- 1. Border
        local borderPos = vec2(self.rectPos.x - 3, self.rectPos.y - 3)
        local borderSize = vec2(self.rectSize.x + 6, self.rectSize.y + 6)
        DrawRect(Rect(borderPos, borderSize, self.borderColor, false))
        
        -- 2. Fill (with press offset)
        local fillPos = self.rectPos
        if self.bActive then
            fillPos = vec2(self.rectPos.x, self.rectPos.y + 2)
        end
        DrawRect(Rect(fillPos, self.rectSize, rectCol, false))
        
        -- 3. Top highlight
        DrawRect(Rect(fillPos, vec2(self.rectSize.x, 1), Color(255, 255, 255, 30), false))
    end
    
    if self.imageEntity then
        local sprite = self.imageEntity:getComponent(Sprite)
        if self.bActive then
            sprite.color = self.activeColor
        elseif hovered then
            sprite.color = self.hoveredColor
        else
            sprite.color = self.baseColor
        end
    end
	
	if self.textEntity then
		local text = self.textEntity:getComponent(TextComponent)
		if self.bActive then
			text.color = self.textActiveColor
		elseif hovered then
			text.color = self.textHoveredColor
		else
			text.color = self.textBaseColor
    end
end
    
    self.bHovered = hovered
    self:OnPressed()
end   