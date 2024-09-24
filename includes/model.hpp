/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:41:38 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/25 00:09:00 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "utils.hpp"

namespace scop {
	class Model {
		public:
		Model() {};
		~Model() {};
		bool parseObj(const std::string &path);
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<Face> faces;
		bool applyTextures = false;
		bool rotateObj = false;
	};
};
