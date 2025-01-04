#include "../game.h"

#include <format>
#include "imgui_internal.h"

void Game::sGUI()
{
	ImGui::Begin("Geometry Wars");

	//ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	//if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	//{
		//if (ImGui::BeginTabItem("Entity Manager"))
		//{
			std::string msg = std::format("Entity amount {}", m_EntityManager.GetEntities().size());
			ImGui::Text(msg.c_str(), m_EntityManager.GetEntities().size());
			if (ImGui::Button("Spawn Enemy"))
				SpawnEnemy();
			
			ImguiDisplayEntities();

			//ImGui::EndTabItem();
		//}

		//ImGui::EndTabBar();
	//}

	ImGui::End();


	if (ImGui::Begin("System"))
	{
		static bool is_paused = m_IsPaused;
		ImGui::Checkbox("Pause flag", &is_paused);
		m_IsPaused = is_paused;

		static bool is_collision_enabled = m_CollisionDetection;
		ImGui::Checkbox("Enable collision", &is_collision_enabled);
		m_CollisionDetection = is_collision_enabled;

		static bool is_cmovement_enabled = m_MovementEnabled;
		ImGui::Checkbox("Enable movement", &is_cmovement_enabled);
		m_MovementEnabled = is_cmovement_enabled;

		static bool immortal_enemies = m_ImmortalEnemies;
		ImGui::Checkbox("Immortal enemies", &immortal_enemies);
		m_ImmortalEnemies = immortal_enemies;

		static int enemy_amount = m_MaxEntitiesAmount;
		ImGui::DragInt("Entity amount", &enemy_amount, 1, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
		m_MaxEntitiesAmount = enemy_amount;

		static float min_enemy_radius = m_MinEntityRadius;
		ImGui::DragFloat("Min Entity Radius", &min_enemy_radius, 0.1f, 0.1f, 50.0f);
		m_MinEntityRadius = min_enemy_radius;

		static int enemy_life_amount = m_EnemyConfig.L;
		ImGui::DragInt("Enemy life amount", &enemy_life_amount, 1, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
		m_EnemyConfig.L = enemy_life_amount;

		static int enemy_spawn_interval = m_EnemyConfig.SI;
		ImGui::DragInt("Enemy spawn interwal", &enemy_spawn_interval, 1, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
		m_EnemyConfig.SI = enemy_spawn_interval;

		if (m_Player->cSuperPower)
		{
			ImGui::SeparatorText("Superpower");

			static bool is_active = m_Player->cSuperPower->m_IsActive;
			ImGui::Checkbox("Is active", &is_active);
			m_Player->cSuperPower->m_IsActive = is_active;

			static int power_radius = m_Player->cSuperPower->m_Radius;
			power_radius = m_Player->cSuperPower->m_Radius;
			ImGui::DragInt("Power Radius", &power_radius, 1, 1, 500, "%d", ImGuiSliderFlags_AlwaysClamp);
			m_Player->cSuperPower->m_Radius = power_radius;
			m_Player->cSuperPower->m_Circle.setRadius(power_radius);

			static Vec2 superpower_pos = Vec2(m_Player->cSuperPower->m_Pos.x, m_Player->cSuperPower->m_Pos.y);
			superpower_pos = Vec2(m_Player->cSuperPower->m_Pos.x, m_Player->cSuperPower->m_Pos.y);
			ImGui::DragFloat2("Activation position", &superpower_pos.x, 1.0f, m_Player->cSuperPower->m_Radius, 2000.0f);
			m_Player->cSuperPower->m_Pos = superpower_pos;

			static int frames_remaining = m_Player->cSuperPower->m_Remaining;
			frames_remaining = m_Player->cSuperPower->m_Remaining;
			ImGui::DragInt("Frames Remaining", &frames_remaining, 1, 1, 2000, "%d", ImGuiSliderFlags_AlwaysClamp);
			m_Player->cSuperPower->m_Remaining = frames_remaining;

			static int frames_cooldown = m_SuperPowerCooldown;
			frames_cooldown = m_Player->cSuperPower->m_Cooldown;
			ImGui::DragInt("Frames Cooldown", &frames_cooldown, 1, 1, 2000, "%d", ImGuiSliderFlags_AlwaysClamp);
			m_Player->cSuperPower->m_Cooldown = frames_cooldown;
		}

	}

	ImGui::End();
}

void Game::ImguiDisplayEntities()
{
	EntityVec& entities = m_EntityManager.GetAllEntities();
	std::string name = std::format("all_entities {}", entities.size());
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		for (SPEntity e : entities)
		{
			ImguiDisplayEntity(e);
			ImGui::Spacing();
		}
	}

	for (std::pair<std::string, EntityVec> p : m_EntityManager.GetEntityMap())
	{
		std::string name = std::format("{} {}", p.first, p.second.size());
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			for (SPEntity e : p.second)
			{
				ImguiDisplayEntity(e);
			}

			ImGui::Spacing();
		}
	}	
}

struct ImguiEntity
{
	Vec2 pos{0.0f, 0.0f};
	float speed{0.0f};

	int score{ 0 };

	float color[3]  {0.0f, 0.0f, 0.0f };
	float outColor[3]{ 0.0f, 0.0f, 0.0f };

	ImguiEntity(){};

};

static std::map<size_t, ImguiEntity > s_imEntities;

void Game::ImguiDisplayEntity(SPEntity& entity)
{
	std::string name = std::format("{}", entity->GetId());

	ImguiEntity i_entity;
	s_imEntities[entity->GetId()] = i_entity;

	if (ImGui::TreeNode(name.c_str()))
	{
		sf::CircleShape& circle = entity->cShape->m_Circle;
					
		ImGui::SeparatorText("Transform");

		i_entity.pos =  Vec2(entity->cTransform->m_Pos.x, entity->cTransform->m_Pos.y);
		ImGui::DragFloat2("Position##", &i_entity.pos.x, 1.0f, circle.getRadius(), 2000.0f);
		entity->cTransform->m_Pos = i_entity.pos;

		i_entity.speed = entity->cTransform->m_Speed;
		ImGui::DragFloat("Speed##", &i_entity.speed, 0.5f, 1.f, 150.f, "%d", ImGuiSliderFlags_AlwaysClamp);
		entity->cTransform->m_Speed = i_entity.speed;

		if (entity->cScore)
		{
			ImGui::SeparatorText("Score");
			i_entity.score = entity->cScore->m_Score;
			ImGui::DragInt("Score##", &i_entity.score, 0, 1, 150, "%d", ImGuiSliderFlags_AlwaysClamp);
			entity->cScore->m_Score = i_entity.score;
		}

		ImGui::SeparatorText("Shape");

		i_entity.color[0] = circle.getFillColor().r / 255.0f;
		i_entity.color[1] = circle.getFillColor().g / 255.0f;
		i_entity.color[2] = circle.getFillColor().b / 255.0f;
			
		i_entity.outColor[0] = circle.getOutlineColor().r / 255.0f;
		i_entity.outColor[1] = circle.getOutlineColor().g / 255.0f;
		i_entity.outColor[2] = circle.getOutlineColor().b / 255.0f;

		ImGui::ColorEdit3("Color##", i_entity.color);
		ImGui::ColorEdit3("Outline color##", i_entity.outColor);

		circle.setFillColor(sf::Color(
			IM_F32_TO_INT8_UNBOUND(i_entity.color[0]),
			IM_F32_TO_INT8_UNBOUND(i_entity.color[1]), 
			IM_F32_TO_INT8_UNBOUND(i_entity.color[2])));

		circle.setOutlineColor(sf::Color(
			IM_F32_TO_INT8_UNBOUND(i_entity.outColor[0]),
			IM_F32_TO_INT8_UNBOUND(i_entity.outColor[1]), 
			IM_F32_TO_INT8_UNBOUND(i_entity.outColor[2])));

		static bool is_alive = entity->IsActive();
		ImGui::Checkbox("Is alive##", &is_alive);

		if(!is_alive)
		{
			entity->Destroy();
		}
		
		ImGui::TreePop();
	}
}