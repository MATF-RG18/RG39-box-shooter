#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
void material_and_light();

GLfloat dx = 0;
GLfloat dy = 0;
GLfloat dz = 0;


static void draw_coordinate_system();
static void draw_box();

int main(int argc, char **argv)
{
	/* Inicijalizuje se GLUT. */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
	/* Kreira se prozor. */
	glutInitWindowSize(650, 650);
	glutInitWindowPosition(350, 50);
	glutCreateWindow("Box shooter");
	
	/* Registruju se callback funkcije. */
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);
	
	/* Obavlja se OpenGL inicijalizacija. */
	glClearColor(0.4, 0.3, 0.5, 0.5);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2);
	
	/* Program ulazi u glavnu petlju. */
	glutMainLoop();
	
	return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			/* Zavrsava se program. */
			exit(0);
			break;
		case 'w':
		case 'W':
			if(dy + 0.1 > 1)
				dy = dy;
			else
				dy = dy + 0.1;
			glutPostRedisplay();
			break;
		case 's':
		case 'S':
			if(dy - 0.1 < 0)
				dy = dy;
			else
				dy = dy - 0.1;
			glutPostRedisplay();
			break;
		case 'a':
		case 'A':
			if(dx - 0.1 < -2)
				dx = dx;
			else
				dx = dx - 0.1;
			glutPostRedisplay();
			break;
		case 'd':
		case 'D':
			if(dx + 0.1 > 2)
				dx = dx;
			else
				dx = dx + 0.1;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

static void on_reshape(int width, int height)
{
	/* Pamte se sirina i visina prozora. */
	window_width = width;
	window_height = height;
}

static void on_display(void)
{
	/* Brise se prethodni sadrzaj prozora. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* Podesava se viewport. */
	glViewport(0, 0, window_width, window_height);
	
	/* Podesava se projekcija. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60,
		1,
		       1, 25);
	
	/* Podesava se tacka pogleda. */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		3, 4, 5,
	   0, 0, 0,
	   0, 1, 0
	);
	
	draw_coordinate_system();
	draw_box();
	
	/* Iscrtavanje kvadrata */
	glColor3f(5, 0, 3);
	glBegin(GL_QUADS);
		glVertex3f(0+dx, 0+dy, 0.1);
		glVertex3f(0+dx, 0.1+dy, 0.1);
		glVertex3f(0.1+dx, 0.1+dy, 0.1);
		glVertex3f(0.1+dx, 0+dy, 0.1);
	glEnd();
	
	glFlush();
	glutPostRedisplay();
	
	/* Slanje novog sadrzaja na ekran*/
	glutSwapBuffers();
}

static void draw_box()
{
	/*Iscrtavanje poda*/
	glColor3f(0.3, 0.3, 0.3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(2, 0, 2);
		glVertex3f(2, 0, -2);
		glVertex3f(-2, 0, -2);
		glVertex3f(-2, 0, 2);
	glEnd();
	
	/* Iscrtavanje levog zida */
	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(-2, 0, 2);
	glVertex3f(-2, 0, -2);
	glVertex3f(-2, 1, -2);
	glVertex3f(-2, 1, 2);
	glEnd();
	
	/* Iscrtavanje prednjeg zida */
	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(-2, 0, -2);
	glVertex3f(2, 0, -2);
	glVertex3f(2, 1, -2);
	glVertex3f(-2, 1, -2);
	glEnd();
	
	/* Iscrtavanje desnog zida */
	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex3f(2, 0, -2);
	glVertex3f(2, 1, -2);
	glVertex3f(2, 1, 2);
	glVertex3f(2, 0, 2);
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

void material_and_light(){
	/* Parametri svetla i podrazumevanog materijala */
	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
	GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
	
	GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
	GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
	GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
	GLfloat shininess = 30;   
	
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	
	/* Postavljaju se svojstva materijala */
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}