#ifndef CAMERA_H_
#define CAMERA_H_

#include "../model/gun.h"

// altezza della telecamera dal campo di battaglia
#define CAMERA_HEIGHT 5

/**Questa classe rappresenta il punto di vista dell'osservatore*/
class Camera
{
public:
	Camera( Gun* g );
	virtual ~Camera();

	/**Metodo che porta la telecamera nella posizione iniziale*/
	void gotoDefaultPosition();
	/**Punta la camera verso la direzione del cannone*/
	void pointToGun();
	/**Aggiorna la vista dalla telecamera*/
	void refreshView();
	/**Posiziona la telecamera leggermente dietro al cannone*/
	void adjustView();

private:

	Gun* gun;             // il cannone a cui � linkata la telecamera

	float angolo,                     // l'angolo di vista
		  cameraX, cameraY, cameraZ;  // la posizione nello spazio della telecamera

};

#endif /*CAMERA_H_*/
