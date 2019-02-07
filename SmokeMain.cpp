

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <stdio.h>
#include <GL/glut.h>




#include"../Phusissdk26/phusis.h"
#define SgridM 32
#define SgridN 32
#define SgridO 32

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
HPALETTE hPalette = 0;

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool isRbuttonDown=FALSE;	
bool isMouseMove=FALSE;


GLfloat trans[3];			/* current translation */
GLfloat rots[2];


int dvel;
float alpha[8];
	
T_World *pWorld = NULL;
T_Space *pSpace = NULL;
T_Object *ObjectList[10];


enum { 
	PAN = 1,				/* pan state bit */
	ROTATE,				/* rotate state bits */
	ZOOM				/* zoom state bit */
};
static void update(int state, int ox, int nx, int oy, int ny)
{
	int dx = ox - nx;
	int dy = ny - oy;

	switch(state) {
	case PAN:
		trans[0] -= dx / 100.0f;
		trans[1] -= dy / 100.0f;
		break;
	case ROTATE:
		rots[0] += (dy * 180.0f) / 500.0f;
		rots[1] -= (dx * 180.0f) / 500.0f;
#define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
		clamp(rots[0]);
		clamp(rots[1]);
		break;
	case ZOOM:
		trans[2] -= (dx+dy) / 100.0f;
		break;
	}
}
void drawSmokeSDen(T_Smoke *pSmoke)
{
	int i, j, k;
	float x, y,z, h, d000, d010, d100, d110,d001, d011, d101, d111;

	h = 1.0f/MAX(MAX(SgridM, SgridN), MAX(SgridN, SgridO));
	alpha[0] =  0.07f;
	alpha[1] =  0.07f;
	alpha[2] =  0.07f;
	alpha[3] =  0.07f;
	alpha[4] =  0.07f;
	alpha[5] =  0.07f;
	alpha[6] =  0.07f;
	alpha[7] =  0.07f;	
	//glColor3f(1.0f,1.0f,1.0f);
glColor3f(1.0f,0.0f,0.0f);
	glBegin ( GL_QUADS );

	for ( i=10; i<=28; i++ ) {
		x = (i-0.5f)*h;
		for ( j=0; j<= SgridN; j++ ) {
			y = (j-0.5f)*h;
			for ( k=10; k<= 28; k++ ) {
				z = (k-0.5f)*h;

				d000 = pSmoke->dens[SMOKEIX(i,j,k)];
				d010 = pSmoke->dens[SMOKEIX(i,j+1,k)];
				d100 = pSmoke->dens[SMOKEIX(i+1,j,k)];
				d110 = pSmoke->dens[SMOKEIX(i+1,j+1,k)];

				d001 = pSmoke->dens[SMOKEIX(i,j,k+1)];
				d011 = pSmoke->dens[SMOKEIX(i,j+1,k+1)];
				d101 = pSmoke->dens[SMOKEIX(i+1,j,k+1)];
				d111 = pSmoke->dens[SMOKEIX(i+1,j+1,k+1)];				



				

				glColor4f ( d111, d111, d111, alpha[7] ); glVertex3f ( x+h,y+h,z+h );
				glColor4f ( d011, d011, d011, alpha[3] ); glVertex3f ( x, y+h, z+h);
				glColor4f ( d001, d001, d001, alpha[1] ); glVertex3f ( x, y, z+h );
				glColor4f ( d101, d101, d101, alpha[5] ); glVertex3f ( x+h, y, z+h );

				/* face 2*/
				glColor4f ( d110, d110, d110, alpha[6] ); glVertex3f ( x+h, y+h, z );
				glColor4f ( d111, d111, d111, alpha[7] ); glVertex3f ( x+h,y+h,z+h );
				glColor4f ( d101, d101, d101, alpha[5] ); glVertex3f ( x+h, y, z+h );
				glColor4f ( d100, d100, d100, alpha[4] ); glVertex3f ( x+h, y, z );

				/*face 3*/
				glColor4f ( d010, d010, d010, alpha[2] ); glVertex3f ( x, y+h, z );
				glColor4f ( d110, d110, d110, alpha[6] ); glVertex3f ( x+h, y+h, z );
				glColor4f ( d100, d100, d100, alpha[4] ); glVertex3f ( x+h, y, z );
				glColor4f ( d000, d000, d000, alpha[0]); glVertex3f ( x, y, z );

				/* face 4*/
				glColor4f ( d011, d011, d011, alpha[3] ); glVertex3f ( x, y+h, z+h);
				glColor4f ( d010, d010, d010, alpha[2]); glVertex3f ( x, y+h, z );
				glColor4f ( d000, d000, d000, alpha[0] ); glVertex3f ( x, y, z );
				glColor4f ( d001, d001, d001, alpha[1] ); glVertex3f ( x, y, z+h );

				/* face 5*/
				glColor4f ( d100, d100, d100, alpha[4] ); glVertex3f ( x+h, y, z );
				glColor4f ( d000, d000, d000, alpha[0] ); glVertex3f ( x, y, z );
				glColor4f ( d001, d001, d001, alpha[1] ); glVertex3f ( x, y, z+h );
				glColor4f ( d101, d101, d101, alpha[5] ); glVertex3f ( x+h, y, z+h );

				/*face 6*/
				glColor4f ( d110, d110, d110, alpha[6] ); glVertex3f ( x+h, y+h, z );
				glColor4f ( d010, d010, d010, alpha[2] ); glVertex3f ( x, y+h, z );
				glColor4f ( d011, d011, d011, alpha[3] ); glVertex3f ( x, y+h, z+h);
				glColor4f ( d111, d111, d111, alpha[7] ); glVertex3f ( x+h, y+h, z+h );	
			}
		}
	}

	glEnd ();

	
}
void draw_axis()
{
	/* draw axis x,y,z	*/
	int i;

	glLineWidth ( 1.0f );
	glBegin (GL_LINES);	

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f (0.5f, 0.f, 0.5f);
	glVertex3f (2.0f, 0.f, 0.5f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f (0.5f, 0.f, 0.5f);
	glVertex3f (0.5f, 2.f, 0.5f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f (0.5f, 0.f, 0.5f);
	glVertex3f (0.5f, 0.f, 2.f);

	glEnd();
}
void drawSmoke(T_Smoke *pSmoke)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0.0f,0.0f,-6.0f);		
	
	glRotatef(rots[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rots[1], 0.0f, 1.0f, 0.0f);
   glScalef(1.5,1.5,1.5);
  
    drawSmokeSDen(pSmoke);

	
	glColor3f(1.0f,0.5f,1.0f);
	/*glBegin(GL_QUADS);
	glVertex3f(-1.0f,1.0f,0.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glEnd();*/

	draw_axis();
	glPopMatrix();
	glFlush();
	SwapBuffers(hDC);
}

void init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	rots[0] = 30;
	rots[1] = -45;
	pSpace = createSimpleSpace();
	pWorld = createWorld(pSpace);
	ObjectList[0] = createSmoke(pWorld);

	setSmokePrevSource((T_Smoke*)ObjectList[0],0,16,8,16);
	setSmokePrevSource((T_Smoke*)ObjectList[0],80,16,5,16);
	setSmokePrevVForce((T_Smoke*)ObjectList[0],0,16,6,16);
	setSmokePrevVForce((T_Smoke*)ObjectList[0],7,16,2,16);
	setSmokeBuoDirect((T_Smoke*)ObjectList[0],0,0.02,0);
	setSmokeBuoyancy((T_Smoke*)ObjectList[0],0.00);
	setSmokeDifussion((T_Smoke*)ObjectList[0],0.00000002);
	setSmokeAddSourceFlag((T_Smoke*)ObjectList[0],true);
	setSmokeAddVorticyEffect((T_Smoke*)ObjectList[0],false);
}
void update()
{
	simulate(pWorld,0.4);
	
	//drawSmoke((T_Smoke*)ObjectList[0]);
	
}
void freeData()
{
	T_Smoke * pSmoke = (T_Smoke*)ObjectList[0];
	freeSmokeData(pSmoke);

}

int DrawGLScene(GLvoid)	;
//
//static void update(int state, int ox, int nx, int oy, int ny)
//{
//	int dx = ox - nx;
//	int dy = ny - oy;
//
//	switch(state) {
//	case PAN:
//		transs[0] -= dx / 100.0f;
//		transs[1] -= dy / 100.0f;
//		break;
//	case ROTATE:
//		rots[0] += (dy * 180.0f) / 500.0f;
//		rots[1] -= (dx * 180.0f) / 500.0f;
//#define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
//		clamp(rots[0]);
//		clamp(rots[1]);
//		break;
//	case ZOOM:
//		transs[2] -= (dx+dy) / 100.0f;
//		break;
//	}
//}


LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return TRUE;										// Initialization Went OK
}



GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	static GLuint    state   = 0;	/* mouse state flag */
	static int omx, omy, mx, my;
  

	switch (uMsg)									// Check For Windows Messages
	{
        
	case WM_CHAR:
		switch (wParam) {
			case 27:			/* ESC key */
				PostQuitMessage(0);
				break;
		
			case 32:       /* 'Space' key */
				setSmokeAddSourceFlag((T_Smoke*)ObjectList[0],true);
				break;
			
			
		}
	
	
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		/* if we don't set the capture we won't get mouse move
		messages when the mouse moves outside the window. */
		SetCapture(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (uMsg == WM_LBUTTONDOWN)
			state |= PAN;
		if (uMsg == WM_RBUTTONDOWN)
			state |= ROTATE;
		return 0;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		/* remember to release the capture when we are finished. */
		ReleaseCapture();
		state = 0;
		return 0;

	case WM_MOUSEMOVE:
		if (state) {
			omx = mx;
			omy = my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			/* Win32 is pretty braindead about the x, y position that
			it returns when the mouse is off the left or top edge
			of the window (due to them being unsigned). therefore,
			roll the Win32's 0..2^16 pointer co-ord range to the
			more amenable (and useful) 0..+/-2^15. */
			if(mx & 1 << 15) mx -= (1 << 16);
			if(my & 1 << 15) my -= (1 << 16);
			update(state, omx, mx, omy, my);
			PostMessage(hWnd, WM_PAINT, 0, 0);
		}
		return 0;

	case WM_PALETTECHANGED:
		if (hWnd == (HWND)wParam)
			break;
		/* fall through to WM_QUERYNEWPALETTE */

	case WM_QUERYNEWPALETTE:
		if (hPalette) {
			UnrealizeObject(hPalette);
			SelectPalette(hDC, hPalette, FALSE);
			RealizePalette(hDC);
			return TRUE;
		}
		return FALSE;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
    
	

	// Create Our OpenGL Window
	if (!CreateGLWindow("SmokeSimulation",1024,768,32,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	init();
	
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					drawSmoke((T_Smoke*)ObjectList[0]);
					//DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("SmokeSimulation",512,512,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	   //烟雾模拟
	update();
	}//与while配对

	//释放内存
	freeData();
	
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}