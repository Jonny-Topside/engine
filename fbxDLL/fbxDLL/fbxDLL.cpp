// fbxDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <fbxsdk.h>
#include "fbxDLL.h"
#include <fstream>

namespace fbx
{
	simpMesh convertMesh(FbxMesh * mesh);
	int numTabs = 0;

	FbxManager* manager;
	FbxIOSettings* io;
	FbxImporter* importer;
	FbxScene* scene;
	const char* simpleFactor; 
	FbxSurfaceMaterial* surfaceMaterial;
	std::vector<FbxFileTexture*> materialPropertyTextures;
	std::vector<FbxProperty*> materialProperties;
	std::vector<const char*> textureFilePath;
	std::vector<FbxDataType> propertyDataTypes;
	std::vector<FbxString> propertyNames;
	std::vector<FbxString*> factor;
	FbxString* factorString = new FbxString;

	void fbxFunctions::setup(const char* name)
	{
		// Initialize the SDK manager. This object handles all our memory management.
		manager = FbxManager::Create();
		scene = FbxScene::Create(manager, "something");
		// Create the IO settings object.
		io = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(io);
		// Create an importer using the SDK manager.
		importer = FbxImporter::Create(manager, "");
		// Use the first argument as the filename for the importer.
		if (!importer->Initialize(name, -1, manager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
			exit(-1);
		}
		// Import the contents of the file into the scene.
		importer->Import(scene);
		// The file is imported; so get rid of the importer.
		importer->Destroy();
	}


	//void PrintNode(FbxNode* pNode) {
	//	fbxFunctions::PrintTabs();
	//	const char* nodeName = pNode->GetName();
	//	FbxDouble3 translation = pNode->LclTranslation.Get();
	//	FbxDouble3 rotation = pNode->LclRotation.Get();
	//	FbxDouble3 scaling = pNode->LclScaling.Get();
	//	// Print the contents of the node.
	//	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
	//		nodeName,
	//		translation[0], translation[1], translation[2],
	//		rotation[0], rotation[1], rotation[2],
	//		scaling[0], scaling[1], scaling[2]
	//	);
	//	numTabs++;
	//	// Print the node's attributes.
	//	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	//		PrintAttribute(pNode->GetNodeAttributeByIndex(i));
	//	// Recursively print the children.
	//	for (int j = 0; j < pNode->GetChildCount(); j++)
	//		PrintNode(pNode->GetChild(j));
	//	numTabs--;
	//	fbxFunctions::PrintTabs();
	//	printf("</node>\n");
	//}

	//void MeshStuff()
	//{
	//	FbxNode* root = scene->GetRootNode();
	//	FbxMesh* mesh = root->GetMesh();
	//	vector<SIMPLE_VERT> loadedVertices;
	//	int vertexCount = mesh->GetControlPointsCount();
	//	//int vertexCount = mesh->GetControlPointsCount();
	//	for (int i = 0; i < vertexCount; ++i)
	//	{
	//		FbxVector4 vert = mesh->GetControlPointAt(i);
	//		
	//		vert[0] = loadedVertices[i].pos[0];
	//		vert[1] = loadedVertices[i].pos[0];
	//		vert[2] = loadedVertices[i].pos[0];
	//		vert[3] = loadedVertices[i].pos[0];
	//		//vert[0]
	//	}
	//	vector<unsigned int> outputIndices;
	//	int polyCnt = mesh->GetPolygonCount();
	//	for (int i = 0; i < polyCnt; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//		{
	//			int indx = mesh->GetPolygonVertex(i, j);
	//			outputIndices.push_back(indx);
	//		}
	//	}
	//}


	//oKAY, What i have to do for this function is make a simple material, just like the simpleMesh
	//In this case, I am going to have to do the exact same thing,
	//extract some data like verts and stuff
	//Just enough to recreate the material after being read from a file

	//void fbxFunctions::getMaterial()
	//{
	//	//vector<FbxProperty*> mats;
	//	vector<FbxSurfaceMaterial*> mats;
	//	vector<FbxProperty*> props;
	//	vector<const char*> propNames;
	//	vector<FbxSurfaceMaterial*> lightedMats; 
	//
	//	//gets the material(s) into a vector
	//	int matcount = scene->GetMaterialCount();
	//	for (unsigned int i = 0; i < matcount; i++)
	//	{
	//		if (scene->GetMaterial(i))
	//			mats.push_back(scene->GetMaterial(i));
	//		propNames.push_back(mats[i]->GetName());
	//	}
	//	//loops through the materials to find the properties of that material
	//	//based on what light it is it will push that new material onto another material vector
	//	for (unsigned int j = 0; j < mats.size(); j++)
	//	{
	//		if (mats[j]->Is<FbxSurfacePhong>())
	//		{
	//			FbxSurfacePhong* pMaterial = FbxSurfacePhong::Create(manager, propNames[j]);
	//			//pMaterial->Ambient.
	//
	//			lightedMats.push_back(pMaterial);
	//		}
	//
	//		else if (mats[j]->Is<FbxSurfaceLambert>())
	//	{
	//		FbxSurfaceLambert* lMaterial = FbxSurfaceLambert::Create(manager, propNames[j]);
	//		lightedMats.push_back(lMaterial);
	//
	//	}
	//
	//
	//		//mats[j]->GetFirstProperty().HasDefaultValue(mats[j]->GetFirstProperty());
	//
	//		for (unsigned int k = 0; k < lightedMats[j]->GetSrcPropertyCount(); k++)
	//		{
	//			//mats[k]->GetSrcProperty(k);
	//			
	//			
	//			//mats[k]->GetDstProperty[k];
	//
	//		}
	//	 }
	//}

//questions for Matt:
//how do i get diffuse/factor data from strings?









	simpMesh fbxFunctions::getTheMesh()
	{

		//FbxPose * pose = nullptr;
		//int catcher = scene->GetPoseCount();
		//for (int i = 0; i < scene->GetPoseCount(); i++)
//{
//	pose = scene->GetPose(i);
//	if (pose->IsBindPose())
//	{
//		break;
//	}
//}
//int poseCount = pose->GetCount();
//FbxNode* node = nullptr;
//for (unsigned int i = 0; i < poseCount; i++)
//{
//	FbxSkeleton* skeleton = pose->GetNode(i)->GetSkeleton();
//	if(skeleton != NULL)
//	if (skeleton->IsSkeletonRoot())
//	{
//		node = pose->GetNode(i);
//		if (node == NULL)
//			continue;
//		else
//			break;
//	}
//
//}
//node = pose->GetNode(0);

		FbxNode* root = scene->GetRootNode();
		FbxMesh* mesh = nullptr;
		FbxProperty* meshProps;
		factorString->Append("Factor", 7);

		//find property can find the properties I need but it will make code less flexible
		//look into it if I can't figure it out
		//	while (material->GetNextProperty(material->GetFirstProperty).IsValid())
		//	{
		//		 
		//
		//
		//	}

		int matCount = scene->GetMaterialCount();
		for (int i = 0; i < root->GetChildCount(); ++i)
		{
			mesh = scene->GetRootNode()->GetChild(i)->GetMesh(); 
			if (mesh)
				break;
		} 

		for (unsigned int j = 0; j < matCount; j++)
		{
			surfaceMaterial = scene->GetMaterial(j);
			//I made an fbxObject here and set it = surfaceMaterial but not sure what its for
		}
		unsigned int i = 0;

		//ERROR HERE WITH DEREFERENCE
		if (surfaceMaterial->Is<FbxSurfacePhong>())
		{
			//cast to phong
			surfaceMaterial = (FbxSurfacePhong*)surfaceMaterial;

			//loop through properties with getnextproperty
			//a = getnextproperty(getfirstproperty) //could be stored in an array
			//b = getnextproperty(a)
			//c = getnextproperty(b)

			//do all code that involves the property vector in the for loop instead of dumping it all in vectors and arrays
			//for( a= getfirstproperty; if(valid); a = currentproperty.getnextproperty(a))
			for (FbxProperty a = surfaceMaterial->GetFirstProperty(); a.IsValid(); a = surfaceMaterial->GetNextProperty(a))
			{
				unsigned int c = 0;
				//getname(will return diffuse/specular/etc) defines component of material ;; 
				//passed off to shaders for lighting (for each of these there may be an [insert name] factor,
				//which will be a scalar value that will
				//scale the property by some degree
				propertyNames.push_back(a.GetName());


				//to get the value of the diffuse/factor use a different function(get and getdatatype)
				//use the name to put it into the proper property index(ie emissive into emissive index)
				//use some kind of comparison to check the getname against all property indexes for a match
				//also write out the enum value of whatever it is, find the enum


				//this might be stupid
				//here i am taking the property name and if there is a name then I add
				//"Factor" if that exists I put it in a custom name array
				//factor.push_back(propertyNames[i]);
				//factor[i]->Append(*factorString, sizeof(factorString));
				//if (factor[i] != nullptr)
				//{
				//	simpleFactor = *factor[i];
				//}

				 
				//what do i do if it is default?
				if (a.HasDefaultValue(a))
				{
					//leave as 0 values for now
					//in the future each one could be represented as having a combo of states(default and texture value)


				}

				//from the (property)name get the data that it is defining
				//check if each diffusespec/etc has a (source) texture if it does get the name of it to save it out properly
				//get pointer to texture and from that get the name of it

				//ERROR SHOWED UP BECAUSE OF DEREFERENCE IN THE PARAMETER
				if (a.GetSrcObject<FbxFileTexture>() != nullptr)
				{
					
					materialPropertyTextures.push_back(a.GetSrcObject<FbxFileTexture>());
					//name will get the certain filepath where there may be a texture and if there is get data from it
					textureFilePath.push_back(materialPropertyTextures[c]->GetRelativeFileName());


				}
				//returns an enum
				propertyDataTypes.push_back(a.GetPropertyDataType());
				EFbxType e = propertyDataTypes[0].GetType();
				
				//a.Get<e>();
				//get returns the value
				//this returns whatever values I want!
				FbxDataType type = a.Get<FbxDataType>();
				//FbxFileTexture tex = a.Get<FbxFileTexture>();
				//FbxString name = a.Get<a.GetName()>();
				type.GetType();
				//take the values and cast them to proper floats and such then those can be used

				c++;
			}

		//	do
		//	{ 
		//		//the problem with this may be that it is using a temp object 
		//		//when i use &surfaceMaterial
		//		//also for getnextproperty it is not good that I am using * do not do that 
		//		//that is probably making it the one address
		//		if (i == 0)
		//			materialProperties.push_back(&surfaceMaterial->GetFirstProperty());
		//		//try making a vector of properties that arent pointers 
		//		else
		//			materialProperties.push_back(&surfaceMaterial->GetNextProperty(*materialProperties[i - 1]));
		//
		//		i++;
		//
		//		int s = materialProperties.size();
		//	} while (materialProperties[i - 1]->IsValid());

			
		}
		//Do not need lambert right now, get phong working first
		else if (surfaceMaterial->Is<FbxSurfaceLambert>())
		{
			//cast to lambert
				   //a = getnextproperty(getfirstproperty)
				   //getname(will return diffuse/specular/etc) defines component of material ;; passed off to shaders for lighting
				   //from the name get the data that it is defining


		}
		simpMaterial simpleMaterial;
		simpMesh simpleMesh = convertMesh(mesh);


		//	simpMaterial simpleMaterial = convertMaterial();
			//writeMesh(simpleMesh);
		return simpleMesh;
	}



	simpMesh convertMesh(FbxMesh * mesh)
	{

		simpMesh jonConvertedMesh;
		const int vertCount = mesh->GetPolygonVertexCount();
		for (unsigned int i = 0; i < vertCount; i++)
		{
			SIMPLE_VERT simpVert;
			simpVert.pos[0] = mesh->GetControlPointAt(i).mData[0];
			simpVert.pos[1] = mesh->GetControlPointAt(i).mData[1];
			simpVert.pos[2] = mesh->GetControlPointAt(i).mData[2];
			simpVert.pos[3] = 1;
			simpVert.color[0] = 1.0f;
			simpVert.color[1] = 1.0f;
			simpVert.color[2] = 1.0f;
			simpVert.color[3] = 1.0f;

			jonConvertedMesh.vertData.push_back(simpVert);
		}
		uint32_t poly = mesh->GetPolygonCount() * 3;
		for (unsigned int j = 0; j < poly; j++)
		{
			//at [1730] exactly the values start defaulting to 0
			jonConvertedMesh.indices.push_back(mesh->GetPolygonVertex(j, 0));
			jonConvertedMesh.indices.push_back(mesh->GetPolygonVertex(j, 1));
			jonConvertedMesh.indices.push_back(mesh->GetPolygonVertex(j, 2));
		}
		return jonConvertedMesh;
	}

	//all i changed for read and write was from >>/<< to .write/.read
	void fbxFunctions::writeMesh(simpMesh jonConvertedMesha)
	{
		ofstream file;
		//file << whatever will return ascii
		//file.write() will return some binary value

		file.open("projectMesh.bin", ios_base::trunc | ios_base::binary);

		//what we know:
		//At about [10] the vert data will no longer be read correctly, in fact not even read
		//at [1730] the verts aren't being written anymore and they are zeroed out
		//the vert size is read correctly, the index Size is not

		if (file.is_open())
		{
			int meshSize = sizeof(SIMPLE_VERT) * (jonConvertedMesha.vertData.size());

			int indSize = sizeof(unsigned int) * jonConvertedMesha.indices.size();

			file.write((const char*)&meshSize, sizeof(meshSize));
			file.write((const char*)&jonConvertedMesha.vertData[0], meshSize);
			//	if (file)
			//		std::cout << "all characters written successfully.\n";

				//bytsize
			file.write((const char*)&indSize, sizeof(indSize));
			file.write((const char*)&jonConvertedMesha.indices[0], indSize);

			file.close();
		}
	}


	simpMesh fbxFunctions::readMesh(const char* filePath)
	{
		simpMesh returnMesh;
		//maybe need a new vector?
		ifstream read;// = filePath;
		read.open(filePath, ios_base::binary);
		if (read.is_open())
		{
			int size;
			read.read((char*)&size, sizeof(size));
			returnMesh.vertData.resize(size / sizeof(SIMPLE_VERT));
			if (read)
				std::cout << "all characters read successfully.\n";
			else
				std::cout << "error: only " << read.gcount() << " could be read\n";
			/* 	for (unsigned int i = 0; i < size; i += sizeof(SIMPLE_VERT))
				{
					for (unsigned int j = 0; j < 4; j++)
					{
			 SIMPLE_VERT takeInVert;

					read >> takeInVert[j].pos;
					read >> takeInVert[j].color;
					}*/
					//returnMesh.vertData.push_back(takeInVert);
					 //	}
					//for (int i = 0; i < size;)
					//{
			read.read((char *)&returnMesh.vertData[0], size);
			//	if (read.eof())
			//	{
			//		printf("reached end of file prematurely.\n");
			//		break;
			//	}
			//	i += read.gcount();
			//	printf("byte read: %d\n", read.gcount());
			//}

			int indSize;
			read.read((char*)&indSize, sizeof(indSize));
			//	unsigned int index;
			returnMesh.indices.resize(indSize / sizeof(unsigned int));
			read.read((char*)&returnMesh.indices[0], indSize);
			//returnMesh.indices.push_back(index);

			read.close();
		}

		return returnMesh;
	}
}


int main(int argc, char** argv) {


}


