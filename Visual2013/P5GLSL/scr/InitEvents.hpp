#ifndef __CONTEXT__
#define __CONTEXT__
#include <iostream>
#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include "globalVariables.hpp"
#include "Scene.hpp"
#include "PLANE.h"

#include <iostream>
using namespace std;

// Inicializo contexto
void initContext(int argc, char** argv)
{
	glutInit(&argc, argv); 
	glutInitContextVersion(3, 3); 
	glutInitContextProfile(GLUT_CORE_PROFILE); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
	glutInitWindowSize(800, 800); 
	glutInitWindowPosition(200, 200); 
	glutCreateWindow("GRAPHIC ENGINE"); 
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl << std::endl;
	
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutPassiveMotionFunc(PassiveMotionFunc);
	
}

// Inicializo OpenGL
void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

// Eventos de teclado
void keyboardFunc(unsigned char key, int x, int y) 
{
	if (editing == true)
	{
		switch (key)
		{
			//Control del motion blur
		case 'q': // Disminuyo peso frame actual
			if ((rgb > 0.0f) && (a < 1.0f))
			{
				rgb -= 0.05f;
				a += 0.05f;
				std::cout << "Valor rgb: " << rgb << std::endl;
				std::cout << "Valor alpha: " << a << std::endl << std::endl;
			}
			break;
		case 'w': // Aumento peso frame actual
			if ((rgb < 1.0f) && (a > 0.0f))
			{
				rgb += 0.05f;
				a -= 0.05f;
				std::cout << "Valor rgb: " << rgb << std::endl;
				std::cout << "Valor alpha: " << a << std::endl << std::endl;
			}
			break;

			//Control de los parámetros del DOF
		case 'a'://Disminuir distancia focal
			focalDistance -= 1.0f;
			std::cout << "Valor distancia focal: " << focalDistance << std::endl << std::endl;
			break;
		case 's'://Aumentar distancia focal
			focalDistance += 1.0f;
			std::cout << "Valor distancia focal: " << focalDistance << std::endl << std::endl;
			break;
		case 'd'://Disminuir distancia de desenfoque máxima
			if (maxDistanceFactor >= -1.0f)
			{
				maxDistanceFactor -= 0.05f;
			}
			std::cout << "Valor distancia de desenfoque máxima: " << maxDistanceFactor << std::endl << std::endl;
			break;

		case 'f'://Aumentar distancia de desenfoque máxima
			if (maxDistanceFactor <= 1.0f)
			{
				maxDistanceFactor += 0.05f;
			}
			std::cout << "Valor distancia de desenfoque máxima: " << maxDistanceFactor << std::endl << std::endl;
			break;
		case 'e':
			editing = false;
			std::cout << "Modo navegación activado" << std::endl << std::endl;
			navigation();
			break;
		case '1':
			std::cout << "Máscara 1 - Sharpen mask" << std::endl << std::endl;
			maskFactor = 1.0 / 1.0;
			mask_user[0][0] = (0.0);
			mask_user[0][1] = (-1.0);
			mask_user[0][2] = (0.0);
			mask_user[1][0] = (-1.0);
			mask_user[1][1] = (5.0);
			mask_user[1][2] = (-1.0);
			mask_user[2][0] = (0.0);
			mask_user[2][1] = (-1.0);
			mask_user[2][2] = (0.0);
			mask_user *= maskFactor;
			break;
		case '2':
			std::cout << "Máscara 1 - Blur" << std::endl << std::endl;
			maskFactor = 1.0f / 9.0f;
			mask_user[0][0] = (1.0);
			mask_user[0][1] = (1.0);
			mask_user[0][2] = (1.0);
			mask_user[1][0] = (1.0);
			mask_user[1][1] = (1.0);
			mask_user[1][2] = (1.0);
			mask_user[2][0] = (1.0);
			mask_user[2][1] = (1.0);
			mask_user[2][2] = (1.0);
			mask_user *= maskFactor;
			break;
		case '3':
			std::cout << "Máscara 1 - Edge enhance" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user[0][0] = (0.0);
			mask_user[0][1] = (-1.0);
			mask_user[0][2] = (0.0);
			mask_user[1][0] = (0.0);
			mask_user[1][1] = (1.0);
			mask_user[1][2] = (0.0);
			mask_user[2][0] = (0.0);
			mask_user[2][1] = (0.0);
			mask_user[2][2] = (0.0);
			mask_user *= maskFactor;
			break;
		case '4':
			std::cout << "Máscara 1 - Edge detect" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user[0][0] = (0.0);
			mask_user[0][1] = (1.0);
			mask_user[0][2] = (0.0);
			mask_user[1][0] = (1.0);
			mask_user[1][1] = (-4.0);
			mask_user[1][2] = (1.0);
			mask_user[2][0] = (0.0);
			mask_user[2][1] = (1.0);
			mask_user[2][2] = (0.0);
			mask_user *= maskFactor;
			break;
		case '5':
			std::cout << "Máscara 1 - Emboss" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user[0][0] = (-2.0);
			mask_user[0][1] = (-1.0);
			mask_user[0][2] = (0.0);
			mask_user[1][0] = (-1.0);
			mask_user[1][1] = (1.0);
			mask_user[1][2] = (1.0);
			mask_user[2][0] = (0.0);
			mask_user[2][1] = (1.0);
			mask_user[2][2] = (2.0);
			mask_user *= maskFactor;
			break;
		case '0':
			std::cout << "Máscara 1 - Sin filtro" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user[0][0] = (0.0);
			mask_user[0][1] = (0.0);
			mask_user[0][2] = (0.0);
			mask_user[1][0] = (0.0);
			mask_user[1][1] = (1.0);
			mask_user[1][2] = (0.0);
			mask_user[2][0] = (0.0);
			mask_user[2][1] = (0.0);
			mask_user[2][2] = (0.0);
			mask_user *= maskFactor;
			break;
		case 'g':
			std::cout << "Máscara 2 - Sharpen mask" << std::endl << std::endl;
			maskFactor = 1.0 / 1.0;
			mask_user2[0][0] = (0.0);
			mask_user2[0][1] = (-1.0);
			mask_user2[0][2] = (0.0);
			mask_user2[1][0] = (-1.0);
			mask_user2[1][1] = (5.0);
			mask_user2[1][2] = (-1.0);
			mask_user2[2][0] = (0.0);
			mask_user2[2][1] = (-1.0);
			mask_user2[2][2] = (0.0);
			mask_user2 *= maskFactor;
			break;
		case 'h':
			std::cout << "Máscara 2 - Blur" << std::endl << std::endl;
			maskFactor = 1.0f / 9.0f;
			mask_user2[0][0] = (1.0);
			mask_user2[0][1] = (1.0);
			mask_user2[0][2] = (1.0);
			mask_user2[1][0] = (1.0);
			mask_user2[1][1] = (1.0);
			mask_user2[1][2] = (1.0);
			mask_user2[2][0] = (1.0);
			mask_user2[2][1] = (1.0);
			mask_user2[2][2] = (1.0);
			mask_user2 *= maskFactor;
			break;
		case 'j':
			std::cout << "Máscara 2 - Edge enhance" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user2[0][0] = (0.0);
			mask_user2[0][1] = (-1.0);
			mask_user2[0][2] = (0.0);
			mask_user2[1][0] = (0.0);
			mask_user2[1][1] = (1.0);
			mask_user2[1][2] = (0.0);
			mask_user2[2][0] = (0.0);
			mask_user2[2][1] = (0.0);
			mask_user2[2][2] = (0.0);
			mask_user2 *= maskFactor;
			break;
		case 'k':
			std::cout << "Máscara 2 - Edge detect" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user2[0][0] = (0.0);
			mask_user2[0][1] = (1.0);
			mask_user2[0][2] = (0.0);
			mask_user2[1][0] = (1.0);
			mask_user2[1][1] = (-4.0);
			mask_user2[1][2] = (1.0);
			mask_user2[2][0] = (0.0);
			mask_user2[2][1] = (1.0);
			mask_user2[2][2] = (0.0);
			mask_user2 *= maskFactor;
			break;
		case 'l':
			std::cout << "Máscara 2 - Emboss" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user2[0][0] = (-2.0);
			mask_user2[0][1] = (-1.0);
			mask_user2[0][2] = (0.0);
			mask_user2[1][0] = (-1.0);
			mask_user2[1][1] = (1.0);
			mask_user2[1][2] = (1.0);
			mask_user2[2][0] = (0.0);
			mask_user2[2][1] = (1.0);
			mask_user2[2][2] = (2.0);
			mask_user2 *= maskFactor;
			break;
		case 'p':
			std::cout << "Máscara 2 - Sin filtro" << std::endl << std::endl;
			maskFactor = 1.0f / 1.0f;
			mask_user2[0][0] = (0.0);
			mask_user2[0][1] = (0.0);
			mask_user2[0][2] = (0.0);
			mask_user2[1][0] = (0.0);
			mask_user2[1][1] = (1.0);
			mask_user2[1][2] = (0.0);
			mask_user2[2][0] = (0.0);
			mask_user2[2][1] = (0.0);
			mask_user2[2][2] = (0.0);
			mask_user2 *= maskFactor;
			break;
		
		default:
			break;
		}
	}

	if (editing == false)
	{
			glm::mat3 locales = glm::mat3(0.0f);
			locales[0].x = cos(angleY);
			locales[2].x = -sin(angleY);
			locales[1].y = 1;
			locales[0].z = sin(angleY);
			locales[2].z = cos(angleY);

			glm::vec3 movimiento = glm::vec3(0.0f);
			float vel = 0.2f;

			switch (key) {
				case 'w'://Avanzar
					movimiento[2] = vel;
					break;
				case 's'://Retroceder
					movimiento[2] = -vel;
					break;
				case 'a'://Mover izquierda
					movimiento[0] = vel;
					break;
				case 'd'://Mover derecha
					movimiento[0] = -vel;
					break;
				case 'c'://Girar derecha
					angleY += 1.0f * 3.14159f / 180;
					break;
				case 'z'://Girar izquierda
					angleY -= 1.0f * 3.14159f / 180;
					break;
				default:
					break;
			}

			movimiento = locales * movimiento;
			posicion = posicion + movimiento;
			updateView();
		}

		switch (key)
		{
		case 'r':
			editing = true;
			std::cout << "Modo edición activado" << std::endl << std::endl;
			edition();
			break;
		default:
			break;
		}
	}

// Eventos de movimiento de ratón
void PassiveMotionFunc(int x, int y)
{
	
	static int x_ant = x;

	if (x > x_ant)
	{
		angleY += 0.2f * 3.14159f / 180;
	}
	if (x < x_ant)
	{
		angleY -= 0.2f * 3.14159f / 180;
	}
	updateView();

	x_ant = x;
	
}

// Eventos de pulsación de botones de ratón
void mouseFunc(int button, int state, int x, int y)
{
}

// Guía de teclado
void navigation()
{
	std::cout << "MODO EDICIÓN:		r" << std::endl;
	std::cout << "Avanzar:		w" << std::endl;
	std::cout << "Retroceder:		s" << std::endl;
	std::cout << "Derecha:		d" << std::endl;
	std::cout << "Izquierda:		a" << std::endl;
	std::cout << "Girar derecha:		c" << std::endl;
	std::cout << "Girar izquierda:	z" << std::endl << std::endl;
}
void edition()
{
	std::cout << "MODO NAVEGACIÓN:		e" << std::endl;
	std::cout << "Máscara 1 - Sin filtro:		0" << std::endl;
	std::cout << "Máscara 1 - Sharpen mask:	1" << std::endl;
	std::cout << "Máscara 1 - Blur:		2" << std::endl;
	std::cout << "Máscara 1 - Edge enhance:	3" << std::endl;
	std::cout << "Máscara 1 - Edge detect:	4" << std::endl;
	std::cout << "Máscara 1 - Emboss:		5" << std::endl;
	std::cout << "Máscara 2 - Sin filtro:		p" << std::endl;
	std::cout << "Máscara 2 - Sharpen mask:	g" << std::endl;
	std::cout << "Máscara 2 - Blur:		h" << std::endl;
	std::cout << "Máscara 2 - Edge enhance:	j" << std::endl;
	std::cout << "Máscara 2 - Edge detect:	k" << std::endl;
	std::cout << "Máscara 2 - Emboss:		l" << std::endl;
	std::cout << "Motion blur ++:			w" << std::endl;
	std::cout << "Motion blur --:			q" << std::endl;
	std::cout << "Distancia focal ++:		s" << std::endl;
	std::cout << "Distancia focal --:		a" << std::endl;
	std::cout << "Dist. desef. máx. ++:		f" << std::endl;
	std::cout << "Dist. desef. máx. --:		d" << std::endl << std::endl;
}



#endif
