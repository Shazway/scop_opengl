/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:44:09 by tmoragli          #+#    #+#             */
/*   Updated: 2024/09/25 01:13:47 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.hpp"

namespace scop {
	bool Model::parseObj(const std::string &path) {
		std::ifstream objFile(path);
		std::string line;

		if (!objFile.is_open())
		{
			std::cerr << "Couldn't open .obj file, wrong path: [" << path << "] given" << std::endl;
			return false;
		}
		// Variables to check the max and min bounds
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

			// Parse vertex
			if (type == "v") {
				Vertex v;
				iss >> v.x >> v.y >> v.z;
				vertices.push_back(v);
				// Check if further bounds found at each vertex
				if (v.x < xmin) xmin = v.x;
				if (v.x > xmax) xmax = v.x;
				if (v.y < ymin) ymin = v.y;
				if (v.y > ymax) ymax = v.y;
				if (v.z < zmin) zmin = v.z;
				if (v.z > zmax) zmax = v.z;
			}
			// Parse face: ignore texture/normal and only use vertex indices for now
			else if (type == "f") {
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
			else if (type == "o")
				iss >> name;
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
		return true;
	}

	void Model::move(double dx, double dy, double dz)
	{
		position.x += dx;
		position.y += dy;
		position.z += dz;
	}
};
