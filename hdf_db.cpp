#include "hdf_db.h"
#include "img_object.h"
#include <iostream>


#include "H5Cpp.h"

using std::cout;
using std::endl;

using namespace H5;
using namespace hdf_db;
using namespace hdf_obj;

const H5std_string DATASET_IMG_INFO("/Images/img_info");
const H5std_string DATASET_INCOMPLETE_L("/Images/incomplete_layout");
const H5std_string DATASET_OBJECT("/Images/Objects/objects");
const int	RANK = 1;

int hdf_db::createDB(string name)
{
	const H5std_string FILE_NAME(name + ".h5");
	hsize_t dim[] = { 3 };	        // dataset dimension at creation
	hsize_t maxdim[] = { H5S_UNLIMITED };
	hsize_t chunk_dims[] = { 2 };

	try {
		Exception::dontPrint();
		
		// Create the file	
		H5File* file = new H5File(FILE_NAME, H5F_ACC_TRUNC); /*overwrite existing files */
		// Create groups
		Group images(file->createGroup("/Images"));
		Group objects(file->createGroup("/Images/Objects"));


		// Create the data space.
		DataSpace *dataspace = new DataSpace(RANK, dim, maxdim);

		// Modify dataset creation property to enable chunking
		DSetCreatPropList prop;
		prop.setChunk(1, chunk_dims);

		// Create the memory datatype.
		CompType imgInfo_type(sizeof(ds_img_inf));
		imgInfo_type.insertMember("Img_id", HOFFSET(ds_img_inf, img_id), PredType::NATIVE_INT);
		imgInfo_type.insertMember("Name", HOFFSET(ds_img_inf, name), PredType::NATIVE_INT);

		// Create the chunked dataset.
		DataSet* dataset;
		dataset = new DataSet(file->createDataSet(DATASET_IMG_INFO, imgInfo_type, *dataspace, prop));

		// create and write data to dataset
		ds_img_inf buff_img_inf[3];
		/*buff_img_inf[0].img_id = 1;
		buff_img_inf[0].name = "img1";
		buff_img_inf[1].img_id = 22;
		buff_img_inf[1].name = "img2";
		buff_img_inf[2].img_id = 33;
		buff_img_inf[2].name = "img3";*/
		for (int i = 0; i < 3; i++)
		{
			buff_img_inf[i].img_id = i;
			buff_img_inf[i].name = i + i;
		}
		dataset->write(buff_img_inf, imgInfo_type);

		// Close the groups and file.
		images.close();
		objects.close();
		prop.close();

		// Release resources
		delete dataset;
		delete file;
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}
	// catch failure caused by the DataSet operations
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	// catch failure caused by the DataSpace operations
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
	}
	// catch failure caused by the DataSpace operations
	catch (DataTypeIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}

//int hdf_db::openDB(std::string name)
//{
//	H5File file(name, H5F_ACC_RDWR);
//	file.openFile(name, )
//}

int hdf_db::write_img_info(string name, std::vector<ds_img_inf> vec_obj)
{
	try {
		H5File file(name, H5F_ACC_RDWR);
		DataSet *dataset = new DataSet(file.openDataSet(DATASET_IMG_INFO));

		const int length = vec_obj.size();
		//ds_img_inf *buff_img_inf = new ds_img_inf[length];
		/*ds_img_inf buff_img_inf[3];
		int i = 0;
		for each (ds_img_inf var in vec_obj)
		{
			buff_img_inf[i].img_id = var.img_id;
			buff_img_inf[i].name = var.name;
			i++;
		}*/
		ds_img_inf buff_img_inf[3];
		for (int i = 0; i < 3; i++)
		{
			buff_img_inf[i].img_id = (i+1)*54;
			buff_img_inf[i].name = i + i;
		}

		CompType imgInfo_type(sizeof(ds_img_inf));
		imgInfo_type.insertMember("Img_id", HOFFSET(ds_img_inf, img_id), PredType::NATIVE_INT);
		imgInfo_type.insertMember("Name", HOFFSET(ds_img_inf, name), PredType::NATIVE_INT);

		// extend dataset
		hsize_t dims_out;
		DataSpace *dataspace = new DataSpace(dataset->getSpace());
		int dims = dataspace->getSimpleExtentDims(&dims_out, NULL);

		hsize_t size[] = { dims_out + vec_obj.size() };
		dataset->extend(size);

		//hsize_t offset[2];
		//hsize_t dimsext[2] = { 0, vec_obj.size() };	// extend dimensions 
		//hid_t filespace = H5Dget_space(dataset->getId());
		//offset[0] = 0;
		//offset[1] = 3;
		//herr_t status = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset, NULL,
		//	dimsext, NULL);

		// Select a hyperslab in extended portion of the dataset.
		hsize_t offset[] = { dims_out };
		hsize_t dimsext[] = { vec_obj.size() };	// extend dimensions 
		dataspace->selectHyperslab(H5S_SELECT_SET, dimsext, offset);

		// Define memory space.
		DataSpace *memspace = new DataSpace(1, dimsext, NULL);

		// Write data to the extended portion of the dataset.
		dataset->write(buff_img_inf, imgInfo_type, *memspace, *dataspace);
		//status = H5Dwrite(dataset->getId(), H5T_NATIVE_INT, memspace, filespace,
		//	H5P_DEFAULT, dataext);

		//delete [] buff_img_inf;
		delete dataspace;
		delete memspace;
		delete dataset;
		file.close();
	}
	catch (FileIException error)
	{
		error.printError();
		return -1;
	}
	// catch failure caused by the DataSet operations
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	// catch failure caused by the DataSpace operations
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
	}
	// catch failure caused by the DataSpace operations
	catch (DataTypeIException error)
	{
		error.printError();
		return -1;
	}

	return 0;
}