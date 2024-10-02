/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:41:38 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/02 02:21:50 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "scop.hpp"

namespace scop {
	class Model {
		public:
			Model() : position{0.0, 0.0, -10.0} {}; // z -10 or on first frame we are inside the object
			~Model() {};
			bool parseObj(const std::string &path);
			void move(double dx, double dy, double dz);

			// Parsing data
			std::string name;
			std::vector<Vertex> vertices;
			std::vector<Face> faces;
			std::vector<texture_coord> texture_coords;
			GLuint textureID = -1;

			// Runtime data
			vec3 position;
			bool applyTextures = false;
			bool xRotation = true;
			bool yRotation = false;
			bool cameraMode = false;
			double xangle = 0.0;
			double yangle = 0.0;
			double rotationspeed = 1.0;
			double objectspeed = 0.1;
	};
	std::ostream& operator<<(std::ostream &os, const Model& obj);
	std::ostream& operator<<(std::ostream &os, const std::vector<int> &vec);
};
