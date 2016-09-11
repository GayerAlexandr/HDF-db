#include "hdf_db.h"
#include "img_object.h"
#include <iostream>

using std::cout;
using std::endl;

using namespace hdf_db;
using namespace hdf_obj;

const H5std_string DATASET_IMG_INFO("/Images/img_info");
const H5std_string DATASET_INCOMPLETE_L("/Images/incomplete_layout");
const H5std_string DATASET_IMG_OBJECT("/Images/Objects/objects");

const int	RANK = 1;
hsize_t dim[] = { 0 };	        // dataset dimension at creation
hsize_t maxdim[] = { H5S_UNLIMITED };
hsize_t chunk_dims[] = { 100 };

int hdf_db::createDB(std::string FILE_NAME_in)
{
	const H5std_string FILE_NAME(FILE_NAME_in + ".h5");
	try {
		Exception::dontPrint();

		// Create the file	
		H5File file(FILE_NAME, H5F_ACC_TRUNC); /*overwrite existing files */
		// Create groups
		Group images(file.createGroup("/Images"));
		Group objects(file.createGroup("/Images/Objects"));

		CreateDataSet_img_info(FILE_NAME);
		CreateDataSet_img_obj(FILE_NAME);
	
		// Close the groups and file.
		images.close();
		objects.close();

		file.close();
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}

int hdf_db::write_img_info(std::string FILE_NAME, std::vector<ds_img_inf> vec_obj)
{
	try {
		ds_img_inf *buff_img_inf = new ds_img_inf[vec_obj.size()];

		int i = 0;
		for each (ds_img_inf var in vec_obj)
		{
			buff_img_inf[i].img_id = var.img_id;
			buff_img_inf[i].name = var.name;
			i++;
		}

		// Create variable-length string datatype.
		hid_t strtype = H5Tcopy(H5T_C_S1);
		herr_t status = H5Tset_size(strtype, H5T_VARIABLE);

		hid_t imgInfo_type = H5Tcreate(H5T_COMPOUND, sizeof(ds_img_inf));
		H5Tinsert(imgInfo_type, "Img_id", HOFFSET(ds_img_inf, img_id), H5T_NATIVE_UINT);
		H5Tinsert(imgInfo_type, "Name", HOFFSET(ds_img_inf, name), strtype);

		H5File file(FILE_NAME, H5F_ACC_RDWR);
		
		DataSet dataset = file.openDataSet(DATASET_IMG_INFO);
		DataSpace fspace1 = dataset.getSpace();
		hsize_t dims_out;
		int dims = fspace1.getSimpleExtentDims(&dims_out, NULL);

		hsize_t size[] = { dims_out + vec_obj.size()  };
		dataset.extend(size);

		// Select a hyperslab.
		fspace1 = dataset.getSpace();
		hsize_t     offset[] = { dims_out };
		hsize_t      dim[] = { vec_obj.size() };            /* data to write dimension */
		fspace1.selectHyperslab(H5S_SELECT_SET, dim, offset);

		
		// Write the data to the hyperslab.
		DataSpace mspace1(RANK, dim);
		dataset.write(buff_img_inf, imgInfo_type, mspace1, fspace1);

		delete buff_img_inf;
		file.close();
		fspace1.close();
		mspace1.close();
		dataset.close();
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataTypeIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}
int hdf_db::CreateDataSet_img_info(std::string FILE_NAME) {
	try {
		H5File file(FILE_NAME, H5F_ACC_RDWR);

		// Create the data space.
		DataSpace dataspace(RANK, dim, maxdim);

		// Modify dataset creation property to enable chunking
		DSetCreatPropList prop;
		prop.setChunk(RANK, chunk_dims);

		// Create variable-length string datatype.
		hid_t strtype = H5Tcopy(H5T_C_S1);
		herr_t status = H5Tset_size(strtype, H5T_VARIABLE);

		// Create the memory datatype.
		hid_t imgInfo_type = H5Tcreate(H5T_COMPOUND, sizeof(ds_img_inf));
		H5Tinsert(imgInfo_type, "Img_id", HOFFSET(ds_img_inf, img_id), H5T_NATIVE_UINT);
		H5Tinsert(imgInfo_type, "Name", HOFFSET(ds_img_inf, name), strtype);

		// Set fill value for the dataset
		ds_img_inf fill_val;
		fill_val.img_id = 0;
		fill_val.name = "";
		prop.setFillValue(imgInfo_type, &fill_val);

		// Create the chunked dataset.
		DataSet* dataset;
		dataset = new DataSet(file.createDataSet(DATASET_IMG_INFO, imgInfo_type, dataspace, prop));

		delete dataset;
		prop.close();
		file.close();
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataTypeIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}

int hdf_db::CreateDataSet_img_obj(std::string FILE_NAME)
{
	try {
		H5File file(FILE_NAME, H5F_ACC_RDWR);

		// Create the data space.
		DataSpace dataspace(RANK, dim, maxdim);

		// Modify dataset creation property to enable chunking
		DSetCreatPropList prop;
		prop.setChunk(RANK, chunk_dims);

		// Create the nested compound point datatype
		hid_t point_type = H5Tcreate(H5T_COMPOUND, sizeof(point));
		H5Tinsert(point_type, "X", HOFFSET(point, x), H5T_NATIVE_INT);
		H5Tinsert(point_type, "Y", HOFFSET(point, y), H5T_NATIVE_INT);
		// Create variable-length datatype.
		hid_t points_type = H5Tvlen_create(point_type);

		// Create the memory datatype.
		hid_t imgObj_type = H5Tcreate(H5T_COMPOUND, sizeof(ds_img_obj));
		H5Tinsert(imgObj_type, "Img_id", HOFFSET(ds_img_obj, img_id), H5T_NATIVE_UINT);
		H5Tinsert(imgObj_type, "Vertices", HOFFSET(ds_img_obj, vertices), points_type);
		H5Tinsert(imgObj_type, "isCrowd", HOFFSET(ds_img_obj, isCrowd), H5T_NATIVE_INT8);

		// Set fill value for the dataset
		ds_img_obj fill_val;
		fill_val.img_id = 0;
		//fill_val.vertices.p = nullptr;
		fill_val.isCrowd = NULL;
		prop.setFillValue(imgObj_type, &fill_val);

		// Create the chunked dataset.
		DataSet* dataset;
		dataset = new DataSet(file.createDataSet(DATASET_IMG_OBJECT, imgObj_type, dataspace, prop));

		delete dataset;
		prop.close();
		file.close();
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataTypeIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}


int hdf_db::write_img_obj(std::string FILE_NAME, std::vector<ds_img_obj> vec_obj)
{
	try {
		// Create the nested compound point datatype
		hid_t point_type = H5Tcreate(H5T_COMPOUND, sizeof(point));
		H5Tinsert(point_type, "X", HOFFSET(point, x), H5T_NATIVE_INT);
		H5Tinsert(point_type, "Y", HOFFSET(point, y), H5T_NATIVE_INT);
		// Create variable-length datatype.
		hid_t points_type = H5Tvlen_create(point_type);

		// Create the memory datatype.
		hid_t imgObj_type = H5Tcreate(H5T_COMPOUND, sizeof(ds_img_obj_buff));
		H5Tinsert(imgObj_type, "Img_id", HOFFSET(ds_img_obj_buff, img_id), H5T_NATIVE_UINT);
		H5Tinsert(imgObj_type, "Vertices", HOFFSET(ds_img_obj_buff, vertices), points_type);
		H5Tinsert(imgObj_type, "isCrowd", HOFFSET(ds_img_obj_buff, isCrowd), H5T_NATIVE_INT8);

		// Get data to write and put it in buffer
		ds_img_obj_buff *buff_img_obj = new ds_img_obj_buff[vec_obj.size()];
		int i = 0;
		for each (ds_img_obj var in vec_obj)
		{
			buff_img_obj[i].img_id = var.img_id;
			buff_img_obj[i].isCrowd = var.isCrowd;
			buff_img_obj[i].vertices.len = var.vertices.size();
			point *ptr = (point *)malloc(buff_img_obj[i].vertices.len * sizeof(point));
			int j = 0;
			for each (point var_point in var.vertices)
			{
				ptr[j].x = var_point.x;
				ptr[j].y = var_point.y;
				j++;
			}
			buff_img_obj[i].vertices.p = (void *)ptr;

			i++;
		}

		H5File file(FILE_NAME, H5F_ACC_RDWR);

		DataSet dataset = file.openDataSet(DATASET_IMG_OBJECT);
		DataSpace fspace1 = dataset.getSpace();
		hsize_t dims_out;
		int dims = fspace1.getSimpleExtentDims(&dims_out, NULL);

		hsize_t size[] = { dims_out + vec_obj.size() };
		dataset.extend(size);

		// Select a hyperslab.
		fspace1 = dataset.getSpace();
		hsize_t     offset[] = { dims_out };
		hsize_t      dim[] = { vec_obj.size() };            /* data to write dimension */
		fspace1.selectHyperslab(H5S_SELECT_SET, dim, offset);

		// Write the data to the hyperslab.
		DataSpace mspace1(RANK, dim);
		dataset.write(buff_img_obj, imgObj_type, mspace1, fspace1);

		file.close();
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataTypeIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}