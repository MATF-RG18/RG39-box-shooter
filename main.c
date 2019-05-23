/* Student: Bojana Ristanovic */
/* Indeks: 45/2016 */
/* Predmet: Racunarska grafika */


/***** NAPOMENA ****/
/*
 * Kod za 
 * 1. on_mouse_motion preuzet i prilagodjen sa https://learnopengl.com/book/offline%20learnopengl.pdf
 * 2. ispis teksta preuzet od koleginice Andjelke Milovanovic
 * 3. pucanje implementirano takodje zahvaljujuci koleginici Andjelki Milovanovic
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_mouse(int button, int state, int x, int y);
static void on_mouse_motion(int x, int y);
void material_and_light();

/* Ugao rotacije oko y ose za pravac kamere*/
float angleY;
/* Ugao rotacije oko x ose za pravac kamere*/
float angleX;
/* Vektor koji predstavlja pravac kamere */
float kx;
float ky;
float kz;

/* Pozicija kamere */
float x;
float y;
float z;

/* Pozicija misa*/
GLfloat lastX;
GLfloat lastY;
/* Osetljivost misa da ne leti po ekranu*/
float sensitivity = 0.2;

/* Odbrojavanje do kraja igre */
GLfloat game_time = 0;
int game_over;
GLfloat diff_time;
GLfloat beginTime;
char disp_time[1000];
int timing;
/* Fleg koji odredjuje stanje tajmera*/
static int timer_active;

/* Iscrtavanje kocki */
float positionx[25];
float positionz[25];
float positiony[25];


/* Tajmer za ispucavanje kuglice */
float t;
static int move_ball;
/* Pozicija kuglice */
float x_ball; 
float y_ball; 
float z_ball; 
/* Vektor pravca kuglice */
float bx;
float by;
float bz;
/* brzina kuglice */
float v;
/* Radijus kuglice */
float radius = 0.05f;

/* Indikator da li je tajmer bio aktivan */
static int ind = 0;

/* Broj osvojenih bodova */
int score;
char disp_score[256];

static void draw_coordinate_system();
static void draw_box();
void cubepositions();
void cube();
static void on_time(int value);
void drawBitmapText();
void drawScore();
void drawPointer(); 
void shoot();
static void on_mouse(int button, int state, int x, int y);
void moving_ball(int value);

void init_lights();
void initialize();

int main(int argc, char **argv)
{
	srand(time(NULL));
	/* Inicijalizuje se GLUT. */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
	/* Kreira se prozor. */
	glutInitWindowSize(750, 650);
	glutInitWindowPosition(350, 100);
	glutCreateWindow("Box shooter");
		
	/* Registruju se callback funkcije. */
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);
	glutPassiveMotionFunc(on_mouse_motion);
	
	
	/* Obavlja se OpenGL inicijalizacija. */
	glClearColor(0.4, 0.3, 0.5, 0.5);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2);
	
	cubepositions();
	initialize();
	
	/* Program ulazi u glavnu petlju. */
	glutMainLoop();
	
	return 0;
}


static void on_reshape(int width, int height)
{
	/* Pamte se sirina i visina prozora. */
	window_width = width;
	window_height = height;
	
	/* Podesava se viewport. */
	glViewport(0, 0, window_width, window_height);
}

static void on_display(void)
{
	/* Brise se prethodni sadrzaj prozora. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	/* Podesava se projekcija. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60,
		1,
		0.0,
		10.0
	);
	/* Namestanje tacke pogleda kamere*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, 1.0f, z, 
		  x+kx, 1.0f+ky, z+kz,
	   0.0f, 1.0f, 0.0f
	);
	
	/* Ukljucivanje osvetljenja */
	init_lights();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	
	glDisable(GL_DEPTH_TEST);
	
	draw_coordinate_system();
	draw_box();
	cube();
	
	/* Incijalizuje poziciju kuglice za ispaljivanje*/
	x_ball = x;
	y_ball =y;
	z_ball = z;
	shoot();
	
	/* Iscrtava nisan */
	drawPointer(); 
	
	/* Ispisivanje vremena */
	drawBitmapText();
	
	/* Ispisivanje rezultata*/
	int score1  = sprintf(disp_score,"%d", score);
	if(score1 < 0)
		exit(1);
	if(!timer_active && ind == 1){
		drawScore();
	}
	
	glutPostRedisplay();
	/* Slanje novog sadrzaja na ekran*/
	glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			/* Zavrsava se program. */
			exit(0);
			break;
		case 'g':
		case 'G':
			if(!timer_active){
				glutTimerFunc(0, on_time, 0);
				beginTime = glutGet(GLUT_ELAPSED_TIME);
				score = 0;
				timer_active = 1;
				ind = 1;
			}
			break;
		case 'r':
		case 'R':
			if(timer_active == 0 && game_over == 1){
				timer_active = 0;
				move_ball = 0;
				game_time = 0;
				game_over = 0;
				score = 0;
				kx = 0.0;
				ky = 0.0;
				kz = -1.0;
				beginTime = glutGet(GLUT_ELAPSED_TIME);
				
				glutPostRedisplay();
			}
		default:
			break;
	}
}

static void on_time(int value){
	/* Provera da li callback dolazi od ogovarajuceg tajmera */
	if(value != 0)
		return;
	
	float newTime = glutGet(GLUT_ELAPSED_TIME);
	diff_time = (newTime - beginTime)/1000.;
	beginTime = newTime; 
	game_time += diff_time;
	
	int timing = sprintf(disp_time,"%.2f", game_time);
	if(timing < 0)
		exit(1);
	
	if(game_time >= 30.00){
		game_over = 1;
		timer_active = 0;
	}
	
	if(timer_active)
		glutTimerFunc(0, on_time, 0);
}

/* Iscrtava nisan */
void drawPointer() 
{  
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix();  
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	glColor3f(1, 0, 0);
	gluOrtho2D(0.0, window_width, window_height, 0.0);
	char display_string[20];
	int words = sprintf(display_string,"%s", "-O-");
	if(words < 0)
		exit(1);
	glRasterPos2i(window_width/2, window_height/2); 
	int d = (int) strlen(display_string);
	for (int i = 0; i < d; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, display_string[i]);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
	glutPostRedisplay();
}

/* Ispisuje vreme */
void drawBitmapText() 
{  
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix();  
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	glColor3f(1, 0, 0);
	gluOrtho2D(0.0, window_width, window_height, 0.0);                 
	char display_string[20];
	int words = sprintf(display_string,"%s", "Time:");
	if(words < 0)
		exit(1);
	glRasterPos2i(window_width/2-5, window_height/2 - 50); 
	int d = (int) strlen(display_string);
	for (int i = 0; i < d; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, display_string[i]);
	glRasterPos2i(window_width/2-5, window_height/2-30); 
	int l = (int) strlen(disp_time);
	for (int i = 0; i < l; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, disp_time[i]);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
	glutPostRedisplay();
}

/* Ispisuje poene */
void drawScore(){
	
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix();  
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();
	
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	glColor3f(1, 0, 0);
	gluOrtho2D(0.0, window_width, window_height, 0.0);                 
	
	char display_string[20];
	
	int words = sprintf(display_string,"%s", "SCORE:");
	
	if(words < 0)
		exit(1);
	glRasterPos2i(window_width/2-20, window_height/2+40); 
	int d = (int) strlen(display_string);
	for (int i = 0; i < d; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, display_string[i]);
	
	glRasterPos2i(window_width/2+5, window_height/2+70); 
	int l = (int) strlen(disp_score);
	for (int i = 0; i < l; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, disp_score[i]);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
	glutPostRedisplay();
}

/* Postavlja slucajnu poziciju kocke */
void cubepositions (void) {
	
	for (int i=0;i<25;i++)
	{
		positionx[i] = rand()/(float)RAND_MAX*9;
		positiony[i] = rand()/(float)RAND_MAX*4;
		positionz[i] = rand()/(float)RAND_MAX*5;
	}
}

/* Iscrtava kocke na slucajnim mestima */
void cube (void) {
	
	for (int i=0;i<24;i++)
	{
		
		glPushMatrix();
		glColor3f(0.4, 1, 0.6);
		glTranslated(positionx[i + 1]+0.5, positiony[i + 1]+0.5, positionz[i + 1]+0.5);
		glutSolidCube(0.5);
		glPopMatrix();
	}
}

static void draw_box()
{
	
	/*Iscrtavanje poda*/
	glColor3f(0.2, 0.2, 0.7);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 0, 10);
	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, 0);
	glEnd();
	
	/* Iscrtavanje levog zida */
	glColor3f(0.3, 0.2, 0.7);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 5, 0);
	glVertex3f(0, 5, 10);
	glEnd();
	
	/* Iscrtavanje prednjeg zida */
	glColor3f(0.2, 0.4, 0.7);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 5, 0);
	glVertex3f(0, 5, 0);
	glEnd();
	
	/* Iscrtavanje desnog zida */
	glColor3f(0.3, 0.2, 0.7);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 5, 0);
	glVertex3f(10, 5, 10);
	glVertex3f(10, 0, 10);
	glEnd();
	
	/* Iscrtavanje plafona */
	glColor3f(0.2, 0.2, 0.7);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(10, 5, 10);
	glVertex3f(10, 5, 0);
	glVertex3f(0, 5, 0);
	glVertex3f(0, 5, 10);
	glEnd();
	
}

static void draw_coordinate_system()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-5, 0, 0);
	glVertex3f(100, 0, 0);
	
	glColor3f(0, 1, 0);
	glVertex3f(0, -5, 0);
	glVertex3f(0, 100, 0);
	
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -5);
	glVertex3f(0, 0, 100);
	
	glEnd();
}

static void on_mouse(int button, int state, int x, int y){
	(void)x;
	(void)y;
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				if(!move_ball && timer_active == 1){
					bx = kx;
					by = ky;
					bz = kz;
					t = 0;
					glutTimerFunc(30, moving_ball, 1);
					move_ball = 1; 
				}
			}		
			break;
		default:
			break;
	}
}

/* Ispaljivanje kuglice */
void shoot(){
	glPushMatrix();
	glColor3f(1, 1, 1);
	
	x_ball = x_ball+t*bx*v;
	y_ball = y_ball+0.9f+t*by*v; 
	z_ball = z_ball+t*bz*v;
	
	glTranslatef(x_ball, y_ball, z_ball);
	glutSolidSphere(radius, 30, 30);
	glPopMatrix();
}

void moving_ball(int value){
	/* Proveravamo da li bas ovaj tajmer */
	if (value != 1)
		return;
	
	/* Ako je loptica izasla iz sobe nema vise loptice */
	if(x_ball > 10.0f || x_ball < 0.0f || 
		y_ball > 6.0f || y_ball < 0.0f  || 
		z_ball > 10.0f || z_ball < 0.0f){
		move_ball = 0;
	t = 0;
	return;        
		}
	
	/* Ako je kuglica pogodila kocku*/
	int i;
	for(i = 0; i < 25; i++){
	if(z_ball  <= positionz[i] + 0.5 && z_ball >= positionz[i] - 0.5
		&& y_ball <= positiony[i] + 0.5 && y_ball >= positiony[i] - 0.5
		&& x_ball >= positionx[i] - 0.5 && x_ball <= positionx[i] + 0.5){
		
		/* Kocka se generise na drugom mestu */
		positionx[i] = rand()/(float)RAND_MAX*9;
		positiony[i] = rand()/(float)RAND_MAX*4;
		positionz[i] = rand()/(float)RAND_MAX*5;
		score += 1;
	
		/* Ako je kuglica pogodila jednu kocku, nestaje*/
		x_ball = 100;
		y_ball = 100;
		z_ball = 100;
			
		}
	}
	
	t += .2f;
	if (move_ball)
		glutTimerFunc(30, moving_ball, value);
}

static void on_mouse_motion(int x, int y){
	
	/* Ako kursor izadje van prozora*/
	if( x > window_width-2.0f){
		glutWarpPointer(window_width/2, window_height/2);
	}
	else if (x < 2.0f){
		glutWarpPointer(window_width/2, window_height/2);
	}
	else if (y > window_height-2.0f){
		glutWarpPointer(window_width/2, window_height/2);
	}
	else if (y < 2.0f){ 
		glutWarpPointer(window_width/2, window_height/2);
	}
	else {
		
		glutSetCursor(GLUT_CURSOR_NONE);
		/* Racuna razliku u pozicijama misa izmedju dva frejma*/
		GLfloat xoffset = x - lastX;
		GLfloat yoffset = lastY - y;
		
		lastX = x;
		lastY = y;
		
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		
		angleX += yoffset;
		angleY += xoffset;
		
		/* Pomeranje misa levo - desno*/
		if(angleX > 45.0f)
			angleX = 45.0f;
		if(angleX < -20.0f)
			angleX = -20.0f;
		/*Pomeranje misa gore - dole*/
		if(angleY > -45.0f)
			angleY = -45.0f;
		if(angleY < -135.0f)
			angleY = -135.0f;
		
		/* Vektor pomeranja kamere */
		kx = cos(M_PI/180.0*angleX)*cos(M_PI/180.0*angleY);
		ky = sin(M_PI/180.0*angleX);
		kz = cos(M_PI/180.0*angleX) * sin(M_PI/180.0*angleY);
	}
}

/* Inicijalizacija pocetnih vrednosti parametara */
void initialize()
{
	/* Ugao rotacije oko y ose za pravac kamere*/
	angleY = 0.0;
	/* Ugao rotacije oko x ose za pravac kamere*/
	angleX = 0.0;
	
	/* Vektor koji predstavlja pravac kamere */
	kx = 0.0f;
	ky = 0.0f;
	kz = -1.0f;
	
	/* Pozicija kamere */
	x = 5.0f;
	y = 0.0f;
	z = 9.0f;
	
	/* Odbrojavanje do kraja igre */
	game_over = 0;
	game_time = 0;
	/* Fleg koji odredjuje stanje tajmera*/
	timer_active = 0;
	
	/* Tajmer za ispucavanje kuglice */
	t = 0;
	move_ball = 0;
	/* Vektor pravca kuglice */
	bx = 0.0f;
	by = 0.0f;
	bz = -1.0f;
	/* brzina kuglice */
	v = 7.0f;
	
	/* Indikator da li je tajmer bio aktivan */
	ind = 0;
	
	/* Broj osvojenih bodova */
	score = 0;
	
}

/* Inicijalizacija svetla */
void init_lights(){
	/* Pozicija svetla (u pitanju je direkcionalno svetlo). */
	GLfloat light_position[] = { 0.5, 0.2, 0.5, 1 };
	
	/* Ambijentalna boja svetla. */
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
	
	/* Difuzna boja svetla. */
	GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
	
	/* Spekularna boja svetla. */
	GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
	
	/* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}