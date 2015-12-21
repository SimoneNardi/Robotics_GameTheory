#include "CoverageAlgorithm.h"
#include "CoverageUtility.h"
#include "LearningAlgorithm.h"
#include "DISLAlgorithm.h"
#include "PIPIPAlgorithm.h"
#include "ParetoEfficientAlgorithm.h"
#include "CoarseCorrelatedAlgorithm.h"
#include "Probability.h"
#include "DiscretizedArea.h"
#include "StructuredArea.h"
#include "Agent.h"
#include "Guard.h"
#include "Neutral.h"
#include "Thief.h"
#include "Sink.h"
#include "World.h"

#include "BaseGeometry/MakePoint2D.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include <Windows.h>

using namespace Robotics;
using namespace Robotics::GameTheory;
using namespace std;
using namespace IDS::BaseGeometry;

const double g_CameraRadius = 7.;

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::setLostBattery(double _lostBattery)
{
	LOSTBATTERY_PERSTEP = _lostBattery;
}

//////////////////////////////////////////////////////////////////////////
Robotics::GameTheory::CoverageAlgorithm::CoverageAlgorithm(
	const std::set< std::shared_ptr<Agent> >& _agent, 
	std::shared_ptr<Area> _space, 
	int _type) 
	: m_world(nullptr)
	, m_learning(nullptr)
	//, m_time(0)
	, m_stats()
	, m_count(0)
{
	m_world = std::make_shared<World>(_agent, _space);

	std::shared_ptr<LearningAlgorithm> l_learning = nullptr;
	if(_type == 0)
		l_learning = std::make_shared<DISLAlgorithm>(m_world->getSpace());
	else if(_type == 1)
		l_learning = std::make_shared<PIPIPAlgorithm>(m_world->getSpace());
	else if(_type == 2)
		l_learning = std::make_shared<ParetoEfficientAlgorithm>(m_world->getSpace());
	else if(_type == 3)
		l_learning = std::make_shared<CoarseCorrelatedAlgorithm>(m_world->getSpace());
	else 
		throw std::exception("Error on selection of the Learning Algorithm.");

	this->setLearningAlgorithm(l_learning);

	m_stats.reset();
}

//////////////////////////////////////////////////////////////////////////
Robotics::GameTheory::CoverageAlgorithm::CoverageAlgorithm(
	const std::set< std::shared_ptr<Agent> >& _agent, 
	std::shared_ptr<DiscretizedArea> _space, 
	int _type) 
	: m_world(nullptr)
	, m_learning(nullptr)
	//, m_time(0)
	, m_stats()
	, m_count(0)
{
	m_world = std::make_shared<World>(_agent, _space);

	std::shared_ptr<LearningAlgorithm> l_learning = nullptr;
	if(_type == 0)
		l_learning = std::make_shared<DISLAlgorithm>(m_world->getSpace());
	else if(_type == 1)
		l_learning = std::make_shared<PIPIPAlgorithm>(m_world->getSpace());
	else if(_type == 2)
		l_learning = std::make_shared<ParetoEfficientAlgorithm>(m_world->getSpace());
	else if(_type == 3)
		l_learning = std::make_shared<CoarseCorrelatedAlgorithm>(m_world->getSpace());
	else 
		throw std::exception("Error on selection of the Learning Algorithm.");

	this->setLearningAlgorithm(l_learning);

	m_stats.reset();
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::setLearningAlgorithm(std::shared_ptr<LearningAlgorithm> _learning)
{
	m_learning = _learning;
	m_learning->setGuards(m_world->getGuards());
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::Reset()
{
	m_count = 0;
	m_stats.reset();
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::updateMonitor()
{
	m_learning->resetValue();
	m_learning->monitoringThieves(m_world->getThieves());
	m_learning->monitoringSinks(m_world->getSinks());
}

//////////////////////////////////////////////////////////////////////////
bool Robotics::GameTheory::CoverageAlgorithm::update(int _nStep, int _monitorUpdateTime, int _thiefJump, bool _continuousUpdate)
{
	bool res = true;

	if(m_count == 0)
		m_stats.reset();

	for(int i = 0; i < _nStep; ++i)
	{
		if(m_learning)
		{
			m_learning->updateTime();
			m_learning->resetCounter();

			if( m_count == 0 || (_monitorUpdateTime > 0 && !( m_count % _monitorUpdateTime )) )
			{
				this->updateMonitor();
			}

			res = m_learning->forwardOneStep();
			if(!res)
				return false;
		}

		if(m_count == 0)
			this->wakeUpAgentIfSecurityIsLow();

		++m_count;
		if(_monitorUpdateTime > 0 && !( m_count % _monitorUpdateTime ) ) 
			// ... il monitor sta fermo ma il ladro si muove.
			m_world->moveThieves(_thiefJump);

		this->wakeUpAgentIfSecurityIsLow();

		if(_continuousUpdate || i == _nStep-1)
			m_stats.addValues(
				m_learning->getTime(), 
				this->numberOfSquaresCoveredByGuards(), 
				m_learning->getPotentialValue(), 
				m_learning->getBenefitValue(), 
				this->getMaximumPotentialValue(),
				this->getSteadyNonCoopertativeBenefitValue(),
				m_learning->getExplorationRate(),
				m_learning->getBatteryValue());
	}
	return res;
}
#if 0
//
////////////////////////////////////////////////////////////////////////////
//void Robotics::GameTheory::CoverageAlgorithm::update(int nStep, int _monitorUpdateTime, int _thiefJump)
//{
//#if 0
//	m_space->setRandomSquareValue();this->wakeUpAgentIfSecurityIsLow();
//#else
//	if(nStep == 0)
//	{
//		if(m_learning)
//		{
//			m_learning->resetCounter();
//
//			if(!( m_count % _monitorUpdateTime ))
//			{
//				m_learning->resetValue();
//				m_learning->monitoringThieves(m_agent);
//			}
//			m_learning->forwardOneStep();
//		}
//
//		if(m_count == 0)
//			this->wakeUpAgentIfSecurityIsLow();
//
//		double l_potValue = m_learning->getPotentialValue();
//		m_potValues.push_back(l_potValue);
//		double l_benefitValue = m_learning->getBenefitValue();
//		m_benefitValues.push_back(l_benefitValue);
//
//		double l_maxThiefValue= this->getMaximumBenefitValue();
//		m_maxThiefValue.push_back(l_maxThiefValue);
//		int l_numsquare = this->numberOfSquaresCoveredByGuards();
//		m_squares.push_back(l_numsquare);
//		int l_time = this->getTime();
//		m_times.push_back(l_time);
//	}
//	else
//	{
//		if(m_count == 0)
//		{
//			m_potValues.clear();
//			m_benefitValues.clear();
//			m_maxThiefValue.clear();
//			m_squares.clear();
//			m_times.clear();
//		}
//
//		for(int i = 0; i < nStep; ++i)
//		{
//			if(m_learning)
//			{
//				++m_time;
//				m_learning->updateTime();
//				m_learning->resetCounter();
//
//				if(!( m_count % _monitorUpdateTime ))
//				{
//					//m_learning->resetTime();
//					m_learning->resetValue();
//
//					//m_learning->updateTime();
//					m_learning->monitoringThieves(m_agent);
//				}
//				
//				m_learning->forwardOneStep();
//			}
//
//			if(m_count == 0)
//				this->wakeUpAgentIfSecurityIsLow();
//
//			++m_count;
//			if( !( m_count % _monitorUpdateTime ) ) // ... il monitor sta fermo.
//				m_world->moveThieves(_thiefJump);
//
//			this->wakeUpAgentIfSecurityIsLow();
//
//			double l_potValue = m_learning->getPotentialValue();
//			m_potValues.push_back(l_potValue);
//			double l_benefitValue = m_learning->getBenefitValue();
//			m_benefitValues.push_back(l_benefitValue);
//
//			double l_maxThiefValue= this->getMaximumBenefitValue();
//			m_maxThiefValue.push_back(l_maxThiefValue);
//			int l_numsquare = this->numberOfSquaresCoveredByGuards();
//			m_squares.push_back(l_numsquare);
//			int l_time = this->getTime();
//			m_times.push_back(l_time);
//		}
//	}
//	return;
//#endif
//}
#endif

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::wakeUpAgentIfSecurityIsLow()
{
	return m_world->wakeUpAgentIfSecurityIsLow();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::Initialize()
{
	//this->randomInitializeAllAgent();

	m_learning->initialize();
}

//////////////////////////////////////////////////////////////////////////
void Robotics::GameTheory::CoverageAlgorithm::randomInitializeAllAgent()
{
	m_world->randomInitializeGuards();
	m_world->randomInitializeNeutrals();
	m_world->randomInitializeThief();
}

//////////////////////////////////////////////////////////////////////////
int CoverageAlgorithm::getNumberOfAgent()
{
	return m_world->getNumberOfAgent();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::removeAllThieves()
{
	return m_world->removeAllThieves();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::removeAllSinks()
{
	return m_world->removeAllSinks();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::setPositionOfSink(AgentPosition const& pos, SinkPtr _agent)
{
	bool found = false;
	/// Put agent in the space
	std::set< SinkPtr > l_agents = m_world->getSinks();
	for(auto it = l_agents.begin(); it != l_agents.end(); ++it)
	{
		SinkPtr l_agent = *it;
		if(_agent != l_agent)
			continue;

		found = true;

		// set position of the agent:
		l_agent->setCurrentPosition(pos);
	}

	if(!found)
	{
		if(!_agent)
			_agent = std::make_shared<Sink>(m_world->getNumberOfAgent(), pos);

		m_world->addSink( _agent );
	}
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::setPositionOfThief(AgentPosition const& pos, ThiefPtr _agent)
{
	bool found = false;
	/// Put agent in the space
	std::set< ThiefPtr > l_agents = m_world->getThieves();
	for(auto it = l_agents.begin(); it != l_agents.end(); ++it)
	{
		ThiefPtr l_agent = *it;
		//if(!l_agent->isThief())
		if(_agent != l_agent)
			continue;

		found = true;

		// compute position of agent and camera:
		//AgentPosition pos = m_space->getRandomPosition();
		// set position of the agent:
		l_agent->setCurrentPosition(pos);
	}

	if(!found)
	{
		if(!_agent)
			_agent = std::make_shared<Thief>(m_world->getNumberOfAgent(), pos);
			
		m_world->addThief( _agent );
	}
}

//////////////////////////////////////////////////////////////////////////
std::vector< std::shared_ptr<Square> > CoverageAlgorithm::getSquares() const
{
	return m_world->getSpace()->getSquares();
}

//////////////////////////////////////////////////////////////////////////
bool CoverageAlgorithm::areaContains(const IDS::BaseGeometry::Point2D & _thiefStartingPt) const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
SquarePtr CoverageAlgorithm::findSquare(IDS::BaseGeometry::Point2D const& point) const
{
	return m_world->getSpace()->getSquare(point);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getGuardsPosition(std::vector<AgentPosition> & _pos)
{
	return m_learning->getGuardsPosition(_pos);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getGuardsCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas)
{
	return m_learning->getGuardsCoverage(_areas);
}

//////////////////////////////////////////////////////////////////////////
int CoverageAlgorithm::getGlobalTrajectoryCoverage()
{
	return m_learning->getGlobalTrajectoryCoverage();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getGuardsSquare(std::vector<std::pair<SquarePtr, AgentActionIndex>> & _pos)
{
	return m_learning->getGuardsSquare(_pos);
}

//////////////////////////////////////////////////////////////////////////
int CoverageAlgorithm::numberOfSquaresCoveredByGuards() const
{
	return m_world->getSpace()->numberOfSquaresCoveredByGuards();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getSinksPosition(std::vector<AgentPosition> & _pos)
{
	return m_world->getSinksPosition(_pos);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getSinksSquare(std::vector<std::pair<std::shared_ptr<Square>,int>> & _pos)
{
	return m_world->getSinksSquare(_pos);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getSinksCoverage( std::vector< std::vector<IDS::BaseGeometry::Point2D> > & _areas)
{
	return m_world->getSinksCoverage(_areas);
}

////////////////////////////////////////////////////////////////////////////
//int CoverageAlgorithm::getTime() const
//{
//	return m_time;
//}

//////////////////////////////////////////////////////////////////////////
int CoverageAlgorithm::getNumberOfSteps(double _stopRate)
{
	return m_learning->getNumberOfSteps(_stopRate);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printPotential(std::string const& _filename, bool printOnFile)
{
	return m_stats.printPotential(_filename, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printBenefit(std::string const& _filename, bool printOnFile)
{
	return m_stats.printBenefit(_filename, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printPotentialIndex(std::string const& name, bool printOnFile)
{
	return m_stats.printPotentialIndex(name, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printBenefitIndex(std::string const& name, bool printOnFile)
{
	return m_stats.printBenefitIndex(name, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printPotentialIndexVersusExplorationRate(std::string const& name, bool printOnFile)
{
	return m_stats.printPotentialIndexVersusExplorationRate(name, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printExplorationRate(std::string const& name, bool printOnFile)
{
	return m_stats.printExplorationRate(name, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printNewPerformanceIndex(std::string const& name, bool _print)
{
	return m_stats.printBenefitIndex(name, _print);
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printNewPerformanceIndexVersusExplorationRate(std::string const& name, bool printOnFile)
{
	return m_stats.printPotentialIndexVersusExplorationRate(name, printOnFile);
}

//////////////////////////////////////////////////////////////////////////
std::string CoverageAlgorithm::getExplorationRateStr()
{
	return m_learning->getExplorationRateStr();
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getExplorationRate()
{
	return m_learning->getExplorationRate();
}

//////////////////////////////////////////////////////////////////////////
std::string CoverageAlgorithm::getBatteryValueStr()
{
	return m_learning->getBatteryValueStr();
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getBatteryValue()
{
	return m_learning->getBatteryValue();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::getThievesPosition(std::vector<AgentPosition> & _pos)
{
	_pos.clear();
	std::set< ThiefPtr > l_thieves = m_world->getThieves();
	for(auto it = l_thieves.begin(); it != l_thieves.end(); ++it)
	{
		_pos.push_back( (*it)->getCurrentPosition() );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct AgentDriver 
{
	IDS::BaseGeometry::Point2D position;
	enum Type
	{
		THIEF =-1,
		NEUTRAL = 0,
		GUARD = 1,
		SINK = 2
	} type;
};

//////////////////////////////////////////////////////////////////////////
void importFromFile(
	std::string const & _filename, 
	std::vector<IDS::BaseGeometry::Point2D> & _boundary,
	std::vector<AgentDriver> & _agents)
{
	std::ifstream iFile(_filename);	// input.txt has integers, one per line

	while (!iFile.eof())
	{
		int numOfXXX;
		iFile >> numOfXXX; //vertices
		for(int i = 0; i < numOfXXX; ++i)
		{
			double x, y;
			iFile >> x;
			iFile >> y;
			_boundary.push_back( makePoint(IDSReal2D(x,y), EucMetric) );
		}
		iFile >> numOfXXX;	// guards
		for(int i = 0; i < numOfXXX; ++i)
		{
			double x, y;
			iFile >> x;
			iFile >> y;
			AgentDriver driver;
			driver.position = makePoint(IDSReal2D(x,y), EucMetric);
			driver.type = AgentDriver::GUARD;
			_agents.push_back(driver);
		}
		iFile >> numOfXXX; // Thieves
		for(int i = 0; i < numOfXXX; ++i)
		{
			double x, y;
			iFile >> x;
			iFile >> y;
			AgentDriver driver;
			driver.position = makePoint(IDSReal2D(x,y), EucMetric);
			driver.type = AgentDriver::THIEF;
			_agents.push_back(driver);
		}
		iFile >> numOfXXX; // Sinks
		for(int i = 0; i < numOfXXX; ++i)
		{
			double x, y;
			iFile >> x;
			iFile >> y;
			AgentDriver driver;
			driver.position = makePoint(IDSReal2D(x,y), EucMetric);
			driver.type = AgentDriver::SINK;
			_agents.push_back(driver);
		}
		iFile >> numOfXXX; // Neutral
		for(int i = 0; i < numOfXXX; ++i)
		{
			double x, y;
			iFile >> x;
			iFile >> y;
			AgentDriver driver;
			driver.position = makePoint(IDSReal2D(x,y), EucMetric);
			driver.type = AgentDriver::NEUTRAL;
			_agents.push_back(driver);
		}
	}
	iFile.close();
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<CoverageAlgorithm> Robotics::GameTheory::CoverageAlgorithm::createFromFile(std::string const & _filename, int _type, int _period)
{
	std::vector<IDS::BaseGeometry::Point2D> l_bound;
	std::vector<AgentDriver> l_agentDriver;

	importFromFile(_filename, l_bound, l_agentDriver);

	/// Create Coverage Algorithm:
	std::shared_ptr<Area> l_space = std::make_shared<StructuredArea>(l_bound);

	int l_id = -1;
	std::set< std::shared_ptr<Agent> >l_agents; 
	for(size_t i = 0; i < l_agentDriver.size(); ++i)
	{
		if(l_agentDriver[i].type != AgentDriver::GUARD)
			continue;

		++l_id;
		AgentPosition l_pos( 
			l_agentDriver[i].position 
			/*l_space->randomPosition()*/, 
			CameraPosition( l_space->getDistance()/15. ) );

		std::shared_ptr<Agent> l_agent = std::make_shared<Guard>(1, l_id, l_pos, _period, _type == 2? 1 : 2);

		l_agents.insert(l_agent);
	}

	std::shared_ptr<CoverageAlgorithm> l_algorithm = std::make_shared<CoverageAlgorithm>(l_agents, l_space, _type);

	for(size_t i = 0; i < l_agentDriver.size(); ++i)
	{
		if(l_agentDriver[i].type != AgentDriver::THIEF)
			continue;

		AgentPosition l_pos( l_space->randomPosition(), CameraPosition( l_space->getDistance()/15. ) );
		Sleep(100);

		ThiefPtr l_agent = std::make_shared<Thief>(l_algorithm->getNumberOfAgent(), l_pos/*l_agentDriver[i].position*/);
		l_algorithm->setPositionOfThief(l_pos, l_agent);
	}

	for(size_t i = 0; i < l_agentDriver.size(); ++i)
	{
		if(l_agentDriver[i].type != AgentDriver::SINK)
			continue;

		AgentPosition l_pos( l_space->randomPosition(), CameraPosition( l_space->getDistance()/15. ) );
		Sleep(100);

		SinkPtr l_agent = std::make_shared<Sink>(l_algorithm->getNumberOfAgent(), l_pos/*l_agentDriver[i].position*/);
		l_algorithm->setPositionOfSink(l_pos, l_agent);
	}

	return l_algorithm;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<CoverageAlgorithm> Robotics::GameTheory::CoverageAlgorithm::createFromAreaFile(
	std::string const & _areaFile, 
	std::string const & _agentFile,
	int _type,
	int _periodIndex,
	double _epsilon)
{
	std::vector<IDS::BaseGeometry::Point2D> l_bound;
	std::vector<AgentDriver> l_agentDriver;
#ifdef _PRINT
	cout << "importing agent"<<endl;
#endif
	importFromFile(_agentFile, l_bound, l_agentDriver);
#ifdef _PRINT
	cout << "imported agent"<<endl;
#endif

	/// Create Coverage Algorithm:
#ifdef _PRINT
	cout << "creating scenario"<<endl;
#endif

	std::shared_ptr<DiscretizedArea> l_space = std::make_shared<DiscretizedArea>(_areaFile);
#ifdef _PRINT
	cout << "created scenario"<<endl;
#endif

#ifdef _PRINT
	cout << "Placing guards"<<endl;
#endif
	int l_id = -1;
	std::set< std::shared_ptr<Agent> >l_agents; 
	for(size_t i = 0; i < l_agentDriver.size(); ++i)
	{
		if(l_agentDriver[i].type != AgentDriver::GUARD)
			continue;

		++l_id;
		
		AgentPosition l_pos( 
			l_agentDriver[i].position 
			/*l_space->randomPosition()*/, 
			CameraPosition( double(l_space->getXStep() + l_space->getYStep())/2. *1.5) );

		Point2D l_point;
		if( l_space->getRandomPosition(l_point) && 0)
		{
			l_pos = AgentPosition ( l_point, CameraPosition( double(l_space->getXStep() + l_space->getYStep())/2. *1.5) );
			Sleep(50);
		}
		
		std::shared_ptr<Agent> l_agent = std::make_shared<Guard>(1, l_id, l_pos, _periodIndex, _type == 2? 1 : 2);
		l_agents.insert(l_agent);
	}
#ifdef _PRINT
	cout << "Placed guards"<<endl;
#endif

#ifdef _PRINT
	cout << "Creating algorithm"<<endl;
#endif
	std::shared_ptr<CoverageAlgorithm> l_algorithm = std::make_shared<CoverageAlgorithm>(l_agents, l_space, _type);
	
	l_algorithm->setExperimentalRate(_epsilon);

#ifdef _PRINT
	cout << "Created algorithm"<<endl;
#endif

#ifdef _PRINT
	cout << "Placing Thief"<<endl;
#endif
	for(size_t i = 0; i < l_agentDriver.size(); ++i)
	{
		if(l_agentDriver[i].type != AgentDriver::THIEF)
			continue;

		AgentPosition l_pos( l_agentDriver[i].position, CameraPosition() );

		Point2D l_point;
		if( l_space->getRandomPosition(l_point) )
		{
			l_pos = AgentPosition ( l_point, CameraPosition() );
			Sleep(50);
		}

		ThiefPtr l_agent = std::make_shared<Thief>(l_algorithm->getNumberOfAgent(), l_pos);
		l_algorithm->setPositionOfThief(l_agent->getCurrentPosition(), l_agent);
	}
#ifdef _PRINT
	cout << "Placed Thief"<<endl;
#endif
	
#ifdef _PRINT
	cout << "Placing Sink"<<endl;
#endif
	for(size_t i = 0; i < l_agentDriver.size(); ++i)
	{
		if(l_agentDriver[i].type != AgentDriver::SINK)
			continue;

		AgentPosition l_pos( l_agentDriver[i].position, CameraPosition() );

		Point2D l_point;
		if( l_space->getRandomPosition(l_point) )
		{
			l_pos = AgentPosition ( l_point, CameraPosition() );
			Sleep(50);
		}

		SinkPtr l_agent = std::make_shared<Sink>(l_algorithm->getNumberOfAgent(), l_pos);
		l_algorithm->setPositionOfSink(l_agent->getCurrentPosition(), l_agent);
	}
#ifdef _PRINT
	cout << "Placed Sink"<<endl;
#endif

	return l_algorithm;
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::setExperimentalRate(double _epsilon)
{
	m_learning->setExperimentalRate(_epsilon);
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getMaximumPotentialValue()
{
	return m_world->getMaximumValue();
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getSteadyNonCoopertativeBenefitValue()
{
	std::set<ThiefPtr> l_thieves = m_world->getThieves();
	return double(l_thieves.size()) * std::log( double(m_world->getGuards().size()) ) * g_maxValue/g_maxValuePossible;
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getMaximumBenefitValue()
{
	// CONTROLLARE
	std::set<ThiefPtr> l_thieves = m_world->getThieves();
	return double(l_thieves.size()) * 100.;// * m_world->getSpace()->getNumberOfValidSquare();
}


double CoverageAlgorithm::getPotentialIndexMediumValue()
{
	return m_stats.getPotentialIndexMediumValue();
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getBenefitIndexMediumValue()
{
	return m_stats.getBenefitIndexMediumValue();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printArea(const std::string & filename)
{
	std::ofstream outFile;
	outFile.open(filename);
	if(outFile.is_open())
	{
		for(int row = 0; row < m_world->getSpace()->getNumRow(); ++row )
		{
			for(int col = 0; col < m_world->getSpace()->getNumCol(); ++col)
			{
				SquarePtr l_square = m_world->getSpace()->getSquare(row,col);
				if(l_square->isValid())
					outFile << "0";
				else
					outFile << "1";
			}
			outFile << endl;
		}
	}
}

#pragma region CONFIGURATION

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getTrajectoryPotentialIndex()
{
	double l_potValue = m_learning->getPotentialValue();
	double l_nonCooperativeSteadyValue = this->getSteadyNonCoopertativeBenefitValue();
	
	return l_potValue/l_nonCooperativeSteadyValue;
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getTrajectoryBenefitIndex()
{
	double l_maxBenefitValue = this->getMaximumPotentialValue();
	double l_benefitValue = m_learning->getBenefitValue();

	return ( l_maxBenefitValue-l_benefitValue ) / l_maxBenefitValue;
}

//////////////////////////////////////////////////////////////////////////
double CoverageAlgorithm::getTrajectoryCoverage()
{
	return m_learning->getGlobalTrajectoryCoverage();
}

//////////////////////////////////////////////////////////////////////////
void CoverageAlgorithm::printPhoto(std::string const& _outputFileName)
{
	std::ofstream l_file;
	l_file.open(_outputFileName);
	if (!l_file.is_open())
		return; 

	m_world->saveConfiguration(l_file);

	double l_potentialIndex = this->getTrajectoryPotentialIndex();
	l_file << "Potential Index " << l_potentialIndex << endl;
	double l_benefitIndex = this->getTrajectoryBenefitIndex();
	l_file << "Benefit Index " << l_benefitIndex << endl;
	double l_coverageIndex = this->getTrajectoryCoverage();
	l_file << "Coverage Index " << l_coverageIndex << endl;
	double l_explorationRate = m_learning->computeExplorationRate();
	l_file << "Esploration Rate " << l_explorationRate << endl;

	l_file.close();
	
	return;
}

#pragma endregion
