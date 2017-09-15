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


	//struct alignas(16) float4 : std::array<float, 4> { using std::array<float, 4>::array;  };


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
		FbxSurfaceMaterial* material;
		FbxFileTexture* fileText;
		for (int i = 0; i < root->GetChildCount(); ++i)
		{
			mesh = scene->GetRootNode()->GetChild(i)->GetMesh();//node->GetMesh();
			material = scene->GetRootNode()->GetChild(i)->GetMaterial(i);
			if (mesh && material)
				break;
		}
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


