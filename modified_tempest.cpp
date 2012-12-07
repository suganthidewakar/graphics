// 
// Controls:    Left Arrow  - Moves Left
//              Right Arrow - Moves Right
//              Space Bar or Up Arrow    - shoots
// ----------------------------------------------------------
// Includes
// ----------------------------------------------------------
#include <stdafx.h>
#include <Windows.h>
#include <MMSystem.h>
#include <stdio.h>

#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


typedef float bla[3];

int width = 500;
int height = 500;
float threshold = 5;
int move = 0;
int numPolygons = 7;//13,
int bulletSegment;
float Ex, Ey, Ez,Ey1,bulletX, bulletY, bulletZ, EyFinal;
int random;
int timer = 0,gameover = 0;
bool isShoot = false;
int score = 0,life = 5, level = 1;
// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void specialKeys();
 
// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y=0; 
double rotate_x=0;


float arrayPoints[28][3] = {
							{ -12, 5, -0.5},{-12,  25.5, -70 },{-20, 25.5,-70},{-20,5,-0.5},
							{ -12, -0.5, -0.5 },{-12,  20, -70},{-12,  25.5, -70 },{ -12, 5, -0.5},
							{-4,-0.5,-0.5},{-4,20,-70},{-12,  20, -70},{-12,-0.5,-0.5},

							{4, -0.5, -0.5 },{ 4, 20, -70},{-4,20,-70},{-4,-0.5,-0.5},
							{12, -0.5, -0.5 },{ 12, 20, -70},{ 4, 20, -70},{4, -0.5, -0.5 },
							{12,5,-0.5},{12,25.5,-70},{ 12, 20, -70},{12, -0.5, -0.5 },
							{18,5,-0.5},{18,25.5,-70},{ 12, 25.5, -70},{12, 5, -0.5 }
						};


float arrayPoints2[52][3] = {{-20,5,-0.5},{-20,25.5,-70},{-25,25.5,-70},{-25,5,-0.5},
							{-20,0,-0.5},{-20,20.5,-70},{-20,25.5,-70},{-20,5,-0.5},
							{-20,-5,-0.5},{-20, 15.5,-70},{-20,20.5,-70},{-20,0,-0.5},
							{ -12, -5, -0.5},{-12,  15.5, -70 },{-20, 15.5,-70},{-20,-5,-0.5},
							{ -12, -9.5, -0.5 },{-12,  10, -70},{-12,  15.5, -70 },{ -12, -5, -0.5},
							{-4,-9.5,-0.5},{-4,10,-70},{-12,  10, -70},{-12,-9.5,-0.5},

							{4, -9.5, -0.5 },{ 4, 10, -70},{-4,10,-70},{-4,-9.5,-0.5},
							{12, -9.5, -0.5 },{ 12, 10, -70},{ 4, 10, -70},{4, -9.5, -0.5 },
							{12,-5,-0.5},{12,15.5,-70},{ 12, 10, -70},{12, -9.5, -0.5 },
							{18,-5,-0.5},{18,15.5,-70},{ 12, 15.5, -70},{12, -5, -0.5 },
							{18,0,-0.5},{18,20.5,-70},{18,15.5,-70},{18,-5,-0.5},
							{18,5,-0.5},{18,25.5,-70},{18,20.5,-70},{18,0,-0.5},
							{23,5,-0.5},{23,25.5,-70},{18,25.5,-70},{18,5,-0.5}
						};



float distance(float* p1,float* p2){
	float dx,dy,dz;
	dx = p1[0] - p2[0];
	dy = p1[1] - p2[1];
	dz = p1[2] - p2[2];
	return sqrt(dx*dx+dy*dy+dz*dz);
}

int generateRand(){
	
	int random;
	random = rand()%numPolygons;
	return random;

}

void reset(void){
	random = generateRand();
	Ex = arrayPoints[random*4 + 2][0];
	Ey = (arrayPoints[random*4 + 2][1] + arrayPoints[random*4 + 1][1])/2;
	Ez = arrayPoints[random*4 + 2][2];
}


void reset2(void){
	random = generateRand();
	Ex = arrayPoints2[random*4 + 2][0];
	Ey = (arrayPoints2[random*4 + 2][1] + arrayPoints2[random*4 + 1][1])/2;
	Ez = arrayPoints2[random*4 + 2][2];
}

void explode(){
	
}

void calcBullet(void){
	float dy,dz,step,incY,incZ;
	int resetFactor = 0;
	dy = arrayPoints[bulletSegment*4+2][1] - arrayPoints[bulletSegment*4 + 3][1];
	dz = arrayPoints[bulletSegment*4+2][2] - arrayPoints[bulletSegment*4 + 3][2];
	if(abs(dy) > abs(dz))
		step = abs(dy);
	else
		step = abs(dz);
	incY = dy / (float) step;
	incZ = dz /(float) step;
	bulletY += incY;
	bulletZ += incZ;
	float bulletArray[3] = {bulletX,bulletY, bulletZ};
	float enemyArray[3] = {Ex,Ey,Ez};
	float dist = distance(bulletArray,enemyArray);
	if(dist < threshold && bulletSegment == random){
		isShoot = false;
		wchar_t* file = L"bomb.wav"; 
		score = score + 10;
		if(score == 100){
			numPolygons = 13;
			level = 2;
			
		}
		//explode();
		if(level == 1)
			reset();
		else if(level == 2)
			reset2();
		PlaySound(file, NULL, SND_ASYNC);
	}
	if(bulletY >= arrayPoints[bulletSegment*4 + 2][1]){
		resetFactor = 1;
	}

	if(bulletZ <= arrayPoints[bulletSegment*4 +2][2]){
		if(resetFactor == 1){
			isShoot = false;
		}
	}
}

void calcBullet2(void){
	float dy,dz,step,incY,incZ;
	int resetFactor = 0;
	dy = arrayPoints2[bulletSegment*4+2][1] - arrayPoints2[bulletSegment*4 + 3][1];
	dz = arrayPoints2[bulletSegment*4+2][2] - arrayPoints2[bulletSegment*4 + 3][2];
	if(abs(dy) > abs(dz))
		step = abs(dy);
	else
		step = abs(dz);
	incY = dy / (float) step;
	incZ = dz /(float) step;
	bulletY += incY;
	bulletZ += incZ;
	float bulletArray[3] = {bulletX,bulletY, bulletZ};
	float enemyArray[3] = {Ex,Ey,Ez};
	float dist = distance(bulletArray,enemyArray);
	if(dist < threshold && bulletSegment == random){
		isShoot = false;
		wchar_t* file = L"bomb.wav"; 
		score = score + 10;
		//explode();
		if(level == 1)
			reset();
		else if(level == 2)
			reset2();
		PlaySound(file, NULL, SND_ASYNC);
	}
	if(bulletY >= arrayPoints2[bulletSegment*4 + 2][1]){
		resetFactor = 1;
	}

	if(bulletZ <= arrayPoints2[bulletSegment*4 +2][2]){
		if(resetFactor == 1){
			isShoot = false;
		}
	}
}




float maxi(float x,float y,float z){
	if(x > y){
		if(x > z)
			return x;
		else
			return z;
	}
	else{
		if(y > z)
			return y;
		else
			return z;
	}
}



void calcEnemyCoord(){
	float dy,dz,step,incY,incZ,dx,incX,dy1,Ey1;
	int resetFactor = 0;
	dy = arrayPoints[random*4+2][1] - arrayPoints[random*4 + 3][1];
	dz = arrayPoints[random*4+2][2] - arrayPoints[random*4 + 3][2];
	dx = arrayPoints[random*4+3][0] - arrayPoints[random*4 + 2][0];
	float xx1 = arrayPoints[random*4+3][0];
	float xx = arrayPoints[random*4+1][0];

	
	step = maxi(abs(dx),abs(dy),abs(dz)	);
	incY = dy / (float) step;
	incZ = dz /(float) step;
	incX = dx /(float) step;
	
	//float 
	Ey -= incY;
	Ey1 -= incY;
	EyFinal = (Ey + Ey1)/2;
	Ez -= incZ;
	Ex = (xx + xx1)/2;
	if(Ey <= arrayPoints[random*4 + 3][1]){
		resetFactor = 1;
	}

	if(Ez >= arrayPoints[random*4 +3][2]){
		
		{
			if(random == move/4){
				wchar_t* file = L"lifeout.wav"; 
				PlaySound(file, NULL, SND_SYNC);
				life--;
				if(life == 0){
					file = L"gameover.wav";
					PlaySound(file, NULL, SND_SYNC);
					gameover = 1;//exit(0);
				}
			}
			if(level == 1)
				reset();
			else if (level == 2)
				reset2();
		}
	}
	if(isShoot){
		if(level == 1)
			calcBullet();
		else if(level == 2)
			calcBullet2();
	}
	glutPostRedisplay();
}


void calcEnemyCoord2(){
	float dy,dz,step,incY,incZ,dx,incX,dy1,Ey1;
	int resetFactor = 0;
	dy = arrayPoints2[random*4+2][1] - arrayPoints2[random*4 + 3][1];
	dz = arrayPoints2[random*4+2][2] - arrayPoints2[random*4 + 3][2];
	dx = arrayPoints2[random*4+3][0] - arrayPoints2[random*4 + 2][0];
	float xx1 = arrayPoints2[random*4+3][0];
	float xx = arrayPoints2[random*4+1][0];

	
	step = maxi(abs(dx),abs(dy),abs(dz)	);
	incY = dy / (float) step;
	incZ = dz /(float) step;
	incX = dx /(float) step;
	
	//float 
	Ey -= incY;
	Ey1 -= incY;
	EyFinal = (Ey + Ey1)/2;
	Ez -= incZ;
	Ex = (xx + xx1)/2;
	if(Ey <= arrayPoints2[random*4 + 3][1]){
		resetFactor = 1;
	}

	if(Ez >= arrayPoints2[random*4 +3][2]){
		//if(resetFactor == 1)
		{
			if(random == move/4){
				wchar_t* file = L"lifeout.wav"; 
				PlaySound(file, NULL, SND_SYNC);
				life--;
				if(life == 0){
					file = L"gameover.wav";
					PlaySound(file, NULL, SND_SYNC);
					gameover = 1;//exit(0);
				}
			}
			if(level == 1)
				reset();
			else if (level == 2)
				reset2();
		}
	}
	if(isShoot){
		if(level == 1)
			calcBullet();
		else if(level == 2)
			calcBullet2();

	}
	glutPostRedisplay();
}



void animate(){
	timer++;
	if(timer >= 1000000){
		timer = 0;
		
		if(level == 1)
			calcEnemyCoord();
		else if(level == 2){
			calcEnemyCoord2();
		
		}
	}
}



void DrawCircle(float cx, float cy,float z, float r, int num_segments) 
{ 
		float x1,y1,x2,y2;
		float angle;
		double radius=r;
		glShadeModel(GL_SMOOTH);
		//x1 = 0.5,y1=0.6;
		x1 = cx, y1 = cy;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x1,y1,z);
 
		for (angle=1.0f;angle<361.0f;angle+=0.2)
		{
			x2 = x1+sin(angle)*radius;
			y2 = y1+cos(angle)*radius;
			glVertex3f(x2,y2,z);
		} 
		glEnd();
}



void shoot(void)
{
  glPointSize(10);
  glBegin(GL_POINTS);
  glColor3f(1,1,1);
  DrawCircle(bulletX,bulletY,bulletZ,0.8,10);
  glEnd();

}
void drawEnemy()
{
 	if(level == 1){
	  glColor3f(1.0,0.6,0.6);
	  DrawCircle(Ex,Ey,Ez,0.7,10);
	}
	else if(level == 2){
		glColor3f(1,0.3,0);
		DrawCircle(Ex,Ey,Ez,1.3,10);
	}
  
}



void drawPlayer(void)
{
  float x = (arrayPoints[move][0] + arrayPoints[move+3][0]) /2;
  float y = (arrayPoints[move][1] + arrayPoints[move+3][1]) /2;
  y = y +4;
  float z = arrayPoints[move][2]-10;
  
  glBegin(GL_TRIANGLES);
  glColor3f(0.7,0.7,0);//glColor3f(1,1,1);
  glVertex3fv(arrayPoints[move]);
  glVertex3fv(arrayPoints[move+3]);
  glVertex3f(x,y,z);
  glEnd();
}


void drawPlayer2(void)
{
  float x = (arrayPoints2[move][0] + arrayPoints2[move+3][0]) /2;
  float y = (arrayPoints2[move][1] + arrayPoints2[move+3][1]) /2;
  y = y +4;
  float z = arrayPoints2[move][2]-10;
  
  glBegin(GL_TRIANGLES);
  glColor3f(0.7,0.7,0);//glColor3f(1,1,1);
  glVertex3fv(arrayPoints2[move]);
  glVertex3fv(arrayPoints2[move+3]);
  glVertex3f(x,y,z);
  glEnd();
}


void drawGrid(void){
	
	int i;
	for( i = 0; i < numPolygons*4; i = i+4){
		glBegin(GL_LINES);
		glColor3f( 0.0, .7, 0.0 ); 
		glVertex3fv(arrayPoints[i]);
		glVertex3fv(arrayPoints[i+1]);
		glVertex3fv(arrayPoints[i+1]);
		glVertex3fv(arrayPoints[i+2]);
		glVertex3fv(arrayPoints[i+2]);
		glVertex3fv(arrayPoints[i+3]);
		glVertex3fv(arrayPoints[i+3]);
		glVertex3fv(arrayPoints[i]);
		glEnd();
	}
}


void drawGrid2(void){
	
	int i;
	for( i = 0; i < numPolygons*4; i = i+4){
		glBegin(GL_LINES);
		glColor3f( 0.0, .7, 0.0 ); 
		glVertex3fv(arrayPoints2[i]);
		glVertex3fv(arrayPoints2[i+1]);
		glVertex3fv(arrayPoints2[i+1]);
		glVertex3fv(arrayPoints2[i+2]);
		glVertex3fv(arrayPoints2[i+2]);
		glVertex3fv(arrayPoints2[i+3]);
		glVertex3fv(arrayPoints2[i+3]);
		glVertex3fv(arrayPoints2[i]);
		glEnd();
	}
}


// ----------------------------------------------------------
// display() Callback function
// ----------------------------------------------------------
void display(){
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,(double)width/(double)height,1,200);
  //  Clear screen and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
	
  // Reset transformations
  glLoadIdentity();
  glTranslatef(0,-5,-25);

  glPushMatrix();
  glColor3f(0.7,0.7,0);
  glTranslatef(-70,100,-172);
  glScalef(0.3,0.2,0.3);
  char str[50] = "hello";
  
 /* if(gameover ==1 )
	  sprintf(str,"Score: %d \nGame over",score); 
  else*/
  sprintf(str,"Score: %d",score); 
	  
  for(char* p = str;*p;p++){
	  glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  }
  glPushMatrix();
  glColor3f(0.7,0.7,0);//glColor3f(1,0,0);
  glTranslatef(-800,-250,0);
  if(gameover == 1){
  char str1[15] = "GAME OVER !!!";
  for(char* p = str1;*p;p++){
  glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  }}
  glPopMatrix();
  glPopMatrix();

  glPushMatrix();
  glColor3f(0.7,0.7,0);
  int i;
  for( i = 0 ; i < life; i++){
	  glTranslatef(4,0,0);
   glBegin(GL_TRIANGLES);
  glVertex3f(1,24,-1);
  glVertex3f(3,27,-1);
  glVertex3f(5,25,-1);
  glEnd();
  }
  
  glPopMatrix();


  if(gameover == 0){
  if(level == 1){
	  drawGrid();
	  drawPlayer();
	  drawEnemy();
	  if(isShoot)
		  shoot();
  }

  else if(level == 2){
	  drawGrid2();
	  drawPlayer2();
	  drawEnemy();
	  if(isShoot)
		  shoot();
  }
  }

   
  glFlush();
  glutSwapBuffers();
 
}
 
// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT){
	  move += 4;
	    if( move > (numPolygons-1)*4)
			move -= 4;
  
	   }
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT){
	  move -=4;
	  if(move < 0)
		  move += 4;
   // rotate_y -= 5;
  }
 
  else if (key == GLUT_KEY_UP || key == '32'){
	   isShoot = true;
	   bulletSegment = move/4;
	   if(level == 1){
	    bulletX = (arrayPoints[bulletSegment*4+3][0] + arrayPoints[bulletSegment*4+0][0]) /2 ;
		bulletY = (arrayPoints[bulletSegment*4+3][1]+arrayPoints[bulletSegment*4+0][1])/2;
		bulletZ = arrayPoints[bulletSegment*4+3][2];
	   }
	   else if(level == 2)
	   {
		   bulletX = (arrayPoints2[bulletSegment*4+3][0] + arrayPoints2[bulletSegment*4+0][0]) /2 ;
			bulletY = (arrayPoints2[bulletSegment*4+3][1]+arrayPoints2[bulletSegment*4+0][1])/2;
			bulletZ = arrayPoints2[bulletSegment*4+3][2];

	   }
 
	shoot();
	  
	  // rotate_x += 5;
  }
 
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;
 
  //  Request display update
  glutPostRedisplay();
 
}


void glutResize (int w, int h)
{	
    width = w;
    height = h;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport (0, 0, w, h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (90, w / h, 1, 9999);

    glutPostRedisplay ();
}

 
// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]){
	int i,j;
	
	random = generateRand();
	if(level == 1){
	Ex = arrayPoints[random*4 + 2][0];
	Ey = (arrayPoints[random*4 + 2][1]+arrayPoints[random*4 + 1][1])/2;
	Ez = arrayPoints[random*4 + 2][2];
	Ey1 = arrayPoints[random*4 + 1][1];
	}
	else if(level == 2){
	Ex = arrayPoints2[random*4 + 2][0];
	Ey = (arrayPoints2[random*4 + 2][1]+arrayPoints2[random*4 + 1][1])/2;
	Ez = arrayPoints2[random*4 + 2][2];
	Ey1 = arrayPoints2[random*4 + 1][1];
	}
  //  Initialize GLUT and process user parameters

  glutInit(&argc,argv);
  glutInitWindowSize(width, height);
 
  //  Request double buffered true color window with Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
  // Create window
  glutCreateWindow("Suganthi's Game");
 
  //  Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);
 
  // Callback functions
  glutDisplayFunc(display);
  glutReshapeFunc (glutResize);
  glutSpecialFunc(specialKeys);
  glutIdleFunc(animate);
 
  //  Pass control to GLUT for events
  glutMainLoop();
 
  //  Return to OS
  return 0;
 
}
