#include "../game.h"
#include <fstream>

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
			}
		}
	}
	ifs.close();
}
