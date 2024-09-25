#include "../game.h"

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

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			/*if (m_CurrentScene()->GetActionMap().find(event.key.code)) == m_CurrentScene->GetActionMap().end())
			continue;

			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			m_CurrentScene()->DoAction(Action(m_CurrentScene()->GetActionMap().at(event.key.code), actionType));
			*/
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
				SpawnBullet(m_Player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ","
					<< event.mouseButton.y << ") \n";
				ActivateSuperpower(Vec2(event.mouseButton.x, event.mouseButton.y));
			}

		}
	}
}