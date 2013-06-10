////////////////////
// Coverage Test  //
////////////////////

#ifndef __COVERAGE_TEST__H__
#define __COVERAGE_TEST__H__

/// GeometricKernel
#include "BaseGeometry/Point2D.h"
#include "BaseGeometry/MakeMetric.h"
#include "BaseGeometry/MakePoint2D.h"

/// Coverage
#include "Coverage/CoverageAlgorithm.h"
#include "Coverage/DiscretizedArea.h"
#include "Coverage/Agent.h"

#include <iostream>
#include <fstream>

using namespace IDS;
using namespace IDS::BaseGeometry;
using namespace Robotics::GameTheory;

typedef std::vector<Point2D> LineString2D;
typedef std::vector<Point2D>::iterator LineString2DIterator;

Point2D g_thiefStartingPt;

LineString2D *g_LeftBoundary;
LineString2D *g_path1 = NULL;
LineString2D *g_path2 = NULL;

////////for drawing purposes
POINT g_drawing_VerticesL[5];
POINT g_drawing_VerticesR[5];
bool	g_drawing_a=false,
		g_drawing_b=false,
		g_drawing_externalBoundary=true,
		g_drawing_r=false,
		g_drawing_thiefPosition=false;
bool g_drawing_mybool = true;
int g_drawing_mode = 0;
int g_drawing_numToLoad = 1;
int g_drawing_loadCounter = 0;

std::vector<Point2D> g_boundary2D;
int g_dim;
bool g_counterclockwise = true;

MetricType g_metric = EucMetric;

class CoverageTest
{
	std::shared_ptr<CoverageAlgorithm> m_algorithm;

public:
	//////////////////////////////////////////////////////////////////////////
	CoverageTest(int nPoints, const Point2D *bound, bool counterclockwise);

	//////////////////////////////////////////////////////////////////////////
	CoverageTest(const vector<Point2D>& bound, bool counterclockwise);

	//////////////////////////////////////////////////////////////////////////
	void addThief(double x,double y)
	{
		/// add thief to scene.

		/// set movement thief.
	}

	//////////////////////////////////////////////////////////////////////////
	void addThief(const Point2D & position)
	{
		/// add thief to scene.

		/// set movement thief.
	}

	//////////////////////////////////////////////////////////////////////////
	void getNumOfSquare(int& nSquare)
	{
		nSquare = m_algorithm->getSquares().size();
	}

	////////////////////////////////////////////////////////////////////////////
	std::vector<SquarePtr> getSquares()
	{
		return m_algorithm->getSquares();
	}

	//////////////////////////////////////////////////////////////////////////
	bool FindSquare(Point2D const& V, SquarePtr &R)
	{
		if(m_algorithm)
			R = m_algorithm->findSquare(V);
		if(R)
			return true;
		else
			return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FindSquare(double x, double y, SquarePtr &R)
	{
		Point2D V = makePoint( IDSReal2D(x,y), g_metric );
		return this->FindSquare(V, R);

		//if( numStart == 1 )
		//{
		//	std::list<Rettangolo>::iterator it1;
		//	for( it1 = Rectangles.begin(); it1 != Rectangles.end(); it1++ )
		//	{
		//		if( V.belongsToRectangle(*it1) )
		//		{
		//			R = *it1;
		//			tri = -1;
		//			return true;
		//		}
		//	}
		//}
		//else
		//{
		//	std::list<Rettangolo>::iterator it1;		
		//	for( it1 = Rectangles.begin(); it1 != Rectangles.end(); it1++ )
		//	{
		//		if( V.belongsToRectangle(*it1) )
		//		{
		//			Vertex W;
		//			int n = it1->DerNum;
		//			if( n == -1 )
		//			{
		//				int i;
		//				for( i = 0; i != numStart; i++ )
		//				{
		//					W = projectOnSegment(*MultiDer[i],*MultiDer[i+1],V);
		//					double d = (V-W).mod();
		//					if( d < min )
		//					{
		//						min = d;
		//						tri = -1;
		//						R = *it1;
		//					}
		//				}
		//			}
		//			else
		//				W = projectOnSegment(*MultiDer[n],*MultiDer[n+1],V);

		//			double d = (V-W).mod();
		//			if( d < min )
		//			{
		//				min = d;
		//				tri = -1;
		//				R = *it1;
		//			}
		//		}
		//	}
		//}

		//// If the corridor is non-autointersecting the first triangle that contains V is the best one
		//if( !autointersecting && numStart == 1 )
		//{
		//	std::list<Triangolo>::iterator it;
		//	for( it = Triangles.begin(); it != Triangles.end(); it++ )
		//	{
		//		if( V.belongsToTriangle(*it) )
		//		{
		//			T = *it;
		//			tri = 1;
		//			return true;
		//		}
		//	}
		//}
		//// If the corridor is autointersecting we have to find the nearest Triangle
		//else
		//{
		//	std::list<Triangolo>::iterator it;
		//	std::list<Triangolo>::iterator best;
		//	for( it = Triangles.begin(); it != Triangles.end(); it++ )
		//	{
		//		double dist = it->principal->D() + (V-*(it->principal)).mod();
		//		if( dist < min && V.belongsToTriangle(*it) )
		//		{
		//			min = dist;
		//			T = *it;
		//			tri = 1;
		//		}
		//	}
		//}
	}

	int CalcMinTrack(const Point2D *obs)
	{
		return 0; //CalcMinTrackMany(obs);
	}

	int RetrieveTrack(const Point2D &obs, std::vector<Point2D> & track)
	{
		return 1;//RetrieveTrackMany(obs,track,false);
	}

	int goForward(int numberOfStep)
	{
		m_algorithm->update(numberOfStep);
		return 1;
	}

	int setThief(const BaseGeometry::Point2D & _thiefStartingPt)
	{
		m_algorithm->setPositionOfThief(_thiefStartingPt);
		return 1;
	}

	bool areaContains(const BaseGeometry::Point2D & _thiefStartingPt)
	{
		return m_algorithm->areaContains(_thiefStartingPt);
	}

	void getGuardsPosition(std::vector< AgentPosition > & _pos);
	void getGuardsSquare(std::vector< std::shared_ptr<Square> > & _pos);
	void getGuardsCoverage(std::vector<LineString2D>& _areas);

	int numberOfSquaresCoveredByGuards();
	int getTime();
	void printGraph(std::string const& name);
	std::string getExplorationRate();
	void printPotential(bool potential);
};

CoverageTest *g_coverageTest= NULL;

// For the window application
int g_window_Ox, g_window_Oy, g_window_rad1, g_window_rad2, g_window_ang1, g_window_ang2, g_window_camp;

int g_numberOfAgents = 8;
int g_numberOfSteps = 1;
bool g_drawSquare = true;
bool g_drawRealArea = false;
bool g_drawone = false;
bool g_partitionCorridor = true;

SquarePtr RR;
enum SpaceElemType
{
	TRIANGLE,
	RECTANGLE,
	SQUARE 
} g_TTRR;

std::ofstream g_explorationFile;

#endif
