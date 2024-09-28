/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:30:45 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/28 20:17:27 by tmoragli         ###   ########.fr       */
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

#define W_WIDTH 800
#define W_HEIGHT 600

namespace scop {
	struct vec3 {
		double x, y, z;
	};
	struct vec4 {
		double x, y, z, w;
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
};
