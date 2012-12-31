/***************************************************************
* Progetto di Informatica grafica e multimedialita' 2006/2007 *
* author: Dal Sasso Tommaso  n.matr.: 507276                  *
***************************************************************/

#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>


// includo i sorgenti degli elementi del gioco
#include "model/enemy.h"
#include "model/gun.h"
#include "view/camera.h"

// definisco le costanti del campo di gioco
// numero di nemici
#define FIELD_DIM 250
#define ENEMY_NUMBER 5


// definisco le variabili globali

static Gun* cannone;   // il (puntatore al) cannone
static Camera* telecamera;   // telecamera di gioco
static Enemy* evil[ ENEMY_NUMBER ];

int winW, winH;                // dimensione della finestra corrente
int mouseX = -1, mouseY = -1;  // ultima posizone del mouse
double currentTime, lastTime = 0;
bool lost = false;    // stato della partita
bool gamePaused = false;  // indica se il gioco è in pausa
bool blending = true; // alpha blending attivo

GLfloat lightPosition[4] = { 0.5, 1, 0, 0 };      //posizione luce

unsigned char
	texCannone[128*128*3],            // texture del cannone
	texGround [512*512*3],
	texWater  [512*512*3],
	texSky    [1024*512*3];

// includo l'implementazione della vista del campo di battaglia
#include "field.cpp"


// dichiarazione funzioni chiamate dal main 
	void changeSize( int, int );
	void draw();
	void keyboard( unsigned char, int, int );
	void mouse( int, int, int, int );
	void passiveMouse( int, int );
	void timeEffect();
	void gameLost();
	bool pauseGame();


	int main( int argc, char** argv ) {
		/* Carico le Texture */
		FILE *f_han;
		if ( (f_han = fopen("Texture/cocci_128x128_RGB.raw", "rb") ) != NULL) {
			fread(texCannone, 128*128*3, 1, f_han);
			fclose(f_han);
		}

		if ( (f_han = fopen("Texture/grass_512x512_RGB.raw", "rb") ) != NULL) {
			fread(texGround, 512*512*3, 1, f_han);
			fclose(f_han);
		}

		if ( (f_han = fopen("Texture/water_512x512_RGB.raw", "rb") ) != NULL) {
			fread(texWater, 512*512*3, 1, f_han);
			fclose(f_han);
		}
		
		if ( (f_han = fopen("Texture/clouds_1024x512_RGB.raw", "rb") ) != NULL) {
			fread(texSky, 1024*512*3, 1, f_han);
			fclose(f_han);
		}

		glutInit( &argc, argv ); // inizializzo la GUI
		glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
		glutInitWindowPosition( -1, -1 );
		glutInitWindowSize( 1000, 700 );
		glutCreateWindow( "Assalto alla Fortezza" );

		glutReshapeFunc( changeSize );
		glutDisplayFunc( draw );
		glutKeyboardFunc( keyboard );
		glutMouseFunc( mouse );
		glutPassiveMotionFunc( passiveMouse );
		glutIdleFunc( timeEffect );

// abilito il face culling
		glEnable( GL_CULL_FACE );
//abilito il test sullo z-buffer
		glEnable( GL_DEPTH_TEST );

// mipmap delle texture
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, texGround );
		
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, texWater );

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, 1024, 512, GL_RGB, GL_UNSIGNED_BYTE, texSky );

// abilito gli effetti luminosi
		glShadeModel( GL_SMOOTH );
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );

		GLfloat ambLight[4] = { 0.2, 0.2, 0.2, 1 };
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambLight ); // luce ambientale
// disabilita il rendering della luce per le facce non visualizzate
		glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, 0 );

// caratteristiche luminose della sorgente
		GLfloat ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
		GLfloat diffuse[4] = { 0.75f, 0.75f, 0.75f, 1.0f };
		GLfloat specular[4] = { 1, 1, 1, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
		glLightfv( GL_LIGHT0, GL_SPECULAR, specular );

		glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );  // posiziono la luce

// inizializzo il cannone
		cannone = new Gun( FIELD_DIM );
		telecamera = new Camera( cannone );

		Enemy::initialize();
// inizializzo l'array di Enemy
		for (int i=0; i<ENEMY_NUMBER; i++) {
			evil[i] = new Enemy( cannone );
			evil[i]->placeEnemy( (FIELD_DIM - FIELD_DIM/10) /2, FIELD_DIM /2 );
		}

// fullscreen!!!
		glutFullScreen();

// faccio sparire il mouse
		glutSetCursor( GLUT_CURSOR_NONE );

		glutMainLoop();

	}


// implementazione delle funzioni

	void changeSize( int w, int h ) {
		winW = w;
		winH = h;
		glViewport( 0, 0, w, h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 40, (double)w / (double)h, 1, 1000 );  // definisco la prospettive
	}


	void draw() {
		glClearColor( 0.5, 0.9, 1, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		telecamera->adjustView();
		telecamera->pointToGun();
		telecamera->refreshView();

// disegno il campo di gioco
		telecamera->gotoDefaultPosition();

		drawSky();
		drawGround();

		if( blending )
			drawReflectedEnemies();

		drawPlayer();
		drawEnemies();
		
		if ( lost ) {
			gameLost();
		}
		
		glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );  // riposiziono la luce
		glTranslatef( lightPosition[0], 4, lightPosition[2] );

		glutSwapBuffers();
	}


	void keyboard( unsigned char c, int x, int y ) {
		switch(c) {
			case 'w':
				cannone->alzaTiro( 1 );
				break;
			case 's':
				cannone->alzaTiro( -1 );
				break;
			case 'a':
				cannone->ruota( 1 );
				break;
			case 'd':
				cannone->ruota( -1 );
				break;

			case 'q':
				exit(0);

			case 'p':
				pauseGame();
				break;
			
			case 'b':
				blending = !blending;
				break;
		}

		glutPostRedisplay();
	}

	void mouse( int button, int state, int x, int y ) {
		if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )  // premuto il tasto sinistro
			cannone->fire();
	}


	/**Cattura i movimenti del mouse per associarli alla canna del cannone*/
	void passiveMouse( int x, int y ) {
// primo movimento del mouse: inizializzo la posizione
		if ( mouseX == -1 && mouseY == -1 ) {
			mouseX = x;
			mouseY = y;
		}

		cannone->ruota( mouseX - x );

		cannone->alzaTiro( mouseY -y );


		if ( x <= 0 || x >= winW -1 ) {
	// riposiziono il mouse al centro dello schermo
			mouseX = winW /2;
			mouseY = winH /2;
			glutWarpPointer( mouseX, mouseY );
		} else {
	// aggiorno l'ultima posizione del mouse
			mouseX = x;
			mouseY = y;
		}

		glutPostRedisplay();
	}


	/**Imposta o pone termine alla pause*/
	bool pauseGame() {
		if ( !gamePaused ) {
	// metto il gioco in pausa
			glutIdleFunc( NULL );
			gamePaused = true;
			return true;
		} else {  // tolgo il gioco dalla pausa
	// non considero il tempo passato in pausa
			lastTime = glutGet( GLUT_ELAPSED_TIME );
			glutIdleFunc( timeEffect );
			gamePaused = false;
			return false;
		}
	}


	/**Pone termine al gioco*/
	void gameLost() {
		glPushMatrix();       // PUSH
		glTranslatef ( cannone->getX(), 0, cannone->getZ() );

// imposto le caratteristiche luminose dell'"esplosione"
		float ambiente[4] = { 0.1, 0.0, 0.0, 1.0 },
diffuse[4] = { 0.9, 0.0, 0.0, 1.0 },
specular[4] = { 0.3, 0.0, 0.0, 1.0 };

glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
glMaterialfv( GL_FRONT, GL_SPECULAR, specular );

glutSolidSphere( 7, 32, 32 );   // effetto di esplosione (un po' grezzo)

glPopMatrix();        // POP
	}

/**Funzione invocata dalla glutIdleFunction che si occupa di eseguire gli *
* spostamenti degli oggetti sul tavolo                                   */
	void timeEffect() {
// salvo il tempo corrente
		currentTime = glutGet( GLUT_ELAPSED_TIME );
// ottengo il tempo passato dall'ultimo ciclo (in millisecondi)
		float elapsedTime = currentTime - lastTime;
// trasformo il tempo in qualcosa di più sensato dei millisecondi
		double usefulTime = elapsedTime /500;  // secondi
// aggiorno LastTime
		lastTime = currentTime;

		if ( cannone->moveBullet( usefulTime ) ) {    // muovo il proiettile del cannone
			for ( int i=0; i< ENEMY_NUMBER; i++ )			  // e controllo le collisioni
				if ( cannone->hit( *evil[i] ) ) {
				evil[i]->placeEnemy( (FIELD_DIM - FIELD_DIM/10) /2, FIELD_DIM /2 );
				break;
				}
		}
		for (int i=0; i<ENEMY_NUMBER; i++) {    // muovo i nemici
			evil[i]->move( usefulTime );
			if ( evil[i]->arrived() ) {
				glutIdleFunc( NULL );   // blocco lo scorrere del tempo
				lost = true;

			}
		}

		cannone->move( usefulTime );
		glutPostRedisplay();    // rendo effettivi gli spostementi
	}
