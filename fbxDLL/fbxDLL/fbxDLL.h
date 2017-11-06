#pragma once
#include "simpleMesh.h"
#ifdef JON_FBX_EXPORTS  
#define FBX_API __declspec(dllexport)   
#else  
#define FBX_API __declspec(dllimport)   
#endif  

namespace fbx
{
	class fbxFunctions
	{
	public:
		static FBX_API void writeMesh(simpMesh jonConvertedMesha);
		static FBX_API simpMesh readMesh(const char* filePath);
		static FBX_API void PrintTabs();
		static FBX_API void setup(const char* name);
		static FBX_API void getMaterial();
		static FBX_API simpMesh getTheMesh();
//		static FBX_API void PrintAttribute(FbxNodeAttribute* pAttribute);
	//	static FBX_API void PrintNode(FbxNode* pNode);
		
	};

}