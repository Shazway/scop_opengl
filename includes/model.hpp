/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:41:38 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/25 01:33:25 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "utils.hpp"

namespace scop {
	class Model {
		public:
		Model() : position{0.0, 0.0, -10.0} {}; // z -10 or we first frame we are inside the object
		~Model() {};
		bool parseObj(const std::string &path);
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<Face> faces;
		Vec3 position;
		bool applyTextures = false;
		bool rotateObj = false;
		void move(double dx, double dy, double dz);
	};
};
