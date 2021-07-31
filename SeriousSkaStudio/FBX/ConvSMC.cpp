#include "StdAfx.h"
#include "Common.h"
#include "ConvSMC.h"
#include <Engine/Ska/ModelInstance.h>
#include "SeriousSkaStudio.h"
#include "CmdSkaStudio.h"



ConvSMC::ConvSMC()
	: m_pFbxManager(NULL)
	, m_pFbxScene(NULL)
{
	init();
}

void ConvSMC::init()
{
	InitializeSdkObjects(m_pFbxManager, m_pFbxScene);

// 	bool lResult = CreateScene(m_pFbxScene);
// 
// 	if(lResult == false)
// 	{
// 		FBXSDK_printf("\n\nAn error occurred while creating the scene...\n");
// 		DestroySdkObjects(lSdkManager, lResult);
// 		return 0;
// 	}
}

bool ConvSMC::convert( CModelInstance* pModel )
{
	int i, j, n, t;
	int nMesh = pModel->mi_aMeshInst.Count();
	int nMeshlod;

	FbxNode* lRootNode = m_pFbxScene->GetRootNode();

	m_pFbxScene->SetName(pModel->GetName());

	for (i = 0; i < nMesh; ++i)
	{
		nMeshlod = pModel->mi_aMeshInst[i].mi_pMesh->msh_aMeshLODs.Count();

		FbxNode * pNode = FbxNode::Create(m_pFbxScene, "");

		for (j = 0; j < nMeshlod; ++j)
		{
			MeshLOD& rMesh = pModel->mi_aMeshInst[i].mi_pMesh->msh_aMeshLODs[j];

			// Vert
			int nVert = rMesh.mlod_aVertices.Count();

			FbxMesh* pMesh = FbxMesh::Create(m_pFbxScene, "");
			FbxSkin* pSkin = FbxSkin::Create(m_pFbxScene, "");
			pMesh->AddDeformer(pSkin);

			FbxLayer* lLayer = pMesh->GetLayer(0);
			if (lLayer == NULL)
			{
				pMesh->CreateLayer();
				lLayer = pMesh->GetLayer(0);
			}

			pMesh->SetName(rMesh.mlod_fnSourceFile.str_String);
			pMesh->InitControlPoints(nVert);
			FbxVector4* lControlPoints = pMesh->GetControlPoints();

			for (n = 0; n < nVert; ++n)
			{
				MeshVertex& rVert = rMesh.mlod_aVertices[n];

				FbxVector4 v(rVert.x, rVert.y, rVert.z);
				
				lControlPoints[n] = v;
			}

			// normal
			//FbxGeometryElementNormal* lNormalElement = pMesh->CreateElementNormal();
			FbxGeometryElementNormal* lNormalElement = FbxGeometryElementNormal::Create(pMesh, "");
			lNormalElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
			lNormalElement->SetReferenceMode(FbxGeometryElement::eDirect);
			lLayer->SetNormals(lNormalElement);

			int nNorm = rMesh.mlod_aNormals.Count();

			for (n = 0; n < nNorm; ++n)
			{
				MeshNormal& rNorm = rMesh.mlod_aNormals[n];
				FbxVector4 v(rNorm.nx, rNorm.ny, rNorm.nz);
				lNormalElement->GetDirectArray().Add(v);
			}

			// tangent
			FbxGeometryElementTangent* lTangentElement = pMesh->CreateElementTangent();
			lTangentElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
			lTangentElement->SetReferenceMode(FbxLayerElement::eDirect);

			int nTan = rMesh.mlod_aTangents.Count();

			for (n = 0; n < nTan; ++n)
			{
				MeshTangent& rTan = rMesh.mlod_aTangents[n];
				FbxVector4 v(rTan.bx, rTan.by, rTan.bz, rTan.tdir);
				lTangentElement->GetDirectArray().Add(v);
			}

			// uv		

			int nUV = rMesh.mlod_aUVMaps.Count();

			for (n = 0; n < nUV; ++n)
			{

				char buf[64] = {0,};

				sprintf(buf, "DiffuseUV_%02d", n);

				//FbxGeometryElementUV* lUVDiffuseElement = pMesh->CreateElementUV(buf);
				FbxGeometryElementUV* lUVDiffuseElement = FbxGeometryElementUV::Create(pMesh, buf);
				FBX_ASSERT( lUVDiffuseElement != NULL);
				lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
				lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eDirect);
				lLayer->SetUVs(lUVDiffuseElement);

				MeshUVMap& rUVMap = rMesh.mlod_aUVMaps[n];

				int nTexCoords = rUVMap.muv_aTexCoords.Count();

				//lUVDiffuseElement->GetDirectArray().SetCount(nTexCoords);

				for (t = 0; t < nTexCoords; ++t)
				{
					MeshTexCoord& rTex = rUVMap.muv_aTexCoords[t];

					FbxVector2 v(rTex.u, rTex.v);
					lUVDiffuseElement->GetDirectArray().Add(v);
				}				
			}

			// weight
			int nVW = rMesh.mlod_aVertexWeights.Count();
			FbxCluster* pCluster;
			int w;

			for (n = 0; n < nVW; ++n)
			{
				MeshVertexWeightInfo& rVW = rMesh.mlod_aVertexWeights[n];

				pCluster = FbxCluster::Create(m_pFbxScene, "");

				for (w = 0; w < 4; ++w)
				{
					if (rVW.mvwi_aubWeights[w] > 0)
					{
						pCluster->AddControlPointIndex(rVW.mvwi_aubIndices[w], (double)rVW.mvwi_aubWeights[w] / 255.0);
						//pCluster->SetLink();
						pCluster->SetLinkMode(FbxCluster::eTotalOne);
						pSkin->AddCluster(pCluster);
					}
				}				
			}

			// indices
			int nIds = rMesh.mlod_aSurfaces.Count();

			FbxGeometryElementUV* pUV = pMesh->GetElementUV("DiffuseUV_00");

			int idPoly = 0;

			for (n = 0; n < nIds; ++n)
			{
				MeshSurface& rSurf = rMesh.mlod_aSurfaces[n];

				int nTri = rSurf.msrf_auwTriangles.Count();

				//pMesh->SetPolygonGroup(idPoly, n);

				for (t = 0; t < nTri; ++t)
				{					
					MeshTriangle& rTri = rSurf.msrf_auwTriangles[t];

					pMesh->BeginPolygon();
					// UWROD[3]
					//rTri.mt_uwVertexIndex;
					for (int ni = 0; ni < 3; ++ni)
					{
						pMesh->AddPolygon(rTri.mt_uwVertexIndex[ni] + rSurf.msrf_iFirstVertex);

						if (pUV != NULL)
							pUV->GetIndexArray().Add(rTri.mt_uwVertexIndex[ni] + rSurf.msrf_iFirstVertex);
					}

					++idPoly;

					pMesh->EndPolygon();
				}

				// Texture
				ShaderParams *pShaderParams = &rSurf.msrf_ShadingParams;
				int nTex = pShaderParams->sp_aiTextureIDs.Count();
				CTextureObject* pTexObj;
				CTFileName fname;

				for (t = 0; t < nTex; ++t)
				{
					INDEX ctti = pModel->mi_aMeshInst[i].mi_tiTextures.Count();

					for (INDEX iti = 0; iti < ctti; iti++)
					{
						TextureInstance& ti = pModel->mi_aMeshInst[i].mi_tiTextures[iti];
						
						if(ti.GetID() == pShaderParams->sp_aiTextureIDs[t])
						{
							pTexObj = &ti.ti_toTexture;

							CTextureData* pData = (CTextureData*)pTexObj->GetData();

							if (pData != NULL)
							{
								fname = pData->GetName();

								fname = fname.FileName().NoExt();

								fname = fname + ".tga";
/*
								FbxLayer* lLayer = pMesh->GetLayer(0);
								// Create a layer element material to handle proper mapping.
								FbxLayerElementMaterial* lLayerElementMaterial = FbxLayerElementMaterial::Create(pMesh, "");

								// This allows us to control where the materials are mapped.  Using eAllSame
								// means that all faces/polygons of the mesh will be assigned the same material.
								lLayerElementMaterial->SetMappingMode(FbxLayerElement::eAllSame);
								lLayerElementMaterial->SetReferenceMode(FbxLayerElement::eIndexToDirect);

								// Save the material on the layer
								lLayer->SetMaterials(lLayerElementMaterial);

								// Add an index to the lLayerElementMaterial.  Since we have only one, and are using eAllSame mapping mode,
								// we only need to add one.
								lLayerElementMaterial->GetIndexArray().Add(0);								
*/
								FbxSurfaceMaterial* lSurfMat = create_material("material", "Phong");
								pNode->AddMaterial(lSurfMat);
								
								FbxFileTexture* pTex = create_textrue(fname.str_String);

								//FbxLayeredTexture* layeredTexture = FbxLayeredTexture::Create(m_pFbxManager, "layered texture");
								//layeredTexture->ConnectSrcObject(pTex);
								//layeredTexture->SetMappingType(FbxTexture::eUV);

								((FbxSurfacePhong*)lSurfMat)->Diffuse.ConnectSrcObject(pTex);
							}
						}
					}
				}
			}

			// weight
			FbxSkin* pSkin = FbxSkin::Create(m_pFbxScene, "");			
			pMesh->AddDeformer(pSkin);

			pNode->SetNodeAttribute(pMesh);
			pNode->SetShadingMode(FbxNode::eTextureShading);

			lRootNode->AddChild(pNode);
		}
	}

	conv_skeleton(pModel);
	conv_anim(pModel);

	SaveScene(m_pFbxManager, m_pFbxScene, "C:\\Users\\Petrus7\\Desktop\\test.fbx");

	CmdConvModel* pCmd = new CmdConvModel;
	pCmd->setData(pModel);
	theApp.addQueue(pCmd);

	return true;
}

bool ConvSMC::conv_mesh( FbxManager* pMgr, FbxScene* pScene )
{

	//FbxMesh::Create(pScene, "")
	return true;
}

FbxSurfaceMaterial* ConvSMC::create_material( const char* mat_name, const char* shading_name )
{
	FbxDouble3 lBlack(0.0, 0.0, 0.0);
	FbxDouble3 lRed(0.0, 1.0, 0.0);
	FbxDouble3 lDiffuseColor(1.0, 1.0, 1.0);

	FbxSurfacePhong* pMaterial = FbxSurfacePhong::Create(m_pFbxScene, mat_name);
	pMaterial->Emissive.Set(lBlack);
	pMaterial->Ambient.Set(FbxDouble3(0.7, 0.7, 0.7));
	pMaterial->Diffuse.Set(lDiffuseColor);
	pMaterial->TransparencyFactor.Set(40.5);
	pMaterial->ShadingModel.Set(shading_name);
	pMaterial->Shininess.Set(0.5);
	pMaterial->Specular.Set(FbxDouble3(0.0, 0.0, 0.0));
	pMaterial->SpecularFactor.Set(FbxDouble(1.0));

	return pMaterial;
}

FbxFileTexture* ConvSMC::create_textrue( const char* tex_name )
{
	FbxFileTexture* lTexture = FbxFileTexture::Create(m_pFbxScene, "");

	// Resource file must be in the application's directory.
	//FbxString lPath = FbxGetApplicationDirectory();
	//FbxString lTexPath = "C:/Users/Petrus7/Desktop/texture/";
	//lTexPath += tex_name;

	// Set texture properties.
	lTexture->SetFileName(tex_name);
	lTexture->SetName("Diffuse Texture");
	lTexture->SetTextureUse(FbxTexture::eStandard);
	lTexture->SetMappingType(FbxTexture::eUV);
	lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
	lTexture->SetSwapUV(false);
	lTexture->SetAlphaSource (FbxTexture::eNone);
	lTexture->SetTranslation(0.0, 0.0);
	lTexture->SetScale(1.0, 1.0);
	lTexture->SetRotation(180.0, 0.0);		// Tex X-Axis flip BMP?

	return lTexture;
}

bool ConvSMC::conv_skeleton( CModelInstance* pModel )
{
	CSkeleton* pSkeleton = pModel->mi_psklSkeleton;

	if (pSkeleton == NULL)
		return false;

	// root µî·Ï
	FbxSkeleton* pSkel = FbxSkeleton::Create(m_pFbxScene, pSkeleton->GetName());
	pSkel->SetSkeletonType(FbxSkeleton::eRoot);
	FbxNode* pNode = FbxNode::Create(m_pFbxScene, "Root");
	pNode->SetNodeAttribute(pSkel);
	pNode->LclTranslation.Set(FbxDouble3(0.0, 0.0, 0.0));
	pNode->LclRotation.Set(FbxDouble3(0.0, 180.0, 0.0));

	FbxNode* lRootNode = m_pFbxScene->GetRootNode();
	lRootNode->AddChild(pNode);

	FbxNode* pNodeParent = pNode;	

	int i, j;
	int nSkelLOD = pSkeleton->skl_aSkeletonLODs.Count();

	for (i = 0; i < nSkelLOD; ++i)
	{
		SkeletonLOD& rSkelLOD = pSkeleton->skl_aSkeletonLODs[i];

		int nBone = rSkelLOD.slod_aBones.Count();

		for (j = 0; j < nBone; ++j)
		{
			SkeletonBone& rBone = rSkelLOD.slod_aBones[j];

			CTString strName = ska_GetStringFromTable(rBone.sb_iID);

			pSkel = FbxSkeleton::Create(m_pFbxScene, strName.str_String);
			pSkel->SetSkeletonType(FbxSkeleton::eLimbNode);
			pNode = FbxNode::Create(m_pFbxScene, strName.str_String);
			pNode->SetNodeAttribute(pSkel);

			FbxDouble3 vPos = FbxDouble3(rBone.sb_qvRelPlacement.vPos(1), 
										 rBone.sb_qvRelPlacement.vPos(2),
										 rBone.sb_qvRelPlacement.vPos(3));

			pNode->LclTranslation.Set(vPos);

			FLOAT3D vecRot;

			rBone.sb_qvRelPlacement.qRot.ToEuler(vecRot);

			FbxDouble3 vRot = FbxDouble3(AngleRad(vecRot(1)), AngleRad(vecRot(2)), AngleRad(vecRot(3)));

			pNode->LclRotation.Set(vRot);
			//pSkel->LimbLength

			pSkel->Size.Set(FbxDouble(rBone.sb_fBoneLength));

			m_mapSkel[rBone.sb_iID] = pNode;

			if (rBone.sb_iParentID >= 0)
			{
				map_skel_iter iter = m_mapSkel.find(rBone.sb_iParentID);

				if (iter == m_mapSkel.end())
					continue;

				pNodeParent = iter->second;
			}

			pNodeParent->AddChild(pNode);
		}
	}

	return true;
}

bool ConvSMC::create_patch( const char* pName )
{
	FbxPatch* lPatch = FbxPatch::Create(m_pFbxScene, pName);



	return true;
}

bool ConvSMC::conv_anim( CModelInstance* pModel )
{
	int	i, j, bn, ev;
	int nAnimSet = pModel->mi_aAnimSet.Count();

	FbxNode* pRoot = m_pFbxScene->GetRootNode();
	FbxMesh* pMesh = pRoot->GetMesh();

	for (i = 0; i < nAnimSet; ++i)
	{
		CAnimSet& rAnimSet = pModel->mi_aAnimSet[i];

		//FbxAnimStack
		FbxAnimStack* pAnimStack = FbxAnimStack::Create(m_pFbxScene, rAnimSet.GetName().str_String);

		int nAnim = rAnimSet.as_Anims.Count();

		for (j = 0; j < nAnim; ++j)
		{
			Animation& rAnim = rAnimSet.as_Anims[j];

			rAnim.an_iID;

			//FbxAnimLayer
			FbxAnimLayer* pAnimLayer = FbxAnimLayer::Create(m_pFbxScene, rAnim.an_fnSourceFile.str_String);
			pAnimStack->AddMember(pAnimLayer);
	
			int nAnimBone = rAnim.an_abeBones.Count();
			FbxNode* pNode;

			for (bn = 0; bn < nAnimBone; ++bn)
			{
				BoneEnvelope& rBE = rAnim.an_abeBones[bn];

				INDEX iID = pModel->mi_psklSkeleton->FindBoneInLOD(rBE.be_iBoneID, 0);

				map_skel_iter iter = m_mapSkel.find(rBE.be_iBoneID);

				if (iter == m_mapSkel.end())
					continue;

				pNode = iter->second;

				// Pos
				int nPos = rBE.be_apPos.Count();

				FbxTime lTime;
				int lKeyIndex = 0;
				FbxAnimCurve* lCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_TRANSLATION, true);

				if (lCurve == NULL)
					continue;

				lCurve->KeyModifyBegin();

				for (ev = 0; ev < nPos; ++ev)
				{
					AnimPos& rPos = rBE.be_apPos[ev];

					lTime.SetSecondDouble(rPos.ap_iFrameNum * rAnim.an_fSecPerFrame);
					lKeyIndex = lCurve->KeyAdd(lTime);
					//lCurve->KeySetValue(lKeyIndex, 0.0);
					lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
				}

				lCurve->KeyModifyEnd();

				// Rot
			}
		}
	}

	return true;
}


