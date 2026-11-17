-- States
State = {}
State.__index = State

function State:Create()
	local this = {}
	setmetatable(this, self)
	return this
end

function State:Enter(params)
	-- Default empty implementation - To be overridden
end

function State:Exit()
	-- Default empty implementation - To be overridden
end

function State:Update(dt)
	-- Default empty implementation - To be overridden
end

-- State Stack -- Class for managing game States
StateStack = {}
StateStack.__index = StateStack

function StateStack:Create()
	local this = 
	{
		stack = {}
	}
	
	setmetatable(this, self)
	return this
end

function StateStack:Update(dt)
	if #self.stack > 0 then 
		self.stack[#self.stack]:Update(dt)
	end
end

function StateStack:Push(state, params)

	if state.Enter then 
		state:Enter(params)
	end
	
	table.insert(self.stack, state)
end

function StateStack:Pop()
	if #self.stack > 0 then
		local top = self.stack[#self.stack]
		if top.Exit then
			top:Exit()
		end
		
		table.remove(self.stack) 
	end
end

function StateStack:Clear()
	while #self.stack > 0 do
		self:Pop()
	end
end

function StateStack:Top()
	return self.stack[#self.stack]
end

-- State Machine -- Class for managing entity, npc, player states
StateMachine = {}
StateMachine.__index = StateMachine

function StateMachine:Create(states)
	local this = 
	{
		states = states or {},
		current = nil,
		currentStateName = "",
	}
	
	setmetatable(this, self)
	return this
end

function StateMachine:Change(stateName, params)
	if self.states[stateName] == nil then
		print(string.format("StateMachine: state [%s] does not exist.", stateName))
		return
	end
	
	if self.current and self.current.Exit then
		self.current:Exit()
	end
	
	self.current = self.states[stateName]
	self.currentStateName = stateName
	
	if self.current.Enter then
		self.current:Enter(params)
	end	
end

function StateMachine:Update(dt)
	if self.current and self.current.Update then
		self.current:Update(dt)
	end
end
