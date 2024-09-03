#include "game.h"
#include "imgui_internal.h"
#include <fstream>

Game::Game(const std::string& config)
{
	Init(config);
}

void Game::Init(const std::string& path)
{
	//TODO: read in config file here
	//      use the premade PlayerConfig, EnemyConfig, BulletConfig variables
	//std::ifstream fin(path);

	//fin >> m_PlayerConfig.SR;

	LoadConfig(path);

	//set up default window parametrs
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
				break;
			}

			/*if (type == "Circle")
			{
				std::string name;
				float x, y, sx, sy, rad;
				int r, g, b;

				ifs >> name >> x >> y >> sx >> sy >> r >> g >> b >> rad;

				CircleSettings cs(x, y, rad);
				cs.SetColor(r, g, b);
				cs.SetName(name);
				cs.SetSpeed(sx, sy);
				AddShape(cs);
				ifs >> type;
			}

			if (type == "Rectangle")
			{
				std::string name;
				float x, y, sx, sy;
				float width, height;
				int r, g, b;

				ifs >> name >> x >> y >> sx >> sy >> r >> g >> b >> width >> height;

				RectangleSettings rs(x, y, width, height);
				rs.SetColor(r, g, b);
				rs.SetName(name);
				rs.SetSpeed(sx, sy);
				AddShape(rs);
				ifs >> type;
			}

			if (type == "Font")
			{
				ifs >> settings.mFont.file >> settings.mFont.size;
				ifs >> settings.mFont.r >> settings.mFont.g >> settings.mFont.b;
				if (!myFont.loadFromFile(settings.GetFontFile()))
				{
					std::cerr << "Could not load font!\n";
					exit(-1);
				}
				ifs >> type;
			}
			*/
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
		//update EnityManager
		m_EntityManager.Update();

		//required update call to imgui
		ImGui::SFML::Update(m_Window, m_DeltaClock.restart());
		ImGui::ShowDemoWindow();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sGUI();
		sRender();

		//increment the current frame
		//may need to be moved when pause implemented
		m_CurrentFrame++;
	}
}

void Game::Update()
{

}

//respawn the  player in the middle of the screen
void Game::SpawnPlayer()
{
	//TODOL finish adding all properties of the player with the correct values from config

	//We create everyentity by calling EntityManager.AddEntity(tag)
	//This returns a std::shared_ptr<Entity>, so we use auto to save typing

	SPEntity entity = m_EntityManager.AddEntity("player");

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(m_Window.getSize().x / 2.0f, m_Window.getSize().y / 2.0f),
		Vec2(1.0f, 1.0f),
		Vec2(m_PlayerConfig.S, m_PlayerConfig.S),
		0.0f);

	//The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline
	entity->cShape = std::make_shared<CoShape>(m_PlayerConfig.SR, m_PlayerConfig.V,
		sf::Color(m_PlayerConfig.FR, m_PlayerConfig.FG, m_PlayerConfig.FB),
		sf::Color(m_PlayerConfig.OR, m_PlayerConfig.OG, m_PlayerConfig.OB),
		m_PlayerConfig.OT);

	//Add input coponent to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	//sinse we want this entity to be our player, set our Game's player variable to this Entity
	//This goes slightly against EntityManager paradism, but we use the player so much it's 
	m_Player = entity;
}

//spawn an enemy at a random position
void Game::SpawnEnemy()
{
	//TODO: make sure the enemy is spawned properly with the m_enemyConfig variable
	//	the enemy must be spawned completely within the bounds of the windows

	//record when the most recent enemy was spawned
	m_LastEnemySpawnedTime = m_CurrentFrame;
}

//spawn the small enemies when a big one (input entity e) explodes
void Game::SpawnSmallEnemies(SPEntity e)
{
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

void Game::sMovement()
{
	if (!m_Player->cShape)
		return;

	sf::CircleShape& circle = m_Player->cShape->m_Circle;

	float left = circle.getPosition().x - circle.getRadius();
	float right = circle.getPosition().x + circle.getRadius();
	float top = circle.getPosition().y - circle.getRadius();
	float botton = circle.getPosition().y + circle.getRadius();

	if (m_Player->cInput->m_Right && right + m_Player->cTransform->m_Speed.x < m_Window.getSize().x)
	{
		m_Player->cTransform->m_Pos.x += m_Player->cTransform->m_Speed.x;
	}

	if (m_Player->cInput->m_Left && left - m_Player->cTransform->m_Speed.x > 0.0f)
	{
		m_Player->cTransform->m_Pos.x -= m_Player->cTransform->m_Speed.x;
	}

	if (m_Player->cInput->m_Up && top - m_Player->cTransform->m_Speed.y > 0.0f)
	{
		m_Player->cTransform->m_Pos.y -= m_Player->cTransform->m_Speed.y;
	}

	if (m_Player->cInput->m_Down && botton + m_Player->cTransform->m_Speed.y < m_Window.getSize().y)
	{
		m_Player->cTransform->m_Pos.y += m_Player->cTransform->m_Speed.y;
	}

}

void Game::sCollision()
{
	for (auto bullet : m_EntityManager.GetEntities("bullet"))
	{

	}

	//TODO: implement all proper collisions between entities
	//	be sure to use the collision radius, Not the shape radius

}

void Game::sEnemySpawner()
{
	//TODO Implement enemy spawner
}

void Game::sLifeSpan()
{
	//TODO: Implement all lifespan functionallity
	//
	//for all entities
	// if entity has no lifespan component, skip it
	// if entity has > 0 remaning lifespan, substruct 1
	// if it has lifespan and is alive
	//	scale it's alpha chanel properly
	// if it has lifespan and it's timme is up
	//	destroy the entity

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
			ImguiDisplayEntities();

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

	static float speed[4] = { entity->cTransform->m_Speed.x, entity->cTransform->m_Speed.y, 0.0f, 0.0f };
	speed[0] = entity->cTransform->m_Speed.x;
	speed[1] = entity->cTransform->m_Speed.y;

	ImGui::DragFloat2("Speed", speed, 1.0f, 0.0f, 100.0f);
	entity->cTransform->m_Speed = Vec2(speed[0], speed[1]);
}

void Game::sRender()
{
	//TODO: change the code below to draw ALL of the entities
	//	sample drawing of the player Entity that we have created
	m_Window.clear();

	//set the position of the shape based on the entity's transform->pos
	m_Player->cShape->m_Circle.setPosition(m_Player->cTransform->m_Pos.x, m_Player->cTransform->m_Pos.y);

	//set the rotation of the shape based on the entity's transform->angle
	m_Player->cTransform->m_Angle += 1.0f;
	m_Player->cShape->m_Circle.setRotation((float)m_Player->cTransform->m_Angle);

	//draw the entity's sf::CircleShape
	m_Window.draw(m_Player->cShape->m_Circle);

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