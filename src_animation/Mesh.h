#pragma once

#include "MeshAnimation.h"
//##################################################################//
// Ogre XML Mesh Class
//##################################################################//


class Mesh { 

public:
	// ---------------------------------------- //
	// Structures
	// ---------------------------------------- //
	struct Triangle
	{
		int index[3];	// vertex    ID
		int material;	// material  ID
	};
	// ---------------------------------------- //
	class Vertex
	{
		public: 

		// general
		vec3f position;
		vec3f texcoord;
		vec3f normal;
		vec4f tangent;

		// skinning information
		float weight[3];
		int boneindex[3],count;

		// initialization
		Vertex()
		{
			count=0;
			weight[0]=weight[1]=weight[2]=0;
			boneindex[0]=boneindex[1]=boneindex[2]=0;
		};
	};
	// ---------------------------------------- //
	class Texture
	{
		public:
		Texture(){envmap=0;gl_handle=-1;}

		std::string filename;
		int gl_handle;
		bool envmap;
	};
	// ---------------------------------------- //
	class Material
	{
		public:

		Material()
		{
			ambient = vec3f ( 0.6 , 0.3 ,0 );
			diffuse = vec3f ( 0.3 , 0.3 ,0.3 );
			specular= vec3f ( 0,0,0 );
			emissive= vec3f ( 0,0,0 );
			diffuse_map.gl_handle=-1;
			emissive_map.gl_handle=-1;
			ambient_map.gl_handle=-1;
			alpha = 1;
			reflect = 0;
			name = "";
		}
		std::string name;
		vec3f diffuse;
		vec3f specular;
		vec3f ambient;
		vec3f emissive;
		float alpha,reflect;
		Texture diffuse_map;
		Texture emissive_map;
		Texture ambient_map;
		Texture bump_map;
	};
	// ---------------------------------------- //
	struct DisplayList // GL displaylist for rendering
	{ 
		int list_id;
		int material;
	};
	// ---------------------------------------- //
	struct LOD // level of detail mesh
	{
		std::vector<Triangle>		triangles;
		std::vector<DisplayList>	displaylists;
	};	
	// ---------------------------------------- //
	// Main OBJ Part
	// ---------------------------------------- //
	bool					skinning;	// skinning on ?
	std::vector<LOD>		lod_meshes;	// triangles & displaylists for all LODs
	std::vector<Vertex>		vertices;	// vertex list
	std::vector<Material>	materials;	// material list
	MeshAnimation			animation;	// animation tracks + skeleton
	// ---------------------------------------- //
	// Constructor / Destructor
	// ---------------------------------------- //
	Mesh(){};
	Mesh(const char* ogrematerialfilename,const char* ogremeshfilenamexml)
	{
		printf("\n\n");
		printf("################################################\n");
		printf("## \n");
		printf("## Loading Mesh %s \n",ogremeshfilenamexml);
		printf("## \n");
		printf("################################################\n\n",ogremeshfilenamexml);

		LoadOgreXML(ogrematerialfilename,ogremeshfilenamexml);

		printf("\n\n");
	}
	Mesh(const char* ogrematerialfilename, const char* ogremeshfilenamexml, const char* ogreskeletonfilenamexml)
	{
		printf("\n\n");
		printf("################################################\n");
		printf("## \n");
		printf("## Loading Mesh %s \n", ogremeshfilenamexml);
		printf("## \n");
		printf("################################################\n\n", ogremeshfilenamexml);

		if (ogreskeletonfilenamexml) animation.LoadSkeletonXML(ogreskeletonfilenamexml);
		if (ogreskeletonfilenamexml) skinning = 1;

		if (!ogremeshfilenamexml)
			LoadOreMaterial(ogrematerialfilename);
		else
			LoadOgreXML(ogrematerialfilename, ogremeshfilenamexml);
		
		printf("\n\n");
	}
	~Mesh(){}
	// ---------------------------------------- //
	void InitDisplaylists()
	{
		loopk(0,lod_meshes.size())
		{
			std::vector<Triangle>		&triangles=lod_meshes[k].triangles;
			std::vector<DisplayList>	&displaylists=lod_meshes[k].displaylists;

			DisplayList dl;	int mtl=0;

			loopi(0,triangles.size())
			{
				Triangle &t=triangles[i];

				if(mtl!=t.material || i==0)
				{
					if(i>0)
					{
						glEnd();
						glEndList();
						displaylists.push_back(dl);
					}
					dl.material= mtl=t.material;
					dl.list_id = glGenLists(1);
					glNewList(dl.list_id, GL_COMPILE);
					glBegin(GL_TRIANGLES);
				}
				
				// Triangle vertices
				loopj(0,3)
				{
					Vertex v=vertices[t.index[j]];

					// normal
					glNormal3f(v.normal.x,v.normal.y,v.normal.z);

					// texture uv + packed tangent
					vec3f tc=v.texcoord;
					float tx=int((v.tangent.x*0.5+0.5)*255.0);
					float ty=int((v.tangent.y*0.5+0.5)*255.0);
					float tz=int((v.tangent.z*0.5+0.5)*255.0);
					tc.z=(tx+ty/256.0+tz/65536.0)*v.tangent.w; // w is 1 or -1
					//glTexCoord3f(tc.x,1.0-tc.y,tc.z);
					glTexCoord3f(tc.x, 1.0 - tc.y, 1);


					// skinning
					float packedboneids=0.0;
					if(skinning) // skinning on
					{
						glColor3f(v.weight[0],v.weight[1],v.weight[2]);
						packedboneids=	float(v.boneindex[0]) +
										float(v.boneindex[1])/ 128.0 +
										float(v.boneindex[2])/(128.0*128.0);
					}
					// position + packed bone ids
					glVertex4f(v.position.x,v.position.y,v.position.z,packedboneids);
				}
			}
			glEnd();
			glEndList();
			displaylists.push_back(dl);

			// save memory	
			std::vector<Triangle>().swap(triangles);
		}
		// save memory					
		std::vector<Vertex>().swap(vertices);
	};
	// -------------- materials ------------ //
	int GetMaterialIndex ( std::string name )
	{
		loopi(0,materials.size() )
			if ( name.compare( materials[i].name ) == 0 ) return i;

		printf("couldnt find material %s\n",name.c_str() );
		return -1;
	}
	void PrintMaterials ()
	{
		printf("\n");
		for(uint i=0;i<materials.size();i++)
		{
			printf("Material %i : %s\n",i,materials[i].name.c_str());
			printf("  Ambient RGB %2.2f %2.2f %2.2f\n",	materials[i].ambient.x,materials[i].ambient.y,materials[i].ambient.z);			
			printf("  Specular RGB %2.2f %2.2f %2.2f\n",materials[i].specular.x,materials[i].specular.y,materials[i].specular.z);
			printf("  Emissive RGB %2.2f %2.2f %2.2f\n",materials[i].emissive.x,materials[i].emissive.y,materials[i].emissive.z);			
			printf("  Diffuse RGB %2.2f %2.2f %2.2f\n",	materials[i].diffuse.x,	materials[i].diffuse.y,	materials[i].diffuse.z);
			printf("  Diff. Tex : %s [Env %d]\n",materials[i].diffuse_map.filename.c_str(),materials[i].diffuse_map.envmap	);
			printf("  Amb . Tex : %s \n",materials[i].ambient_map.filename.c_str()	);
			printf("  Bump  Tex : %s \n",materials[i].bump_map.filename.c_str()	);
			printf("  Alpha %2.2f\n", materials[i].alpha);
		}
		printf("\n");
	}
	// ----------------------------- //
	void Draw(vec3f pos=vec3f(0,0,0),vec3f rot=vec3f(0,0,0),vec3f scale=vec3f(1,1,1), int lod=0,bool drawskeleton=0)
	{
		if(lod>=lod_meshes.size()) return;
		std::vector<DisplayList> &displaylists=lod_meshes[lod].displaylists;


		matrix44 mat;
		glGetFloatv(GL_MODELVIEW_MATRIX,  &mat.m[0][0]);
		vec4f lightvec_objspc=mat*lightvec;
		lightvec_objspc.norm();

		glPushMatrix();		
		//glTranslatef(pos.x,pos.y,pos.z); //设置当前模型位置
		glRotatef(rot.x*360/(2*M_PI),1,0,0);
		glRotatef(rot.y*360/(2*M_PI),0,1,0);
		glRotatef(rot.z*360/(2*M_PI),0,0,1);

		//kumahime修改
		glScalef(scale.x, scale.y, scale.z);

		if(drawskeleton)
		{
			for (int i = 0; i < animation.bones.size(); i++)
			{
				glPushMatrix();
				//glColor3f(1,1,1);
				glMultMatrixf((float*)animation.bones[i].matrix.m);
				//printf("x%f y%f z%f\n", animation.bones[i].matrix.m[3][0], animation.bones[i].matrix.m[3][1], animation.bones[i].matrix.m[3][2]);
				glutSolidCube(0.1);
				glPopMatrix();
			}
			glPopMatrix();
			return;
		}
		
		if(lod_meshes[0].displaylists.size()==0) InitDisplaylists();

		static Shader tri_shader ("../shader/mesh");
		static int tex_default=ogl_tex_bmp("../data/white.png");
		static int tex_default_bump=ogl_tex_bmp("../data/grey.png");	

		tri_shader.begin();
		if(skinning) 
		{
			// skinning enabled
			matrix44 bones[100];
			//loopi(0,animation.bones.size())	bones[i]=animation.bones[i].invbindmatrix*animation.bones[i].matrix;
			loopi(0, animation.bones.size())	bones[i] = animation.bones[i].invbindmatrix*animation.bones[i].matrix;
			tri_shader.setUniformMatrix4fv("bones",animation.bones.size(),&bones[0].m[0][0]);
			tri_shader.setUniform1i("use_skinning",1);
		}
		else
		{
			// skinning disabled
			tri_shader.setUniform1i("use_skinning",0);
		}

		tri_shader.setUniform4f("lightvec",lightvec_objspc.x,lightvec_objspc.y,lightvec_objspc.z,0);

		loopi(0,displaylists.size())
		{
			DisplayList dl=displaylists[i];			
			Material &mtl=materials[dl.material];
			vec3f di=mtl.diffuse;
			vec3f am=mtl.ambient+mtl.emissive;
			vec3f sp=mtl.specular;
			int tex_dif = (mtl.diffuse_map.gl_handle<0) ? tex_default:mtl.diffuse_map.gl_handle;
			int tex_amb = (mtl.ambient_map.gl_handle<0) ? tex_default:mtl.ambient_map.gl_handle;
			int tex_bump= (mtl.bump_map.gl_handle<0)	? tex_default_bump:mtl.bump_map.gl_handle;
			ogl_bind(2,tex_bump);
			ogl_bind(1,tex_amb);
			ogl_bind(0,tex_dif);				
			tri_shader.setUniform4f("ambient",am.x,am.y,am.z,0);
			tri_shader.setUniform4f("diffuse",di.x,di.y,di.z,0);
			tri_shader.setUniform4f("specular",sp.x,sp.y,sp.z,0);
			tri_shader.setUniform1i("texDiffuse",0);
			tri_shader.setUniform1i("texAmbient",1);
			tri_shader.setUniform1i("texBump"	,2);
			tri_shader.setUniform1f("dif_envmap", mtl.diffuse_map.envmap ? 1 : 0 );
			glCallList(dl.list_id);
		}
		tri_shader.end();
		loopi(0,3) ogl_bind(2-i,tex_default);
		glPopMatrix();
		ogl_check_error();
	}
	void LoadOreMaterial(const char* name_material)
	{
		FILE* fn = fopen(name_material, "rb");
		if (!fn) error_stop("File %s not found!\n", name_material);

		char line[1000], name[1000];
		memset(line, 0, 1000);
		memset(name, 0, 1000);
		Material mat, empty_mat;

		while (fgets(line, 1000, fn) != NULL)
		{
			if (sscanf(line, "material %s", name) == 1){ materials.push_back(mat); mat = empty_mat; mat.name = name; }
			sscanf(line, " ambient %f %f %f", &mat.ambient.x, &mat.ambient.y, &mat.ambient.z);
			sscanf(line, " diffuse %f %f %f", &mat.diffuse.x, &mat.diffuse.y, &mat.diffuse.z);
			sscanf(line, " specular %f %f %f", &mat.specular.x, &mat.specular.y, &mat.specular.z);
			sscanf(line, " emissive %f %f %f", &mat.emissive.x, &mat.emissive.y, &mat.emissive.z);
			if (sscanf(line, " texture %s", name) == 1)
			{
				Texture *texture = &mat.diffuse_map; bool bump = 0;
				if (strcmp(name, "_unit") == 0)continue;
				if (strcmp(name, "_bump") == 0)
				{
					bump = 1;
					sscanf(line, " texture_bump %s", name);
					texture = &mat.bump_map;
				}
				if (strcmp(name, "_ambient") == 0)
				{
					sscanf(line, " texture_ambient %s", name);
					texture = &mat.ambient_map;
				}
				texture->filename = get_path(name_material) + std::string(name);
				Bmp bmp(texture->filename.c_str());
				if (bump) bmp.MakeBump();
				texture->gl_handle = ogl_tex_bmp(bmp, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
				printf("gl_handle:%d\n", texture->gl_handle);
			}
			if (sscanf(line, " env_map %s", name) == 1)
			{
				if (strcmp(name, "spherical") == 0)	mat.diffuse_map.envmap = 1;
			}
		}
		materials.push_back(mat);
		PrintMaterials();
	};

	void LoadOreMesh(const char* name_mesh)
	{
		FILE* fn = fopen(name_mesh, "rb");
		if (!fn){ printf("File %s not found!\n", name_mesh); while (1); }

		char line[1000], name[1000];
		memset(line, 0, 1000);
		memset(name, 0, 1000);

		Triangle tri; vec3f v; int a1, a2; float w;
		tri.material = 0;

		int offset = 0;

		std::vector<Triangle> triangles;
		std::vector<int>  lodmeshindex_offset;	// for LOD
		std::vector<int>  lodmeshindex_material;// for LOD		

		int normal_id = 0, texcoord_id = 0, tangent_id = 0;

		while (fgets(line, 1000, fn) != NULL)
		{
			if (sscanf(line, " <face v1=\"%d\" v2=\"%d\" v3=\"%d\" />", &tri.index[0], &tri.index[1], &tri.index[2]) == 3)
			{
				loopi(0, 3) tri.index[i] += offset;
				triangles.push_back(tri);
			}
			if (sscanf(line, " <position x=\"%f\" y=\"%f\" z=\"%f\" />", &v.x, &v.y, &v.z) == 3){ Vertex a; a.position = v; vertices.push_back(a); }
			if (sscanf(line, " <normal x=\"%f\" y=\"%f\" z=\"%f\" />", &v.x, &v.y, &v.z) == 3) vertices[normal_id++].normal = v;
			if (sscanf(line, " <texcoord u=\"%f\" v=\"%f\" />", &v.x, &v.y) == 2)  vertices[texcoord_id++].texcoord = v;
			if (sscanf(line, " <tangent x=\"%f\" y=\"%f\" z=\"%f\" w=\"%f\"", &v.x, &v.y, &v.z, &w) == 4)vertices[tangent_id++].tangent = vec4f(v.x, v.y, v.z, w);
			if (sscanf(line, " <vertexboneassignment vertexindex=\"%d\" boneindex=\"%d\" weight=\"%f\" />", &a1, &a2, &w) == 3)
			{
				a1 += offset;
				if (a1 >= vertices.size()) error_stop("vertexboneassignment %d (%d) >= %d", a1, a1 - offset, vertices.size());

				int i = vertices[a1].count;
				if (i<3)
				{
					vertices[a1].weight[i] = w;
					vertices[a1].boneindex[i] = a2;
					vertices[a1].count++;
				}
			}
			if (sscanf(line, "        <submesh material=\"%s\" usesharedvertices", name) == 1)
			{
				offset = vertices.size();
				name[strlen(name) - 1] = 0;
				tri.material = GetMaterialIndex(name);
				lodmeshindex_offset.push_back(offset);
				lodmeshindex_material.push_back(tri.material);
			}
			if (sscanf(line, " <lodfacelist submeshindex=\"%d\" numfaces=", &a1) == 1)
			{
				if (a1<0 || a1 >= lodmeshindex_offset.size()) error_stop("submeshindex %d >= %d", a1, lodmeshindex_offset.size());
				offset = lodmeshindex_offset[a1];
				tri.material = lodmeshindex_material[a1];
			}
			if (sscanf(line, " <lodgenerated value=\"%f\">", &w) == 1)
			{
				LOD l; l.triangles = triangles;
				lod_meshes.push_back(l);
				triangles.clear();
			}
		}

		if (triangles.size()>0)
		{
			LOD l; l.triangles = triangles;
			lod_meshes.push_back(l);
		}

		loopi(0, lod_meshes.size()) printf("LOD %d Triangles %d\n", i, lod_meshes[i].triangles.size());
		printf("\n");
	}

	// ---------- Load ogre Obj ------------ //
	void LoadOgreXML(const char* name_material, const char* name_mesh)
	{
		LoadOreMaterial(name_material);
		LoadOreMesh(name_mesh);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#######################################################################//
// Costume类(由于不在这里编写会出现很多未知错误，所以将类放在了Mesh.h之中。)
//#######################################################################//
#include <windows.h>

static const int DATA_LEN = 3; //规定数据长度为3，给线程用
extern HGLRC glrc1;
extern HGLRC glrc2;
extern HDC dc;

struct loadData
{
	Mesh* m_Costume;
	char strs[3][60];
	bool* loading;
	HDC hDC;
};
class Costume
{
public:
	
	Costume(costumeType type);

	void setCostume(const char* ogrematerialfilename, const char* ogremeshfilenamexml, const char* ogreskeletonfilenamexml); //阻塞，小心
	void drawCostume(const NUI_SKELETON_BONE_ORIENTATION* boneData, const NUI_SKELETON_DATA* skeletonData, vector2 videoSize);
	bool isLoading();
	void adjustBone(int, GLfloat);

//	void addBone_xyz(GLfloat x, GLfloat y, GLfloat z);
private:
	Mesh* m_Costume = new Mesh(); //穿着的服装

	costumeType m_CostumeType;
	loadData loadDs;
	char* strs[DATA_LEN]; //数据
	int* boneArray; //骨骼数据顺序存放
	int boneArrayLen;
	bool loading = false;  //读取中标记
	bool haveCos = false; //有无服装
	HANDLE loadThread = NULL;  //读取线程句柄
	DWORD dwThread;

	void setBone(Mesh* m, int id, const Vector4* totalRotation, Vector4 Position, vector2 videoSize)
	{
		//matrix44 matrix(quaternion(totalRotation->x, totalRotation->y, totalRotation->z, totalRotation->w));
		m->animation.bones[id].matrix.set(quaternion(totalRotation->x, totalRotation->y, totalRotation->z, totalRotation->w));
		/*
		if (id == 0)
			m->animation.bones[id].matrix.set(matrix);
		else
			m->animation.bones[id].matrix = matrix * m->animation.bones[id].matrix;

		loopi(0, m->animation.bones[id].childs.size())
		{
			m->animation.bones[m->animation.bones[id].childs[i]].matrix.set(m->animation.bones[id].matrix);
		}*/

		//m->animation.bones[id].matrix.set_translation(vector3(Position.x * videoSize.x, Position.y  * videoSize.y, Position.z)); //无缩放
		//m->animation.bones[id].matrix.set_translation(vector3(Position.x * videoSize.x * 0.87, Position.y * videoSize.y, Position.z));
		m->animation.bones[id].matrix.set_translation(vector3(Position.x * videoSize.x * 0.87 * 1, Position.y  * videoSize.y * 1, Position.z * 1));
	}
};

Costume::Costume(costumeType type)
{
	int COCT;
	switch (type)
	{
	case costumeType::HAT:
		boneArrayLen = 1;
		boneArray = new int[boneArrayLen];
		boneArray[0] = 3;
		break;
	case costumeType::CLOTH:
		boneArrayLen = 9;
		boneArray = new int[boneArrayLen];
		COCT = 1;
		loopi(0, 8)
		{
			if (2 == i | 5 == i)
				COCT++;
			boneArray[i] = COCT + i;
		}
		boneArray[8] = 0;
		break;
	case costumeType::TROUSER:
		boneArrayLen = 7;
		boneArray = new int[boneArrayLen];
		boneArray[0] = 0;
		COCT = 11;
		loopi(1, 7)
		{
			if (4 == i)
				COCT++;
			boneArray[i] = COCT + i;
		}
		break;
	case costumeType::SHOES:
		boneArrayLen = 2;
		boneArray = new int[boneArrayLen];
		boneArray[0] = 15;
		boneArray[1] = 19;
		break;
	case costumeType::TEST:
		boneArrayLen = 7;
		boneArray = new int[boneArrayLen];
		boneArray[0] = 0;

		break;
	default:
		boneArrayLen = 20;
		boneArray = new int[boneArrayLen];
		for (int i = 0; i < 20; i++)
		{
			boneArray[i] = i;
		}
		break;
	}
	loadDs.loading = &loading;
	loadDs.m_Costume = m_Costume;
	m_CostumeType = type;
}

DWORD WINAPI load(LPVOID lpParameter) //openGL创建线程问题多多，先放一放
{
	loadData* data = (loadData*)lpParameter;
	*data->loading = true; //标记load为读取中

	//wglMakeCurrent(dc, glrc2);

	data->m_Costume->LoadOreMesh(data->strs[1]); //初始化服装3D模型
	//wglMakeCurrent(NULL, NULL);
	*data->loading = false; //标记load为读取完毕
	return 1;
}

void Costume::setCostume(const char* ogrematerialfilename, const char* ogremeshfilenamexml, const char* ogreskeletonfilenamexml)
{
	/*
	delete(m_Costume);
	m_Costume = new Mesh(ogrematerialfilename, ogremeshfilenamexml, ogreskeletonfilenamexml); //初始化服装
	loading = false;
	*/

	strcpy(loadDs.strs[0], ogrematerialfilename);
	strcpy(loadDs.strs[1], ogremeshfilenamexml);
	strcpy(loadDs.strs[2], ogreskeletonfilenamexml);
	if (loadThread)
		CloseHandle(loadThread);
	loading = true;
	haveCos = true;
	delete(m_Costume);
	m_Costume = new Mesh(loadDs.strs[0], NULL, loadDs.strs[2]); //初始化Mesh(包括骨骼和材质) 但是没有加入mesh数据！ 由于线程中使用openGL的函数会不安全，没有搞定，就用了这种方法...
	loadDs.m_Costume = m_Costume;
	loadThread = CreateThread(NULL, 0, load, &loadDs, 0, &dwThread); //创建线程，读取最消耗资源的3D模型
}

void Costume::drawCostume(const NUI_SKELETON_BONE_ORIENTATION* boneData, const NUI_SKELETON_DATA* skeletonData,vector2 videoSize)
{
	if (!(loading || !haveCos))
	{
		loopi(0, boneArrayLen)
		{

			char str[3];
			sprintf(str, "%d", boneArray[i]);
			Vector4 totalRotation = boneData[boneArray[i]].absoluteRotation.rotationQuaternion; //骨骼旋转角度（四元数，并没有骨骼位置信息）
			int child_id = m_Costume->animation.GetBoneIndexOf(str);
			if (NUI_SKELETON_POSITION_KNEE_LEFT == boneArray[i] || NUI_SKELETON_POSITION_KNEE_RIGHT == boneArray[i])
			{
				setBone(m_Costume, child_id, &totalRotation, skeletonData->SkeletonPositions[boneArray[i] - 1], videoSize);
				continue;
			}
			setBone(m_Costume, child_id, &totalRotation, skeletonData->SkeletonPositions[boneArray[i]], videoSize);
		}
		//if (costumeType::CLOTH == m_CostumeType)
			//m_Costume->Draw(vec3f(0, 0, 2), vec3f(0, 0, 0), vec3f(-1.5, 1.5, 1.5));
		//else
			m_Costume->Draw(vec3f(0, 0, 2), vec3f(0, 0, 0), vec3f(-1., 1., 1.));
	}
}

bool Costume::isLoading()
{
	return loading;
}

void Costume::adjustBone(int id, GLfloat roat)
{
	m_Costume->animation.bones[id].matrix.rotate_z(roat);

}