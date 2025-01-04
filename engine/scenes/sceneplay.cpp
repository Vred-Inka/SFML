#include "sceneplay.h"
#include "../physics.h"
#include "../assets.h"
#include "../gameengine.h"
#include "../components.h"
#include "../action.h"
#include "../physics.h"


#include <iostream>
#include <fstream>

ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath)
	:Scene(gameEngine)
	,m_LevelPath(levelPath)
{
	Init(m_LevelPath);
}

void ScenePlay::Init(const std::string& levelPath)
{
	RegisterAction(sf::Keyboard::P, "PAUSE");
	RegisterAction(sf::Keyboard::Escape, "QUIT");
	RegisterAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	RegisterAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	RegisterAction(sf::Keyboard::G, "TOGGLE_GRID");

	RegisterAction(sf::Keyboard::A, "LEFT");
	RegisterAction(sf::Keyboard::D, "RIGHT");
	RegisterAction(sf::Keyboard::W, "JUMP");
	RegisterAction(sf::Keyboard::S, "DOWN");
	RegisterAction(sf::Keyboard::Space, "SHOOT");

	RegisterAction(sf::Keyboard::Left, "LEFT");
	RegisterAction(sf::Keyboard::Right, "RIGHT");
	RegisterAction(sf::Keyboard::Up, "UP");
	RegisterAction(sf::Keyboard::Down, "DOWN");

	//REgister all the other gameplay Actions

	m_GridText.setCharacterSize(12);
	m_GridText.setFont(m_Game->GetAssets().GetFont("Tech"));

	LoadLevel(levelPath);
}

Vec2 ScenePlay::GridToMidPixel(float x, float y, std::shared_ptr<Entity> entity)
{
	//TODO: This function takes in a grid (x,y) position and an entity
	// REturn a vec2 indicating where the center position of the entity should be
	// You must use the entity's Animation size to position it correctly
	// Size of the grid width and height and height is stored in m_GridSize.x/y
	// the bottom-left corner of the Aniation should align with the bottom left of the grif cell
	Vec2 pos (x * m_GridSize.x, m_Game->GetWindow().getSize().y - y * m_GridSize.y);
	
	if (entity->HasComponent<CAnimation>())
	{
		pos.x += entity->GetComponent<CAnimation>().m_Animation.GetSize().x / 2.0f;
		pos.y -= entity->GetComponent<CAnimation>().m_Animation.GetSize().y / 2.0f;
	}
	
	return pos;
}

void ScenePlay::LoadLevelFromConfig(const std::string& filename)
{
	std::ifstream ifs(filename);

	if (ifs.is_open()) {
		std::string type;
		ifs >> type;
		while (!ifs.eof())
		{
			if (type == "Tile" || type == "Dec")
			{
				// Tile/Dec N GX GY
				//	Animation name  N    std::string
				//	Grid X / Y      GX, GY  float, float

				std::string name;
				float x, y;
				ifs >> name >> x >> y;

				SPEntity tile = m_EntityManager.AddEntity(type);

				tile->AddComponent<CAnimation>(m_Game->GetAssets().GetAnimation(name), true);
				tile->AddComponent<CTransform>(GridToMidPixel(x,y, tile));

				if (type == "Tile")
				{
					tile->AddComponent<CBoundingBox>(m_Game->GetAssets().GetAnimation(name).GetSize());
				}

				ifs >> type;
			}

			if (type == "Player")
			{
				//Player GX GY CW CH SX SH SY SM GRW B
				//Grid X / Y        GX, GY  float, float
				//BoundingBox W / H CW, CH  float, float
				//Speed            SX    float
				//Jump speed       SY    float
				//MAX speed        SM    float
				//Gravity          GRW   float
				//Bullet animation B    float

				std::string fontName;
				std::string fontPath;
				ifs >> m_PlayerConfig.X >> m_PlayerConfig.Y;
				ifs >> m_PlayerConfig.W >> m_PlayerConfig.H;
				ifs >> m_PlayerConfig.SPEED >> m_PlayerConfig.JUMP >> m_PlayerConfig.MAXSPEED;
				ifs >> m_PlayerConfig.GRAVITY >> m_PlayerConfig.WEAPON;
				ifs >> type;
			}
		}
	}

	ifs.close();
}

void ScenePlay::LoadLevel(const std::string& filename)
{
	// reset the entity manager every time we load a level
	m_EntityManager = EntityManager();

	LoadLevelFromConfig(filename);
	SpawnPlayer();	
}

void ScenePlay::SpawnPlayer()
{
	m_Player = m_EntityManager.AddEntity("player");
	m_Player->AddComponent<CAnimation>(m_Game->GetAssets().GetAnimation("Run"), true);
	m_Player->AddComponent<CTransform>(GridToMidPixel(m_PlayerConfig.X, m_PlayerConfig.Y, m_Player));
	m_Player->AddComponent<CBoundingBox>(Vec2(m_PlayerConfig.W, m_PlayerConfig.H));
	m_Player->AddComponent<CGravity>(1.5f);
}

void ScenePlay::SpawnBullet()
{
//TODO
}

void ScenePlay::sDoAction(const Action& action)
{
	if (action.GetType() == "START")
	{
		if (action.GetName() == "TOGGLE_TEXTURE") { m_DrawTextures = !m_DrawTextures; }
		if (action.GetName() == "TOGGLE_COLLISION") { m_DrawCollision = !m_DrawCollision; }
		if (action.GetName() == "TOGGLE_GRID") { m_DrawGrid = !m_DrawGrid; }
		if (action.GetName() == "PAUSE") { SetPaused(!m_Paused); }
		if (action.GetName() == "QUIT") { OnEnd(); }
	}

	if (action.GetName() == "JUMP")
	{
		m_Player->GetComponent<CInput>().m_Jump = action.GetType() == "START" ? true : false;
	}

	if (action.GetName() == "UP")
	{
		m_Player->GetComponent<CInput>().m_Up = action.GetType() == "START" ? true : false;
	}

	if (action.GetName() == "DOWN")
	{
		m_Player->GetComponent<CInput>().m_Down = action.GetType() == "START" ? true : false;
	}

	if (action.GetName() == "LEFT")
	{
		m_Player->GetComponent<CInput>().m_Left = action.GetType() == "START" ? true : false;
	}	
	
	if (action.GetName() == "RIGHT")
	{
		m_Player->GetComponent<CInput>().m_Right = action.GetType() == "START" ? true : false;
	}

}

void ScenePlay::OnEnd()
{// m_Game->ChangeScene();
}

void ScenePlay::Update()
{
	if (!m_Paused)
	{
		m_Game->GetWindow().clear(sf::Color(100, 100, 255));
	}
	else
	{
		m_Game->GetWindow().clear(sf::Color(50, 50, 150));
	}

	m_EntityManager.Update();

	//TODO: pause functionality
	sMovement();

	//sLifeSpan();
	sCollision();
	sAnimation();
	sRender();
	m_Game->GetWindow().display();

}

static const float moveSpeed = 0.6f;
static const float maxSpeed = 4.0f;

void ScenePlay::sMovement()
{
	for (SPEntity& e : m_EntityManager.GetEntities("player"))
	{
		CTransform& ta = m_Player->GetComponent<CTransform>();

		bool direction = ta.m_Velocity.x > 0 ? true : false;
		if (!e->GetComponent<CState>().m_IsInAir)
		{
			ta.m_Velocity.y = 0.0f;
		}

		if (e->GetComponent<CInput>().m_Up == true)
		{
			ta.m_Velocity.y -= moveSpeed;
		}

		if (e->GetComponent<CInput>().m_Jump == true && !e->GetComponent<CState>().m_IsInAir)
		{
			ta.m_Velocity.y -= moveSpeed*30;
		}

		if (e->GetComponent<CInput>().m_Down == true)//s && !e->GetComponent<CState>().m_IsOnGround)
		{
			ta.m_Velocity.y += moveSpeed;
		}

		if (e->GetComponent<CInput>().m_Left == true)
		{
			ta.m_Velocity.x -= moveSpeed;
			e->GetComponent<CTransform>().m_Scale.x = -1;
		}
		else if(e->GetComponent<CInput>().m_Right == true)
		{
			ta.m_Velocity.x += moveSpeed;
			e->GetComponent<CTransform>().m_Scale.x = 1;
		}
		else
		{
			ta.m_Velocity.x *= 0.5f;
		}

		if (e->HasComponent<CGravity>() && e->GetComponent<CState>().m_IsInAir)
		{
			ta.m_Velocity.y += e->GetComponent<CGravity>().m_Gravity;
		}

		if (ta.m_Velocity.x > maxSpeed)
		{
			ta.m_Velocity.x = maxSpeed;
		}

		if (ta.m_Velocity.x < 0 && ta.m_Velocity.x < -maxSpeed)
		{
			ta.m_Velocity.x = -maxSpeed;
		}

		if (ta.m_Velocity.y > maxSpeed*10)
		{
			ta.m_Velocity.y = maxSpeed;
		}

		ta.m_Pos += ta.m_Velocity;

		if (ta.m_Pos.x < 0)
		{
			ta.m_Pos.x = 0;
		}


		if (e->HasComponent<CAnimation>())
		{
			if (!e->GetComponent<CState>().m_IsInAir)
			{
				if (ta.m_Velocity.x == 0)
				{
					if (e->GetComponent<CAnimation>().m_Animation.GetName() != "Stand")
					{
						e->AddComponent<CAnimation>(m_Game->GetAssets().GetAnimation("Stand"), true);
					}
				}
				else
				{
					if (e->GetComponent<CAnimation>().m_Animation.GetName() != "Run")
					{
						e->AddComponent<CAnimation>(m_Game->GetAssets().GetAnimation("Run"), true);
					}
				}
			}
		}
		
		// check max speed
	}

	


	//TODO: 
	// player movement / jumping based on it's CInput component
	// gravity effect
	// maximum player speed in both x and y
	


	//setting entities scale.x to 1/-1 will make face to the left/right
}

void ScenePlay::sLifeSpan()
{
}
void ScenePlay::sCollision()
{
	bool wasInAir = m_Player->GetComponent<CState>().m_IsInAir;
	bool isInAir = true;

	for (SPEntity& e : m_EntityManager.GetEntities("Tile"))
	{
		Vec2 overlap = Physics::GetOverlap(m_Player, e, *m_Game);

		if (overlap.x > 0 && overlap.y>0)
		{
			isInAir = false;
			//std::cout << "Collide" << overlap.x << " " << overlap.y << std::endl;

		}
	}

	m_Player->GetComponent<CState>().m_IsInAir = isInAir;

	if (isInAir == true && wasInAir != isInAir)
	{
		//m_Player->AddComponent<CAnimation>(m_Game->GetAssets().GetAnimation("Air"));
	}

	
	//if collide from air set state ground
	// if tile hited from bottom react
	// 
	
	// Remember :  SFML's (0.0) position is on the top-left corner
	//	that means junpin will have a negative y-component
	//	and gravity will have positive y-component
	// also, sometimes BElow something else will have y value greater that it
	// also, sometimes above something else will have y value less that it
	// 
	// implement Physics::getOverlap() and use here
	// get +y value and -y value when collide from top/buttom
	// 
	// implement bullet / tile collissions
	// destroy the title if it has a brick animation
	// 
	// implement player/ ile collision and resolutions
	// update the cState component of the player to store whether 
	// it is currently on the ground or in the air. This will be
	// used by the animation system
	// 
	// chack to see if the player has fallen down a hole (y> height())
	// 
	// do not let the player walk off the left side of the map
	// 
}
void ScenePlay::sAnimation()
{
	for (SPEntity& e : m_EntityManager.GetAllEntities())
	{
		CTransform& transform = e->GetComponent<CTransform>();

		if (e->HasComponent<CAnimation>())
		{
			e->GetComponent<CAnimation>().m_Animation.Update();
			m_Player->GetComponent<CAnimation>().m_Animation.GetSprite().setScale(transform.m_Scale.x, 1.0f);
		}
	}
}

void ScenePlay::sRender()
{
/*	if (!m_Paused)
	{
		m_Game->GetWindow().clear(sf::Color(100, 100, 255));
	}
	else
	{
		m_Game->GetWindow().clear(sf::Color(50, 50, 150));
	}
	*/

	Vec2& pos = m_Player->GetComponent<CTransform>().m_Pos;
	float windowCenterX = std::max(m_Game->GetWindow().getSize().x / 2.0f, pos.x);
	sf::View view = m_Game->GetWindow().getView();
	view.setCenter(windowCenterX, m_Game->GetWindow().getSize().y - view.getCenter().y);
	m_Game->GetWindow().setView(view);

	if (m_DrawTextures)
	{
		for (SPEntity& e: m_EntityManager.GetAllEntities())
		{
			CTransform& transform = e->GetComponent<CTransform>();

			if (e->HasComponent<CAnimation>())
			{
				Animation& anim = e->GetComponent<CAnimation>().m_Animation;
				if (anim.HasEnded())
				{
					//e->Destroy();
					continue;
				}

				anim.GetSprite().setRotation(transform.m_Angle);
				anim.GetSprite().setPosition(transform.m_Pos.x, transform.m_Pos.y);
				anim.GetSprite().setScale(transform.m_Scale.x, transform.m_Scale.y);
				m_Game->GetWindow().draw(anim.GetSprite());
			}
		}
	}

	if (m_DrawCollision)
	{
		for (SPEntity& e : m_EntityManager.GetAllEntities())
		{
			sf::Text text(e->GetTag(), m_Game->GetAssets().GetFont("Mario"));
			text.setCharacterSize(14);
			text.setFillColor(sf::Color::Red);
			Vec2 pos = e->GetComponent<CTransform>().m_Pos;
			CBoundingBox& bbox = e->GetComponent<CBoundingBox>();

			text.setPosition(pos.x - bbox.m_HalfSize.x, pos.y -7 );
			m_Game->GetWindow().draw(text);

			if (e->HasComponent<CBoundingBox>())
			{

				CTransform& transform = e->GetComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(bbox.m_Size.x - 1.f, bbox.m_Size.y - 1.0f));
				rect.setOrigin(sf::Vector2f(bbox.m_HalfSize.x, bbox.m_HalfSize.y));
				rect.setPosition(transform.m_Pos.x, transform.m_Pos.y);
				rect.setFillColor(sf::Color(0,0,0,0));
				rect.setOutlineColor(sf::Color(255,255,255,255));
				rect.setOutlineThickness(1);
				m_Game->GetWindow().draw(rect);
			}
		}
	}

	if (m_DrawGrid)
	{
		//DrawLine(Vec2(0.f, 0.0f), Vec2(500.f, 500.0f));


		float leftX = m_Game->GetWindow().getView().getCenter().x - GetWidth() / 2.0f;
		float rightX = leftX + GetWidth() + m_GridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_GridSize.x);

		for (float x = nextGridX; x < rightX; x += m_GridSize.x)
		{
			DrawLine(Vec2(x, 0.0f), Vec2(x, GetHeight()));
		}

		for (float y = 0.0f; y  < GetHeight(); y += m_GridSize.y)
		{
			DrawLine(Vec2(leftX, GetHeight() - y), Vec2(rightX, GetHeight() - y));

			for (float x = nextGridX; x < rightX; x += m_GridSize.x)
			{
				std::string xCell = std::to_string((int)x/(int)m_GridSize.x);
				std::string yCell = std::to_string((int)y/(int)m_GridSize.y);
				m_GridText.setString("(" + xCell + "," +yCell +")");
				m_GridText.setPosition(x+3, GetHeight() - y - m_GridSize.y + 2);
				m_Game->GetWindow().draw(m_GridText);
			}
		}
	}

	//ImGui::SFML::Render(m_Window);

	//m_Game->GetWindow().display();
}