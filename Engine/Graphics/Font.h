#ifndef SE_INCL_FONT_H
#define SE_INCL_FONT_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/FileName.h>
#include <Engine/Base/Serial.h>

// font flags
#define FNF_FIXED     (1UL<<0)  // treat font as fixed width (adjustable in run-time)
#define FNF_CELLBASED (1UL<<1)  // old font format - printout whole char cell
#define FNF_DISPOSED  (1UL<<2)  // mark that texture coords was disposed (if texture was disposed also)


// some default fonts
ENGINE_API extern CFontData *_pfdDisplayFont;
ENGINE_API extern CFontData *_pfdConsoleFont;


/*
 * font letter description
 */
class ENGINE_API CFontCharData {
public:
  PIX fcd_pixXOffset, fcd_pixYOffset; // offset of letter inside tex file (in pixels)
  PIX fcd_pixStart, fcd_pixEnd;       // position and size adjustment for current letter
  // constructor
  CFontCharData(void);
  // simple stream functions
  void Read_t(  CTStream *inFile);
  void Write_t( CTStream *outFile);
};


/*
 * font description
 */
class ENGINE_API CFontData : public CSerial {
// implementation
public:
  PIX   fd_pixCharSpacing, fd_pixLineSpacing;  // intra character and intra line spacing
  PIX   fd_pixCharWidth,   fd_pixCharHeight;   // maximum character width and height
  PIX   fd_pixShadowSpacing;
  ULONG fd_ulFlags;
  BOOL  fd_bSmallCaps;
  FLOAT fd_fSmallCapsStretch;

  CTFileName fd_fnTexture;
  class CFontCharData fd_fcdFontCharData[256];
  class CTextureData *fd_ptdTextureData;

// interface
public:
  CFontData();
  ~CFontData();
  inline PIX  GetWidth(void)         const { return fd_pixCharWidth;   };
  inline PIX  GetHeight(void)        const { return fd_pixCharHeight;  };
  inline PIX  GetCharWidth(char chr) const {
    if( IsFixedWidth()){
      return fd_pixCharWidth;
    } else {
      return fd_fcdFontCharData[UBYTE(chr)].fcd_pixEnd-fd_fcdFontCharData[UBYTE(chr)].fcd_pixStart;
    }
  };
  inline PIX  GetCharSpacing(void)   const { return fd_pixCharSpacing; };
  inline PIX  GetLineSpacing(void)   const { return fd_pixLineSpacing; };
  inline BOOL IsFixedWidth(void)     const { return fd_ulFlags & FNF_FIXED; };
  inline void SetCharSpacing( PIX pixSpacing) { fd_pixCharSpacing = pixSpacing; };
  inline void SetLineSpacing( PIX pixSpacing) { fd_pixLineSpacing = pixSpacing; };
  inline void SetFixedWidth(void)    { fd_ulFlags |=  FNF_FIXED; };
  inline void SetVariableWidth(void) { fd_ulFlags &= ~FNF_FIXED; };
  inline void SetSpaceWidth( FLOAT fWidthRatio) { // relative to char cell width (1/2 is default)
              fd_fcdFontCharData[' '].fcd_pixEnd = (PIX)(fd_pixCharWidth*fWidthRatio); }
  inline BOOL IsCharDefined(char chr) const { 
    return 
      fd_fcdFontCharData[UBYTE(chr)].fcd_pixXOffset!=fd_fcdFontCharData[31].fcd_pixXOffset ||
      fd_fcdFontCharData[UBYTE(chr)].fcd_pixYOffset!=fd_fcdFontCharData[31].fcd_pixYOffset;
  };

  void Read_t(  CTStream *inFile); // throw char *
  void Write_t( CTStream *outFile); // throw char *
  void Make_t( const CTFileName &fnTexture, PIX pixCharWidth, PIX pixCharHeight,
               CTFileName &fnOrderFile, BOOL bUseAlpha);
  void Clear();
};


#endif  /* include-once check. */

