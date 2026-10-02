--[[ 
CoroutineScheduler is responsible for:
	- Centralized hub for coroutine control 
	- Creating Coroutines
	- Storing active coroutines and resuming them during update
	- Removing finished coroutines

Each Coroutine starts when an event happens and runs until it's finished, then cleans itself up automatically
		
Usage Examples:
	- Pickups that play animations, wait, then apply an effect
	- Delayed actions without timers
	- Cutscenes that sequence events.
	- Dialogue systems that pause and resume
	- Scripted interactions that unfold over time
	- AI Behaviors, Timed Effects, UI Sequences, Tutorials, Boss Fight Scripting
--]]

CoroutineScheduler = {}
CoroutineScheduler.__index = CoroutineScheduler

function CoroutineScheduler:Create()
	local this = 
	{
		-- Holds not just coroutines, but data
		coroutines = {}
	}
	
	setmetatable(this, self)
	return this
end

-- Each function can have its own arguments to pass into the resume function so we use a varadic argument.
function CoroutineScheduler:Add(func, ...)
	-- If NOT a function
	assert(type(func) == "function", "All coroutines must take in a function as the first argument")

	-- Creates the actual coroutine and returns the thread object (not actually in parallel)
	local co = coroutine.create(func)
	-- Active coroutine table
	table.insert(self.coroutines, {co = co, args = { ... } } )
end

-- Updating the coroutines (all the resuming and removing coroutines when they are dead)
function CoroutineScheduler:Update()
	-- Removes dead coroutines from the table by iterating backwards to not mess up the iterators
	for i = #self.coroutines, 1, -1 do
		local entry = self.coroutines[i]
		
		local status = coroutine.status(entry.co)
		-- A coroutine starts in a suspended state when it's created
		if status == "dead" then
			table.remove(self.coroutines, i)

			--Print("Removed finished coroutine...")
			
		elseif status == "suspended" then
			-- Unpack returns the elements of a given table as separate, individual values.  
			-- This allows you to easily pass table contents as arguments to functions or assign them to multiple variables in a single statement.
			local bSuccess, error = coroutine.resume(entry.co, table.unpack(entry.args))
			
			if not bSuccess then
				
				PrintError("Coroutine failed: " .. tostring(error))
				table.remove(self.coroutines, i)
			end		
			
		elseif status == "running" then
			-- .............. --
			-- ..Do nothing.. --
			-- .............. --
		else
			-- Unknown state, remove it
			PrintError("The coroutine failed. Unknown status: " .. status)
			table.remove(self.coroutines, i)
		end
		
		--Print("Updating coroutines...")
	end
end

gScheduler = CoroutineScheduler:Create()