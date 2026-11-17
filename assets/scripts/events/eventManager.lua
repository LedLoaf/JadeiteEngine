-- EventManager
EventManager = {}
EventManager.__index = EventManager

function EventManager:Create()
	local this = 
	{
		listeners = {},
		eventQueue = {},
	}
	
	setmetatable(this, self)
	return this
end

function EventManager:AddListener(eventType, callback)
	
	if not self.listeners[eventType] then 
		self.listeners[eventType] = {}
	end
	
	table.insert(self.listeners[eventType], callback)
end

function EventManager:RemoveListener(eventType, callback)
	local list = self.listeners[eventType]
	if not list then
		return
	end
	
	for i = #list, 1, -1 do
		if list[i] == callback then
			table.remove(list, i)
			break
		end
	end
end

function EventManager:EmitEvent(event)
	assert(event.type ~= nil, "All Events must have a type member!")
	
	local list = self.listeners[event.type]
	if not list then
		return 
	end
	
	for _, callback in ipairs(list) do
		callback(event)
	end	
end

function EventManager:EnqueueEvent(event)
	assert(event.type ~= nil, "All Events must have a type member!")
	if not self.eventQueue[event.type] then 
		self.eventQueue[event.type] = {}
	end
	
	table.insert(self.eventQueue[event.type], event)
end

function EventManager:Update(eventType)
	if not eventType then
		for _, v in pairs(self.eventQueue) do
			for i = #v, 1, -1 do
				local event = v[i]
				if event then
					self:EmitEvent(event)
				end
				
				table.remove(v, i)
			end
			-- TODO: Maybe clear table?
		end
		
		return
	end
	
	local events = self.eventQueue[eventType]
	if events then 
		for i = 1, #events do 
			local event = events[i]
			if event then 
				self:EmitEvent(event)
			end
		end
		
		events = {}
	end
end

gEventManager = EventManager:Create()
