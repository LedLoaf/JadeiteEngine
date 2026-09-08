-- Follow Camera

FollowCamera = {}
FollowCamera.__index = FollowCamera

function FollowCamera:Create(cam ,params)
	local this = 
	{
		m_Cam = cam or Camera.get(),
		m_Scale = params.scale or cam.getScale() or 1.0,
		m_MinX = params.minX or 0,
		m_MinY = params.minY or 0,
		m_MaxX = params.maxX,
		m_MaxY = params.maxY,
		m_Springback = params.springback or 1.0
	}
	this.m_Cam.setPosition(vec2(this.m_MinX, this.m_MinY))
	this.m_Cam.setScale(this.m_Scale)
	
	setmetatable(this, self)
	return this
end

function FollowCamera:Update(entityID)

	local entity = Entity(entityID)
	local transform = entity:getComponent(Transform)
	local sprite = entity:getComponent(Sprite)
	
	local halfSpriteW = 0.0
	local halfSpriteH = 0.0
	
	if sprite then
		halfSpriteW = sprite.width  * transform.scale.x * 0.5
		halfSpriteH = sprite.height * transform.scale.y * 0.5
	end
	
	local camPos    = self.m_Cam.getPosition()
	local camScale  = self.m_Cam.getScale()
	local camWidth  = self.m_Cam.getWidth()
	local camHeight = self.m_Cam.getHeight()
	
	-- Compute camera center in world units
	local halfWidth  = (camWidth / camScale) * 0.5
	local halfHeight = (camHeight / camScale) * 0.5
	
	local newCamPos = vec2(
		transform.position.x + halfSpriteW - halfWidth,
		transform.position.y + halfSpriteH - halfHeight
	)
	
	-- Clamp camera bounds in world units
	newCamPos.x = J2D_clamp(newCamPos.x, self.m_MinX, self.m_MaxX - camWidth / camScale)
	newCamPos.y = J2D_clamp(newCamPos.y, self.m_MinY, self.m_MaxY - camHeight / camScale)
	
	self.m_Cam.setPosition(vec2(
		J2D_lerp(camPos.x, newCamPos.x, self.m_Springback),
		J2D_lerp(camPos.y, newCamPos.y, self.m_Springback)
	))
end