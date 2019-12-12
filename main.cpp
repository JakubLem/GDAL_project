#include <iostream>
#include <vector>
#include "ogrsf_frmts.h"
//chuj
std::vector<std::vector<double>> readPointShapeFile(const  char* name, const  char* layer)// , double scale)
{
	//	double multx = 1.0 / (scale*lon_m_PL);
		//double multy = 1.0 / (scale*lat_m);


	std::vector<std::vector<double>> result;

	GDALAllRegister();
	GDALDataset* poDS = static_cast<GDALDataset*>(
		GDALOpenEx(name, GDAL_OF_VECTOR, NULL, NULL, NULL));
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	OGRLayer* poLayer = poDS->GetLayerByName(layer);
	OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
	poLayer->ResetReading();
	OGRFeature* poFeature;
	OGRFieldDefn oField1("Number", OFTString);

	std::vector<double> punkt;

	result.clear();
	int i = 0;
	while ((poFeature = poLayer->GetNextFeature()) != NULL)
	{
		for (int iField = 0; iField < poFDefn->GetFieldCount(); iField++)
		{
			OGRFieldDefn* poFieldDefn = poFDefn->GetFieldDefn(iField);
			switch (poFieldDefn->GetType())
			{
			case OFTInteger:
				//				printf("%d,", poFeature->GetFieldAsInteger(iField));
				break;
			case OFTInteger64:
				//			printf(CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64(iField));
				break;
			case OFTReal:
				//		printf("%.3f,", poFeature->GetFieldAsDouble(iField));
				break;
			case OFTString:
				//	printf("%s,", poFeature->GetFieldAsString(iField));
				break;
			default:
				//printf("%s,", poFeature->GetFieldAsString(iField));
				break;
			}
		}



		OGRGeometry* poGeometry = poFeature->GetGeometryRef();
		if (poGeometry == NULL) printf("NULL");
		//printf("%d", poGeometry->getGeometryType());
		if (wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPoint)
		{
			OGRMultiPoint* poMultipoint = (OGRMultiPoint*)poGeometry;
			int NumberOfGeometries = poMultipoint->getNumGeometries();
			for (int j = 0; j < NumberOfGeometries; j++)
			{
				OGRGeometry* poPointGeometry = poMultipoint->getGeometryRef(j);
				OGRPoint* poPoint = (OGRPoint*)poPointGeometry;
				double x = poPoint->getX();d
				double y = poPoint->getY();
				punkt.push_back(x);// / multx);
				punkt.push_back(y);// / multy);
			//	printf("%d%s%f%s%f\n", i, " ", x, " ",y );
			}
		}
		else
		{
			printf("no point geometry\n");
		}
		result.push_back(punkt);
		punkt.clear();
		OGRFeature::DestroyFeature(poFeature);
		i++;
	}
	GDALClose(poDS);
	return result;
}

int main()
{
	readPointShapeFile("test.shp", "test");
	return 0;
}