#include "game.h"
#include "imgui_internal.h"
#include <fstream>
#include <ctime>

#include <format>

Game::Game(const std::string& config)
{
	Init(config);
}

void Game::Init(const std::string& path)
{
	LoadConfig(path);

	m_Window.create(sf::VideoMode(m_SystemConfig.H, m_SystemConfig.W), "Game 2.0");
	m_Window.setFramerateLimit(m_SystemConfig.FPS);

	if (!m_Font.loadFromFile(m_SystemConfig.Font.file))
	{
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	ImGui::SFML::Init(m_Window, true);

	SpawnPlayer();
}


void Game::LoadConfig(const std::string& path)
{
	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::string type;
		ifs >> type;
		while (!ifs.eof())
		{
			if (type == "Window")
			{
				ifs >> m_SystemConfig.H >> m_SystemConfig.W >> m_SystemConfig.FPS;
				int fullScreen;
				ifs >> fullScreen;
				m_SystemConfig.FS = fullScreen > 0 ? true : false;
				ifs >> type;
			}

			if (type == "Font")
			{
				ifs >> m_SystemConfig.Font.file >> m_SystemConfig.Font.S;
				ifs >> m_SystemConfig.Font.R >> m_SystemConfig.Font.G >> m_SystemConfig.Font.B;				
				ifs >> type;
			}
			
			if (type == "Player")
			{
				ifs >> m_PlayerConfig.SR >> m_PlayerConfig.CR >> m_PlayerConfig.S;
				ifs >> m_PlayerConfig.FR >> m_PlayerConfig.FG >> m_PlayerConfig.FB;
				ifs >> m_PlayerConfig.OR >> m_PlayerConfig.OG >> m_PlayerConfig.OB;
				ifs >> m_PlayerConfig.OT >> m_PlayerConfig.V;

				ifs >> type;
			}

			if (type == "Enemy")
			{
				ifs >> m_EnemyConfig.SR >> m_EnemyConfig.CR;
				ifs >> m_EnemyConfig.SMIN >> m_EnemyConfig.SMAX;
				ifs >> m_EnemyConfig.OR >> m_EnemyConfig.OG >> m_EnemyConfig.OB;
				ifs >> m_EnemyConfig.OT >> m_EnemyConfig.VMIN >> m_EnemyConfig.VMAX;
				ifs >> m_EnemyConfig.L >> m_EnemyConfig.SI;

				ifs >> type;
			}			

			if (type == "Bullet")
			{
				ifs >> m_BulletConfig.SR >> m_BulletConfig.CR >> m_BulletConfig.S;
				ifs >> m_BulletConfig.FR >> m_BulletConfig.FG >> m_BulletConfig.FB;
				ifs >> m_BulletConfig.OR >> m_BulletConfig.OG >> m_BulletConfig.OB;
				ifs >> m_BulletConfig.OT >> m_BulletConfig.V >> m_BulletConfig.L;

				ifs >> type;
				break;
			}
		}
	}
	ifs.close();
}

void Game::Run()
{
	//TODO:  add pause functionality in here
	// some systems should function while paused (rendering)
	// some systems shouldn't (movement/input)

	while (m_IsRunning)
	{
		Update();
		m_CurrentFrame++;
	}
}

void Game::Update()
{
	m_EntityManager.Update();

	ImGui::SFML::Update(m_Window, m_DeltaClock.restart());
	ImGui::ShowDemoWindow();

	sEnemySpawner();
	sMovement();
	sCollision();
	sUserInput();
	sLifeSpan();
	sGUI();
	sRender();
}

//respawn the  player in the middle of the screen
void Game::SpawnPlayer()
{
	SPEntity entity = m_EntityManager.AddEntity("player");

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(m_Window.getSize().x / 2.0f, m_Window.getSize().y / 2.0f),
		Vec2(m_PlayerConfig.S, m_PlayerConfig.S),
		m_PlayerConfig.S,
		0.0f);

	entity->cShape = std::make_shared<CoShape>(m_PlayerConfig.SR, m_PlayerConfig.V,
		sf::Color(m_PlayerConfig.FR, m_PlayerConfig.FG, m_PlayerConfig.FB),
		sf::Color(m_PlayerConfig.OR, m_PlayerConfig.OG, m_PlayerConfig.OB),
		m_PlayerConfig.OT);

	entity->cCollision = std::make_shared<CCollision>(m_PlayerConfig.CR);

	entity->cInput = std::make_shared<CInput>();
	m_Player = entity;
}

template <typename T>
T get_random(T min, T max)
{
	static std::default_random_engine e;
	std::uniform_real_distribution<> dis(min, max+1); // range [0, 1)
	return dis(e);
}

void Game::SpawnEnemy()
{
	SPEntity entity = m_EntityManager.AddEntity("enemy");

	int x = get_random(m_EnemyConfig.SR, (int)m_Window.getSize().x - m_EnemyConfig.SR);
	int y = get_random(m_EnemyConfig.SR, (int)m_Window.getSize().y - m_EnemyConfig.SR);
	float speed = get_random(m_EnemyConfig.SMIN, m_EnemyConfig.SMAX);
	float angel = get_random(0.0f, 1.0f);

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(x, y),
		//Vec2(1.0f, 1.0f),
		Vec2(speed, speed),
		speed,
		angel);

	int v = get_random(m_EnemyConfig.VMIN, m_EnemyConfig.VMAX);
	sf::Color color(get_random(0,255), get_random(0, 255), get_random(0, 255));
	entity->cShape = std::make_shared<CoShape>(
		m_EnemyConfig.SR, 
		v,
		color,
		sf::Color(m_EnemyConfig.OR, m_EnemyConfig.OG, m_EnemyConfig.OB),
		m_EnemyConfig.OT);

	entity->cLifeSpan = std::make_shared<CLifeSpan>(m_EnemyConfig.L);
	entity->cCollision = std::make_shared<CCollision>(m_EnemyConfig.CR);

	//record when the most recent enemy was spawned
	m_LastEnemySpawnedTime = m_CurrentFrame;
}

//spawn the small enemies when a big one (input entity e) explodes
void Game::SpawnSmallEnemies(SPEntity& e)
{
	if (e->GetTag() == "small_enemy")
		return;

	int v = e->cShape->m_Circle.getPointCount();
	sf::CircleShape& circle = e->cShape->m_Circle;

	for (size_t i = 0; i < circle.getPointCount(); i++)
	{
		SPEntity entity = m_EntityManager.AddEntity("small_enemy");
		float angel = (1.0f / v) * i;

		//Vec2 d{ e->cTransform->m_Pos.x - cTransform->m_Pos.x,
		//	e->cTransform->m_Pos.y - cTransform->m_Pos.y };

		//float r = cCollision->m_Radius + e.cCollision->m_Radius;
		//double at = atan2f(d.y, d.x);

		//return d.x * d.x + d.y * d.y < r * r;

		//vec2 velocity{e->cTransform->m_Speed * cos(),e->cTransform->m_Speed * sin()}


		entity->cTransform = std::make_shared<CTransform>(
			e->cTransform->m_Pos,
			//e->cTransform->m_Scale,
			e->cTransform->m_Velocity,
			e->cTransform->m_Speed,
			angel);

		entity->cShape = std::make_shared<CoShape>(
			circle.getRadius() / 2,
			v,
			circle.getFillColor(),
			circle.getFillColor(),
			circle.getOutlineThickness());

		if (e->cCollision)
		{
			entity->cCollision = std::make_shared<CCollision>(e->cCollision->m_Radius / 2);
		}

		if (e->cLifeSpan)
		{
			entity->cLifeSpan = std::make_shared<CLifeSpan>(e->cLifeSpan->m_Total);
		}
	}
	//when we create the smaller enemy, we have to read the values of the original
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points of the original enemy
}

//spawns a bullet from given entity to a target location
void Game::SpawnBullet(SPEntity& entity, const Vec2& target)
{
	//TODO: implement the spawning of a bullet which travels toward target
	//	- bullet speed is given as a scalar speed
	//	- you must set the velocity by using formula in notes
}

void Game::SpawnSpecialWeapon(SPEntity)
{
	//TODO: implement
}

void Game::UpdatePlayerMove()
{
	if (!m_Player->cShape)
		return;

	sf::CircleShape& circle = m_Player->cShape->m_Circle;

	float left = circle.getPosition().x - circle.getRadius();
	float right = circle.getPosition().x + circle.getRadius();
	float top = circle.getPosition().y - circle.getRadius();
	float botton = circle.getPosition().y + circle.getRadius();

	if (m_Player->cInput->m_Right && right + m_Player->cTransform->m_Velocity.x < m_Window.getSize().x)
	{
		m_Player->cTransform->m_Pos.x += m_Player->cTransform->m_Velocity.x;
	}

	if (m_Player->cInput->m_Left && left - m_Player->cTransform->m_Velocity.x > 0.0f)
	{
		m_Player->cTransform->m_Pos.x -= m_Player->cTransform->m_Velocity.x;
	}

	if (m_Player->cInput->m_Up && top - m_Player->cTransform->m_Velocity.y > 0.0f)
	{
		m_Player->cTransform->m_Pos.y -= m_Player->cTransform->m_Velocity.y;
	}

	if (m_Player->cInput->m_Down && botton + m_Player->cTransform->m_Velocity.y < m_Window.getSize().y)
	{
		m_Player->cTransform->m_Pos.y += m_Player->cTransform->m_Velocity.y;
	}
}

void Game::UpdateEntitiesMove()
{
	for ( SPEntity& e : m_EntityManager.GetEntitiesN())
	{
		if (e->GetTag() == "player")
			continue;

		UpdateEnemyMove(e);
	}
}

void Game::UpdateEnemyMove(SPEntity& e)
{
	if (!e->cShape)
		return;

	sf::CircleShape& circle = e->cShape->m_Circle;

	float left = circle.getPosition().x - circle.getRadius();
	float right = circle.getPosition().x + circle.getRadius();
	float top = circle.getPosition().y - circle.getRadius();
	float botton = circle.getPosition().y + circle.getRadius();

	if (left <= 0 || right >= m_Window.getSize().x)
	{
		e->cTransform->m_Velocity.x *= -1.0f;
	}

	if (top <= 0 || botton >= m_Window.getSize().y)
	{
		e->cTransform->m_Velocity.y *= -1.0f;
	}

	e->cTransform->m_Pos.x += e->cTransform->m_Velocity.x;
	e->cTransform->m_Pos.y -= e->cTransform->m_Velocity.y;
}

void Game::sMovement()
{
	UpdatePlayerMove();
	UpdateEntitiesMove();
}

void Game::sCollision()
{
	for (SPEntity& e : m_EntityManager.GetEntitiesN())
	{
		if (e->GetTag() == "player")
			continue;
		
		if (!e->IsActive())
			continue;

		if (m_Player->HasCollision(*e))
		{
			e->Destroy();
			SpawnSmallEnemies(e);
		}
	}


	for (auto bullet : m_EntityManager.GetEntities("bullet"))
	{

	}

	//TODO: implement all proper collisions between entities
	//	be sure to use the collision radius, Not the shape radius

}

void Game::sEnemySpawner()
{
	if (m_IsPaused)
		return;

	if ( m_EntityManager.GetEntities().size() >= m_MaxEntitiesAmount )
		return;

	if (m_CurrentFrame - m_LastEnemySpawnedTime < m_EnemyConfig.SI)
		return;

	SpawnEnemy();
}

void Game::UpdateLife(SPEntity& e)
{
	if (!e->IsActive())
		return;

	if (!e->cLifeSpan)
		return;

	e->cLifeSpan->m_Remaining--;

	if (e->cLifeSpan->m_Remaining == 0)
	{
		e->Destroy();
	}

	float opacity = 255 * ((float)e->cLifeSpan->m_Remaining / (float)e->cLifeSpan->m_Total);

	sf::Color fillcolor = e->cShape->m_Circle.getFillColor();
	fillcolor.a = opacity;
	e->cShape->m_Circle.setFillColor(fillcolor);

	sf::Color outlinecolor = e->cShape->m_Circle.getFillColor();
	outlinecolor.a = opacity;	
	e->cShape->m_Circle.setOutlineColor(outlinecolor);
}

void Game::sLifeSpan()
{
	for (SPEntity& e : m_EntityManager.GetEntitiesN())
	{
		UpdateLife(e);
	}
}

void Game::sGUI()
{
	ImGui::Begin("Geometry Wars");

	//ImguiDisplayEntity(m_Player);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Entity Manager"))
		{
			std::string msg = std::format("Entity amount {}", m_EntityManager.GetEntities().size());
			ImGui::Text(msg.c_str(), m_EntityManager.GetEntities().size());
			ImguiDisplayEntities();

			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("System"))
		{
			static bool is_paused = m_IsPaused;
			ImGui::Checkbox("Pause flag", &is_paused);
			m_IsPaused = is_paused;

			static int enemy_amount = m_MaxEntitiesAmount;
			ImGui::DragInt("Entity amount", &enemy_amount, 1, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
			m_MaxEntitiesAmount = enemy_amount;

			static int enemy_life_amount = m_EnemyConfig.L;
			ImGui::DragInt("Enemy life amount", &enemy_life_amount, 1, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
			m_EnemyConfig.L = enemy_amount;

			static int enemy_spawn_interval = m_EnemyConfig.SI;
			ImGui::DragInt("Enemy spawn interwal", &enemy_spawn_interval, 1, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
			m_EnemyConfig.SI = enemy_spawn_interval;

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Game::ImguiDisplayEntities()
{
	for (SPEntity e : m_EntityManager.GetEntitiesN())
	{
		if (ImGui::TreeNode(e->GetTag().c_str()))
		{
			ImguiDisplayEntity(e);
			ImGui::TreePop();
		}
	}
}

void Game::ImguiDisplayEntity(SPEntity& entity)
{
	//IMGUI_DEMO_MARKER("Widgets/Basic/ColorEdit3, ColorEdit4");
	sf::CircleShape& circle = entity->cShape->m_Circle;
	static float col1[3] = { circle.getFillColor().r / 255.0f, circle.getFillColor().g / 255.0f, circle.getFillColor().b/255.0f };
	static float col2[4] = { circle.getOutlineColor().r / 255.0f, circle.getOutlineColor().g / 255.0f, circle.getOutlineColor().b / 255.0f };
	ImGui::ColorEdit3(entity->GetTag().c_str(), col1 );	
	ImGui::ColorEdit3("id", col2);

	circle.setFillColor(sf::Color(IM_F32_TO_INT8_UNBOUND(col1[0]), IM_F32_TO_INT8_UNBOUND(col1[1]), IM_F32_TO_INT8_UNBOUND(col1[2])));
	circle.setOutlineColor(sf::Color(IM_F32_TO_INT8_UNBOUND(col2[0]), IM_F32_TO_INT8_UNBOUND(col2[1]), IM_F32_TO_INT8_UNBOUND(col2[2])));

	ImGui::SeparatorText("Transform");
	static float position[4] = { entity->cTransform->m_Pos.x, entity->cTransform->m_Pos.y, 0.0f, 0.0f };
	position[0] = entity->cTransform->m_Pos.x;
	position[1] = entity->cTransform->m_Pos.y;

	ImGui::DragFloat2("Position", position, 1.0f, circle.getRadius(), 2000.0f);
	entity->cTransform->m_Pos = Vec2(position[0], position[1]);

	static float speed[4] = { entity->cTransform->m_Velocity.x, entity->cTransform->m_Velocity.y, 0.0f, 0.0f };
	speed[0] = entity->cTransform->m_Velocity.x;
	speed[1] = entity->cTransform->m_Velocity.y;

	ImGui::DragFloat2("Speed", speed, 1.0f, 0.0f, 100.0f);
	entity->cTransform->m_Velocity = Vec2(speed[0], speed[1]);
}

void Game::DrawEntity(SPEntity& e)
{
	e->cShape->m_Circle.setPosition(e->cTransform->m_Pos.x, e->cTransform->m_Pos.y);
	e->cTransform->m_Angle += 1.0f;
	e->cShape->m_Circle.setRotation((float)e->cTransform->m_Angle);

	m_Window.draw(e->cShape->m_Circle);
}

void Game::sRender()
{
	m_Window.clear();

	DrawEntity(m_Player);

	for (SPEntity& e : m_EntityManager.GetEntitiesN())
	{
		DrawEntity(e);
	}

	//draw the ui last
	ImGui::SFML::Render(m_Window);

	m_Window.display();
}

void Game::sUserInput()
{
	//TODO:  handle the user input here
	//	note tthat you should only be setting the player's input component variables here
	//	you should not implement the player's movement logic here
	//	the movement system will read the variables you set in this function

	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		//pass the event to imgui to be passed
		ImGui::SFML::ProcessEvent(m_Window, event);

		//this event triggers when the windows is closed
		if (event.type == sf::Event::Closed)
		{
			m_IsRunning = false;
		}

		//this event is triggered when key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_Player->cInput->m_Up = true;
				std::cout << "W Key Pressed\n";
				break;
			case sf::Keyboard::S:
				m_Player->cInput->m_Down = true;
				std::cout << "S Key Pressed\n";
				break;
			case sf::Keyboard::D:
				m_Player->cInput->m_Right = true;
				std::cout << "D Key Pressed\n";
				break;
			case sf::Keyboard::A:
				m_Player->cInput->m_Left = true;
				std::cout << "A Key Pressed\n";
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_Player->cInput->m_Up = false;
				std::cout << "W Key Released\n";
				break;
			case sf::Keyboard::S:
				m_Player->cInput->m_Down = false;
				std::cout << "S Key Released\n";
				break;
			case sf::Keyboard::D:
				m_Player->cInput->m_Right = false;
				std::cout << "D Key Released\n";
				break;
			case sf::Keyboard::A:
				m_Player->cInput->m_Left = false;
				std::cout << "A Key Released\n";
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			//this line ignores mouse events if Imgui is the being clicked
			if (ImGui::GetIO().WantCaptureMouse)
				continue;

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ","
					<< event.mouseButton.y << ") \n";
				//call spawn bullet here
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ","
					<< event.mouseButton.y << ") \n";
				//call spawn special weapon here
			}

		}
	}
}