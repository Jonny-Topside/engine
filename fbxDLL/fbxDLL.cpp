// fbxDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <fbxsdk.h>
#include "fbxDLL.h"
#include <fstream>
#include <DirectXMath.h>
#define UV_STRIDE 2
#define VERTEX_STRIDE 4
#define TRIANGLE_VERTEX_COUNT 3
#define NORMAL_STRIDE 3
namespace fbx
{

	struct joints
	{
		FbxNode* node;
		int parent;
	};
	struct jointTransform
	{
		DirectX::XMFLOAT4X4 transform;
		int parent;
	};
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};
	struct keyFrame
	{
		vector<jointTransform> joints;
		double time;
	};
	struct animationClips
	{
		vector<keyFrame> frames;
		double duration;
	};

	//forward decs
	vector<joints> getJointNodes(FbxSkeleton* skull);

	simpMesh convertMesh(FbxMesh * mesh);
	int numTabs = 0;

	FbxManager* manager;
	FbxIOSettings* io;
	FbxImporter* importer;
	FbxScene* scene;


	animationClips  clip;
	FbxAnimStack* animStack;
	FbxTimeSpan timeSpan;
	FbxTime duration;
	FbxPose* bindPose;
	FbxSkeleton* skeltal;
	std::vector<jointTransform> finalVecOfJointTransforms;

	const char* simpleFactor;
	FbxSurfaceMaterial* surfaceMaterial;
	std::vector<FbxFileTexture*> materialPropertyTextures;
	std::vector<FbxProperty*> materialProperties;
	std::vector<const char*> textureFilePath;
	std::vector<FbxDataType> propertyDataTypes;
	std::vector<FbxString> propertyNames;
	std::vector<FbxString*> factor;



	vector<vec2> pushingUVs;
	vector<vec3> pushingNormals;
	//maybe make this a vector of vectors
	vector<vec4> pushingJointTransforms;
	void matrixCopy(FbxAMatrix& matrixIn, DirectX::XMFLOAT4X4& matrixOut)
	{
		matrixOut._11 = matrixIn.mData[0][0];
		matrixOut._12 = matrixIn.mData[0][1];
		matrixOut._13 = matrixIn.mData[0][2];
		matrixOut._14 = matrixIn.mData[0][3];
		matrixOut._21 = matrixIn.mData[1][0];
		matrixOut._22 = matrixIn.mData[1][1];
		matrixOut._23 = matrixIn.mData[1][2];
		matrixOut._24 = matrixIn.mData[1][3];
		matrixOut._31 = matrixIn.mData[2][0];
		matrixOut._32 = matrixIn.mData[2][1];
		matrixOut._33 = matrixIn.mData[2][2];
		matrixOut._34 = matrixIn.mData[2][3];
		matrixOut._41 = matrixIn.mData[3][0];
		matrixOut._42 = matrixIn.mData[3][1];
		matrixOut._43 = matrixIn.mData[3][2];
		matrixOut._44 = matrixIn.mData[3][3];
	}

	void getTheTransforms()
	{
		vector<joints> vecOfJointNodes;

		vecOfJointNodes = getJointNodes(skeltal);

		int sze = vecOfJointNodes.size();
		for (unsigned int i = 0; i < sze; i++)
		{
			jointTransform tempTrans;
			tempTrans.transform._11 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[0][0];
			tempTrans.transform._12 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[0][1];
			tempTrans.transform._13 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[0][2];
			tempTrans.transform._14 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[0][3];
			tempTrans.transform._21 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[1][0];
			tempTrans.transform._22 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[1][1];
			tempTrans.transform._23 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[1][2];
			tempTrans.transform._24 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[1][3];
			tempTrans.transform._31 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[2][0];
			tempTrans.transform._32 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[2][1];
			tempTrans.transform._33 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[2][2];
			tempTrans.transform._34 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[2][3];
			tempTrans.transform._41 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[3][0];
			tempTrans.transform._42 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[3][1];
			tempTrans.transform._43 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[3][2];
			tempTrans.transform._44 = vecOfJointNodes[i].node->EvaluateGlobalTransform().mData[3][3];

			vec4 tempPos;
			tempPos.x = tempTrans.transform._41;
			tempPos.y = tempTrans.transform._42;
			tempPos.z = tempTrans.transform._43;
			tempPos.w = tempTrans.transform._44;
		//	pushingJointTransforms.push_back(tempPos);

			tempTrans.parent = vecOfJointNodes[i].parent;
			finalVecOfJointTransforms.push_back(tempTrans);
		}
		int b = 0;
	}

	vec4 takePositionsFromMatrix(DirectX::XMFLOAT4X4 matrix)
	{
		vec4 returning;
		 
		 returning.x = matrix._41 ;
		 returning.y = matrix._42 ;
		 returning.z = matrix._43 ;
		 returning.w = matrix._44 ;

		return returning;
	}

	void getBoneNodes(FbxSkeleton* skelly, vector<joints>& jointsToGo, int parent)
	{
		joints tempNode;
	 
		tempNode.parent = parent;
		tempNode.node = skelly->GetNode();
		int index = jointsToGo.size();
		jointsToGo.push_back(tempNode);
		int children = tempNode.node->GetChildCount();
		for (unsigned int i = 0; i < children; i++)
		{
			getBoneNodes(tempNode.node->GetChild(i)->GetSkeleton(), jointsToGo, index);
		}
	}

	vector<joints> getJointNodes(FbxSkeleton* skelRoot)
	{
		vector<joints> tempJoint;
		getBoneNodes(skelRoot, tempJoint, -1);
		return tempJoint;
	}

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

		//uncomment when ready for animating

		int poseCount = scene->GetPoseCount();
		for (unsigned int l = 0; l < poseCount; l++)
		{
			if (scene->GetPose(l)->IsBindPose())
			{
				bindPose = scene->GetPose(l);
				break;
			}
		}

		for (unsigned int r = 0; r < bindPose->GetCount(); r++)
		{

			if (nullptr != bindPose->GetNode(r)->GetSkeleton())
			{
				skeltal = bindPose->GetNode(r)->GetSkeleton();
				if (skeltal->IsSkeletonRoot())
					break;
				else
					skeltal = nullptr;
			}
			//	break;
		}

		
		//try finding a simple data type to convert each frame to

		 animStack = scene->GetCurrentAnimationStack();
		 timeSpan = animStack->GetLocalTimeSpan();
		 duration = timeSpan.GetDuration();
		 duration.GetFrameCount();
		 clip.duration = duration.GetSecondDouble();
		 FbxLongLong frmCnt = duration.GetFrameCount(FbxTime::EMode::eFrames24);
		 vector<joints> jointerNodes = getJointNodes(skeltal);
		 FbxTime time;
		 
		 for ( FbxLongLong i = 1i64; i < frmCnt; i++)
		 {
		 	keyFrame tempKeyFrame;
		 	time.SetFrame(i, FbxTime::EMode::eFrames24);
		 	tempKeyFrame.time = time.GetSecondDouble();
		 	for (long j = 0; j < jointerNodes.size(); j++)
		 	{
		 		jointTransform innerJointTransform;
		 		innerJointTransform.parent = jointerNodes[j].parent;
		 		matrixCopy(jointerNodes[j].node->EvaluateGlobalTransform(time), innerJointTransform.transform);
		 		tempKeyFrame.joints.push_back(innerJointTransform);
		 	}
		 	clip.frames.push_back(tempKeyFrame);
		 	//clip.frames[0].joints[0].transform;
		 }
		 int hhh = 0;


		// The file is imported; so get rid of the importer.
		importer->Destroy();
	}

	//I am multiplying the factor in the functions I am using so there is no need to get it from anywhere else,
	static vec3 getDiffuse(FbxSurfacePhong* surfaceMat)
	{
		FbxDouble3 diffVec = surfaceMat->Diffuse.Get();
		FbxDouble diffFac = surfaceMat->DiffuseFactor.Get();

		//specular/ambient lights will only be good for lambertian surfaces
		FbxSurfacePhong* p;

		vec3 float3;
		for (int i = 0; i < 3; i++)
		{
			diffVec[i] *= diffFac;

		}
		float3.x = diffVec[0];
		float3.y = diffVec[1];
		float3.z = diffVec[2];
		return float3;
	}

	static vec3 getEmissive(FbxSurfacePhong* surfaceMat)
	{
		FbxDouble3 emissVec = surfaceMat->Emissive.Get();
		FbxDouble  emissFac = surfaceMat->EmissiveFactor.Get();

		vec3 float3;
		for (int i = 0; i < 3; i++)
		{
			emissVec[i] *= emissFac;

		}
		float3.x = emissVec[0];
		float3.y = emissVec[1];
		float3.z = emissVec[2];
		return float3;
	}

	static vec3 getAmbient(FbxSurfacePhong* surfaceMat)
	{
		FbxDouble3 ambVec = surfaceMat->Ambient.Get();
		FbxDouble  ambFac = surfaceMat->AmbientFactor.Get();

		vec3 float3;
		for (int i = 0; i < 3; i++)
		{
			ambVec[i] *= ambFac;

		}
		float3.x = ambVec[0];
		float3.y = ambVec[1];
		float3.z = ambVec[2];
		return float3;
	}

	static vec3 getSpecular(FbxSurfacePhong* surfaceMat)
	{
		FbxDouble3 specVec = surfaceMat->Specular.Get();
		FbxDouble  specFac = surfaceMat->SpecularFactor.Get();

		vec3 float3;
		for (int i = 0; i < 3; i++)
		{
			specVec[i] *= specFac;

		}
		float3.x = specVec[0];
		float3.y = specVec[1];
		float3.z = specVec[2];
		return float3;
	}

	static vec3 getTransparency(FbxSurfacePhong* surfaceMat)
	{
		FbxDouble3 TransVec = surfaceMat->TransparentColor.Get();
		FbxDouble  TransFac = surfaceMat->TransparencyFactor.Get();

		vec3 float3;
		for (int i = 0; i < 3; i++)
		{
			TransVec[i] *= TransFac;

		}
		float3.x = TransVec[0];
		float3.y = TransVec[1];
		float3.z = TransVec[2];
		return float3;
	}

	static vec3 getReflection(FbxSurfacePhong* surfaceMat)
	{
		FbxDouble3 ReflectVec = surfaceMat->Reflection.Get();
		FbxDouble  ReflectFac = surfaceMat->ReflectionFactor.Get();

		vec3 float3;
		for (int i = 0; i < 3; i++)
		{
			ReflectVec[i] *= ReflectFac;

		}
		float3.x = ReflectVec[0];
		float3.y = ReflectVec[1];
		float3.z = ReflectVec[2];
		return float3;
	}

	static double getShininess(FbxSurfacePhong* surfaceMat)
	{
		//FbxDouble3 ShinyVec = surfaceMat->Shininess.Get();
		FbxDouble  ShinyFac = surfaceMat->Shininess.Get();
		double newShiny = ShinyFac;

		return newShiny;
	}



	 
	// Count the polygon count of each material
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;


	bool getUvsAndNormals(const FbxMesh *pMesh)
	{
		FbxArray<SubMesh*> mSubMeshes;
		bool mHasNormal(false);
		bool mHasUV(false);
		bool mAllByControlPoint = true;
		if (!pMesh->GetNode())
			return false;

		const int lPolygonCount = pMesh->GetPolygonCount();

		// Count the polygon count of each material
		FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
		FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
		if (pMesh->GetElementMaterial())
		{
			lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
			lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
			if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
			{
				FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
				if (lMaterialIndice->GetCount() == lPolygonCount)
				{
					// Count the faces of each material
					for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
					{
						const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
						if (mSubMeshes.GetCount() < lMaterialIndex + 1)
						{
							mSubMeshes.Resize(lMaterialIndex + 1);
						}
						if (mSubMeshes[lMaterialIndex] == NULL)
						{
							mSubMeshes[lMaterialIndex] = new SubMesh;
						}
						mSubMeshes[lMaterialIndex]->TriangleCount += 1;
					}

					// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
					// if, in the loop above, we resized the mSubMeshes by more than one slot.
					for (int i = 0; i < mSubMeshes.GetCount(); i++)
					{
						if (mSubMeshes[i] == NULL)
							mSubMeshes[i] = new SubMesh;
					}

					// Record the offset (how many vertex)
					const int lMaterialCount = mSubMeshes.GetCount();
					int lOffset = 0;
					for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
					{
						mSubMeshes[lIndex]->IndexOffset = lOffset;
						lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
						// This will be used as counter in the following procedures, reset to zero
						mSubMeshes[lIndex]->TriangleCount = 0;
					}
					FBX_ASSERT(lOffset == lPolygonCount * 3);
				}
			}
		}

		// All faces will use the same material.
		if (mSubMeshes.GetCount() == 0)
		{
			mSubMeshes.Resize(1);
			mSubMeshes[0] = new SubMesh();
		}

		// Congregate all the data of a mesh to be cached in VBOs.
		// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
		mHasNormal = pMesh->GetElementNormalCount() > 0;
		mHasUV = pMesh->GetElementUVCount() > 0;
		FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
		FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
		if (mHasNormal)
		{
			lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
			if (lNormalMappingMode == FbxGeometryElement::eNone)
			{
				mHasNormal = false;
			}
			if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
			{
				mAllByControlPoint = false;
			}
		}
		if (mHasUV)
		{
			lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
			if (lUVMappingMode == FbxGeometryElement::eNone)
			{
				mHasUV = false;
			}
			if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
			{
				mAllByControlPoint = false;
			}
		}

		// Allocate the array memory, by control point or by polygon vertex.
		int lPolygonVertexCount = pMesh->GetControlPointsCount();
		if (!mAllByControlPoint)
		{
			lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
		}
		float * lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
		unsigned int * lIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_COUNT];
		float * lNormals = NULL;
		if (mHasNormal)
		{
			lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
		}
		float * lUVs = NULL;
		FbxStringList lUVNames;
		pMesh->GetUVSetNames(lUVNames);
		const char * lUVName = NULL;
		if (mHasUV && lUVNames.GetCount())
		{
			lUVs = new float[lPolygonVertexCount * UV_STRIDE];
			lUVName = lUVNames[0];
		}

		// Populate the array with vertex attribute, if by control point.
		const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
		FbxVector4 lCurrentVertex;
		FbxVector4 lCurrentNormal;
		FbxVector2 lCurrentUV;
		if (mAllByControlPoint)
		{
			const FbxGeometryElementNormal * lNormalElement = NULL;
			const FbxGeometryElementUV * lUVElement = NULL;
			if (mHasNormal)
			{
				lNormalElement = pMesh->GetElementNormal(0);
			}
			if (mHasUV)
			{
				lUVElement = pMesh->GetElementUV(0);
			}
			for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
			{
				// Save the vertex position.
				lCurrentVertex = lControlPoints[lIndex];
				lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
				lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
				lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
				lVertices[lIndex * VERTEX_STRIDE + 3] = 1;

				// Save the normal.
				if (mHasNormal)
				{
					int lNormalIndex = lIndex;
					if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
					}
					lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
					lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
					lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
				}

				// Save the UV.
				if (mHasUV)
				{
					int lUVIndex = lIndex;
					if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
					}
					lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
					lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
					lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
				}
			}

		}

		int lVertexCount = 0;
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
		{
			// The material for current face.
			int lMaterialIndex = 0;
			if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
			{
				lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
			}

			// Where should I save the vertex attribute index, according to the material
			const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset +
				mSubMeshes[lMaterialIndex]->TriangleCount * 3;
			for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
			{
				const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

				if (mAllByControlPoint)
				{
					lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
				}
				// Populate the array with vertex attribute, if by polygon vertex.
				else
				{
					lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

					lCurrentVertex = lControlPoints[lControlPointIndex];
					lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
					lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
					lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
					lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;

					if (mHasNormal)
					{

					}
					vec2 uvToPush;
					uvToPush.x = 0;
					uvToPush.y = 0;
					pushingUVs.push_back(uvToPush);
					vec3 normalToPush;
					normalToPush.x = 0;
					normalToPush.y = 0;
					normalToPush.z = 0;
					pushingNormals.push_back(normalToPush);
				}
				++lVertexCount;
			}
			mSubMeshes[lMaterialIndex]->TriangleCount += 1;
		}
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
		{
			for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
			{
				bool lUnmappedUV;
				int uvIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

				pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
				lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
				lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

				pushingUVs[uvIndex].x = lCurrentUV[0];
				pushingUVs[uvIndex].y = 1 - lCurrentUV[1];

				pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
				lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
				lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
				lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
				pushingNormals[uvIndex].x = lCurrentNormal[0];
				pushingNormals[uvIndex].y = lCurrentNormal[1];
				pushingNormals[uvIndex].z = lCurrentNormal[2];
			}
		}


		return true;
	}

	/*
		bool getNormals(const FbxMesh * pMesh)
		{
			FbxArray<SubMesh*> mSubMeshes;
			bool mHasNormal(false);
			bool mHasUV(false);
			bool mAllByControlPoint = true;
			{
				if (!pMesh->GetNode())
					return false;

				const int lPolygonCount = pMesh->GetPolygonCount();

				// Count the polygon count of each material
				FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
				FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
				if (pMesh->GetElementMaterial())
				{
					lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
					lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
					if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
					{
						FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
						if (lMaterialIndice->GetCount() == lPolygonCount)
						{
							// Count the faces of each material
							for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
							{
								const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
								if (mSubMeshes.GetCount() < lMaterialIndex + 1)
								{
									mSubMeshes.Resize(lMaterialIndex + 1);
								}
								if (mSubMeshes[lMaterialIndex] == NULL)
								{
									mSubMeshes[lMaterialIndex] = new SubMesh;
								}
								mSubMeshes[lMaterialIndex]->TriangleCount += 1;
							}

							// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
							// if, in the loop above, we resized the mSubMeshes by more than one slot.
							for (int i = 0; i < mSubMeshes.GetCount(); i++)
							{
								if (mSubMeshes[i] == NULL)
									mSubMeshes[i] = new SubMesh;
							}

							// Record the offset (how many vertex)
							const int lMaterialCount = mSubMeshes.GetCount();
							int lOffset = 0;
							for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
							{
								mSubMeshes[lIndex]->IndexOffset = lOffset;
								lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
								// This will be used as counter in the following procedures, reset to zero
								mSubMeshes[lIndex]->TriangleCount = 0;
							}
							FBX_ASSERT(lOffset == lPolygonCount * 3);
						}
					}
				}

				// All faces will use the same material.
				if (mSubMeshes.GetCount() == 0)
				{
					mSubMeshes.Resize(1);
					mSubMeshes[0] = new SubMesh();
				}

				// Congregate all the data of a mesh to be cached in VBOs.
				// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
				mHasNormal = pMesh->GetElementNormalCount() > 0;
				mHasUV = pMesh->GetElementUVCount() > 0;
				FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
				FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
				if (mHasNormal)
				{
					lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
					if (lNormalMappingMode == FbxGeometryElement::eNone)
					{
						mHasNormal = false;
					}
					if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
					{
						mAllByControlPoint = false;
					}
				}
				if (mHasUV)
				{
					lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
					if (lUVMappingMode == FbxGeometryElement::eNone)
					{
						mHasUV = false;
					}
					if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
					{
						mAllByControlPoint = false;
					}
				}

				// Allocate the array memory, by control point or by polygon vertex.
				int lPolygonVertexCount = pMesh->GetControlPointsCount();
				if (!mAllByControlPoint)
				{
					lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
				}
				float * lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
				unsigned int * lIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_COUNT];
				float * lNormals = NULL;
				if (mHasNormal)
				{
					lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
				}
				float * lUVs = NULL;
				FbxStringList lUVNames;
				pMesh->GetUVSetNames(lUVNames);
				const char * lUVName = NULL;
				if (mHasUV && lUVNames.GetCount())
				{
					lUVs = new float[lPolygonVertexCount * UV_STRIDE];
					lUVName = lUVNames[0];
				}

				// Populate the array with vertex attribute, if by control point.
				const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
				FbxVector4 lCurrentVertex;
				FbxVector4 lCurrentNormal;
				FbxVector2 lCurrentUV;
				if (mAllByControlPoint)
				{
					const FbxGeometryElementNormal * lNormalElement = NULL;
					const FbxGeometryElementUV * lUVElement = NULL;
					if (mHasNormal)
					{
						lNormalElement = pMesh->GetElementNormal(0);
					}
					if (mHasUV)
					{
						lUVElement = pMesh->GetElementUV(0);
					}
					for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
					{
						// Save the vertex position.
						lCurrentVertex = lControlPoints[lIndex];
						lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
						lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
						lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
						lVertices[lIndex * VERTEX_STRIDE + 3] = 1;

						// Save the normal.
						if (mHasNormal)
						{
							int lNormalIndex = lIndex;
							if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
							{
								lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
							}
							lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
							lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
							lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
							lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);
						}

						// Save the UV.
						if (mHasUV)
						{
							int lUVIndex = lIndex;
							if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
							{
								lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
							}
							lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
							lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
							lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);
						}
					}

				}

				int lVertexCount = 0;
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
				{
					// The material for current face.
					int lMaterialIndex = 0;
					if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
					{
						lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
					}

					// Where should I save the vertex attribute index, according to the material
					const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset +
						mSubMeshes[lMaterialIndex]->TriangleCount * 3;
					for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
					{
						const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);

						if (mAllByControlPoint)
						{
							lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
						}
						// Populate the array with vertex attribute, if by polygon vertex.
						else
						{
							lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

							lCurrentVertex = lControlPoints[lControlPointIndex];
							lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
							lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
							lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
							lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;

							if (mHasNormal)
							{
								pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
								lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
								lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
								lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);

								vec3 normalToPush;
								normalToPush.x = lCurrentNormal[0];
								normalToPush.y = lCurrentNormal[1];
								normalToPush.z = lCurrentNormal[2];
								pushingNormals.push_back(normalToPush);
							}


						}
						++lVertexCount;
					}
					mSubMeshes[lMaterialIndex]->TriangleCount += 1;
				}


				return true;
			}
		}
		*/

	static std::vector<vec4> getTangents(FbxMesh* mesh)
	{
		int tangentCount = mesh->GetElementTangentCount();
		FbxGeometryElementTangent* tangentElement;
		std::vector<vec4> totalTangents;
		FbxVector4 transferVec;



		tangentElement = mesh->GetElementTangent(0);
		for (unsigned int i = 0; i < tangentCount; i++)
		{
			vec4 currentTangent;
			transferVec = tangentElement->GetDirectArray().GetAt(i);

			currentTangent.x = transferVec[0];
			currentTangent.y = transferVec[1];
			currentTangent.z = transferVec[2];
			currentTangent.w = transferVec[3];
			totalTangents.push_back(currentTangent);
		}

		return totalTangents;
	}




	//diffuse might be the one but look for the file texture and that is the right one



	//the mesh has position, uvs, normals not the material
	//look up how to apply diffuse, specular, emissive to a material

	simpMaterial fbxFunctions::getMaterial()
	{
		for (unsigned int j = 0; j < scene->GetMaterialCount(); j++)
		{
			surfaceMaterial = scene->GetMaterial(j);
		}


		for (unsigned int i = 0; i < clip.frames.size(); i++)
		{
			for (unsigned int j = 0; j < clip.frames[i].joints.size(); j++)
			{
			//	clip.frames[i].joints[j].transform

			pushingJointTransforms.push_back(takePositionsFromMatrix(clip.frames[i].joints[j].transform));
		 	}
		}
	 	for (unsigned int i = 0; i < pushingJointTransforms.size(); i++)
	 	{
	 		customMaterial.jointTransforms.push_back(pushingJointTransforms[i]);
	 	}

		//ERROR HERE WITH DEREFERENCE
		if (surfaceMaterial->Is<FbxSurfacePhong>())
		{
			//cast to phong
			surfaceMaterial = (FbxSurfacePhong*)surfaceMaterial;

			customMaterial.diffuse = getDiffuse((FbxSurfacePhong*)surfaceMaterial);
			customMaterial.emissive = getEmissive((FbxSurfacePhong*)surfaceMaterial);
			customMaterial.ambient = getAmbient((FbxSurfacePhong*)surfaceMaterial);
			customMaterial.specular = getSpecular((FbxSurfacePhong*)surfaceMaterial);
			customMaterial.transparency = getTransparency((FbxSurfacePhong*)surfaceMaterial);
			customMaterial.reflection = getReflection((FbxSurfacePhong*)surfaceMaterial);
			customMaterial.shininess = getShininess((FbxSurfacePhong*)surfaceMaterial);
			//do all code that involves the property vector in the for loop instead of dumping it all in vectors and arrays
			//for( a= getfirstproperty; if(valid); a = currentproperty.getnextproperty(a))

			int x = 0;
			int numberForDeterminingCorrectTexture = 0;

			for (FbxProperty a = surfaceMaterial->GetFirstProperty(); a.IsValid(); a = surfaceMaterial->GetNextProperty(a))
			{
				unsigned int c = 0;
				if (a.HasDefaultValue(a))
				{
					a.Set(1);
					//leave as 0 values for now
					//in the future each one could be represented as having a combo of states(default and texture value)
					c++;
					x++;
					continue;
				}


				//getname(will return diffuse/specular/etc) defines component of material ;; 
				//passed off to shaders for lighting (for each of these there may be an [insert name] factor),
				//which will be a scalar value that will
				//scale the property by some degree


				//values that have textures: emissiveColor, diffuseColor, specularColor, 
				propertyNames.push_back(a.GetName());

				//ambientColor is a color
				//ambient is a vector(of and for what?)
				//ambientFactor is a double
				if (x == 1)
				{
					FbxDataType rel = a.GetPropertyDataType();
					FbxString str = rel.GetName();
				}
				//right here buddy
				FbxDataType shittyDataType = a.GetPropertyDataType();
				EFbxType emnum = shittyDataType.GetType();
				FbxString fuckingString = shittyDataType.GetName();

				//need
				if (FbxDoubleDT == shittyDataType)
				{
					double wee = a.Get<FbxDouble>();
					int y = 9;
				}
				//need
				else if (FbxDouble3DT == shittyDataType || FbxColor3DT == shittyDataType)
				{
					FbxDouble3 lDouble3 = a.Get<FbxDouble3>();


					FbxVector4 lVect;
					lVect[0] = lDouble3[0];
					lVect[1] = lDouble3[1];
					lVect[2] = lDouble3[2];
				}
				//no need
				else if (FbxBoolDT == shittyDataType)
				{
					a.Get<FbxBool>();
				}
				//no need
				else if (FbxFloatDT == shittyDataType)
				{
					float wee = a.Get<FbxFloat>();

				}
				//no need
				else if (FbxDouble2DT == shittyDataType)
				{
					FbxDouble2 lDouble2 = a.Get<FbxDouble2>();
					FbxVector2 lVect;
					lVect[0] = lDouble2[0];
					lVect[1] = lDouble2[1];

				}
				//no need
				else if (FbxDouble4DT == shittyDataType || FbxColor4DT == shittyDataType)
				{
					FbxDouble4 lDouble4 = a.Get<FbxDouble4>();
					FbxVector4 lVect;
					lVect[0] = lDouble4[0];
					lVect[1] = lDouble4[1];
					lVect[2] = lDouble4[2];
					lVect[3] = lDouble4[3];

				}
				//no need
				else if (FbxDouble4x4DT == shittyDataType)
				{
					FbxDouble4x4 lDouble44 = a.Get<FbxDouble4x4>();
					for (int j = 0; j < 4; ++j)
					{

						FbxVector4 lVect;
						lVect[0] = lDouble44[j][0];
						lVect[1] = lDouble44[j][1];
						lVect[2] = lDouble44[j][2];
						lVect[3] = lDouble44[j][3];

					}

				}

				//to get the value of the diffuse/factor use a different function(get and getdatatype)
							//use the name to put it into the proper property index(ie emissive into emissive index)
							//use some kind of comparison to check the getname against all property indexes for a match
							//also write out the enum value of whatever it is, find the enum

					//from the (property)name get the data that it is defining
							//check if each diffusespec/etc has a (source) texture if it does get the name of it to save it out properly
							//get pointer to texture and from that get the name of it

							//ERROR SHOWED UP BECAUSE OF DEREFERENCE IN THE PARAMETER
				if (a.GetSrcObject<FbxFileTexture>(c) != nullptr)
				{
					materialPropertyTextures.push_back(a.GetSrcObject<FbxFileTexture>());
					void* customTexture = a.GetSrcObject<FbxFileTexture>();
					//name will get the certain filepath where there may be a texture and if there is get data from it
					//get the texture path x

					textureFilePath.push_back(materialPropertyTextures[numberForDeterminingCorrectTexture]->GetRelativeFileName());


					FbxFileTexture;
					//get the texture resource file
					//get the vector values 
					//get the factor data(multiply to the same component that it is named after)
					numberForDeterminingCorrectTexture++;
				}
				//customMaterial.path = textureFilePath[0];

				//returns an enum
				propertyDataTypes.push_back(a.GetPropertyDataType());



				//take the values and cast them to proper floats and such then those can be used

				c++;
				x++;
			}
			int breaker = 0;

			for (int t = 0; t < textureFilePath.size(); t++)
				customMaterial.path.push_back(textureFilePath[t]);

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

		return customMaterial;
	}

	simpMesh fbxFunctions::getTheMesh()
	{
		FbxNode* root = scene->GetRootNode();
		FbxMesh* mesh = nullptr;


		int matCount = scene->GetMaterialCount();
		for (int i = 0; i < root->GetChildCount(); ++i)
		{
			mesh = scene->GetRootNode()->GetChild(i)->GetMesh();
			if (mesh)
				break;
		}
		getTangents(mesh);
		simpMesh simpleMesh = convertMesh(mesh);
		return simpleMesh;
	}



	simpMesh convertMesh(FbxMesh * mesh)
	{

		simpMesh jonConvertedMesh;
		const int vertCount = mesh->GetPolygonVertexCount();
		uint32_t poly = mesh->GetPolygonCount();

		//this is setting up uvs and normals
		getUvsAndNormals(mesh);
		
		getTheTransforms();


		for (unsigned int j = 0; j < poly; j++)
		{
			//at [1730] exactly the values start defaulting to 0
			jonConvertedMesh.indices.push_back(mesh->GetPolygonVertex(j, 0));
			jonConvertedMesh.indices.push_back(mesh->GetPolygonVertex(j, 1));
			jonConvertedMesh.indices.push_back(mesh->GetPolygonVertex(j, 2));
		}


		for (unsigned int i = 0; i < vertCount; i++)
		{
			//position data
			simpVert.pos[0] = mesh->GetControlPointAt(i).mData[0];
			simpVert.pos[1] = mesh->GetControlPointAt(i).mData[1];
			simpVert.pos[2] = mesh->GetControlPointAt(i).mData[2];
			simpVert.pos[3] = 1;
			//color data(white rn)
			simpVert.color[0] = 1.0f;
			simpVert.color[1] = 1.0f;
			simpVert.color[2] = 1.0f;
			simpVert.color[3] = 1.0f;
			//uv data
			simpVert.uv[0] = pushingUVs[i].x;
			simpVert.uv[1] = pushingUVs[i].y;
			//normal data
			simpVert.normals[0] = pushingNormals[i].x;
			simpVert.normals[1] = pushingNormals[i].y;
			simpVert.normals[2] = pushingNormals[i].z;



			//pos x 
			//indices x
			// normals x
			//tangents 
			//uvs x

			jonConvertedMesh.vertData.push_back(simpVert);
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

	void fbxFunctions::writeMaterial(simpMaterial jonConvertedMaterial)
	{
		ofstream fileOut;
		fileOut.open("projectMaterial.bin", ios_base::trunc | ios_base::binary);

		if (fileOut.is_open())
		{
			for (unsigned int i = 0; i < customMaterial.path.size(); i++)
			{
				int byteSizeOfPath = sizeof(strlen(customMaterial.path[i]));
				fileOut.write((const char*)&byteSizeOfPath, sizeof(byteSizeOfPath));
				fileOut.write((const char*)&customMaterial.path[i], byteSizeOfPath);
			}
			//fileOut.write((const char*)&customMaterial.path[0], sizeof(jonConvertedMaterial.path) * jonConvertedMaterial.path.size());

			fileOut.write((const char *)&customMaterial.texture, sizeof(customMaterial.texture));

			fileOut.write((const char *)&customMaterial.ambient, sizeof(customMaterial.ambient));
			fileOut.write((const char *)&customMaterial.diffuse, sizeof(customMaterial.diffuse));
			fileOut.write((const char *)&customMaterial.emissive, sizeof(customMaterial.emissive));
			fileOut.write((const char *)&customMaterial.reflection, sizeof(customMaterial.reflection));
			fileOut.write((const char *)&customMaterial.shininess, sizeof(customMaterial.shininess));
			fileOut.write((const char *)&customMaterial.specular, sizeof(customMaterial.specular));
			fileOut.write((const char *)&customMaterial.transparency, sizeof(customMaterial.transparency));
			int jointSize = sizeof(vec4) * customMaterial.jointTransforms.size();
			fileOut.write((const char*)&jointSize, sizeof(jointSize));
			fileOut.write((const char *)&customMaterial.jointTransforms[0], jointSize);

			fileOut.close();
		}


	}

	simpMaterial fbxFunctions::readMaterial(const char* filePath)
	{
		simpMaterial returnMat;
		ifstream read;
		int jointSze;
		read.open(filePath, ios_base::binary);
		if (read.is_open())
		{
			returnMat.path.resize(customMaterial.path.size());
			for (unsigned int i = 0; i < returnMat.path.size(); i++)
			{
				int sizeOfPath;
				read.read((char*)&sizeOfPath, sizeof(sizeOfPath));
				read.read((char*)&returnMat.path[i], sizeOfPath);
			}
			//	read.read((char*)&returnMat.path[0], sizeof(returnMat.path.size()));

			read.read((char*)&returnMat.texture, sizeof(returnMat.texture));
			read.read((char*)&returnMat.ambient, sizeof(returnMat.ambient));
			read.read((char*)&returnMat.diffuse, sizeof(returnMat.diffuse));
			read.read((char*)&returnMat.emissive, sizeof(returnMat.emissive));
			read.read((char*)&returnMat.reflection, sizeof(returnMat.reflection));
			read.read((char*)&returnMat.shininess, sizeof(returnMat.shininess));
			read.read((char*)&returnMat.specular, sizeof(returnMat.specular));
			read.read((char*)&returnMat.transparency, sizeof(returnMat.transparency));
			read.read(( char*)&jointSze, sizeof(jointSze));
			returnMat.jointTransforms.resize(jointSze / sizeof(vec4));
			read.read(( char *)&returnMat.jointTransforms[0], jointSze);

			read.close();
		}
		return returnMat;
	}


	simpMesh fbxFunctions::readMesh(const char* filePath)
	{
		simpMesh returnMesh;
		//maybe need a new vector?
		ifstream read;
		read.open(filePath, ios_base::binary);
		if (read.is_open())
		{
			int size;
			read.read((char*)&size, sizeof(size));
			returnMesh.vertData.resize(size / sizeof(SIMPLE_VERT));
			//if (read)
			//	std::cout << "all characters read successfully.\n";
			//else
			//	std::cout << "error: only " << read.gcount() << " could be read\n";



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


