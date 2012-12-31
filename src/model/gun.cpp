#include "gun.h"
#include <cmath>

Gun::Gun( float fieldDimension )
:fieldDimension( fieldDimension ), X(fieldDimension/2), Z(fieldDimension/2),
xSpeed(0), zSpeed(0), alzo(MIN_ALZO), direzione(0), bullet(0) {}

Gun::~Gun() {
	if ( !bullet )      // se il cannone ha un proiettile attivo
		delete bullet;
}

//metodi get:
float Gun::getX() const { return X; }

float Gun::getZ() const { return Z; }

float Gun::getAlzo() const { return alzo; }

float Gun::getDirezione() const { return direzione; }

Bullet Gun::getBullet() const { return *bullet; }


/**Il metodo aumenta l'alzo del cannone, se possibile*/
void Gun::alzaTiro( float degree ) {
	alzo += degree * MOVIMENTO_CANNA /2;
		if ( alzo > MAX_ALZO )
			alzo = MAX_ALZO;
		if ( alzo < MIN_ALZO )
			alzo = MIN_ALZO;
}


void Gun::ruota( float degree ) {
	direzione += degree * MOVIMENTO_CANNA;
	
	if ( direzione >= 360 )
		direzione -= 360;
	
	if ( direzione <= 0 )
		direzione += 360;
}


void Gun::fire() {
	if ( !bullet ) {
		// sparo il proiettile
		bullet = new Bullet( getX(), 0.1, getZ(), alzo, direzione, 30 );
		
		// effetto del rinculo
		xSpeed += GUN_FLOATING_SPEED * sin( direzione *M_PI/180 );
		zSpeed += GUN_FLOATING_SPEED * cos( direzione *M_PI/180 );
		
	}
}


bool Gun::moveBullet( double elapsedTime ) {
	if ( bullet ) {
		if ( bullet->move( elapsedTime ) ) {
			return true;
		}
		else
			delete bullet;
			bullet = 0;
			return false;
	}
	return false;
}

bool Gun::hit( const Enemy& e ) {
	if ( bullet ) {
		if ( bullet->hit( e ) ) {
			delete bullet;
			bullet = 0;
			return true;
		}
	}
	return false;
}

bool Gun::hasActiveBullet() const { return bullet; }

void Gun::moveX(float x)
{
	if( X + x < fieldDimension && X + x > 0 )
		X += x;
}

void Gun::moveZ(float z)
{
	if( Z + z < fieldDimension && Z + z > 0 )
		Z += z;
}

void Gun::move( double elapsedTime )
{
	X -= xSpeed * elapsedTime;
	Z -= zSpeed * elapsedTime;
	
	if( X <= 0 ) {
		X = 0;
		xSpeed = -xSpeed;
	}
		
	if( X >= fieldDimension ) {
		X = fieldDimension;
		xSpeed = -xSpeed;
	}
	
	if( Z <= 0 ) {
		Z = 0;
		zSpeed = -zSpeed;
	}
	
	if( Z >= fieldDimension ) {
		Z = fieldDimension;
		zSpeed = -zSpeed;
	}
	
	xSpeed *= DECELERATION;
	zSpeed *= DECELERATION;
}
