#include "Enemy.h"

#define DEFAULT_MACHINEGUNGUY_WIDTH		16
#define DEFAULT_MACHINEGUNGUY_HEIGHT	32

class MachineGunGuy :
	public Enemy
{


public:
	MachineGunGuy();
	MachineGunGuy(float x, float y);
	~MachineGunGuy();

	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt);

	void autoMove(float range);
};

