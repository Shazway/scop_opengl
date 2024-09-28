/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:41:38 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/28 20:16:33 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "utils.hpp"

namespace scop {
	class Model {
		public:
			Model() : position{0.0, 0.0, -10.0} {}; // z -10 or on first frame we are inside the object
			~Model() {};
			bool parseObj(const std::string &path);
			void move(double dx, double dy, double dz);

			std::string name;
			std::vector<Vertex> vertices;
			std::vector<Face> faces;
			std::vector<texture_coord> texture_coords;
			vec3 position;

			bool applyTextures = false;
			bool xRotation = true;
			bool yRotation = false;
	};
	std::ostream& operator<<(std::ostream &os, const Model& obj);
	std::ostream& operator<<(std::ostream &os, const std::vector<int> &vec);
};
