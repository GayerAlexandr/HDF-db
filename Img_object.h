#pragma once
#ifndef IMGOBJECTS_H
#define IMGOBJECTS_H

#include <string>
using std::string;

namespace hdf_obj 
{
	typedef struct point {
		int x, y;
	}point;

	typedef struct img_object
	{
		string name;
		point* vertices;
		bool isCrowd;
	}img_object;

	typedef struct ds_img_obj
	{
		int img_id;
		point* vertices;
		bool isCrowd;
	}ds_img_obj;

	typedef struct ds_img_inf
	{
		int img_id;
		//string name;
		int name;
	}ds_img_inf;

	typedef struct ds_incomplete_layout
	{
		int img_id;
		int type_id;
	}ds_incomplete_layout;

	typedef struct ds_type
	{
		int type_id;
		string name;
	}ds_type;
}


#endif

