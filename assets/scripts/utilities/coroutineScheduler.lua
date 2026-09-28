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
	
	print("Add coroutine function: " .. ...)
	
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
			print("Removed finished coroutine")
		elseif status == "suspended" then
			-- Unpack returns the elements of a given table as separate, individual values.  
			-- This allows you to easily pass table contents as arguments to functions or assign them to multiple variables in a single statement.
			local bSuccess, error = coroutine.resume(entry.co, table.unpack(entry.args))
			
			if not bSuccess then
				print("Coroutine failed. Error: " ..tostring(error))
				table.remove(self.coroutines, i)
			end			
		elseif status == "running" then
			-- Do nothing...		
		else
			-- Unknown state remove it
			print("The coroutine failed. Unknown status: " .. status)
			table.remove(self.coroutines, i)
		end
		
		print("Updating coroutines...")
	end
end

gScheduler = CoroutineScheduler:Create()