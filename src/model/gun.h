#ifndef GUN_H_
#define GUN_H_

#include "bullet.h"

// pi greco
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// velocità di spostamento della canna del cannone
#define MOVIMENTO_CANNA 0.3
#define MIN_ALZO 0
#define MAX_ALZO 45
#define DECELERATION 0.995
#define GUN_FLOATING_SPEED 0.9

class Gun
{
public:
	Gun( float );
	virtual ~Gun();
	
	// metodi get
	float getX() const;
	float getZ() const;
	float getAlzo() const;
	float getDirezione() const;
	Bullet getBullet() const;
	
	// metodi di movimento
	void alzaTiro( float degree );
	void ruota( float degree );
	/**Muove il player lungo l'asse delle x*/
	void moveX( float x );
	/**Muove il player lungo l'asse delle z*/
	void moveZ( float z );
	/**Effettua lo spostamento del player secondo la velocita'*/
	void move( double elapsedTime );
	
	/**Spara un proiettile*/
	void fire();
	/**Effettua lo spostemento del proiettile, se presente e ritorna true *
	 * finchè il proiettile continua ad esistere */
	bool moveBullet( double elapsedTime );
	/**Controlla se il proiettile ha colpito il nemico selezionato*/
	bool hit( const Enemy& e );
	/**Controlla se il cannone ha proiettili attivi*/
	bool hasActiveBullet() const;
	

	
private:
	// campi dati
	float
	X, Z,           // posizione del cannone nel campo di gioco
 	xSpeed, zSpeed, // velocita' relativa agli assi x e z
	alzo,           // alzo in gradi centigradi della canna del cannone
	direzione,      // direzione in gradi verso cui il cannone punta
	fieldDimension; // dimensione del campo di gioco

	Bullet* bullet;     // puntatore al proiettile
};

#endif /*GUN_H_*/
