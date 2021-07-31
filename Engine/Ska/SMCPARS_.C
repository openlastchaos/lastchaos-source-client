
/*  A Bison parser, made from smcpars.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	c_float	258
#define	c_int	259
#define	c_string	260
#define	c_modelinstance	261
#define	k_PARENTBONE	262
#define	k_SE_SMC	263
#define	k_SE_END	264
#define	k_NAME	265
#define	k_TFNM	266
#define	k_MESH	267
#define	k_SKELETON	268
#define	k_ANIMSET	269
#define	K_ANIMATION	270
#define	k_TEXTURES	271
#define	k_OFFSET	272
#define	k_COLISION	273
#define	k_ALLFRAMESBBOX	274
#define	k_ANIMSPEED	275
#define	k_COLOR	276
#define	k_ANIMEFFECT	277

#line 1 "smcpars.y"

#include <Engine/StdH.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include "ParsingSmbs.h"

extern BOOL _bRememberSourceFN;
BOOL bOffsetAllreadySet = FALSE;
#line 14 "smcpars.y"

#define YYERROR_VERBOSE 1                  
// if error occurs in parsing
void yyerror(char *str)
{
  // just report the string
  ThrowF_t("File '%s'\n %s (line %d)",SMCGetBufferName(), str, SMCGetBufferLineNumber());
};                 

#line 26 "smcpars.y"
typedef union {
  int i;
  float f;
  const char *str;
  CModelInstance *pmi;
  float f6[6];
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		129
#define	YYFLAG		-32768
#define	YYNTBASE	27

#define YYTRANSLATE(x) ((unsigned)(x) <= 277 ? yytranslate[x] : 58)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    26,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    23,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    24,     2,    25,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,    10,    11,    14,    16,    18,    20,    22,    24,
    26,    28,    30,    32,    33,    35,    40,    41,    43,    47,
    52,    53,    55,    57,    60,    76,    77,    79,    93,    94,
    97,   111,   112,   124,   125,   132,   137,   140,   145,   147,
   150,   154,   155,   157,   161,   162,   165,   170,   171,   174,
   179,   181,   183
};

static const short yyrhs[] = {    -1,
    41,    10,     5,    23,    28,    24,    29,    25,     0,     0,
    30,    29,     0,    45,     0,    47,     0,    48,     0,    51,
     0,    43,     0,    35,     0,    39,     0,    33,     0,    31,
     0,     0,    32,     0,    22,    11,     5,    23,     0,     0,
    34,     0,    21,     4,    23,     0,    18,    24,    36,    25,
     0,     0,    37,     0,    38,     0,    37,    38,     0,     5,
    24,    57,    26,    57,    26,    57,    26,    57,    26,    57,
    26,    57,    23,    25,     0,     0,    40,     0,    19,    57,
    26,    57,    26,    57,    26,    57,    26,    57,    26,    57,
    23,     0,     0,    42,    41,     0,    17,    57,    26,    57,
    26,    57,    26,    57,    26,    57,    26,    57,    23,     0,
     0,     7,     5,    23,    41,    10,     5,    23,    44,    24,
    29,    25,     0,     0,    12,    11,     5,    23,    46,    53,
     0,    13,    11,     5,    23,     0,    14,    50,     0,    14,
    24,    49,    25,     0,    50,     0,    49,    50,     0,    11,
     5,    23,     0,     0,    52,     0,    15,     5,    23,     0,
     0,    53,    54,     0,    16,    24,    55,    25,     0,     0,
    55,    56,     0,     5,    11,     5,    23,     0,     3,     0,
     4,     0,     4,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    72,    91,    95,    96,   100,   101,   102,   103,   104,   105,
   106,   107,   108,   112,   113,   117,   126,   127,   131,   139,
   145,   146,   150,   151,   155,   165,   166,   170,   179,   189,
   196,   215,   245,   254,   260,   263,   270,   271,   275,   276,
   280,   288,   289,   293,   305,   306,   310,   314,   315,   319,
   326,   330,   337
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","c_float",
"c_int","c_string","c_modelinstance","k_PARENTBONE","k_SE_SMC","k_SE_END","k_NAME",
"k_TFNM","k_MESH","k_SKELETON","k_ANIMSET","K_ANIMATION","k_TEXTURES","k_OFFSET",
"k_COLISION","k_ALLFRAMESBBOX","k_ANIMSPEED","k_COLOR","k_ANIMEFFECT","';'",
"'{'","'}'","','","parent_model","@1","components","component","anim_effect_opt",
"anim_effect","mdl_color_opt","mdl_color","colision_header","colision_opt","colision_array",
"colision","all_frames_bbox_opt","all_frames_bbox","offset_opt","offset","child_model",
"@2","mesh","@3","skeleton","animset_header","animset_array","animset","animation_opt",
"animation","opt_textures","textures","textures_array","texture","float_const", NULL
};
#endif

static const short yyr1[] = {     0,
    28,    27,    29,    29,    30,    30,    30,    30,    30,    30,
    30,    30,    30,    31,    31,    32,    33,    33,    34,    35,
    36,    36,    37,    37,    38,    39,    39,    40,    41,    41,
    42,    44,    43,    46,    45,    47,    48,    48,    49,    49,
    50,    51,    51,    52,    53,    53,    54,    55,    55,    56,
    57,    57,    -1
};

static const short yyr2[] = {     0,
     0,     8,     0,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     0,     1,     4,     0,     1,     3,     4,
     0,     1,     1,     2,    15,     0,     1,    13,     0,     2,
    13,     0,    11,     0,     6,     4,     2,     4,     1,     2,
     3,     0,     1,     3,     0,     2,     4,     0,     2,     4,
     1,     1,     1
};

static const short yydefact[] = {    29,
     0,     0,    29,    51,    52,     0,     0,    30,     0,     0,
     0,     1,     0,     0,     0,     3,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     3,    13,    15,
    12,    18,    10,    11,    27,     9,     5,     6,     7,     8,
    43,     0,     0,     0,     0,     0,     0,    37,     0,    21,
     0,     0,     0,     2,     4,     0,    29,     0,     0,     0,
     0,    39,    44,     0,     0,    22,    23,     0,    19,     0,
     0,     0,    34,    36,    41,    38,    40,     0,    20,    24,
     0,    16,     0,     0,    45,     0,     0,     0,     0,    35,
     0,     0,    31,    32,     0,    46,     0,     0,     0,    48,
     0,     0,     3,     0,     0,     0,     0,     0,    47,    49,
     0,     0,    33,     0,     0,     0,     0,     0,     0,    50,
     0,    28,     0,     0,     0,    25,     0,     0,     0
};

static const short yydefgoto[] = {   127,
    14,    27,    28,    29,    30,    31,    32,    33,    65,    66,
    67,    34,    35,     2,     3,    36,    99,    37,    85,    38,
    39,    61,    48,    40,    41,    90,    96,   104,   110,     6
};

static const short yypact[] = {   -10,
     2,     0,   -10,-32768,-32768,   -17,     9,-32768,     2,   -12,
     3,-32768,     2,    -7,     5,     6,     2,    27,    22,    23,
    11,    31,    13,     2,    34,    28,    15,     6,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    16,    18,    38,    39,    40,    35,-32768,    25,    44,
    24,    29,    46,-32768,-32768,     2,   -10,    30,    32,    37,
     1,-32768,-32768,    33,    36,    44,-32768,     2,-32768,    41,
    42,    48,-32768,-32768,-32768,-32768,-32768,     2,-32768,-32768,
    45,-32768,     2,    49,-32768,    47,     2,    43,    52,    51,
     2,    53,-32768,-32768,    56,-32768,    55,     2,    59,-32768,
     2,    58,     6,    -2,    60,     2,    62,    54,-32768,-32768,
     2,    64,-32768,    57,    65,     2,    70,     2,    71,-32768,
    69,-32768,     2,    73,    63,-32768,    72,    77,-32768
};

static const short yypgoto[] = {-32768,
-32768,   -27,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    -3,-32768,-32768,    -1,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   -31,-32768,-32768,-32768,-32768,-32768,-32768,    -9
};


#define	YYLAST		114


static const short yytable[] = {    11,
    55,     8,   108,    15,     4,     5,     1,    42,     9,     7,
    12,    46,    18,    10,    51,    62,    16,    19,    20,    21,
    22,    46,   109,    23,    24,    76,    25,    26,    13,    77,
    17,    43,    44,    45,    47,    49,    50,    52,    53,    54,
    57,    56,    58,    59,    60,    46,    71,    63,    64,    68,
    70,    69,    73,    89,    74,    72,    78,    84,    81,    75,
    79,   117,    80,    82,   114,    93,    95,    83,    86,     0,
    87,   128,    91,    88,    94,   107,   129,    92,    98,   100,
   101,    97,   103,   106,     0,   111,   113,   126,   102,   116,
   118,   105,   120,   122,   123,   125,   112,     0,     0,     0,
     0,   115,     0,     0,     0,     0,   119,     0,   121,     0,
     0,     0,     0,   124
};

static const short yycheck[] = {     9,
    28,     3,     5,    13,     3,     4,    17,    17,    26,    10,
    23,    11,     7,     5,    24,    47,    24,    12,    13,    14,
    15,    11,    25,    18,    19,    25,    21,    22,    26,    61,
    26,     5,    11,    11,    24,     5,    24,     4,    11,    25,
    23,    26,     5,     5,     5,    11,    56,    23,     5,    26,
     5,    23,    23,     5,    23,    57,    24,    10,    68,    23,
    25,     5,    66,    23,    11,    23,    16,    26,    78,    -1,
    26,     0,    26,    83,    23,   103,     0,    87,    26,    24,
    26,    91,    24,    26,    -1,    26,    25,    25,    98,    26,
    26,   101,    23,    23,    26,    23,   106,    -1,    -1,    -1,
    -1,   111,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
    -1,    -1,    -1,   123
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok 	(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY 	-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR 	goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()	(!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }      \
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar; 		/*  the lookahead symbol		*/
YYSTYPE yylval; 		/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc; 		/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far	*/
#endif	/* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.	*/
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.	*/
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.	*/

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return 	*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in	yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.	*/
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.	*/
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.	*/
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 73 "smcpars.y"
{
  if(_yy_mi==0) {
    yyerror("_yy_mi = NULL");
  }
  // create new model instance
  // _yy_mi = CreateModelInstance($3);
  _yy_mi->SetName(yyvsp[-1].str);
  // set its offset
  _yy_mi->SetOffset(yyvsp[-3].f6);
  // mark offset as read
  bOffsetAllreadySet = FALSE;
  // check if flag to remember source file name is set
  if(_bRememberSourceFN)
  {
    // remember source file name
    _yy_mi->mi_fnSourceFile = CTString(SMCGetBufferName());
  }
;
    break;}
case 16:
#line 118 "smcpars.y"
{
  if(CTString(yyvsp[-1].str)!="") {
	_yy_mi->ReadAnimEffectFile((CTString)yyvsp[-1].str);
  }
;
    break;}
case 19:
#line 132 "smcpars.y"
{
  COLOR c = yyvsp[-1].i;
  // _yy_mi->SetModelColor($2);
;
    break;}
case 20:
#line 140 "smcpars.y"
{

;
    break;}
case 25:
#line 156 "smcpars.y"
{
  FLOAT3D vMin = FLOAT3D(yyvsp[-12].f, yyvsp[-10].f, yyvsp[-8].f);
  FLOAT3D vMax = FLOAT3D(yyvsp[-6].f,yyvsp[-4].f,yyvsp[-2].f);
  // add new colision box to current model instance
  _yy_mi->AddColisionBox(yyvsp[-14].str,vMin,vMax);
;
    break;}
case 28:
#line 171 "smcpars.y"
{
  // add new colision box to current model instance
  _yy_mi->mi_cbAllFramesBBox.SetMin(FLOAT3D(yyvsp[-11].f, yyvsp[-9].f, yyvsp[-7].f));
  _yy_mi->mi_cbAllFramesBBox.SetMax(FLOAT3D(yyvsp[-5].f,yyvsp[-3].f,yyvsp[-1].f));
;
    break;}
case 29:
#line 180 "smcpars.y"
{
  // set offset with default offset values
  yyval.f6[0] = 0;
  yyval.f6[1] = 0;
  yyval.f6[2] = 0;
  yyval.f6[3] = 0;
  yyval.f6[4] = 0;
  yyval.f6[5] = 0;
;
    break;}
case 30:
#line 190 "smcpars.y"
{
  // return new offset
  memcpy(yyval.f6,yyvsp[-1].f6,sizeof(float)*6);
;
    break;}
case 31:
#line 197 "smcpars.y"
{
  // if offset is not set
  if(!bOffsetAllreadySet)
  {
    // set offset
    yyval.f6[0] = yyvsp[-11].f;
    yyval.f6[1] = yyvsp[-9].f;
    yyval.f6[2] = yyvsp[-7].f;
    yyval.f6[3] = yyvsp[-5].f;
    yyval.f6[4] = yyvsp[-3].f;
    yyval.f6[5] = yyvsp[-1].f;
    // mark it as set now
    bOffsetAllreadySet = TRUE;
  }
;
    break;}
case 32:
#line 216 "smcpars.y"
{
  // get parent ID
  int iParentBoneID;
  if(CTString(yyvsp[-5].str)!="") {
    iParentBoneID = ska_FindID(yyvsp[-5].str);
  } else {
    iParentBoneID = -1;
  }
  // if(iParentBoneID<0) iParentBoneID=0;
  // remember current model instance in parent bone token
  yyvsp[-6].pmi = _yy_mi;
  // set _yy_mi as new child
  _yy_mi = CreateModelInstance(yyvsp[-1].str);
  // add child to parent model instance 
  yyvsp[-6].pmi->AddChild(_yy_mi);
  // add offset
  _yy_mi->SetOffset(yyvsp[-3].f6);
  // set its parent bone
  _yy_mi->SetParentBone(iParentBoneID);
  // 
  bOffsetAllreadySet = FALSE;
  // if flag to remember source file is set
  if(_bRememberSourceFN)
  {
    // remember source name
    _yy_mi->mi_fnSourceFile = CTString(SMCGetBufferName());
  }
;
    break;}
case 33:
#line 246 "smcpars.y"
{
   // set parent model instance to _yy_mi again
  _yy_mi = yyvsp[-10].pmi;
;
    break;}
case 34:
#line 255 "smcpars.y"
{
  // add mesh to current model instance
  _yy_mi->AddMesh_t((CTString)yyvsp[-1].str);
;
    break;}
case 36:
#line 264 "smcpars.y"
{
  // add skeleton to current model instance
  _yy_mi->AddSkeleton_t((CTString)yyvsp[-1].str);
;
    break;}
case 41:
#line 281 "smcpars.y"
{
  // add animset to curent model instnce 
  _yy_mi->AddAnimSet_t((CTString)yyvsp[-1].str);
;
    break;}
case 44:
#line 294 "smcpars.y"
{
  // set new clear state in model instance
  _yy_mi->NewClearState(1);
  // get anim ID
  INDEX iAnimID = ska_GetIDFromStringTable(yyvsp[-1].str);
  // add animation to curent model instance
  _yy_mi->AddAnimation(iAnimID,AN_LOOPING,1,0);
;
    break;}
case 50:
#line 320 "smcpars.y"
{
  // add texture to current model instance
  _yy_mi->AddTexture_t((CTString)yyvsp[-1].str,yyvsp[-3].str,NULL);
;
    break;}
case 51:
#line 327 "smcpars.y"
{
  yyval.f = yyvsp[0].f;
;
    break;}
case 52:
#line 331 "smcpars.y"
{
  yyval.f = (float)yyvsp[0].i;
;
    break;}
case 53:
#line 338 "smcpars.y"
{
  yyval.i = yyvsp[0].i;
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.	*/

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 343 "smcpars.y"

