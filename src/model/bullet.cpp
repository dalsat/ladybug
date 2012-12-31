#include "bullet.h"
#include <cmath>
#include "enemy.h"


Bullet::Bullet( float startX, float startY, float startZ,
				float ang, float dir, float pow )
	:bullX(startX), bullY(startY), bullZ(startZ), alzo(ang),
	 direzione(dir), potenza(pow)
{
	// scompongo le componenti della velocita':
	// gittata = velocita' rispetto al piano
	float gittata = potenza * cos(alzo *M_PI/180);
	// velocita' rispetto ad Y
	VY = potenza * sin(alzo *M_PI/180);
	
	// velocita' rispetto ad X
	VX = gittata * sin(direzione *M_PI/180);
	// velocita' rispetto ad Z
	VZ = gittata * cos(direzione *M_PI/180);

}

Bullet::~Bullet() {}


// metodi get
float Bullet::getX() const { return bullX; }

float Bullet::getY() const { return bullY; }

float Bullet::getZ() const { return bullZ; }


// altri metodi
bool Bullet::move( double elapsedTime ) {
	bullX += (VX * elapsedTime);
	bullY += (VY * elapsedTime);
	bullZ += (VZ * elapsedTime);
	VY -= G * elapsedTime;
	return bullY > 0;
}

bool Bullet::hit(const Enemy& e) const {
	return (
		fabs( bullX - e.getX() ) < BULLET_RADIUS + ENEMY_RADIUS &&
		fabs( bullZ - e.getZ() ) < BULLET_RADIUS + ENEMY_RADIUS &&
		bullY < 5 );
}
