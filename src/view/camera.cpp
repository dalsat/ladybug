#include "camera.h"
#include <GL/glut.h>

// costruttore e distruttore
Camera::Camera( Gun* g ): gun(g), cameraX( g->getX() ),
	cameraY( CAMERA_HEIGHT ), cameraZ( g->getZ() ), angolo(0) {}

Camera::~Camera() {}


// metodi della classe
void Camera::gotoDefaultPosition() {
	//glTranslatef( 0, -5,-150 );
	cameraX = gun->getX();
//	cameraY = gun->getY();
	cameraZ = gun->getZ();
	glTranslatef( -cameraX, -cameraY, -cameraZ );
}

void Camera::pointToGun() {
	angolo = gun->getDirezione() + 180;
}


void Camera::refreshView() {
	glRotatef( -angolo, 0, 1, 0 );
}

void Camera::adjustView() {
	glTranslatef( 0, 0, -20 );   // mi porto dietro il cannone
	glRotatef( 10, 1, 0, 0 );    // inclino un po' l'angolo di visuale
}
