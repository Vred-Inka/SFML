#pragma once
#include <memory>
#include "Component.h"
#include "CShape.h"

class Entity
{
private:
	const size_t m_Id = 0;
	const std::string m_Tag{ "Default" };
	bool m_Active = true;
	
	//Entity() {};

public:
	std::shared_ptr<CTransform>  cTransform;
	std::shared_ptr<CCollision>  cCollision;
	std::shared_ptr<CoShape>	 cShape;
	std::shared_ptr<CInput>		 cInput;
	std::shared_ptr<CScore>		 cScore;
	std::shared_ptr<CLifeSpan>	 cLifeSpan;
	std::shared_ptr<CSuperPower> cSuperPower;

	const std::string m_Name{ "" };

	Entity(const std::string& tag, size_t id);

	const std::string& GetTag() const { return m_Tag; };

	//void AddComponent<T>(args);
	//void GetComponent<T>(args);

	void Destroy() { m_Active = false; };
	bool IsActive() const { return m_Active; };
	size_t GetId() const { return m_Id; };

	bool HasCollision(Entity& e);
	bool HasCollision(const Vec2& pos, int radius);

};

