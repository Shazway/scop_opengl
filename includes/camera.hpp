/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:26:25 by tmoragli          #+#    #+#             */
/*   Updated: 2024/10/03 01:56:56 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "scop.hpp"

namespace scop {
	class camera {
		public:
			camera() : position{0.0, 0.0, 0.0} {};
			~camera() {};
			void move(double forward, double strafe);
			void reset();

			vec3 position;
			double xangle = 0.0;
			double yangle = 0.0;
			double rotationspeed = 1;
			double movementspeed = 0.1;
	};
};
