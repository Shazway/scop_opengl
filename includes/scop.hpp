/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:30:45 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/02 02:04:07 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Useful includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <GL/glut.h>
#include <array>
#include <limits>
#include <memory>
#include <cmath>

#define CTRL 114
#define W_WIDTH 800
#define W_HEIGHT 600

namespace scop {
	struct vec3 {
		double x, y, z;

		vec3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}
	};

	struct vec4 {
		float x, y, z, w;
	};
	struct texture_coord {
		// Horizontal texture coordinate
		float u;
		// Vertical texture coordinate
		float v;
	};

	struct Vertex {
		double x, y, z;
	};

	struct Face {
		// Holds indices of vertices for a face
		std::vector<int> vertexIndices;
		std::vector<int> textureIndices;
	};

	struct Color {
		//red, green, blue, opacity
		double r;
		double g;
		double b;
		double o;
		Color(): r(0), g(0), b(0), o(0) {}
		Color(double r, double g, double b, double o): r(r), g(g), b(b), o(o) {}
		Color(double r, double g, double b): r(r), g(g), b(b), o(1) {}
	};

	struct rgb {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	// Constants
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
	const std::vector<Color> grey_nuances = {
		{0.1f, 0.1f, 0.1f},
		{0.2f, 0.2f, 0.2f},
		{0.3f, 0.3f, 0.3f},
		{0.4f, 0.4f, 0.4f},
		{0.5f, 0.5f, 0.5f},
		{0.6f, 0.6f, 0.6f},
		{0.7f, 0.7f, 0.7f},
		{0.8f, 0.8f, 0.8f},
	};
	const double movespeed = 0.1;
};
