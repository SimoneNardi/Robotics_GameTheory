// corridor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Resource.h"
#include "BaseGeometry/MinTrackManager.h"
#include "BaseGeometry/PointEuclidean2D.h"
#include "MinTrackTest.h"
#include "CoverageTest.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <map>

using namespace std;

double const PI = 3.1415926536;
int const kk = 10000;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Curve(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	SetNumberOfGuards(HWND, UINT, WPARAM, LPARAM);


//////////////////////////////////////////////////////////////////////////
void MarkPoint(LONG x, LONG y, HDC hdc, int R, int G, int B, int thickness)
{
	HPEN markerPen=CreatePen(PS_SOLID, thickness, RGB(R,G,B));
	HBRUSH myBrush=CreateSolidBrush(RGB(R,G,B));
	HGDIOBJ oldPen;
	HGDIOBJ oldBrush=SelectObject(hdc,myBrush);

	oldPen=SelectObject(hdc,markerPen);

	for( int i = -thickness; i!= thickness; i++ )
	{
		MoveToEx(hdc, (int) x - thickness, (int) y - i, (LPPOINT) NULL); 
		LineTo(hdc, (int) x + thickness, (int) y - i); 
		MoveToEx(hdc, (int) x - i, (int) y - thickness, (LPPOINT) NULL); 
		LineTo(hdc, (int) x - i, (int) y + thickness);
	}
	SelectObject(hdc,oldPen);
	SelectObject(hdc,oldBrush);
	DeleteObject(markerPen);
	DeleteObject(myBrush);
}

//////////////////////////////////////////////////////////////////////////
void DrawPath(LineString *path, HDC hdc, int width, int height,
										 int R, int G, int B, int thickness)
{
	HPEN myPen=CreatePen(PS_SOLID, thickness, RGB(R,G,B));
	HGDIOBJ oldPen=SelectObject(hdc,myPen);

	if(path->size()>0){
		LineStringIterator vert;
		vert=path->begin();			
		MoveToEx(hdc, (int) ((*vert).v[0]/kk*(double)width), (int) ((*vert).v[1]/kk*(double)height), (LPPOINT) NULL); 
				
		for(vert=path->begin();vert!=path->end();){
			vert++;
			if(vert==path->end())
				break;
			LineTo(hdc, (int) ((*vert).v[0]/kk*(double)width), (int) ((*vert).v[1]/kk*(double)height)); 
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void DrawTriangle(Triangolo *tri, HDC hdc, int width, int height,
										 int R, int G, int B, int thickness)
{
	HPEN myPen=CreatePen(PS_SOLID, thickness, RGB(0,0,0));
	HBRUSH myBrush=CreateSolidBrush(RGB(R,G,B));
	HGDIOBJ oldPen=SelectObject(hdc,myPen);
	HGDIOBJ oldBrush=SelectObject(hdc,myBrush);

	POINT points[3];
	points[0].x=tri->principal->v[0]/kk*(double)width;
	points[0].y=tri->principal->v[1]/kk*(double)height;
	points[1].x=tri->v1.v[0]/kk*(double)width;
	points[1].y=tri->v1.v[1]/kk*(double)height;
	points[2].x=tri->v2.v[0]/kk*(double)width;
	points[2].y=tri->v2.v[1]/kk*(double)height;

	Polygon(hdc,points,3);

	SelectObject(hdc,oldPen);
	SelectObject(hdc,oldBrush);
	DeleteObject(myPen);
	DeleteObject(myBrush);
}

//////////////////////////////////////////////////////////////////////////
void DrawRectangle(Rettangolo *rect, HDC hdc, int width, int height,
										 int R, int G, int B, int thickness)
{
	HPEN myPen=CreatePen(PS_SOLID, thickness, RGB(0,0,0));
	HBRUSH myBrush=CreateSolidBrush(RGB(R,G,B));
	HGDIOBJ oldPen=SelectObject(hdc,myPen);
	HGDIOBJ oldBrush=SelectObject(hdc,myBrush);

	POINT points[4];
	for(int i=0; i!=4; i++)
	{
		points[i].x=rect->v[i].v[0]/kk*(double)width;
		points[i].y=rect->v[i].v[1]/kk*(double)height;
	}

	Polygon(hdc,points,4);

	SelectObject(hdc,oldPen);
	SelectObject(hdc,oldBrush);
	DeleteObject(myPen);
	DeleteObject(myBrush);
}

class ColorMap;

/////////////////////////////////////////////////////////////////////////
class MyColour
{
public:
	int R;
	int G;
	int B;

	void setRandom()
	{
		R = rand()%255;
		G = rand()%255;
		B = rand()%255;
	}

	void setMonitor()
	{
		R = rand()%255;
		G = rand()%255;
		B = rand()%255;
	}
};

//////////////////////////////////////////////////////////////////////////
class ColorMap
{
public:
	std::map<double, MyColour> m_legend;

	inline ColorMap() : m_legend() 
	{
		MyColour local;
		local.R = 0;
		local.G = 0;
		local.B = 255;
		m_legend.insert(std::make_pair(0., local));

		local.R = 40;
		local.G = 0;
		local.B = 255;
		m_legend.insert(std::make_pair(10., local));

		local.R = 80;
		local.G = 0;
		local.B = 255;
		m_legend.insert(std::make_pair(20., local));

		local.R = 120;
		local.G = 0;
		local.B = 255;
		m_legend.insert(std::make_pair(30., local));

		local.R = 160;
		local.G = 0;
		local.B = 255;
		m_legend.insert(std::make_pair(40., local));

		local.R = 200;
		local.G = 0;
		local.B = 255;
		m_legend.insert(std::make_pair(50., local));

		local.R = 0;
		local.G = 20;
		local.B = 255;
		m_legend.insert(std::make_pair(60., local));

		local.R = 0;
		local.G = 60;
		local.B = 255;
		m_legend.insert(std::make_pair(70., local));

		local.R = 0;
		local.G = 100;
		local.B = 255;
		m_legend.insert(std::make_pair(80., local));

		local.R = 0;
		local.G = 140;
		local.B = 150;
		m_legend.insert(std::make_pair(90., local));

		local.R = 0;
		local.G = 180;
		local.B = 200;
		m_legend.insert(std::make_pair(100., local));
	}

	inline MyColour getColor(double _val) 
	{
		int l_val = _val / 10.;
		return m_legend[l_val*10];
	}
};

//////////////////////////////////////////////////////////////////////////
void DrawAllPolygons(HDC hdc, int width, int height, int thickness)
{
	if( minTrackForTest == 0 )
		return;

	srand ( time(NULL) );
	if( drawone )
	{
		MyColour Col;
		Col.setRandom();
		if( TTRR == 1 )
			DrawTriangle( &TT, hdc, width, height, Col.R, Col.G, Col.B, thickness);
		else if( TTRR == -1 )
			DrawRectangle(&RR, hdc, width, height, Col.R, Col.G, Col.B, thickness);
		return;
	}
	Triangolo *tri;
	Rettangolo *rett;
	int nTri,nRett;
	if(minTrackForTest == 0)
		return;
	minTrackForTest->getTriAndRett(nTri,nRett);
	tri = new Triangolo[nTri];
	rett = new Rettangolo[nRett];
	minTrackForTest->copyTriAndRett(nTri,nRett,tri,rett);
	MyColour Col;
	map<Vertex*,MyColour> mm;

	for( int i = nTri-1; i != -1; i-- )
	{
		if( mm.find(tri[i].principal) == mm.end() )
		{
			Col.setRandom();
			mm[tri[i].principal] = Col;
		}
		DrawTriangle( &tri[i], hdc, width, height,
					  mm[tri[i].principal].R, mm[tri[i].principal].G, mm[tri[i].principal].B,
					  thickness);
	}

	map<int,MyColour> mm2;

	for( int i = 0; i != nRett; i++ )
	{
		if( mm2.find(rett[i].DerNum) == mm2.end() )
		{
			Col.setRandom();
			mm2[rett[i].DerNum] = Col;
		}
		DrawRectangle(&rett[i], hdc, width, height,
					  mm2[rett[i].DerNum].R, mm2[rett[i].DerNum].G, mm2[rett[i].DerNum].B,
					  thickness);
	}
	delete[] tri;
	delete[] rett;
}

//////////////////////////////////////////////////////////////////////////
void DrawZones(HDC hdc, int width, int height, int thickness)
{
	if( minTrackForTest == 0 )
		return;

	srand ( time(NULL) );

	map<Vertex*,MyColour> mm;
	map<int,MyColour> mm2;
	MyColour Col;

	int camp;
	if( thickness == 3 )
		camp = 80;
	if( thickness == 2 )
		camp = 50;
	if( thickness == 1 )
		camp = 25;

	int puntiInterni = 0;
	int puntiEsterni = 0;

	for( int i = 0; i < kk; i+= camp )
	{
		for( int j = 0; j < kk; j+= camp )
		{
			if(	!minTrackForTest->FindTri(i,-j,TT,RR,TTRR) )
			{
				puntiEsterni++;
				continue;
			}

			puntiInterni++;
			Col.setRandom();
			if( TTRR == 1 )
			{
				if( mm.find(TT.principal) == mm.end() )
				{
					Col.setRandom();
					mm[TT.principal] = Col;
				}
				double I = i*width/kk;
				int ii = floor(I);
				double J = -j*height/kk;
				int jj = floor(J);
				MarkPoint(ii, jj, hdc, mm[TT.principal].R, mm[TT.principal].G, mm[TT.principal].B, thickness);
			}
			else if( TTRR == -1 )
			{
				if( mm2.find(RR.DerNum) == mm2.end() )
				{
					Col.setRandom();
					mm2[RR.DerNum] = Col;
				}
				double I = i*width/kk;
				int ii = floor(I);
				double J = -j*height/kk;
				int jj = floor(J);
				MarkPoint(ii, jj, hdc, mm2[RR.DerNum].R, mm2[RR.DerNum].G, mm2[RR.DerNum].B, thickness);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void ReverseBoundary()
{
	int n;
	if( dim % 2 == 0 )
		n = dim/2;
	else
		n = (dim+1)/2;
	for ( int count = 0; count != n; count++ )
	{
		double x = Boundaryy[count].coord()[0];
		double y = Boundaryy[count].coord()[1];
		Boundaryy[count]=PointEuclidean2D(Boundaryy[dim-count-1].coord()[0],Boundaryy[dim-count-1].coord()[1]);
		Boundaryy[dim-count-1]=PointEuclidean2D(x,y);
	}
}

//////////////////////////////////////////////////////////////////////////
void MakeCurve()
{
	dim = 2*camp+2;
	Boundaryy = new Point2D[dim];

	double ang;
	ang = ang2 - ang1;
	ang *= 2*PI/(360*camp);
	double angg1 = ang1;
	angg1 *= 2*PI/(360);
	double angg2 = ang2;
	angg2 *= 2*PI/(360);
	for( int i = 0; i != camp+1; i++ )
	{
		Boundaryy[i]=PointEuclidean2D(Ox + rad1*cos((double)(angg1+i*ang)),Oy + rad1*sin((double)(angg1+i*ang)));
		Boundaryy[camp+1+i]=PointEuclidean2D(Ox + rad2*cos((double)(angg2-i*ang)), Oy + rad2*sin((double)(angg2-i*ang)));

	}
	if( ang > 0 )
		ReverseBoundary();
}

//////////////////////////////////////////////////////////////////////////
void SaveBoundary()
{
	int count;
	FILE*f = fopen("..\\Boundary.txt","w");
	fprintf(f,"%d\n\n",dim);
	for ( count = 0; count != dim; count++ )
		fprintf(f,"%d %lf %lf\n",count,Boundaryy[count].coord()[0],Boundaryy[count].coord()[1]);
	fclose(f);
}

//////////////////////////////////////////////////////////////////////////
bool LoadBoundary(bool inverti = false, int index = -1)
{
	int count = 0;
	FILE*f;
	if( index < -1 || index > 99 )
		return false;
	if( index == -1 )
	{
		f = fopen("..\\Boundary.txt","r");
		if (!f)
			return false;
	}
	else
	{
		if( mode == -3 )
		{
			const int nn = 25;
			char name[nn] = "..\\TestM\\Boundary   .txt";
			int t = index % 10;
			name[nn-6] = t+'0';
			t = (index-t)/10;
			name[nn-7] = t+'0';
			f = fopen(name,"r");
		}
		else
		{
			const int nn = 24;
			char name[nn] = "..\\Test\\Boundary   .txt";
			int t = index % 10;
			name[nn-6] = t+'0';
			t = (index-t)/10;
			name[nn-7] = t+'0';
			f = fopen(name,"r");
		}
		if (!f)
			return false;
	}
	fscanf(f,"%d",&dim);
	Boundaryy = new Point2D[dim];

	double minX = IDSMath::LargeReal;
	double maxX = -IDSMath::LargeReal;
	double minY = IDSMath::LargeReal;
	double maxY = -IDSMath::LargeReal;
	for ( count = 0; count != dim; count++ )
	{
		int qwe;
		fscanf(f,"%d",&qwe);
		if( count == 0 && qwe != 0 )
		{
			numOfGuards = qwe;
			fscanf(f,"%d",&qwe);
		}
		double x,y;
		fscanf(f,"%lf",&x);
		fscanf(f,"%lf",&y);
		Boundaryy[count]=PointEuclidean2D(x,y);

		if( x < minX )
			minX = x;
		if( x > maxX )
			maxX = x;
		if( y < minY )
			minY = y;
		if( y > maxY )
			maxY = y;
	}

	if( minX < 25 || minX > 9000 || maxX < 1000 || maxX > 9850 ||
		minY < -9185 || minY > -1000 || maxY < -8500 || maxY > -15 )
	{
		double scaleX = 9825/(maxX-minX);
		double scaleY = 9160/(maxY-minY);

		for ( count = 0; count != dim; count++ )
			Boundaryy[count]=PointEuclidean2D((Boundaryy[count].coord()[0]-minX)*scaleX,
											 -9185 + (Boundaryy[count].coord()[1]-minY)*scaleY);
	}

	counterclockwise = true;
	if( inverti )
	{
		counterclockwise = false;
		for ( count = 0; count != dim; count++ )
			Boundaryy[count]=PointEuclidean2D(kk - Boundaryy[count].coord()[0], Boundaryy[count].coord()[1]);
	}
	fclose(f);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void SetStandardBoundary()
{
	int count = 0;
	dim = LeftBoundary->size();
	Boundaryy = new Point2D[dim];
	for( LineStringIterator it = LeftBoundary->begin() ; it != LeftBoundary->end() ; it++ )
	{
		Boundaryy[count]=PointEuclidean2D(it->v[0],it->v[1]);
		count++;
	}
}

//////////////////////////////////////////////////////////////////////////
// mode:
// 0  :	 standard
// 1  :	 read from file
// 2  :	 read from file and invert
// 3  :	 written here
bool SetBoundaryy(int mode = 0)
{
	delete[] Boundaryy;
	counterclockwise = true;
	if( mode == 0 )
	{
		SetStandardBoundary();
		if( dim > 1 )
			SaveBoundary();
		return true;
	}
	else if( mode == 1 )
	{
		if(!LoadBoundary())
			return false;
	}
	else if( mode == 2 )
	{
		if(!LoadBoundary(true))
			return false;
	}
	else if( mode == 3 )
	{
		MakeCurve();
		SaveBoundary();
	}

	LeftBoundary->clear();
	if(minTrackForTest)
	{
		delete minTrackForTest;
		minTrackForTest = NULL;
	}
	for( int i = 0; i != dim; i++ )
	{
		Vertex V(Boundaryy[i].coord()[0],Boundaryy[i].coord()[1]);
		LeftBoundary->push_back(V);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CORRIDOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CORRIDOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CORRIDOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CORRIDOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////////
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, width, height, j=0;
	PAINTSTRUCT ps;
	HDC hdc;
	wstringstream tmp_str;
	RECT winrect;
	Vertex V1,V2,VB;
	LineStringIterator vert,vert2;
	
	switch (message)
	{
	case WM_CREATE:

		LeftBoundary = new LineString;
		a=false;b=false;l=true;r=false;ab=false; 

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_HELP_GUIDE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_GUIDE), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_MODE_DRAWPATH:
			mode = 0;
			loadCounter = 0;
			a=false; b=false;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			l=true;
			r=false;
			ab=false;
			mybool = true;
			break;
		case ID_MODE_LOADPATH:
			mode = 1;
			loadCounter = 0;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			a = false;
			l = false;
			ab = true;
			mybool = true;
			break;
		case ID_MODE_LOADINVERTEDPATH:
			mode = 2;
			loadCounter = 0;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			a = false;
			l = false;
			ab = true;
			mybool = true;
			break;
		case ID_MODE_DRAWCURVE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CURVE), hWnd, Curve);
			loadCounter = 0;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			a = false;
			l = false;
			ab = true;
			mybool = true;
			break;
		case ID_MODE_TESTALLPAT:
			mode = -1;
			loadCounter = 0;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			a = false;
			l = false;
			ab = true;
			mybool = true;
			break;
		case ID_MODE_TESTALLPATHSINVERTED:
			mode = -2;
			loadCounter = 0;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			a = false;
			l = false;
			ab = true;
			mybool = true;
			break;
		case ID_MODE_TESTMULTISTARTPATHS:
			mode = -3;
			loadCounter = 0;
			LeftBoundary->clear();
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			a = false;
			l = false;
			ab = true;
			mybool = true;
			break;
		case ID_FILE_NUMBER_OF_GUARDS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_GUARDS), hWnd, SetNumberOfGuards);
			if(minTrackForTest)
			{
				delete minTrackForTest;
				minTrackForTest = NULL;
			}
			break;
		case ID_FILE_STARTSEGMENTINSIDE:
			derIsInside = !derIsInside;
			{HMENU hmenu = GetMenu(hWnd);
			if(derIsInside)
				CheckMenuItem(hmenu,ID_FILE_STARTSEGMENTINSIDE,MF_CHECKED);
			else
				CheckMenuItem(hmenu,ID_FILE_STARTSEGMENTINSIDE,MF_UNCHECKED);
			EndMenu();}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case ID_FILE_DRAWTRIANGLES:
			drawtriangles = !drawtriangles;
			{HMENU hmenu = GetMenu(hWnd);
			if(drawtriangles)
				CheckMenuItem(hmenu,ID_FILE_DRAWTRIANGLES,MF_CHECKED);
			else
				CheckMenuItem(hmenu,ID_FILE_DRAWTRIANGLES,MF_UNCHECKED);
			EndMenu();}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case ID_FILE_HIGHLIGHT:
			drawone = !drawone;
			{HMENU hmenu = GetMenu(hWnd);
			if(drawone)
				CheckMenuItem(hmenu,ID_FILE_HIGHLIGHT,MF_CHECKED);
			else
				CheckMenuItem(hmenu,ID_FILE_HIGHLIGHT,MF_UNCHECKED);
			EndMenu();}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case ID_FILE_PARTITIONCORRI:
			partitioncorridor = !partitioncorridor;
			{HMENU hmenu = GetMenu(hWnd);
			if(partitioncorridor)
				CheckMenuItem(hmenu,ID_FILE_PARTITIONCORRI,MF_CHECKED);
			else
				CheckMenuItem(hmenu,ID_FILE_PARTITIONCORRI,MF_UNCHECKED);
			EndMenu();}
			if( mode < 0 && mybool )
				loadCounter--;
			break;
		case ID_FILE_DRAWREALPARTITION:
			drawreal = !drawreal;
			{HMENU hmenu = GetMenu(hWnd);
			if(drawreal)
				CheckMenuItem(hmenu,ID_FILE_DRAWREALPARTITION,MF_CHECKED);
			else
				CheckMenuItem(hmenu,ID_FILE_DRAWREALPARTITION,MF_UNCHECKED);
			EndMenu();}
			break;
		case ID_FILE_UNDO:
			if( l )
			{
				LeftBoundary->pop_back();
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		GetWindowRect(hWnd,&winrect);
		width=winrect.right-winrect.left;
		height=winrect.top-winrect.bottom;
	
		if(l | r | ab)
			DrawPath(LeftBoundary,hdc,width,height,255,0,0,1);

		if(a) MarkPoint((int)(A.v[0]*width),(int)(A.v[1]*height),hdc,128,0,128,10);

		if(!mybool){
			if( drawtriangles )
			{
				if( drawreal )
					DrawZones(hdc,width,height,1);
				else
					DrawAllPolygons(hdc,width,height,1);
			}
			DrawPath(path1,hdc,width,height,0,0,0,5);
			if( path2 )
				DrawPath(path2,hdc,width,height,0,0,255,5);
		}
		

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//deallocate list
		delete LeftBoundary;
		PostQuitMessage(0);
		break;
///////User Interface
		
	case WM_LBUTTONUP:

		GetWindowRect(hWnd,&winrect);
		if(l){
			VB.v[0]=((double)LOWORD(lParam)/(winrect.right-winrect.left)*kk);
			VB.v[1]=((double)HIWORD(lParam)/(winrect.top-winrect.bottom)*kk);
			LeftBoundary->insert(LeftBoundary->end(),VB);

		}else if(ab){
		if(!a){
			A.v[0]=((double)LOWORD(lParam)/(winrect.right-winrect.left)*kk);
			A.v[1]=((double)HIWORD(lParam)/(winrect.top-winrect.bottom)*kk);
			b=true;
			if( path1 );
			delete path1;
			path1 = new LineString;

			if( mode < 0 && mybool && loadCounter != numToLoad )
			{
				FILE* f;
				if( mode == -3 )
					f = fopen("..\\TestM\\Test.txt","r");
				else
					f = fopen("..\\Test\\Test.txt","r");
				fscanf(f,"%d",&numToLoad);
				fclose(f);

				bool loaded;
				if( mode == -3 )
					loaded = LoadBoundary(false,loadCounter);
				else
					loaded = LoadBoundary(mode+1,loadCounter);

				if(!loaded)
				{
					MessageBox(hWnd,L"Error: Corridor not loaded!",L"Error",MB_OK | MB_ICONERROR);
					mode = 0;
					mybool = true;
				}
				else
				{
					LeftBoundary->clear();
					if(minTrackForTest)
					{
						delete minTrackForTest;
						minTrackForTest = NULL;
					}
					for( int i = 0; i != dim; i++ )
					{
						Vertex V(Boundaryy[i].coord()[0],Boundaryy[i].coord()[1]);
						LeftBoundary->push_back(V);
					}
					if( partitioncorridor )
					{
						try
						{
							if( numOfGuards < 0 )
							{
								vector<IDSReal2D> bb;
								vector<IDSReal2D> ss;
								for(int i = 0; i != dim + numOfGuards - 1; i++)
								{
									bb.push_back(Boundaryy[i].coord());
								}
								if( path2 )
									delete path2;
								path2 = new LineString;
								for(int i = dim + numOfGuards - 1; i != dim; i++)
								{
									ss.push_back(Boundaryy[i].coord());
									path2->push_back(Vertex(Boundaryy[i][0],Boundaryy[i][1]));
								}

								///Create Point From Vertex:
								std::vector<Point2D> bb_2D;
								for(size_t i = 0; i != bb.size(); i++)
								{
									bb_2D.push_back(PointEuclidean2D(bb[i]));
								}

								std::vector<Point2D> ss_2D;
								for(size_t i = 0; i != ss.size(); i++)
								{
									ss_2D.push_back(PointEuclidean2D(ss[i]));
								}

								minTrackForTest= (MinTrackForTest*)MinTrackManager::Init(bb_2D,ss_2D);
							}
							else if(!derIsInside)
							{
								if( path2 )
								{
									delete path2;
									path2 = 0;
								}
								minTrackForTest= new MinTrackForTest(dim,Boundaryy,numOfGuards,counterclockwise);
							}
							else
							{
								vector<Point2D> v;
								vector<Point2D> st;
								int i;
								for(i = 0; i != dim - numOfGuards - 1; i++)
								{
									v.push_back(Boundaryy[i]);
								}
								if( path2 )
									delete path2;
								path2 = new LineString;
								for(i = dim - numOfGuards - 1; i != dim; i++)
								{
									st.push_back(Boundaryy[i]);
									path2->push_back(Vertex(Boundaryy[i][0],Boundaryy[i][1]));
								}
								minTrackForTest= new MinTrackForTest(v,st,counterclockwise);
							}
						}catch(...) {MessageBox(hWnd,L"Error: bad Corridor!",L"Error",MB_OK | MB_ICONERROR);}

						/*if( __GetError() == 2 )
							MessageBox(hWnd,L"Error: bad Corridor!",L"Error",MB_OK | MB_ICONERROR);*/
						mybool = false;
					}
					loadCounter++;
					if( loadCounter == numToLoad )
					{
						loadCounter = 0;
						mode = 0;
					}
				}
			}
			else if( mybool )
			{
				// 0 if you want to set boundary with mouse
				// 1 if you want to load saved boundary
				// 2 if you want to load saved boundary and invert it
				// 3 if you want to write boundary
				if(!SetBoundaryy( mode ))
				{
					MessageBox(hWnd,L"Error: Corridor not loaded!",L"Error",MB_OK | MB_ICONERROR);
					mode = 0;
					mybool = true;
				}
				else if( partitioncorridor )
				{
					try
					{
						if( numOfGuards < 0 )
						{
							vector<IDSReal2D> bb;
							vector<IDSReal2D> ss;
							for(int i = 0; i != dim + numOfGuards - 1; i++)
							{
								bb.push_back(Boundaryy[i].coord());
							}
							if( path2 )
								delete path2;
							path2 = new LineString;
							for(int i = dim + numOfGuards - 1; i != dim; i++)
							{
								ss.push_back(Boundaryy[i].coord());
								path2->push_back(Vertex(Boundaryy[i][0],Boundaryy[i][1]));
							}

							///Create Point From Vertex:
							std::vector<Point2D> bb_2D;
							for(size_t i = 0; i != bb.size(); i++)
							{
								bb_2D.push_back(PointEuclidean2D(bb[i]));
							}

							std::vector<Point2D> ss_2D;
							for(size_t i = 0; i != ss.size(); i++)
							{
								ss_2D.push_back(PointEuclidean2D(ss[i]));
							}

							minTrackForTest= (MinTrackForTest*)MinTrackManager::Init(bb_2D,ss_2D);
						}
						else if(!derIsInside)
						{
							if( path2 )
							{
								delete path2;
								path2 = 0;
							}
							minTrackForTest= new MinTrackForTest(dim, Boundaryy, numOfGuards, counterclockwise);
						}
						else
						{
								vector<Point2D> v;
								vector<Point2D> st;
								int i;
								for(i = 0; i != dim - numOfGuards - 1; i++)
								{
									v.push_back(Boundaryy[i]);
								}
								if( path2 )
									delete path2;
								path2 = new LineString;
								for(i = dim - numOfGuards - 1; i != dim; i++)
								{
									st.push_back(Boundaryy[i]);
									path2->push_back(Vertex(Boundaryy[i][0],Boundaryy[i][1]));
								}
								minTrackForTest= new MinTrackForTest(v,st,counterclockwise);
						}
					}catch(...) {MessageBox(hWnd,L"Error: bad Corridor!",L"Error",MB_OK | MB_ICONERROR);}
					/*if( __GetError() == 2 )
						MessageBox(hWnd,L"Error: bad Corridor!",L"Error",MB_OK | MB_ICONERROR);*/
					mybool = false;
				}
			}

			if( false /*extendedObstacle*/ )
			{
				static vector<IDSReal2D> obs;
				if( obs.size() == 5 )
					obs.clear();
				obs.push_back( IDSReal2D(A.v[0],A.v[1]) );

				if(minTrackForTest)
				{
					vector<IDSReal2D> track = minTrackForTest->RetrieveExtendedObstacleTrack(obs);

					path1->clear();
					for(int i = 0; i < track.size(); i++)
					{
						Vertex temp;
						temp.v[0] = track[i][0];
						temp.v[1] = track[i][1];
						path1->insert(path1->end(), temp);
					}

					if( path2 )
						delete path2;
					path2 = new LineString;
					for(int i = 0; i < obs.size(); i++)
					{
						Vertex temp;
						temp.v[0] = obs[i][0];
						temp.v[1] = obs[i][1];
						path2->insert(path2->end(), temp);
					}
				}
			}
			else
			{
				IDSReal2D obs=IDSReal2D(A.v[0],A.v[1]);
				Point2D obs1=PointEuclidean2D(A.v[0],A.v[1]);
				int n = -1;
				if(minTrackForTest)
					n = minTrackForTest->CalcMinTrack(&obs1);

				if ( n > 0 )
				{
					/// Per fare nuova applicazione di test Automatic Mintrack Test
					FILE*f = fopen("..\\Output.txt","a");
					fprintf(f,"%lf %lf %lf\n",A.v[0],A.v[1],minTrackForTest->ComputeMinDist(obs, false));
					fclose(f);
					////////////////////////

					minTrackForTest->FindTri(A.v[0],A.v[1],TT,RR,TTRR);
					Point2D *track;
					track = new Point2D[n];

					minTrackForTest->RetrieveTrack(&obs1, track);

					path1->clear();
					for(int i = 0; i != n; i++)
					{
						Vertex temp;
						temp.v[0]=(track[i].coord()[0]);
						temp.v[1]=(track[i].coord()[1]);
						path1->insert(path1->end(),temp);
					}
				}
				else
					TTRR = 0;
			}
		}else{
			a=false; b=false;
			delete path1;
			path1 = 0;
			delete path2;
			path2 = 0;
		}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);


		break;

		case WM_RBUTTONUP:
			if(l){
				l = false;
				ab = true;
			}
			else if(ab)
			{
				if( mode == 0 )
				{
					a=false; b=false;
					LeftBoundary->clear();
					if(minTrackForTest)
					{
						delete minTrackForTest;
						minTrackForTest = NULL;
					}
					l=true;
					r=false;
					ab=false;
					mybool = true;
				}
				else if( mode < 0 )
				{
					a=false;
					LeftBoundary->clear();
					if(minTrackForTest)
					{
						delete minTrackForTest;
						minTrackForTest = NULL;
					}
					mybool = true;
				}
			}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//////////////////////////////////////////////////////////////////////////
void setvalue(HWND hDlg,int nIDDlgItem,int& n)
{
	WORD len;
	char c[16];
	n = 0;
    // Get the characters. 
	len = SendDlgItemMessage(hDlg,nIDDlgItem,EM_LINELENGTH,(WPARAM) 0,(LPARAM) 0);
	SendDlgItemMessage(hDlg,nIDDlgItem,EM_GETLINE,(WPARAM) 0,(LPARAM) c);
	for( int i = 0; i != len; i++ )
	{
		int exp = len-i-1;
		n += (c[2*i]-48) * pow(10.0,exp);
	}
}

//////////////////////////////////////////////////////////////////////////
// Message handler for curve box.
INT_PTR CALLBACK Curve(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK )
		{
			mode = 3;

			setvalue(hDlg, IDC_EDIT4, rad2);
			if( rad2 == 0 ){
				Ox=5000;
				Oy=-5000;
				rad1=2000;
				rad2=3500;
				ang1=90;
				ang2=0;
				camp=30;}
			else{
			setvalue(hDlg, IDC_EDIT1, Ox);
			setvalue(hDlg, IDC_EDIT2, Oy); Oy = -Oy;
			setvalue(hDlg, IDC_EDIT3, rad1);
			setvalue(hDlg, IDC_EDIT5, ang1);
			setvalue(hDlg, IDC_EDIT6, ang2);
			setvalue(hDlg, IDC_EDIT7, camp);}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if ( LOWORD(wParam) == IDCANCEL )
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Message handler for start box.
INT_PTR CALLBACK SetNumberOfGuards(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK )
		{
			setvalue(hDlg, IDC_EDITT, numOfGuards);
			if( numOfGuards == 0 )
			{
				numOfGuards = 1;
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if ( LOWORD(wParam) == IDCANCEL )
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}