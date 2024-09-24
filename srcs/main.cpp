/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:19:07 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/24 23:19:33 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
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

//globals TODO: move in class
std::vector<Vertex> vertices;
std::vector<Face> faces;

bool applyTextures = false;
bool rotateObj = false;

// Rotation angle
float angle = 0.0f;

void init() {
    glEnable(GL_DEPTH_TEST);
    
    // Grey background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
}

// .obj parser: v, and f
void parseObj(const std::string& filename) {
    std::ifstream objFile(filename);
    std::string line;

    // Variables to check the max and min bounds of the object
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double zmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    double zmax = std::numeric_limits<double>::lowest();

    while (std::getline(objFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            // Parse vertex
            Vertex v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
            // Check for bounds of the object at each vertex
            if (v.x < xmin) xmin = v.x;
            if (v.x > xmax) xmax = v.x;
            if (v.y < ymin) ymin = v.y;
            if (v.y > ymax) ymax = v.y;
            if (v.z < zmin) zmin = v.z;
            if (v.z > zmax) zmax = v.z;
        }
        else if (type == "f") {
            // Parse face: ignore texture/normal and only use vertex indices for now
            Face f;
            std::string vertex;
            while (iss >> vertex) {
                std::stringstream ss(vertex);
                std::string vertexIndex;
                std::getline(ss, vertexIndex, '/');
                f.vertexIndices.push_back(std::stoi(vertexIndex) - 1);
            }
            faces.push_back(f);
        }
    }
    // Centering formula
    double centerX = (xmin + xmax) / 2.0;
    double centerY = (ymin + ymax) / 2.0;
    double centerZ = (zmin + zmax) / 2.0;

    // Applying the centering transformation to each vertex
    for (auto& vertex : vertices) {
        vertex.x -= centerX;
        vertex.y -= centerY;
        vertex.z -= centerZ;
    }
}

void drawObj() {
    int colorIndex = 0;
    for (const auto& face : faces) {
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
            const Vertex& v = vertices[idx];
            glVertex3f(v.x, v.y, v.z);
        }

        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Move back to view object
    glTranslatef(0.0f, 0.0f, -10.0f);

    // Rotate object (right rotation)
    glRotatef(angle, 0.0f, -1.0f, 0.0f);
    
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
    parseObj(std::string(argv[1])); 

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("scop");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
