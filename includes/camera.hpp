/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:26:25 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/01 22:29:03 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "scop.hpp"

namespace scop {
	class camera {
		public:
			camera() : position{0.0, 0.0, 0.0} {}; // z -10 or on first frame we are inside the object
			~camera() {};

			bool inClampRange();

			vec3 position;
			double clamp = 90.0;
			double xangle = 0.0;
			double yangle = 0.0;
			double cameraYaw = 0.0;
			double cameraPitch = 0.0;
			double rotationspeed = 0.05;
			double movementspeed = 0.1;
	};
};