#include "Enemy.h"

#define DEFAULT_MACHINE_GUN_GUY_WIDTH	16
#define DEFAULT_MACHINE_GUN_GUY_HEIGHT	32
#define DEFAULT_MACHINE_GUN_GUY_VELOCITY	2

class MachineGunGuy :
	public Enemy
{


public:
	MachineGunGuy();
	MachineGunGuy(float x, float y);
	~MachineGunGuy();

	void Init();

	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt, GameObject &player);

	void autoMove(float range);
};

