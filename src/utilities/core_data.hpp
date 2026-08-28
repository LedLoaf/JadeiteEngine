#pragma once 
#include <cstdint>

#define CORE_DATA() CoreData::GetInstance()

namespace jadeite
{
class CoreData
{
public:
	static CoreData& GetInstance();
	
	// Make non-copyable
	CoreData(const CoreData&) = delete;
	CoreData& operator=(const CoreData&) = delete;
	
	void SetWindowWidth( int windowWidth );
	void SetWindowHeight( int windowHeight);
	float MetersToPixels() const;
	float PixelsToMeters() const;
	
	void SetScaledWidth( int scaledWidth );
	void SetScaledHeight( int scaledHeight);
	
	inline float ScaledWidth() const { return m_ScaledWidth; }
	inline float ScaledHeight() const { return m_ScaledHeight; }
	
	inline int32_t GetVelocityIterations() const { return m_VelocityIterations; }
	inline int32_t GetPositionIterations() const { return m_PositionIterations; }
	
	inline void SetVelocityIterations(int32_t newIteration) { m_VelocityIterations = newIteration; }
	inline void SetPositionIterations(int32_t newIteration) { m_PositionIterations = newIteration; }
	
	inline float GetGravity() const { return m_Gravity; }
	inline void SetGravity(float newGravity) { m_Gravity = newGravity; }
	
	inline void EnablePhysics( ) { m_bPhysicsEnabled = true; } 
	inline void DisablePhysics( ) { m_bPhysicsEnabled = false; } 
	
	inline void PausePhysics( ) { m_bPhysicsPaused = true; } 
	inline void UnpausePhysics( ) { m_bPhysicsPaused = false; } 
	
	inline bool IsPhysicsEnabled() const { return m_bPhysicsEnabled; }
	inline bool IsPhysicsPaused() const { return m_bPhysicsPaused; }
	
private:
	CoreData();
	~CoreData() = default;

private:
	/* The width of the screen in meters. */
	float m_ScaledWidth;
	/* The height of the screen in meters. */
	float m_ScaledHeight;
	/* The default gravity force. */
	float m_Gravity;
	/* The width of the screen in pixels. */
	float m_WindowWidth;
	/* The height of the screen in pixels. */
	float m_WindowHeight;
	/* The velocity tuning parameter for accuracy in Box2D. */
	int32_t m_VelocityIterations;
	/* The position tuning parameter for accuracy in Box2D. */
	int32_t m_PositionIterations;
	/* Flag to toggle physics on/off. */
	bool m_bPhysicsEnabled;
	/* Flag to toggle pause physics. */
	bool m_bPhysicsPaused;
};

} // jadeite
