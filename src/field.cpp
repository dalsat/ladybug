/* file di rappresentazione del campo di gioco: *
* costruisce la vista degli elementi di gioco  */

// parametri del campo di gioco

// dimensione del campo di gioco
//#define QUAD_DIM 70 //175
//#define QUADS_N 3

// valori di blending
#define WATER_BLEND 0.6f
#define WATER_DEPTH 3
#define REFLECTION_BLEND 0.3f

#include <cmath>
//#include <iostream>
//using namespace std;

// variabili globali di utilita'
static const int quads_n = 3;
static bool isCannaAllocated = false;
static bool isInit = false;
static GLUquadricObj * canna;

// funzioni del campo di gioco

/**
 * Disegna un piano con la texture data
 * 
 */
void drawPlane( unsigned char* texture, int texX, int texY )
{
	// *** IMPOSTAZIONI ***
	
	// imposto i parametri della texture
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE/*MODULATE*/ );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texX, texY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );
	
	glEnable( GL_TEXTURE_2D );
	
	// *** FINE IMPOSTAZIONI ***

	float quadDimension = (float)FIELD_DIM /(quads_n -1);
	
	// *** DISEGNO ***
		
	glPushMatrix();                      // PUSH
	
	glBegin( GL_QUADS );     // disegno il terreno
	
	// imposto le normali per il terreno
	glNormal3f( 0, 1, 0 );

	for ( int i=0; i<FIELD_DIM; i+=quadDimension )
		for ( int j=0; j<FIELD_DIM; j+=quadDimension ) {
			glTexCoord2f( 0, 0 );
			glVertex3f( i, 0, j );
			glTexCoord2f( 1, 0 );
			glVertex3f( i, 0, j+quadDimension );
			glTexCoord2f( 1, 1 );
			glVertex3f( i+quadDimension, 0, j+quadDimension );
			glTexCoord2f( 0, 1 );
			glVertex3f( i+quadDimension, 0, j );
		}
	glEnd();
	
	glPopMatrix();                       // POP
	
	// *** FINE DISEGNO ***
	
	glDisable( GL_TEXTURE_2D );
}

/**Disegna il terreno*/
void drawGround()
{
	
	// imposto le caratteristiche luminose del materiale
	float
	ambiente[4] = { 0.0, 0.1, 0.0, 1.0 },
 	diffuse[4] = { 0.0, 0.7, 0.0, 1.0 },
 	specular[4] = { 0.0, 0.1, 0.0, 1.0 };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );

	if( blending ) {
		// ** disegno il fondale dello stagno
		glPushMatrix();         // PUSH
		glTranslatef( 0, -WATER_DEPTH, 0 );
		drawPlane( texGround, 512, 512 );
		glPopMatrix();		// POP
		
		// ** disegno i bordi dello stagno
		float quadDimension = (float)FIELD_DIM /(quads_n -1);
		glPushMatrix();                      // PUSH
		
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texGround );
		
		glEnable( GL_TEXTURE_2D );
		glDisable( GL_CULL_FACE );

		glNormal3f( -1, 0, 0 );
		
		for( int i=0; i<4; i++ ) { // disegno i 4 bordi
			// disegno un singolo bordo
			glBegin( GL_QUADS );
			for( int j=0; j<FIELD_DIM; j+=quadDimension ) {
				glTexCoord2f( 0, 0 );
				glVertex3f( 0, 0, j );
				glTexCoord2f( 1, 0 );
				glVertex3f( 0, 0, j+quadDimension );
				glTexCoord2f( 1, 1 );
				glVertex3f( 0, -WATER_DEPTH, j+quadDimension );
				glTexCoord2f( 0, 1 );
				glVertex3f( 0, -WATER_DEPTH, j );
			}
			glEnd();
			glRotatef( 90, 0, 1, 0 );
			glTranslatef( -FIELD_DIM, 0, 0 );
		}
		
		glEnable( GL_CULL_FACE );
		glDisable( GL_TEXTURE_2D );
		glPopMatrix();                       // POP
		
		glEnable( GL_BLEND );
	} // END_IF( blending )
	
	// ** disegno la superficie dello stagno
	// reimposto le reazioni luminose dei materiali
	ambiente[3] = WATER_BLEND;
	diffuse [3] = WATER_BLEND;
	specular[3] = WATER_BLEND;
	
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	
	drawPlane( texWater, 512, 512 );

	glDisable( GL_BLEND );


}


/**Disegno il cielo*/
void drawSky()
{
	glPushMatrix();                      // PUSH

// imposto il materiale e colore del cielo
	float
	ambienteSky[4] = { 0.5, 0.8, 1.0, 1.0 },
	diffuseSky[4] = { 0.5, 0.5, 0.1, 1.0 },
	specularSky[4] = { 0.0, 0.0, 0.0, 1.0 };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambienteSky );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuseSky );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specularSky );

	GLfloat
	s_vect[4] = { 0, 0.5, 0, 0 },
	t_vect[4] = { 0.5, 0, 0, 0 };

	// imposto la texture del cielo
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texSky);
	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGenfv( GL_S, GL_OBJECT_PLANE, s_vect );
	glTexGenfv( GL_T, GL_OBJECT_PLANE, t_vect );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glEnable( GL_TEXTURE_2D );

	// rendo visibile l'interno della sfera
	glFrontFace( GL_CW );
	glTranslatef( FIELD_DIM /2, 0, FIELD_DIM /2 );
// 	glEnable( GL_AUTO_NORMAL );

	glScalef( FIELD_DIM, FIELD_DIM, FIELD_DIM );
	glutSolidSphere( 1, 32, 32 );
//  	glDisable( GL_AUTO_NORMAL );

	glDisable( GL_TEXTURE_GEN_S );
	glDisable( GL_TEXTURE_GEN_T );
	glDisable( GL_TEXTURE_2D );

	// ripristino il face culling iniziale
	glFrontFace( GL_CCW );
	glPopMatrix();                       // POP
}



// GLUquadricObj * canna = gluNewQuadric();

/**Disegna il cannone*/
void drawCannon()
{
	// imposto le caratteristiche luminose del materiale
	float
	ambiente[4] = { 0.4, 0.0, 0.0, REFLECTION_BLEND },
 	diffuse[4] = { 0.2, 0.0, 0.0, REFLECTION_BLEND },
  	specular[4] = { 0.2, 0.0, 0.0, REFLECTION_BLEND };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );

	glPushMatrix();                          // PUSH

	// incollo texture
	GLfloat s_vect[4] = { 0, 0.1, 0, 0 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, texCannone);

	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
	glTexGenfv( GL_S, GL_OBJECT_PLANE, s_vect );
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
	glTexGenfv( GL_T, GL_OBJECT_PLANE, s_vect );


	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glEnable( GL_TEXTURE_2D );

	// filtri di magnificazione e mignificazione
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



	glTranslatef( cannone->getX(), 0, cannone->getZ() );  // posiziono il cannone al centro
	// disegno la cupola del cannone
	glutSolidSphere( 1, 32, 32 );

	glDisable( GL_TEXTURE_GEN_S );
	glDisable( GL_TEXTURE_GEN_T );
	glDisable( GL_TEXTURE_2D );

	glPushMatrix();                          // PUSH

	// oriento la direzione del cannone
	glRotatef( cannone->getDirezione(), 0, 1, 0 );
	// oriento l'alzo del cannone
	glRotatef( -cannone->getAlzo(), 1, 0, 0 );

	// disegno la canna del cannone
	float ambienteL[4] = { 0.0, 0.0, 0.0, 0.0 },
	diffuseL[4] = { 0.0, 0.0, 0.0, 0.0 },
	specularL[4] = { 0.0, 0.0, 0.0, 0.0 };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambienteL );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuseL );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specularL );

	if( !isCannaAllocated ) {
		canna = gluNewQuadric();    // la quadric va deallocata!!!
		isCannaAllocated = true;
	}

	gluQuadricDrawStyle( canna, GLU_FILL );
	gluQuadricNormals( canna, GLU_SMOOTH );  // setto le normali
	gluCylinder( canna, 0.3, 0.3, 2, 32, 32 );

	glPopMatrix();                           //POP
	glPopMatrix();                           //POP
}


/**Disegna un proiettile*/
void drawBullet( const Bullet& b )
{
	// imposto le caratteristiche luminose del materiale
	float
	ambiente[4] = { 0.1, 0.1, 0.1, REFLECTION_BLEND },
	diffuse[4] = { 0.7, 0.7, 0.7, REFLECTION_BLEND },
	specular[4] = { 0.3, 0.3, 0.3, REFLECTION_BLEND };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );


	glPushMatrix();                          // PUSH
	glTranslatef( b.getX(), b.getY(), b.getZ() );
	glutSolidSphere(BULLET_RADIUS, 32, 32);

	glPopMatrix();                           // POP
}

void drawLightAurea( const Bullet& b )
{
	// imposto le caratteristiche luminose del materiale
	float
	ambiente[4] = { 0.7, 0.7, 0.0, REFLECTION_BLEND },
	diffuse[4] = { 0.7, 0.7, 0.1, REFLECTION_BLEND },
	specular[4] = { 0.3, 0.3, 0.3, REFLECTION_BLEND };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );


	glPushMatrix();                          // PUSH
	glTranslatef( b.getX(), b.getY(), b.getZ() );

	
	glEnable( GL_BLEND );
	
	glutSolidSphere( BULLET_RADIUS *3, 32, 32 );

	glDisable( GL_BLEND );

	glPopMatrix();                           // POP
}

/**Disegna un singolo nemico*/
void drawEnemy( Enemy* e )
{
	// imposto le caratteristiche luminose del materiale
	float
	ambiente[4] = { 0.0, 0.0, 0.3, REFLECTION_BLEND },
	diffuse[4] = { 0.0, 0.0, 0.2, REFLECTION_BLEND },
	specular[4] = { 0.0, 0.0, 0.1, REFLECTION_BLEND };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambiente );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );

	glPushMatrix();                          // PUSH
	//glColor4f( 0, 0, 1, 0.1f );
	// mi posiziono
	glTranslatef( e->getX(), ENEMY_RADIUS, e->getZ() );
	//disegno il nemico
	glutSolidSphere( ENEMY_RADIUS, 32, 32 );
	glPopMatrix();                           // POP
}

/**Disegna tutti i nemici*/
void drawEnemies()
{
	for ( int i=0; i<ENEMY_NUMBER; i++ )
		drawEnemy( evil[i] );
}

/**Disegna il campo da gioco*/
void drawPlayer()
{
	if ( cannone->hasActiveBullet() )
		drawBullet( cannone->getBullet() );
	drawCannon();
}

/**Disegna tutti i nemici riflessi*/
void drawReflectedEnemies()
{

	glPushMatrix();
		glDisable( GL_DEPTH_TEST );
		
		
// 		if( cannone->hasActiveBullet() )
// 			drawLightAurea( cannone->getBullet() );
		
		glEnable( GL_BLEND );
//		glEnable( GL_NORMALIZE );
//		glFrontFace( GL_CW );
		
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glScalef( 1.0, -1.0, 1.0 ); // mirroring sull'asse delle y
		
		drawEnemies();
		drawPlayer();
		
/*		if( cannone->hasActiveBullet() )
			drawLightAurea( cannone->getBullet() );*/
		
//		glFrontFace( GL_CCW );
//		glDisable( GL_NORMALIZE );
		glDisable( GL_BLEND );
		
		glEnable( GL_DEPTH_TEST );
	glPopMatrix();
}
