#ifndef		CONV_SMC_H_
#define		CONV_SMC_H_

#include <fbxsdk.h>

class CModelInstance;

class ConvSMC
{
public:
	ConvSMC();

	void init();
	bool convert(CModelInstance* pModel);

private:
	typedef std::map<int, FbxNode*>		map_skel;
	typedef map_skel::iterator			map_skel_iter;

	bool conv_mesh(FbxManager* pMgr, FbxScene* pScene);
	FbxSurfaceMaterial* create_material(const char* mat_name, const char* shading_name);
	FbxFileTexture* create_textrue(const char* tex_name);
	bool conv_skeleton(CModelInstance* pModel);
	bool create_patch(const char* pName);
	bool conv_anim(CModelInstance* pModel);
	//FbxNode* change_mesh(CMeshLOD);

	FbxManager* m_pFbxManager;
	FbxScene* m_pFbxScene;
	bool lResult;
	map_skel m_mapSkel;
};


#endif		// CONV_SMC_H_