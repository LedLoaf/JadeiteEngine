#pragma once
#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
class Camera
{
public:
	Camera();
	Camera(int width, int height);
	~Camera();
	
	void Update();
	
	inline void SetPosition(float x, float y) { m_Position = glm::vec2{ x, y }; m_bNeedsUpdate = true; }
	inline void SetPosition(const glm::vec2& pos) { m_Position = pos; m_bNeedsUpdate = true; }
	inline glm::vec2 GetPosition() const { return m_Position; }
	
	inline void SetScale(float scale) { m_Scale = scale; m_bNeedsUpdate = true; }
	inline float GetScale() const { return m_Scale; }
	inline glm::mat4 GetCameraMatrix() const { return m_CameraMatrix; }
	
	static void CreateLuaBind(sol::state& lua, Camera& camera);
	
private:
	void Initialize();
	
private:
	glm::vec2 m_Position;
	glm::mat4 m_CameraMatrix;
	glm::mat4 m_ProjectionMatrix;
	int m_Width;
	int m_Height;
	float m_Scale;
	bool m_bNeedsUpdate;
};
} // jadeite
