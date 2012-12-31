#include "enemy.h"
#include <cmath>
#include <ctime>
#include <cstdlib>

Enemy::Enemy( Gun* gun ) : speed(0), target(gun) {}

Enemy::~Enemy() {}


// metodi get
float Enemy::getX() const { return x; }

float Enemy::getZ() const { return z; }


// altri metodi
bool Enemy::move( double elapsedTime ) {
	// muovi
	float xComp = target->getX() - x;     // distanza dal target sull'asse x
	float zComp = target->getZ() - z;	  // distanza dal target sull'asse z
	
	float distanza = sqrt( xComp*xComp + zComp*zComp ); // distanza dal target
	
	// movimento sull'asse x
	x += ( xComp / distanza ) * speed * elapsedTime;
	z += ( zComp / distanza ) * speed * elapsedTime;
	
	return arrived();	
}
	
bool Enemy::arrived() const {
	// il nemico è arrivato sul bersaglio?
	return ( fabs( x - target->getX() ) < 1 && fabs( z - target->getZ() ) < 1 );
}


void Enemy::placeEnemy( float minDistance, float maxDistance ) {
	/* il nemico viene posizionato in una zona all'interno del campo da *
	 * gioco e all'esterno del quadrato di minDistance come semilato    */
	
	bool posizione = rand() %2;    /* se true parte dai bordi orizzonrali  *
								    * altrimenti parte dai bordi verticali */
	
	if ( posizione ) {			// bordi orizzontali: x qualunque valore, z
								// compreso nelle fasce [0..maxDistance-minDistance]
		x = rand() %(int)(maxDistance *2);	// e [maxDistance + minDistance..2*maxDistance]
		z = ( (minDistance+maxDistance) * (rand() %2) ) + rand() %(int)(maxDistance - minDistance);
	}
	else {
		x = ( (minDistance+maxDistance) * (rand() %2) ) + rand() %(int)(maxDistance - minDistance);
		z = rand() %(int)(maxDistance *2);
	}
	
	speed = ( rand() % 3 ) + 1; //(( rand() % 2 ) /3) +0.01;
}

void Enemy::initialize() { srand( time(0) ); }
