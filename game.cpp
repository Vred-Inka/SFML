#include "game.h"

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

	//set up default window parametrs
	m_Window.create(sf::VideoMode(1280, 720), "Game 2.0");
	m_Window.setFramerateLimit(60);

	ImGui::SFML::Init(m_Window, true);

	SpawnPlayer();
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

	//Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and
	entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f), Vec2(1.0f, 1.0f), 0.0f);

	//The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline
	entity->cShape = std::make_shared<CoShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255,0,0), 3.0f);

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
	//TODO: implement all movement in this function
	//	you should read the m_player->cInput component to determine if the player is moving

	//sample movement speed update
	m_Player->cTransform->m_Pos.x += m_Player->cTransform->m_Speed.x;
	m_Player->cTransform->m_Pos.y += m_Player->cTransform->m_Speed.y;
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

	ImGui::Text("Stuff goes here");

	ImGui::End();
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
				std::cout << "W Key Pressed\n";
				//TODO: set player's input component "up" to true
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W Key Released\n";
				//TODO: set player's input component "up" to false
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