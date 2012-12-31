#ifndef ENEMY_H_
#define ENEMY_H_

#include "gun.h"
#define ENEMY_RADIUS 2

/**Classe che rappresenta un singolo nemico*/
class Enemy
{
public:
	/**Costruisce un nemico di velocità s che si dirige verso il punto (toX,toY)*/
	Enemy( Gun* gun );
	virtual ~Enemy();
	
	// metodi di get
	float getX() const;
	float getZ() const;
	
	// altri metodi
	/**Avvia la generazione di numeri pseudocasuali per posizionare i nemici*/
	static void initialize();
	/**Avvicina il nemico al cannone e ritorna true se lo raggiunge*/
	bool move( double elapsedTime );
	/**Ritorna true se il nemico ha raggiunto il bersaglio*/
	bool arrived() const;
	/**Posiziona il nemico all'interno del campo da gioco in una posizione valida
	 * e gli assegna una nuova velocita' */
	void placeEnemy( float minDistance, float maxDistance );
	
private:
	float x, z;         // posizione del nemico nel piano
	double speed;        // velocità del nemico
	Gun* target;        // il bersaglio sul campo verso cui i nemici si spostano
};

#endif /*ENEMY_H_*/
