#pragma once
#ifndef HDFDB_H
#define HDFDB_H

#include <string>
#include <vector>
#include "img_object.h"
using namespace hdf_obj;

namespace hdf_db
{
	int createDB(std::string name);
	//int openDB(std::string name);
	int write_img_info(string name, std::vector<ds_img_inf> vec_obj);
};

#endif