#pragma once
#ifndef IMGOBJECTS_H
#define IMGOBJECTS_H

#include <string>
#include <vector>
#include "H5Cpp.h"
using namespace H5;

namespace hdf_obj 
{
	typedef struct point {
		point(int x, int y)
		{
			point::x = x;
			point::y = y;
		}
		int x, y;
	}point;

	typedef struct img_object
	{
		char *name;
		std::vector<point> vertices;
		bool isCrowd;
	}img_object;

	typedef struct ds_img_obj
	{
		int img_id;
		std::vector<point> vertices;
		bool isCrowd;
	}ds_img_obj;
	typedef struct ds_img_obj_buff
	{
		int img_id;
		hvl_t vertices;
		bool isCrowd;
	}ds_img_obj_buff;

	typedef struct ds_img_inf
	{
		int img_id;
		char *name;
	}ds_img_inf;

	typedef struct ds_incomplete_layout
	{
		int img_id;
		int type_id;
	}ds_incomplete_layout;

	typedef struct ds_type
	{
		int type_id;
		char *name;
	}ds_type;
}


#endif

