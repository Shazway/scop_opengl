/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 00:06:10 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/03 01:30:17 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.hpp"
#include "model.hpp"
#include "matrix.hpp"
#include "camera.hpp"

using namespace scop;

// Globals
Model model;
camera cam;
mat4 projectionMatrix;
mat4 viewMatrix;
bool keyStates[256] = {false};
bool specialKeyStates[256] = {false};

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

void specialKeyPress(int key, int x, int y) {
	(void)x;
	(void)y;
	specialKeyStates[key] = true;
}

void specialKeyRelease(int key, int x, int y) {
	(void)x;
	(void)y;
	specialKeyStates[key] = false;
}

void keyPress(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	keyStates[key] = true;
	if (key == 't') model.applyTextures = !model.applyTextures;
	if (key == 'x') model.xRotation = !model.xRotation;
	if (key == 'y') model.yRotation = !model.yRotation;
	if (key == 'r') model.yRotation = model.xRotation = false; // Stop all rotations
	if (key == 'c') model.cameraMode = !model.cameraMode; // Toggle camera mode
	if (key == 27) {
		glutLeaveMainLoop();
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
	if (model.applyTextures && !model.texture_coords.empty()) {
		glBindTexture(GL_TEXTURE_2D, model.textureID);
		glEnable(GL_TEXTURE_2D);
	}
	// Iterate on all faces of the model
	for (const auto& face : model.faces) {
		if (!model.applyTextures) {
			// Cycle through the colors list for each face
			const auto& color = grey_nuances[colorIndex % grey_nuances.size()];
			glColor3f(color.r, color.g, color.b);
		}
		else if (model.applyTextures && model.texture_coords.empty()) {
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
			const Vertex &v = model.vertices[vertexIndex];

			if (model.applyTextures && i < face.textureIndices.size()) {
				// Apply texture coordinates if enabled and available
				size_t textCoordIndex = face.textureIndices[i];
				const texture_coord &tex = model.texture_coords[textCoordIndex];
				// Set texture coordinates for the vertex
				glTexCoord2f(tex.u, tex.v);
			}
			glVertex3d(v.x, v.y, v.z);
		}
		glEnd();
	}
	if (!model.applyTextures && !model.texture_coords.empty())
		glDisable(GL_TEXTURE_2D);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Matrix operations
	double radX;
	double radY;
	glMatrixMode(GL_MODELVIEW);
	viewMatrix = mat4::identity();
	radX = cam.xangle * (M_PI / 180.0);
	radY = cam.yangle * (M_PI / 180.0);
	viewMatrix *= mat4::rotate(radY, 1.0, 0.0, 0.0);
	viewMatrix *= mat4::rotate(radX, 0.0, 1.0, 0.0);
	viewMatrix *= mat4::translate(cam.position.x, cam.position.y, cam.position.z);

	mat4 modelMatrix = mat4::translate(model.position.x, model.position.y, model.position.z);
	radX = model.xangle * (M_PI / 180.0);
	radY = model.yangle * (M_PI / 180.0);
	modelMatrix *= mat4::rotate(radX, 0.0, 1.0, 0.0);
	modelMatrix *= mat4::rotate(radY, 1.0, 0.0, 0.0);
	glLoadMatrixd((viewMatrix * modelMatrix).data[0].data());

	// Draw object
	drawObj();

	glutSwapBuffers();
}

void update(int value) {
	(void)value;
	if (!model.cameraMode) // Move the model
	{
		// Increasing the speed for the model
		if (keyStates['j']) model.rotationspeed += 0.01;
		if (keyStates['k']) model.rotationspeed -= 0.01;
		if (keyStates['o']) model.modelspeed += 0.01;
		if (keyStates['p']) model.modelspeed -= 0.01;
		if (model.modelspeed < 0.0) model.modelspeed = 0.0;

		// Moving the model around
		model.move(keyStates);
	}
	else // Move the camera
	{
		// Camera movement
		if (keyStates['z']) cam.move(1.0, 0.0);
		if (keyStates['w']) cam.move(1.0, 0.0);
		if (keyStates['q']) cam.move(0.0, 1.0);
		if (keyStates['a']) cam.move(0.0, 1.0);
		if (keyStates['s']) cam.move(-1.0, 0.0);
		if (keyStates['d']) cam.move(0.0, -1.0);
		if (keyStates[' ']) cam.position.y -= cam.movementspeed;
		if (keyStates['v']) cam.position.y += cam.movementspeed;

		// Camera rotations
		if (specialKeyStates[GLUT_KEY_LEFT]) cam.xangle += cam.rotationspeed;
		if (specialKeyStates[GLUT_KEY_RIGHT]) cam.xangle -= cam.rotationspeed;
		if (specialKeyStates[GLUT_KEY_UP] && cam.yangle < 45.0) cam.yangle += cam.rotationspeed;
		if (specialKeyStates[GLUT_KEY_DOWN] && cam.yangle > -45.0) cam.yangle -= cam.rotationspeed;
		if (cam.xangle > 360.0)
			cam.xangle = 0.0;
		else if (cam.xangle < 0.0)
			cam.xangle = 360.0;
	}
	if (model.xRotation)
	{
		model.xangle += model.rotationspeed;
		if (model.xangle > 360.0)
			model.xangle = 0.0;
		else if (model.xangle < 0.0)
			model.xangle = 360.0;
	}
	if (model.yRotation)
	{
		model.yangle += model.rotationspeed;
		if (model.yangle > 360.0)
			model.yangle = 0.0;
		else if (model.yangle < 0.0)
			model.yangle = 360.0;
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
	projectionMatrix *= mat4::perspective(45.0 / 180.0, (double)width / (double)height, 1.0, 100.0);
	glLoadMatrixd((projectionMatrix).data[0].data());
}

void initGlutWindow(int ac, char **av) {
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow(model.name.c_str());
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
	glutSpecialFunc(specialKeyPress);
	glutSpecialUpFunc(specialKeyRelease);
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Error: Missing path to obj file or texture" << std::endl;
		std::cerr << "Try: ./scop path/to/object.obj path/to/texture.ppm" << std::endl;
		std::cerr << "The texture must be a ppm file" << std::endl;
		return 1;
	}
	if (!model.parseObj(std::string(argv[1]))) {
		std::cerr << "Error parsing file" << std::endl;
		return 1;
	}
	if (model.texture_coords.empty())
		std::cout << "No texture available for this object" << std::endl;
	initGlutWindow(argc, argv);
	initGlutEvents();
	model.textureID = loadTexture(argv[2]);
	glutMainLoop();
	return 0;
}
