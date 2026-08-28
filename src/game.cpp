#include "game.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/font.hpp"
#include "rendering/default_shaders.hpp"
#include "rendering/camera.hpp"
#include "rendering/vertex.hpp"
#include "rendering/batch_renderer.hpp"
#include "rendering/text_batch_renderer.hpp"

#include "rendering/shape_batch_renderer.hpp"
#include "rendering/shape.hpp"


#include "inputs/keyboard.hpp"
#include "inputs/mouse.hpp"
#include "inputs/gamepad.hpp"

#include "utilities/asset_manager.hpp"
#include "utilities/utilities.hpp"
#include "utilities/core_data.hpp"

#include "sounds/music_player.hpp"
#include "sounds/sound_player.hpp"

#include "ecs/components.hpp"
#include "ecs/entity.hpp"
#include "ecs/registry.hpp"
#include "scripting/glm_bindings.hpp"
#include "scripting/script_bindings.hpp"

#include "physics/box2d_wrappers.hpp"
#include "physics/physics_component.hpp"
#include "physics/contact_listener.hpp"
#include <emscripten.h>

namespace jadeite
{
	
struct InputContext
{
	std::shared_ptr<Keyboard> pKeyboard{ nullptr };
	std::shared_ptr<Mouse> pMouse{ nullptr };
	std::shared_ptr<Gamepad> pGamepad{ nullptr };
	
	void Update()
	{
		if (pKeyboard) pKeyboard->Update();
		if (pMouse) pMouse->Update();
		if (pGamepad) pGamepad->Update();
	}
};

struct AudioContext 
{
	std::shared_ptr<MusicPlayer> pMusicPlayer{ nullptr };
	std::shared_ptr<SoundPlayer> pSoundPlayer{ nullptr };
};

struct CameraContext
{
	std::unique_ptr<Camera> pCamera{ nullptr };
	//std::unique_ptr<Camera> pUICamera{ nullptr };
	
	void Update()
	{
		if (pCamera) pCamera->Update();
		//if (pUICamera) pUICamera->Update();
	}
};

// Helpful aliases
using InputCtxPtr = std::shared_ptr<InputContext>;
using SolStatePtr = std::shared_ptr<sol::state>;
using CameraPtr = std::shared_ptr<Camera>;
using CameraContextPtr = std::shared_ptr<CameraContext>;
using BatchRendererPtr = std::shared_ptr<BatchRenderer>;
using TextRendererPtr = std::shared_ptr<TextBatchRenderer>;
using ShapeRendererPtr = std::shared_ptr<ShapeRenderer>;
using AudioCtxPtr = std::shared_ptr<AudioContext>;
using AssetManagerPtr = std::shared_ptr<AssetManager>;
using ContactListenerPtr = std::shared_ptr<ContactListener>;

constexpr float OneOverSixty = 1.f / 60.f;

Game::Game()
	: m_pWindow{ nullptr }
	, m_GLContext{}
	, m_Event{}
	, m_pRegistry{ nullptr }
	, m_MainScript{}
	, m_bRunning{ false }
{
	
}

Game::~Game() = default;

void Game::Run()
{
	SDL_GL_MakeCurrent(m_pWindow, m_GLContext);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    SDL_GetWindowSize(m_pWindow, &width, &height);
    glViewport(0.f, 0.f, width, height);

    ProcessEvents();
    Update();
    Render();
}

bool Game::Initialize()
{
	if (!InitSDL())
	{
		std::cerr << "Failed to initialize SDL.\n";
		return false;
	}
	
	if (!InitializeRegistry())
	{
		std::cerr << "Failed to initialize the registry.\n";
		return false;
	}
	
	if (!LoadShaders())
	{
		std::cerr << "Failed to load shaders successfully.\n";
		return false;
	}
		
	RegisterMetaComponents();
	RegisterLuaBindings();
	
	if (!LoadMainScript())
	{
		std::cerr << "Failed to load main lua script.\n";
		return false;
	}
	 
	return true;
}

// Initialize Functions
bool Game::InitializeRegistry()
{
	m_pRegistry = std::make_unique<Registry>();
	auto pLuaState = m_pRegistry->AddToContext<SolStatePtr>(std::make_shared<sol::state>());
	pLuaState->open_libraries(
		sol::lib::base, 
		sol::lib::math,
		sol::lib::table,
		sol::lib::string,
		sol::lib::os
	);
	
	// Load camera context to registry
	auto pCameraContext = std::make_shared<CameraContext>();
	pCameraContext->pCamera = std::make_unique<Camera>(800, 600);
	//pCameraContext->pUICamera = std::make_unique<Camera>(800, 600);
	m_pRegistry->AddToContext<CameraContextPtr>(std::move(pCameraContext));
	
	// Load input context to registry
	auto pInputContext = std::make_shared<InputContext>();
	pInputContext->pKeyboard = std::make_shared<Keyboard>();
	pInputContext->pMouse = std::make_shared<Mouse>();
	pInputContext->pGamepad = std::make_shared<Gamepad>();
	m_pRegistry->AddToContext<InputCtxPtr>(std::move(pInputContext));
	
	// Load Audio context to registry
	auto pAudioContext = std::make_shared<AudioContext>();
	pAudioContext->pMusicPlayer = std::make_shared<MusicPlayer>();
	pAudioContext->pSoundPlayer = std::make_shared<SoundPlayer>();
	m_pRegistry->AddToContext<AudioCtxPtr>(std::move(pAudioContext));	
		
	m_pRegistry->AddToContext<BatchRendererPtr>(std::make_shared<BatchRenderer>());
	m_pRegistry->AddToContext<TextRendererPtr>(std::make_shared<TextBatchRenderer>());
	m_pRegistry->AddToContext<ShapeRendererPtr>(std::make_shared<ShapeRenderer>());
	
	m_pRegistry->AddToContext<AssetManagerPtr>(std::make_shared<AssetManager>());
	
	auto pPhysicsWorld = std::make_shared<b2World>(b2Vec2{ 0.f, 9.8f});
	auto pContactListener = std::make_shared<ContactListener>();
	pPhysicsWorld->SetContactListener(pContactListener.get());
	
	m_pRegistry->AddToContext<PhysicsWorld>(std::move(pPhysicsWorld));
	m_pRegistry->AddToContext<ContactListenerPtr>(std::move(pContactListener));
	
	return true;
}

bool Game::LoadMainScript()
{
	auto& pLuaState = m_pRegistry->GetContext<SolStatePtr>();
	auto result = pLuaState->safe_script_file("assets/scripts/main.lua");
	if (!result.valid())
	{
		std::cerr << "Failed to load main lua script.\n";
		return false;
	}
	
	sol::optional<sol::table> optMainTable = (*pLuaState)["main"];
	if (!optMainTable)
	{
		std::cerr << "Failed to load main script. \"main\" table does not exist.\n";
		return false;
	}
	
	sol::optional<sol::function> optUpdateFunc = (*optMainTable)["update"];
	if (!optUpdateFunc) 
	{
		std::cerr << "Failed to load main script. \"update\" function does not exist.\n";
		return false;
	}
	
	m_MainScript.update = optUpdateFunc.value();
	
	// TODO: handle the main script render function
	
	return true;
}

bool Game::LoadShaders()
{
	auto& pAssetManager = m_pRegistry->GetContext<AssetManagerPtr>();
	if (!pAssetManager->AddShaderFromMemory("basic", DefaultShaders::basicShaderVert, DefaultShaders::basicShaderFrag))
	{
		std::cerr << "Failed to load basic shader.\n";
		return false;
	}
	
	if (!pAssetManager->AddShaderFromMemory("font", DefaultShaders::fontShaderVert, DefaultShaders::fontShaderFrag))
	{
		std::cerr << "Failed to load font shader.\n";
		return false;
	}
	
	if (!pAssetManager->AddShaderFromMemory("shape", DefaultShaders::shapeShaderVert, DefaultShaders::shapeShaderFrag))
	{
		std::cerr << "Failed to load shape shader.\n";
		return false;
	}
	
	return true;
}

bool Game::InitSDL()
{
	std::cout << "Initializing SDL...\n";
	if (SDL_Init(SDL_INIT_EVERYTHING & ~(SDL_INIT_TIMER | SDL_INIT_HAPTIC)) < 0)
	{
		std::cerr << "SDL Initialization falied: " << SDL_GetError() << "\n";
		return false;
	}
	
	// Create the window 
	m_pWindow = SDL_CreateWindow(
		"SDL2 and Emscripten Test", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		800, 
		600, 
		SDL_WINDOW_OPENGL
	);
	
	if (!m_pWindow)
	{
		std::cerr << "Failed to create SDL window: " << SDL_GetError() << "\n";
		return false;
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
	m_GLContext = SDL_GL_CreateContext( m_pWindow );
	
	SDL_GL_SetSwapInterval(1);
	
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	
	std::cout << "SDL Initialized successfully.\n";
		
	return true;
}

void Game::RegisterMetaComponents()
{
	Entity::RegisterMetaComponent<Identification>();
	Entity::RegisterMetaComponent<TransformComponent>();
	Entity::RegisterMetaComponent<SpriteComponent>();
	Entity::RegisterMetaComponent<BoxCollider>();
	Entity::RegisterMetaComponent<CircleCollider>();
	Entity::RegisterMetaComponent<AnimationComponent>();
	Entity::RegisterMetaComponent<RigidBodyComponent>();
	Entity::RegisterMetaComponent<TextComponent>();
	Entity::RegisterMetaComponent<PhysicsComponent>();
	
	Registry::RegisterMetaComponent<Identification>();
	Registry::RegisterMetaComponent<TransformComponent>();
	Registry::RegisterMetaComponent<SpriteComponent>();
	Registry::RegisterMetaComponent<BoxCollider>();
	Registry::RegisterMetaComponent<CircleCollider>();
	Registry::RegisterMetaComponent<AnimationComponent>();
	Registry::RegisterMetaComponent<RigidBodyComponent>();
	Registry::RegisterMetaComponent<TextComponent>();
	Registry::RegisterMetaComponent<PhysicsComponent>();
}

void Game::RegisterLuaBindings()
{
	auto& pAssetManager = m_pRegistry->GetContext<AssetManagerPtr>();
	auto& pLuaState = m_pRegistry->GetContext<SolStatePtr>();
	auto& pCameraContext = m_pRegistry->GetContext<CameraContextPtr>();
	auto& pInputContext = m_pRegistry->GetContext<InputCtxPtr>();
	
	auto& pAudioContext = m_pRegistry->GetContext<AudioCtxPtr>();
	auto& pPhysicsWorld = m_pRegistry->GetContext<PhysicsWorld>();
	
	AssetManager::CreateLuaBind(*pLuaState, *pAssetManager);
	Camera::CreateLuaBind(*pLuaState, *pCameraContext->pCamera);
	Vertex::CreateLuaBind(*pLuaState);
	ComponentBinder::CreateLuaBind(*pLuaState);
	PhysicsComponent::CreateLuaBind(*pLuaState, pPhysicsWorld);
	
	Entity::CreateLuaBind(*pLuaState, *m_pRegistry);
	Registry::CreateLuaBind(*pLuaState, *m_pRegistry);
	ShapeBinder::CreateLuaBind(*pLuaState, *m_pRegistry);
	GlmBinder::CreateLuaBind(*pLuaState);
	Keyboard::CreateLuaBind(*pLuaState, *pInputContext->pKeyboard);
	Mouse::CreateLuaBind(*pLuaState, *pInputContext->pMouse);
	Gamepad::CreateLuaBind(*pLuaState, *pInputContext->pGamepad);
	ScriptFuncBinder::CreateLuaBind(*pLuaState);
	MusicPlayer::CreateLuaBind(*pLuaState, *pAudioContext->pMusicPlayer, *pAssetManager);
	SoundPlayer::CreateLuaBind(*pLuaState, *pAudioContext->pSoundPlayer, *pAssetManager);
	utilities::JadeiteUtilities::CreateLuaBind(*pLuaState, *pAssetManager);
	Texture::CreateLuaBind(*pLuaState);
}

// Game Loop Functions
void Game::ProcessEvents()
{
	auto& pInputContext = m_pRegistry->GetContext<InputCtxPtr>();
	while(SDL_PollEvent(&m_Event))
	{		
		switch ( m_Event.type )
		{
		case SDL_QUIT:
			emscripten_cancel_main_loop();
			break;
		case SDL_KEYDOWN:
			pInputContext->pKeyboard->OnKeyPressed(m_Event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			pInputContext->pKeyboard->OnKeyReleased(m_Event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			pInputContext->pMouse->OnBtnPressed(m_Event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			pInputContext->pMouse->OnBtnReleased(m_Event.button.button);
			break;
		case SDL_MOUSEWHEEL:
			pInputContext->pMouse->SetMouseWheelValues(m_Event.wheel.x, m_Event.wheel.y);
			break;
		case SDL_MOUSEMOTION:
			pInputContext->pMouse->SetMouseMoving( true );
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			pInputContext->pGamepad->OnBtnPressed(m_Event.cbutton.button);
			break;
		case SDL_CONTROLLERBUTTONUP:
			pInputContext->pGamepad->OnBtnReleased(m_Event.cbutton.button);
			break;
		case SDL_CONTROLLERDEVICEADDED:
		{
			std::cout << "Added Controller\n";
			if (!pInputContext->pGamepad->IsGamepadPresent())
			{
				
				pInputContext->pGamepad->SetController(
					MakeSharedController(
						SDL_GameControllerOpen(m_Event.jdevice.which)
					)
				);
			}
			
			break;
		}
		case SDL_CONTROLLERDEVICEREMOVED:
		{
			std::cout << "Removed controller\n";
			if (pInputContext->pGamepad->IsGamepadPresent())
			{
				pInputContext->pGamepad->RemoveController();
			}
			
			break;
		}
		case SDL_JOYAXISMOTION:
			pInputContext->pGamepad->SetAxisPositionValue(m_Event.jaxis.axis, m_Event.jaxis.value);
			break;
		case SDL_JOYHATMOTION:
			pInputContext->pGamepad->SetJoystickHatValue(m_Event.jhat.value);
			break;
		default: 
			break;
		}
	}
}

void Game::Update()
{
	auto& pAssetManager = m_pRegistry->GetContext<AssetManagerPtr>();
	auto& pCameraContext = m_pRegistry->GetContext<CameraContextPtr>();
	auto& pShapeRenderer = m_pRegistry->GetContext<ShapeRendererPtr>();
	
	auto pShapeShader = pAssetManager->GetShader("shape");
	if (pShapeShader)
	{
		auto mat = pCameraContext->pCamera->GetCameraMatrix();
		pShapeShader->Enable();
		pShapeShader->SetUniformMat4("uProjection", mat);
	}

	pShapeRenderer->Begin();
	
	if (m_MainScript.update.valid())
	{
		m_MainScript.update();
	}
	
	auto& coreData = CORE_DATA();
	if (coreData.IsPhysicsEnabled() && !coreData.IsPhysicsPaused())
	{
		auto& pPhysicsWorld = m_pRegistry->GetContext<PhysicsWorld>();
		pPhysicsWorld->Step(OneOverSixty, coreData.GetVelocityIterations(), coreData.GetPositionIterations());
		
		UpdatePhysics();
	}
	
	m_pRegistry->GetContext<InputCtxPtr>()->Update();
	pCameraContext->Update();
}

void Game::Render()
{
	RenderSprites();
    RenderText();
    RenderShapes();
    
    SDL_GL_SwapWindow(m_pWindow);
}

void Game::UpdatePhysics()
{
	auto& coreData = CORE_DATA();
	const float halfScaledWidth = coreData.ScaledWidth() * 0.5f;
	const float halfScaledHeight = coreData.ScaledHeight() * 0.5f;
	const float M2P = coreData.MetersToPixels();
	
	auto boxView = m_pRegistry->GetRegistry().view<PhysicsComponent, TransformComponent, BoxCollider>();
	for ( auto entity : boxView )
	{
		auto& physics = boxView.get<PhysicsComponent>(entity);
		auto pRigidBody = physics.GetBody();
		if ( !pRigidBody )
			continue;
		
		if ( pRigidBody->GetType() == b2BodyType::b2_staticBody )
			continue;
		
		auto& transform = boxView.get<TransformComponent>(entity);
		auto& boxCollider = boxView.get<BoxCollider>(entity);
		const auto& bodyPosition = pRigidBody->GetPosition();
		
		transform.position.x = 
			(halfScaledWidth + bodyPosition.x) * M2P - 
			(boxCollider.width * transform.scale.x) * 0.5f - 
			boxCollider.offset.x;
		
		transform.position.y = 
			(halfScaledHeight + bodyPosition.y) * M2P - 
			(boxCollider.height * transform.scale.y) * 0.5f - 
			boxCollider.offset.y;
			
		if ( !pRigidBody->IsFixedRotation() )
		{
			transform.rotation = glm::degrees( pRigidBody->GetAngle() );
		}
	}
	
	auto circleView = m_pRegistry->GetRegistry().view<PhysicsComponent, TransformComponent, CircleCollider>();
	for ( auto entity : circleView )
	{
		auto& physics = circleView.get<PhysicsComponent>(entity);
		auto pRigidBody = physics.GetBody();
		if ( !pRigidBody )
			continue;
		
		if ( pRigidBody->GetType() == b2BodyType::b2_staticBody )
			continue;
		
		auto& transform = circleView.get<TransformComponent>(entity);
		auto& circleCollider = circleView.get<CircleCollider>(entity);
		const auto& bodyPosition = pRigidBody->GetPosition();
		
		transform.position.x = 
			(halfScaledWidth + bodyPosition.x) * M2P - 
			(circleCollider.radius * transform.scale.x) - 
			circleCollider.offset.x;
		
		transform.position.y = 
			(halfScaledHeight + bodyPosition.y) * M2P - 
			(circleCollider.radius * transform.scale.y) - 
			circleCollider.offset.y;
			
		if ( !pRigidBody->IsFixedRotation() )
		{
			transform.rotation = glm::degrees( pRigidBody->GetAngle() );
		}
	}
	
}

void Game::RenderText()
{
	auto& pAssetManager = m_pRegistry->GetContext<AssetManagerPtr>();
	
	auto& pCameraContext = m_pRegistry->GetContext<CameraContextPtr>();
	auto& pTextBatchRenderer = m_pRegistry->GetContext<TextRendererPtr>();
	
	auto textCamMat = pCameraContext->pCamera->GetCameraMatrix();
	auto textView = m_pRegistry->GetRegistry().view<TextComponent>();
	auto pFontShader = pAssetManager->GetShader("font");
	if (!pFontShader)
	{
		std::cerr << "Failed to render text. Font shader does not exist.\n";
		return;
	}
	
	pTextBatchRenderer->Begin();

	pFontShader->Enable();
	pFontShader->SetUniformMat4("uProjection", textCamMat);
		
	for (auto entity : textView)
	{ 
		Entity ent{ *m_pRegistry, entity };
		const auto& text = ent.GetComponent<TextComponent>();
		if (text.bHidden)
			continue;
		
		auto pFont = pAssetManager->GetFont(text.sFontName);
		if (!pFont)
			continue;
		
		const auto& transform = ent.GetComponent<TransformComponent>();
		pTextBatchRenderer->AddText(text.sText, pFont, transform.position, text.color);
	}
	
	pTextBatchRenderer->End();
	pTextBatchRenderer->Render();
	
	pFontShader->Disable();
}

void Game::RenderSprites()
{
	auto& pAssetManager = m_pRegistry->GetContext<AssetManagerPtr>();
	auto& pCameraContext = m_pRegistry->GetContext<CameraContextPtr>();
	auto& pBatchRenderer = m_pRegistry->GetContext<BatchRendererPtr>();
	
	auto pShader = pAssetManager->GetShader("basic");
	if (!pShader)
	{
		std::cerr << "Failed to render sprites. Basic shader does not exist.\n";
		return;
	}
	
	pShader->Enable();
	
	auto camMat = pCameraContext->pCamera->GetCameraMatrix();
	pShader->SetUniformMat4("uProjection", camMat);
	
	auto view = m_pRegistry->GetRegistry().view<TransformComponent, SpriteComponent>();
	pBatchRenderer->Begin();
	
	for (auto entity : view )
	{
		const auto& transform = view.get<TransformComponent>(entity);
		const auto& sprite = view.get<SpriteComponent>(entity);
		
		auto pTexture = pAssetManager->GetTexture(sprite.sTexture);
		if (!pTexture || sprite.bHidden)
			continue;
		
		glm::vec4 pos{
			transform.position.x,
			transform.position.y,
			sprite.width * transform.scale.x,
			sprite.height * transform.scale.y
		};
		
		glm::vec4 uvs{
			sprite.uvs.u,
			sprite.uvs.v,
			sprite.uvs.uvWidth,
			sprite.uvs.uvHeight
		};
		
		pBatchRenderer->AddSprite(pos, uvs, sprite.layer, pTexture->GetID(), sprite.color);
	}
	
	pBatchRenderer->End();
	pBatchRenderer->Render();	
	pShader->Disable();
}

void Game::RenderShapes()
{
	auto& pAssetManager = m_pRegistry->GetContext<AssetManagerPtr>();
	auto& pCameraContext = m_pRegistry->GetContext<CameraContextPtr>();
	auto& pShapeRenderer = m_pRegistry->GetContext<ShapeRendererPtr>();
	
	auto boxView = m_pRegistry->GetRegistry().view<TransformComponent, BoxCollider>();
	auto circleView = m_pRegistry->GetRegistry().view<TransformComponent, CircleCollider>();
	
	for ( auto entity : boxView )
	{
		const auto& transform = boxView.get<TransformComponent>( entity );
		const auto& boxCollider = boxView.get<BoxCollider>( entity );
		
		pShapeRenderer->AddRectangle(
			transform.position + boxCollider.offset,
			glm::vec2{
				boxCollider.width * transform.scale.x,
				boxCollider.height * transform.scale.y
			},
			Color{255, 0, 0, 135}
		);
	}
	
	for ( auto entity : circleView )
	{
		const auto& transform = circleView.get<TransformComponent>( entity );
		const auto& circleCollider = circleView.get<CircleCollider>( entity );
		
		pShapeRenderer->AddCircle(
			glm::vec2{
				transform.position.x + (circleCollider.radius * transform.scale.x) + circleCollider.offset.x,
				transform.position.y + (circleCollider.radius * transform.scale.y) + circleCollider.offset.y
			},
			circleCollider.radius * transform.scale.x,
			Color{255, 0, 0, 135}
		);
	}
	
	auto pShapeShader = pAssetManager->GetShader("shape");
	if (!pShapeShader)
	{
		std::cerr << "Failed to render shapes. Basic shader does not exist.\n";
		return;
	}
	
	pShapeShader->Enable();
	auto camMat = pCameraContext->pCamera->GetCameraMatrix();
	pShapeShader->SetUniformMat4("uProjection", camMat);
	
	pShapeRenderer->End();
	pShapeRenderer->Render();
	
	pShapeShader->Disable();
}

void Game::CleanUp()
{
	SDL_GL_DeleteContext(m_GLContext);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

} // jadeite