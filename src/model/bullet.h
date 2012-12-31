#ifndef BULLET_H_
#define BULLET_H_

class Enemy;   // predichiarazione della classe Enemy

// accelerazione di gravita'
#define G 9.8
// pigreco
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// diametro dei proiettili
#define BULLET_RADIUS 0.4

class Bullet
{
public:
	Bullet( float startX, float startY, float startZ,
			float ang, float dir, float pow );
	virtual ~Bullet();
	
	// metodi get
	float getX() const;
	float getY() const;
	float getZ() const;
	
	// altri metodi
	/**Muove il proiettile e ritorna true finche' il proiettile esiste*/
	bool move( double elapsedTime );
	/**Verifica se il proiettile ha colpito un nemico*/
	bool hit( const Enemy& ) const;
	
	
private:
	float bullX, bullY, bullZ, // posizone del proiettile
		  VX, VY, VZ,          // vettori dell'accelerazione
		  alzo, direzione, potenza;
		  
};

#endif /*BULLET_H_*/
