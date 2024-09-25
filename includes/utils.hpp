/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:30:45 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/25 02:36:57 by tmoragli         ###   ########.fr       */
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
#include <string>
#include <GL/glut.h>
#include <array>
#include <limits>

#define W_WIDTH 800
#define W_HEIGHT 600

namespace scop {
	struct Vertex {
		double x, y, z;
	};
	struct Vec3 {
		double x, y, z;
	};
	struct Vec4 {
		double x, y, z, w;
	};

	struct Face {
		// Holds indices of vertices for a face
		std::vector<int> vertexIndices;
	};

	struct Color {
		//red, green, blue, opacity
		double r;
		double g;
		double b;
		double o;

		Color(double r, double g, double b, double o): r(r), g(g), b(b), o(o) {}
		Color(double r, double g, double b): r(r), g(g), b(b), o(1) {}
	};
};
