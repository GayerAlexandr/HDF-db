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
	//t1.name = "img1";
	t1.name = 234;
	vc.push_back(t1);
	t2.img_id = 2;
	//t2.name = "img2";
	t2.name = 435;
	vc.push_back(t2);
	t3.img_id = 3;
	//t3.name = "img3";
	t3.name = 643;
	vc.push_back(t3);

	write_img_info("db_prototype.h5", vc);
	//write_img_info("db_prototype.h5", vc);

	return 0;
}