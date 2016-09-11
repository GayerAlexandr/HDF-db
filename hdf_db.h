#pragma once
#ifndef HDFDB_H
#define HDFDB_H

#include <string>
#include "img_object.h"
using namespace hdf_obj;

namespace hdf_db
{
	int createDB(std::string FILE_NAME);
	int write_img_info(std::string FILE_NAME, std::vector<ds_img_inf> vec_obj);
	int write_img_obj(std::string FILE_NAME, std::vector<ds_img_obj> vec_obj);

	int CreateDataSet_img_info(std::string FILE_NAME);
	int CreateDataSet_img_obj(std::string FILE_NAME);

};

#endif