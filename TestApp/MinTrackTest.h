////////////////////
//Global variables//
////////////////////

#ifndef __MINTRACKTEST__H__
#define __MINTRACKTEST__H__

#include "BaseGeometry/Vertex.h"

using namespace IDS::BaseGeometry;

Vertex A;
LineString *LeftBoundary;
LineString *path1 = 0;
LineString *path2 = 0;

////////for drawing purposes
POINT VerticesL[5];
POINT VerticesR[5];
bool a=false,b=false,l=true,r=false,ab=false;
bool mybool = true;
int mode = 0;
int numToLoad = 1;
int loadCounter = 0;

Point2D* Boundaryy;
int dim;
bool counterclockwise = true;

class MinTrackForTest: public MinTrackManager
{
public:
	//////////////////////////////////////////////////////////////////////////
	MinTrackForTest(int nPoints, const Point2D *bound,int nStart, bool counterclockwise):
	  MinTrackManager(nPoints, bound,nStart,counterclockwise){}

	  //////////////////////////////////////////////////////////////////////////
	  MinTrackForTest(const vector<Point2D>& bound,
		  const Point2D& initial_start,  const Point2D& initial_end,
		  bool counterclockwise):
	  MinTrackManager(bound,initial_start,initial_end,counterclockwise){}

	  //////////////////////////////////////////////////////////////////////////
	  MinTrackForTest(const vector<Point2D>& bound,
		  const vector<Point2D>& start,
		  bool counterclockwise):
	  MinTrackManager(bound,start,counterclockwise){}

	  //////////////////////////////////////////////////////////////////////////
	  void getTriAndRett(int& nTri,int& nRett)
	  {
		  nTri = this->Triangles.size();
		  nRett = this->Rectangles.size();
	  }

	  //////////////////////////////////////////////////////////////////////////
	  void copyTriAndRett(int nTri,int nRett,Triangolo* tri,Rettangolo* rett)
	  {
		  int i;
		  std::list<Triangolo>::iterator it = Triangles.begin();
		  for( i = 0; i != nTri; i++ )
		  {
			  tri[i] = *it;
			  it++;
		  }
		  std::list<Rettangolo>::iterator it2 = Rectangles.begin();
		  for( i = 0; i != nRett; i++ )
		  {
			  rett[i] = *it2;
			  it2++;
		  }
	  }

	  //////////////////////////////////////////////////////////////////////////
	  bool FindTri(double x,double y,Triangolo &T,Rettangolo &R,int &tri)
	  {
		  Vertex V(x,y);
		  double min = IDSMath::Infinity;

		  if( numStart == 1 )
		  {
			  std::list<Rettangolo>::iterator it1;
			  for( it1 = Rectangles.begin(); it1 != Rectangles.end(); it1++ )
			  {
				  if( V.belongsToRectangle(*it1) )
				  {
					  R = *it1;
					  tri = -1;
					  return true;
				  }
			  }
		  }
		  else
		  {
			  std::list<Rettangolo>::iterator it1;		
			  for( it1 = Rectangles.begin(); it1 != Rectangles.end(); it1++ )
			  {
				  if( V.belongsToRectangle(*it1) )
				  {
					  Vertex W;
					  int n = it1->DerNum;
					  if( n == -1 )
					  {
						  int i;
						  for( i = 0; i != numStart; i++ )
						  {
							  W = projectOnSegment(*MultiDer[i],*MultiDer[i+1],V);
							  double d = (V-W).mod();
							  if( d < min )
							  {
								  min = d;
								  tri = -1;
								  R = *it1;
							  }
						  }
					  }
					  else
						  W = projectOnSegment(*MultiDer[n],*MultiDer[n+1],V);

					  double d = (V-W).mod();
					  if( d < min )
					  {
						  min = d;
						  tri = -1;
						  R = *it1;
					  }
				  }
			  }
		  }

		  // If the corridor is non-autointersecting the first triangle that contains V is the best one
		  if( !autointersecting && numStart == 1 )
		  {
			  std::list<Triangolo>::iterator it;
			  for( it = Triangles.begin(); it != Triangles.end(); it++ )
			  {
				  if( V.belongsToTriangle(*it) )
				  {
					  T = *it;
					  tri = 1;
					  return true;
				  }
			  }
		  }
		  // If the corridor is autointersecting we have to find the nearest Triangle
		  else
		  {
			  std::list<Triangolo>::iterator it;
			  std::list<Triangolo>::iterator best;
			  for( it = Triangles.begin(); it != Triangles.end(); it++ )
			  {
				  double dist = it->principal->D() + (V-*(it->principal)).mod();
				  if( dist < min && V.belongsToTriangle(*it) )
				  {
					  min = dist;
					  T = *it;
					  tri = 1;
				  }
			  }
		  }

		  if( min != IDSMath::Infinity )
			  return true;
		  else
			  return false;
	  }
	  //////////////////////////////////////////////////////////////////////////
	  int CalcMinTrack(const Point2D *obs)
	  {
		  return CalcMinTrackMany(obs);
	  }
	  //////////////////////////////////////////////////////////////////////////
	  int RetrieveTrack(const Point2D *obs, Point2D *track)
	  {
		  return RetrieveTrackMany(obs,track,false);
	  }
};

MinTrackForTest *minTrackForTest= NULL;

// For the window application
int Ox,Oy,rad1,rad2,ang1,ang2,camp;
int startSeg = 3;
bool derIsInside = false;

bool drawtriangles = true;
bool partitioncorridor = true;
bool drawreal = false;

bool drawone = false;
Triangolo TT;
Rettangolo RR;
int TTRR;


#endif
