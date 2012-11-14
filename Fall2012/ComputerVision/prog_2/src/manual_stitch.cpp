/*
 * manual_stitch.cpp
 * 
 * Copyright 2012 bcf <bcf@localhost.localdomain>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "manual_stitch.h"


manual_stitch::manual_stitch()
   : stitcher()
{
   
}

manual_stitch::manual_stitch(string file_name_1, string file_name_2, const unsigned int num_points)
   : stitcher(file_name_1, file_name_2, num_points)
{
   
}


manual_stitch::~manual_stitch()
{
   
}

