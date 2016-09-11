#include "Img_object.h"
#include "hdf_db.h"
#include <iostream>

using namespace hdf_obj;
using namespace hdf_db;


int main()
{
	createDB("db_prototype");
	
	std::vector<ds_img_inf> vc;
	ds_img_inf t1, t2, t3;
	t1.img_id = 1;
	t1.name = "img1";
	vc.push_back(t1);

	t2.img_id = 2;
	t2.name = "img2";
	vc.push_back(t2);

	t3.img_id = 3;
	t3.name = "img3";
	vc.push_back(t3);

	write_img_info("db_prototype.h5", vc);


	std::vector<ds_img_obj> vc2;
	ds_img_obj d1, d2;
	d1.img_id = 1;
	d1.vertices.push_back(point(1, 2));
	d1.vertices.push_back(point(4, 5));
	d1.vertices.push_back(point(3, 2));
	d1.isCrowd = 0;
	vc2.push_back(d1);

	d2.img_id = 2;
	d2.vertices.push_back(point(1, 2));
	d2.vertices.push_back(point(4, 5));
	d2.vertices.push_back(point(3, 2));
	d2.vertices.push_back(point(35, 25));
	d2.isCrowd = 1;
	vc2.push_back(d2);

	write_img_obj("db_prototype.h5", vc2);


	return 0;
}