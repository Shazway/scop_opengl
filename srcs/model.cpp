/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:44:09 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/10 17:31:37 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "model.hpp"

namespace scop {
	bool Model::parseObj(const std::string &path)
	{
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

		while (std::getline(objFile, line))
		{
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			// Parse vertex
			if (type == "v")
			{
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
			// Texture coordinates
			else if (type == "vt")
			{
				texture_coord vt = {0, 0};
				iss >> vt.u >> vt.v;
				texture_coords.push_back(vt);
			}
			// Parse face: ignore texture/normal and only use vertex indices for now
			else if (type == "f")
			{
				Face f;
				std::string vertex;
				while (iss >> vertex)
				{
					std::stringstream ss(vertex);
					std::string vertexIndex("");
					std::string textureIndex("");

					std::getline(ss, vertexIndex, '/');
					std::getline(ss, textureIndex, '/');
					f.vertexIndices.push_back(atoi(vertexIndex.c_str()) - 1);
					// Some obj files don't have 'vt' (no textures)
					if (!texture_coords.empty())
					{
						f.textureIndices.push_back(atoi(textureIndex.c_str()) - 1);
					}
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
		for (auto& vertex : vertices)
		{
			vertex.x -= centerX;
			vertex.y -= centerY;
			vertex.z -= centerZ;
		}
		return true;
	}

	void Model::move(const bool keyStates[256])
	{
		if (keyStates['z']) position.y += modelspeed;
		if (keyStates['w']) position.y += modelspeed;
		if (keyStates['q']) position.x -= modelspeed;
		if (keyStates['a']) position.x -= modelspeed;
		if (keyStates['s']) position.y -= modelspeed;
		if (keyStates['d']) position.x += modelspeed;
		if (keyStates['+']) position.z -= modelspeed;
		if (keyStates['-']) position.z += modelspeed;
	}

	void Model::reset()
	{
		position = vec3(0.0, 0.0, -10.0);
		applyTextures = false;
		xRotation = true;
		yRotation = false;
		cameraMode = false;
		xangle = 0.0;
		yangle = 0.0;
		rotationspeed = 1.0;
		modelspeed = 0.1;
	}

	std::ostream& operator<<(std::ostream &os, const std::vector<int> &vec)
	{
		os << "[";
		for (size_t i = 0; i < vec.size(); i++)
		{
			os << vec[i];
			if (i < vec.size() - 1)
				os << " ";
		}
		os << "]";
		return (os);
	}

	std::ostream& operator<<(std::ostream& os, const Model& model)
	{
		os << "Name: " << "[" << model.name << "]" << std::endl;
		os << "Vertices: " << std::endl;
		for (size_t i = 0; i < model.vertices.size(); i++)
		{
			os << "i: " << i << " [" <<
			model.vertices[i].x << ", " <<
			model.vertices[i].y << ", " <<
			model.vertices[i].z << "]" << std::endl;
		}
		os << std::endl;

		os << "Faces: " << model.faces.size() << std::endl;
		for (size_t i = 0; i < model.faces.size(); i++)
		{
			os << "Face index: " << i << std::endl;
			os << "-------------------" << std::endl;
			os << "Vertex indices" << std::endl;
			os << model.faces[i].vertexIndices << std::endl;
			os << "-------------------" << std::endl;
			os << "Texture indices" << std::endl;
			os << model.faces[i].textureIndices << std::endl;
			os << "-------------------" << std::endl;
		}
		os << std::endl;
		
		os << "Texcoords: " << std::endl;
		for (size_t i = 0; i < model.texture_coords.size(); i++)
		{
			os << "i: " << i << " [" <<
			model.texture_coords[i].u << ", " <<
			model.texture_coords[i].v << "]" << std::endl;
		}
		os << std::endl;

		os << "Position: " << "[x: " << model.position.x << "]";
		os << "[y: " << model.position.y << "]";
		os << "[z: " << model.position.z << "]" << std::endl;
		os << std::endl;

		os << "applyTextures: " << model.applyTextures << std::endl;
		os << "xRotation: " << model.xRotation << std::endl;
		os << "yRotation: " << model.yRotation;
		return (os);
	}
};
