/*
 *  Serious Sam
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */
//extern BOOL _bWindowChanging;    // ignores window messages while this is set		// yjpark
//extern HWND _hwndMain;
extern BOOL _bIMEProc; // 이기환 수정 11.12

// init/end main window management
void MainWindow_Init(void);
void MainWindow_End(void);
// close the main application window
void CloseMainWindow(void);
// open the main application window for windowed mode
void OpenMainWindowNormal(PIX pixSizeI, PIX pixSizeJ);
void ResetMainWindowNormal(void);
// open the main application window for fullscreen mode
void OpenMainWindowFullScreen(PIX pixSizeI, PIX pixSizeJ);
// open the main application window invisible
void OpenMainWindowInvisible(void);
