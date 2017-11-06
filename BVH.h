#pragma once

#include <d3d11.h>
#include "DAY1.h"
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h> 

class BVH
{
public:
	BVH();
	~BVH();

	struct node
	{
		int type;
		node* parent;

		union
		{
			struct
			{
				node* left;
				node* right;
			};

		};
		
		XMFLOAT3 normals;
		XMFLOAT3 centroid;







	};

};