AssetDefs = 
{
	textures = 
	{
		{ name = "characters", path = "assets/textures/characters.png", bPixelArt = true },
		{ name = "tileset1", path = "assets/textures/tilesets/tileset1.png", bPixelArt = true },
	
	},
	fonts = 
	{
		{ name = "pixel16", path = "assets/fonts/pixel/pixel.ttf", fontSize = 16.0 },
		{ name = "pixel32", path = "assets/fonts/pixel/pixel.ttf", fontSize = 32.0 },
	},
	soundfx = 
	{
		{ name = "bump", path = "assets/sounds/soundfx/bump.wav" },
		{ name = "death", path = "assets/sounds/soundfx/death.wav" },
		{ name = "finish_row", path = "assets/sounds/soundfx/finish_row.wav" },
	},
	music = 
	{
		{ name = "main", path = "assets/sounds/music/main_theme.ogg" },
		{ name = "game_over", path = "assets/sounds/music/game_over.mp3" },
	},
	shaders = 
	{
		-- Currently all shaders are engine side shaders.
	}
}

function LoadAssets(assets)
	for k, v in pairs(assets) do 
		for i = 1, #v do 
			local asset = v[i]
			if k == "textures" then 
				if not AssetManager.addTexture(asset.name, asset.path, asset.bPixelArt) then
					print("Failed to load texture [" ..asset.name .."]")
				end
			elseif k == "fonts" then 
				if not AssetManager.addFont(asset.name, asset.path, asset.fontSize) then
					print("Failed to load font [" ..asset.name .."]")
				end
			elseif k == "soundfx" then 
				if not AssetManager.addSoundfx(asset.name, asset.path) then
					print("Failed to load soundfx [" ..asset.name .."]")
				end
			elseif k == "music" then 
				if not AssetManager.addMusic(asset.name, asset.path) then
					print("Failed to load music [" ..asset.name .."]")
				end
			end
		end
	end
end