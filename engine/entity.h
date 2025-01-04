#pragma once
#include <memory>
#include "Components.h"
#include <tuple>
#include "CShape.h"

class EntityManager;

typedef std::tuple<
	CTransform,
	CLifeSpan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState
> ComponentTuple;

class Entity
{
	friend class EntityManager;

	const size_t m_Id = 0;
	const std::string m_Tag{ "Default" };
	bool m_Active = true;
	ComponentTuple m_Components;


public:
	std::shared_ptr<CTransform>  cTransform;
	std::shared_ptr<CCollision>  cCollision;
	std::shared_ptr<CoShape>	 cShape;
	std::shared_ptr<CInput>		 cInput;
	std::shared_ptr<CScore>		 cScore;
	std::shared_ptr<CLifeSpan>	 cLifeSpan;
	std::shared_ptr<CSuperPower> cSuperPower;

	const std::string m_Name{ "" };

	const std::string& GetTag() const { return m_Tag; };

	//void AddComponent<>();
	//void GetComponent<T>(args);
	Entity() {};
	Entity(const std::string& tag, size_t id);

	void Destroy() { m_Active = false; };
	bool IsActive() const { return m_Active; };
	size_t GetId() const { return m_Id; };

	bool HasCollision(Entity& e);
	bool HasCollision(const Vec2& pos, int radius);

	template<typename T>
	bool HasComponent() const
	{
		return GetComponent<T>().has;
	}

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs)
	{
		auto& component = GetComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& GetComponent()
	{
		return std::get<T>(m_Components);
	}

	template <typename T>
	const T& GetComponent() const
	{
		return std::get<T>(m_Components);
	}


	template <typename T>
	void RemoveComponent()
	{
		GetComponent<T>() = T();
	}

};

