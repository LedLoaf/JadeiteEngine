-- Coroutines in Lua Examples


--*** MAIN PROGRAM LOOP ***

local co = coroutine.create(
	function(name)
		print("Hello "..name)
		coroutine.yield()
		print("Resumed "..name)
	end
)

print(co)

main = 
{
	update = function()

		local status = coroutine.status(co)
		
		if status == "dead" then
			print("Coroutine has finished...")
		elseif status == "suspended" then
			print("Coroutine is suspended try to resume")
			local bSuccess, error = coroutine.resume(co,"World")
			
			if not bSuccess then
				print("Coroutine Error: "..tostring(error))
			end
		end

	end
}
--*************************
