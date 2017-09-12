/*-
 * Copyright (C) 2017  Dridi Boukelmoune
 * All rights reserved.
 *
 * Author: Dridi Boukelmoune <dridi.boukelmoune@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vapi/vapi_options.h"
#include "vut_options.h"

#define TEMPLATE_OPT_w							\
	VOPT("w:", "[-w <filename>]", "Output filename",		\
	    "Redirect output to file, the file will be overwritten."	\
	)

VUT_OPT_d
VUT_GLOBAL_OPT_D
VUT_OPT_h
VUT_OPT_k
VSL_OPT_L
VUT_OPT_n
VUT_GLOBAL_OPT_P
VUT_OPT_q
VUT_OPT_r
VUT_OPT_t
TEMPLATE_OPT_w
