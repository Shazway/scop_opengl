/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:06:10 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/01 18:41:18 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.hpp"
#include "model.hpp"
#include "matrix.hpp"

using namespace scop;

// Globals
Model obj;
bool keyStates[256];

mat4 projectionMatrix;

rgb *loadPPM(const std::string &path, int &width, int &height) {
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error: Couldn't open texture file " << path << std::endl;
		return nullptr;
	}

	std::string magicNumber;
	file >> magicNumber;
	if (magicNumber != "P6") {
		std::cerr << "Error: Unsupported PPM format " << magicNumber << std::endl;
		return nullptr;
	}

	file >> width >> height;
	int maxColor;
	file >> maxColor;
	file.get();  // Eat the newline character after maxColor

	// Check if the color depth is 8 bits per channel
	if (maxColor != 255) {
		std::cerr << "Error: Unsupported max color value " << maxColor << std::endl;
		return nullptr;
	}

	// Allocate memory for the Color data (one Color struct per pixel)
	rgb* data = new rgb[width * height];
		
	// Read the pixel data into the array of Color structs
	file.read(reinterpret_cast<char*>(data), 3 * width * height);  // 3 bytes per pixel (Color)

	if (!file) {
		std::cerr << "Error: Couldn't read texture data from " << path << std::endl;
		delete[] data;
		return nullptr;
	}

	return data;
}

GLuint loadTexture(const std::string& filename) {
	int width, height;
	rgb* data = loadPPM(filename, width, height);
	if (!data) {
		std::cerr << "Error: Couldn't load texture from " << filename << std::endl;
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Upload the pixel data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Free the RGB data after uploading
	delete[] data;

	return textureID;
}

void keyPress(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	keyStates[key] = true;
	if (key == 't') obj.applyTextures = !obj.applyTextures;
	if (key == 'x') obj.xRotation = !obj.xRotation;
	if (key == 'y') obj.yRotation = !obj.yRotation;
	if (key == 'r') obj.yRotation = obj.xRotation = false; // Stop all rotations
	if (key == 'j') obj.rotationspeed += 0.01;
	if (key == 'k') obj.rotationspeed -= 0.01;
	if (key == 27) {
		// TODO: exit properly when pressing escape
	}
}

void keyRelease(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	keyStates[key] = false;
}

void drawObj() {
	int colorIndex = 0;
	// Apply textures if active and available
	if (obj.applyTextures && !obj.texture_coords.empty()) {
		glBindTexture(GL_TEXTURE_2D, obj.textureID);
		glEnable(GL_TEXTURE_2D);
	}
	// Iterate on all faces of the model
	for (const auto& face : obj.faces) {
		if (!obj.applyTextures) {
			// Cycle through the colors list for each face
			const auto& color = grey_nuances[colorIndex % grey_nuances.size()];
			glColor3f(color.r, color.g, color.b);
		}
		else if (obj.applyTextures && obj.texture_coords.empty()) {
			// Cycle through the colors list for each face
			const auto& color = colors[colorIndex % colors.size()];
			glColor3f(color.r, color.g, color.b);
		}
		// Disable colors
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		colorIndex++;

		// Triangles or quads
		if (face.vertexIndices.size() == 3)
			glBegin(GL_TRIANGLES);
		else if (face.vertexIndices.size() == 4)
			glBegin(GL_QUADS);

		for (size_t i = 0; i < face.vertexIndices.size(); i++) {
			int vertexIndex = face.vertexIndices[i];
			const Vertex &v = obj.vertices[vertexIndex];

			if (obj.applyTextures && i < face.textureIndices.size()) {
				// Apply texture coordinates if enabled and available
				size_t textCoordIndex = face.textureIndices[i];
				const texture_coord &tex = obj.texture_coords[textCoordIndex];
				// Set texture coordinates for the vertex
				glTexCoord2f(tex.u, tex.v);
			}
			glVertex3d(v.x, v.y, v.z);
		}
		glEnd();
	}
	if (!obj.applyTextures && !obj.texture_coords.empty())
		glDisable(GL_TEXTURE_2D);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Matrix operations
	glMatrixMode(GL_MODELVIEW);
	mat4 modelMatrix = mat4::translate(obj.position.x, obj.position.y, obj.position.z);
	modelMatrix *= mat4::rotate(obj.xangle, 0.0, 1.0, 0.0);
	modelMatrix *= mat4::rotate(obj.yangle, 1.0, 0.0, 0.0);
	glLoadMatrixd(modelMatrix.data[0].data());

	// Draw object
	drawObj();

	glutSwapBuffers();
}

void update(int value) {
	(void)value;
	if (keyStates['z']) obj.position.y += movespeed;
	if (keyStates['w']) obj.position.y += movespeed;
	if (keyStates['q']) obj.position.x -= movespeed;
	if (keyStates['a']) obj.position.x -= movespeed;
	if (keyStates['s']) obj.position.y -= movespeed;
	if (keyStates['d']) obj.position.x += movespeed;
	if (keyStates['+']) obj.position.z -= movespeed;
	if (keyStates['-']) obj.position.z += movespeed;

	if (obj.xRotation)
	{
		obj.xangle += obj.rotationspeed;
		if (obj.xangle > 360.0)
			obj.xangle = 0.0;
		else if (obj.xangle < 0.0)
			obj.xangle = 360.0;
	}
	if (obj.yRotation)
	{
		obj.yangle += obj.rotationspeed;
		if (obj.yangle > 360.0)
			obj.yangle = 0.0;
		else if (obj.yangle < 0.0)
			obj.yangle = 360.0;
	}
	glutPostRedisplay();
	// Call update every 8 milliseconds (~120 FPS)
	glutTimerFunc(8, update, 0);
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);

	// Apply projection matrix operations
	glMatrixMode(GL_PROJECTION);
	projectionMatrix = mat4::identity();
	projectionMatrix = mat4::perspective(45.0 / 180.0, (double)width / (double)height, 1.0, 100.0);
	glLoadMatrixd(projectionMatrix.data[0].data());
}

void initGlutWindow(int ac, char **av) {
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow(obj.name.c_str());
	glEnable(GL_DEPTH_TEST);
	// Grey background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
}

void initGlutEvents() {
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(8, update, 0); // 8 ticks per second update, 120 fps~
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Error: Missing path to obj file or texture" << std::endl;
		std::cerr << "Try: ./scop path/to/object.obj path/to/texture.ppm" << std::endl;
		std::cerr << "The texture must be a ppm file" << std::endl;
		return 1;
	}
	if (!obj.parseObj(std::string(argv[1]))) {
		std::cerr << "Error parsing file" << std::endl;
		return 1;
	}
	if (obj.texture_coords.empty())
		std::cout << "No texture available for this object" << std::endl;
	initGlutWindow(argc, argv);
	initGlutEvents();
	obj.textureID = loadTexture(argv[2]);
	glutMainLoop();
	return 0;
}
