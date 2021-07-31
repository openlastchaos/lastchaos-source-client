#include "stdafx.h"
#include "CmdSkaStudio.h"
#include "SeriousSkaStudio.h"
#include "MainFrm.h"


//------------------------------------------------------------------------

void CmdSkaSelectParticleEffect::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{

		pMain->m_dlgCEffectProperty.m_dlgCParticleEffectProperty.SelectEffect( wp_, lp_ );
	}
}

//------------------------------------------------------------------------

void CmdSkaTestEffect::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{		
		pMain->m_dlgEffectEditType.TestEffectSystem();
	}
}

void CmdSkaMainFrmCreateTexture::execute()
{
	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{		
		pMain->CreateTexture();
	}
}

void CmdSkaNewFile::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->FileNew();
	}
}

void CmdSkaOpenFile::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->FileOpen();
	}
}

void CmdSkaImportComvert::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ImportConvert();
	}
}

void CmdSkaComvertRecursive::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ConvertRecursive();
	}
}

void CmdSkaComvertTexRecursive::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ConvertTextureRecursive();
	}
}

void CmdSkaEffectFileOpen::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->EffectOpen();
	}
}

void CmdSkaEffectFileOpenAsAdd::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->EffectOpenAsAdd();
	}
}

void CmdSkaEffectFileSave::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->SaveEffect( strPath_.c_str() );
	}
}

void CmdSkaExportTexture::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ExportTexture( strPath_.c_str() );
	}
}

void CmdSkaExportEffectInfo::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ExportEffectInfo();
	}
}

void CmdExecuteEffect::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ExecuteEffect( bAutoSpell_ );
	}
}

void CmdSkaCreateItemSmcData::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->CreateItemSmcData();
	}
}

void CmdConvModel::execute()
{
	if (pModel_ == NULL)
		return;

	int i, j;

	int nMesh = pModel_->mi_aMeshInst.Count();

	for (i = 0; i < nMesh; ++i)
	{
		int nTex = pModel_->mi_aMeshInst[i].mi_tiTextures.Count();

		for (j = 0; j < nTex; ++j)
		{
			TextureInstance& rTexInst = pModel_->mi_aMeshInst[i].mi_tiTextures[j];

			CTextureData* pData = (CTextureData*)rTexInst.ti_toTexture.GetData();

			CTFileName fnTexture = rTexInst.ti_toTexture.GetName();

			{				
				CImageInfo ii;

				for (INDEX iFrame = 0; iFrame < pData->td_ctFrames; ++iFrame)
				{
					pData->Export_t( ii, iFrame);
					// obtain name for export file
					CTFileName fnFrame;
					if( pData->td_ctFrames == 1)
					{
						fnFrame = fnTexture.FileName().NoExt()+".tga";
					}
					else
					{
						fnFrame.PrintF("%s%03d.tga", (const char *)fnTexture.FileName().NoExt(), iFrame);
					}

					fnFrame = "C:\\Users\\Petrus7\\Desktop\\" + fnFrame;

					// if file exists, ask for substitution name
					if( FileExists( fnFrame) && iFrame==0 )
					{
						// Skip
						continue;
// 						CTString strDefaultDir = fnFrame.FileDir();
// 						CTString strDefaultFile = fnFrame.FileName()+fnFrame.FileExt();
// 						// invoke "Save as" dialog
// 						fnFrame = _EngineGUI.FileRequester( "Save As", FILTER_TGA FILTER_END,
// 							"Export texture directory", strDefaultDir, strDefaultFile, NULL, FALSE);
					}
					if( fnFrame != "")
					{
						ii.SaveTGA_t(fnFrame);
						ii.Clear();
					}
				}
			}
		}
	}
}
