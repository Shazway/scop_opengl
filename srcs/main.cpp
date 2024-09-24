/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:06:10 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/25 01:54:03 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "model.hpp"
using namespace scop;

//Constants
//r, g, b
//Panel of colors to distinguish faces
const std::vector<Color> colors = {
	{1.0f, 0.0f, 0.0f},  // Red
	{0.0f, 1.0f, 0.0f},  // Green
	{0.0f, 0.0f, 1.0f},  // Blue
	{1.0f, 1.0f, 0.0f},  // Yellow
	{1.0f, 0.0f, 1.0f},  // Magenta
	{0.0f, 1.0f, 1.0f},  // Cyan
	{0.5f, 0.5f, 0.5f},  // Gray
	{1.0f, 0.5f, 0.0f},  // Orange
	{0.5f, 0.0f, 0.5f},  // Purple
	{0.0f, 0.5f, 0.5f},  // Teal
};
//TODO remove globals
Model obj;

// Rotation angle
float angle = 0.0f;

void init() {
	glEnable(GL_DEPTH_TEST);
	// Grey background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
}

// Keys outside of GLUT constants range
void specialKeyhook(unsigned char key, int x, int y) {
	if (key == '+')
		obj.move(0.0, 0.0, 0.1);
	else if (key == '-')
		obj.move(0.0, 0.0, -0.1);
	glutPostRedisplay();
}

// Keys within range of GLUT constants
void keyhook(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		obj.move(0.0, 0.1, 0.0);
	else if (key == GLUT_KEY_DOWN)
		obj.move(0.0, -0.1, 0.0);
	else if (key == GLUT_KEY_LEFT)
		obj.move(-0.1, 0.0, 0.0);
	else if (key == GLUT_KEY_RIGHT)
		obj.move(0.1, 0.0, 0.0);
	glutPostRedisplay();
}

void drawObj() {
	int colorIndex = 0;
	for (const auto& face : obj.faces) {
		// Cycle through the color list for each face
		const auto& color = colors[colorIndex % colors.size()];
		glColor3f(color.r, color.g, color.b);
		colorIndex++;

		// Triangles or quads
		if (face.vertexIndices.size() == 3)
			glBegin(GL_TRIANGLES);
		else if (face.vertexIndices.size() == 4)
			glBegin(GL_QUADS);

		for (int idx : face.vertexIndices) {
			const Vertex& v = obj.vertices[idx];
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Apply position to the object
	glTranslatef(obj.position.x, obj.position.y, obj.position.z);
	// Rotate object (right rotation)
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
		
	// Draw
	drawObj();
	glutSwapBuffers();
}

void update(int value) {
	(void)value;
	angle += 2.0f;
	if (angle > 360.0f)
		angle -= 360.0f;
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);  // Call update every 16 milliseconds (~60 FPS)
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		
	// Set up perspective projection
	gluPerspective(45.0, (float)width / height, 1.0, 100.0);
		
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
	if (argc == 1)
	{
		std::cerr << "Error: Missing path to obj file" << std::endl;
		std::cerr << "Try: ./scop path/to/object.obj" << std::endl;
		return 0;
	}
	if (!obj.parseObj(std::string(argv[1])))
	{
		std::cerr << "Error parsing file" << std::endl;
		return 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow(obj.name.c_str());
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(8, update, 0); // 8 ticks per second update, 120 fps~
	glutSpecialFunc(keyhook);
	glutKeyboardFunc(specialKeyhook);

	glutMainLoop();
	return 0;
}
