#include "Enemy.h"

#define DEFAULT_BAT_WIDTH	48
#define DEFAULT_BAT_HEIGHT	32

class Bat :
	public Enemy
{


public:
	Bat();
	Bat(float x, float y);
	~Bat();

	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt);

	void autoMove(float range);
};

