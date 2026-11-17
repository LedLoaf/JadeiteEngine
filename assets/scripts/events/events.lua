-- Events
Event = {}
Event.__index = Event

function Event:Create(eventType)
	assert(eventType, "All events must have a type")
	return setmetatable({ type = eventType }, self)
end

CoinCollectEvent = setmetatable({}, {__index = Event})
CoinCollectEvent.__index = CoinCollectEvent

function CoinCollectEvent:Create(amount)
	local this = Event.Create(self, "CoinCollectEvent")
	this.amount = amount or 1
	return this
end

HealthChangeEvent = setmetatable({}, {__index = Event})
HealthChangeEvent.__index = HealthChangeEvent

function HealthChangeEvent:Create(amount)
	local this = Event.Create(self, "HealthChangeEvent")
	this.amount = amount or 1
	return this
end
