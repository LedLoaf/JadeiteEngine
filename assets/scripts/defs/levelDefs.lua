-- Level Definitions
-- This is just a simple module that has all of the levels
-- We set them up basically as lua paths.

local _levels = {
    [1] = "defs.maps.level1",
    [2] = "defs.maps.level2",
    -- Add more levels as needed
}

local LevelHandler = {}

function LevelHandler:GetLevelDef(lvl)
    if lvl == nil then
        print("ERROR: Failed to get level def. Level passed in is invalid.")
        return nil
    end
    
    local levelDef = _levels[lvl]
    if not levelDef then
        print("ERROR: Failed to get level def. Level passed in is invalid.")
        return nil
    end
    
    return levelDef
end

return LevelHandler