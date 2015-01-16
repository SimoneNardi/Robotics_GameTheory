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
Point2D g_sinkPt;

LineString2D *g_boundary;
std::vector<Point2D> g_boundary2D;

////////for drawing purposes
POINT g_drawing_VerticesL[5];
POINT g_drawing_VerticesR[5];
bool	g_drawing_externalBoundary=true,
		g_drawing_thiefPosition=false,
		g_drawing_sink = false;

int g_drawing_mode = 0;

int g_drawing_numToLoad = 1;

int g_drawing_loadCounter = 0;

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
		m_algorithm->update(numberOfStep,-1);
		return 1;
	}

	std::shared_ptr<CoverageAlgorithm> getAlgorithm() {return m_algorithm;}

	inline int setThief(const BaseGeometry::Point2D & _thiefStartingPt, std::shared_ptr<Thief> _agent)
	{
		m_algorithm->setPositionOfThief(_thiefStartingPt, _agent);
		return 1;
	}

	inline int setSink(const BaseGeometry::Point2D & _sinkStartingPt, std::shared_ptr<Sink> _agent)
	{
		m_algorithm->setPositionOfSink(_sinkStartingPt, _agent);
		return 1;
	}

	inline void removeAllThieves()
	{
		return m_algorithm->removeAllThieves();
	}

	inline void removeAllSinks()
	{
		return m_algorithm->removeAllSinks();
	}

	bool areaContains(const BaseGeometry::Point2D & _thiefStartingPt)
	{
		return m_algorithm->areaContains(_thiefStartingPt);
	}

	void getGuardsPosition(std::vector< AgentPosition > & _pos);
	void getGuardsSquare(std::vector< std::pair<std::shared_ptr<Square>, AgentActionIndex> > & _pos);
	void getGuardsCoverage(std::vector<LineString2D>& _areas);

	void getSinksSquare(std::vector< std::pair<std::shared_ptr<Square>,int> > & _pos);
	void getSinksCoverage(std::vector<LineString2D>& _areas);

	int numberOfSquaresCoveredByGuards();
	//int getTime();
	void printPotential(std::string const& name, bool printOnFile = true);
	void printBenefit(std::string const& name, bool printOnFile = true);
	void printPotentialIndex(std::string const& name, bool printOnFile = true);
	void printNewPerformanceIndex(std::string const& name, bool printOnFile = true);
	void printNewPerformanceIndexVersusExplorationRate(std::string const& name, bool printOnFile = true);
	void printExplorationRate(std::string const& name, bool printOnFile = true);

	std::string getExplorationRateStr();
	std::string getBatteryValueStr();
	double getExplorationRate();

	void exportOnFile(std::string const& filename);

	void updateMonitor();
};

CoverageTest *g_coverageTest= NULL;

// For the window application
int g_window_Ox, g_window_Oy, g_window_rad1, g_window_rad2, g_window_ang1, g_window_ang2, g_window_camp;

int g_numberOfAgents = 3;
int g_numberOfSteps = 1;
int g_agentsPeriod = 1;
bool g_drawSquare = true;
bool g_drawRealArea = false;
bool g_drawone = false;
bool g_PrintPotential = true;
bool g_pareto = false;
bool g_correlated = false;
bool g_DISL = true;
bool g_PIPIP = false;

SquarePtr RR;
enum SpaceElemType
{
	TRIANGLE,
	RECTANGLE,
	SQUARE 
} g_TTRR;

std::ofstream g_explorationFile;

#endif
