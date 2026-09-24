#pragma once
#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include <memory>
#include <vector>

namespace jadeite
{

struct MainGameScript
{
	sol::protected_function update{ sol::lua_nil_t{} };
	sol::protected_function render{ sol::lua_nil_t{} };
};

class Game
{
public:
	Game();
	~Game();
	
	void Run();
	bool Initialize();
	
private:
	/* Initialize functions */
	bool InitializeRegistry();
	bool LoadMainScript();
	bool LoadShaders();
	bool InitSDL();
	
	/* Register functions */
	void RegisterMetaComponents();
	void RegisterLuaBindings();
	
	/* Standard engine loop functions */
	void ProcessEvents();
	void Update();
	void Render();
	
	/* Component updates */
	void UpdateAnimations();
	void UpdatePhysics();
	
	/* Render functions */
	void RenderText();
	void RenderSprites();
	void RenderShapes();
	
	/* Cleanup of everything */
	void CleanUp();
	
	
private:
	SDL_Window* m_pWindow;
	SDL_GLContext m_GLContext;
	SDL_Event m_Event;
	std::unique_ptr<class Registry> m_pRegistry;
	MainGameScript m_MainScript;
	bool m_bRunning;
	
	bool m_bShowCollisionBox;			// Flag for revealing or hiding the collision boxes
};

} // jadeite