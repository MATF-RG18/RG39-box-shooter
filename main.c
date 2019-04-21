#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>

/* Deklaracije callback funkcija. */
void on_keyboard(unsigned char key, int x, int y);
void on_display(void);
void material_and_light();

int main(int argc, char **argv)
{
	/* Inicijalizuje se GLUT. */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
	/* Kreira se prozor. */
	glutInitWindowSize(650, 650);
	glutInitWindowPosition(350, 50);
	glutCreateWindow(argv[0]);
	
	/* Poziv callback funkcija. */
	glutDisplayFunc(on_display);
	glutKeyboardFunc(on_keyboard);
	
	/* OpenGL inicijalizacija. */
	glClearColor(0.5, 0.3, 0.4, 0.5);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	/* Postavljaju se svojstva prvog svetla */
	glEnable(GL_LIGHT0);
	
	/* Program ulazi u glavnu petlju. */
	glutMainLoop();
	
	return 0;
}

void on_keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			/* Zavrsava se program. */
			exit(0);
			break;
	} 
}

void on_display(void)
{
	/* Brise se prethodni sadrzaj prozora. */
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	
	/* Inicijalizuje se matrica transformacije. */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/* Nova slika se salje na ekran. */
	glutSwapBuffers();
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