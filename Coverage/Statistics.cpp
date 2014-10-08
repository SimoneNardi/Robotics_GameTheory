#include "Statistics.h"
#include "IDSMath.h"
#include <iostream>

#include "IDSGraphicsC.h"

namespace Robotics
{
	namespace GameTheory 
	{
		//////////////////////////////////////////////////////////////////////////
		void Statistics::reset()
		{
			m_potValues.clear();
			m_benefitValues.clear();
			m_maxBenefitValue.clear();
			m_NonCooperativeSteadyValue.clear();
			m_squares.clear();
			m_times.clear();
			m_explorationRate.clear();
			m_performanceIndex.clear();
			m_oldPerformanceIndex.clear();
			m_explorationRate.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::addValues(int time, int square, double potValue, double benefitValue, double maxBenefitValue, double NonCooperativeSteadyValue, double _explorationRate)
		{
			m_times.push_back(time);
			m_squares.push_back(square);
			m_potValues.push_back(potValue);
			m_benefitValues.push_back(benefitValue);
			m_maxBenefitValue.push_back(maxBenefitValue);
			m_NonCooperativeSteadyValue.push_back(NonCooperativeSteadyValue);
			m_performanceIndex.push_back(potValue/NonCooperativeSteadyValue);
			m_oldPerformanceIndex.push_back( ( maxBenefitValue-benefitValue ) / maxBenefitValue );
			m_explorationRate.push_back(_explorationRate);
		}

		//////////////////////////////////////////////////////////////////////////
		//	Benefit Value
		double Statistics::getBoxPlotValue()
		{
			double tot = 0;
			for(size_t i = 0; i < m_benefitValues.size(); ++i)
				tot+= (m_maxBenefitValue[i] - m_benefitValues[i]) / m_maxBenefitValue[i];

			double l_value = 1.;
			if(m_benefitValues.size() != 0)
				l_value = tot / double(m_benefitValues.size());
			else
				std::cout << "NoData" << "\t";

			if(fabs(l_value) < IDSMath::TOLERANCE2)
				l_value = 0;

			//std::cout << "Value: " << l_value << "\t";
			return l_value;
		}

		//////////////////////////////////////////////////////////////////////////
		//	Non Cooperative Index
		double Statistics::getBoxPlotValueNewIndex()
		{
			double tot = 0;
			for(size_t i = 0; i < m_performanceIndex.size(); ++i)
				tot+= m_performanceIndex[i];

			double l_value = 1.;
			if(m_performanceIndex.size() != 0)
				l_value = tot / double(m_performanceIndex.size());
			else
				std::cout << "NoData" << "\t";

			if(fabs(l_value) < IDSMath::TOLERANCE2)
				l_value = 0;

			//std::cout << "Value: " << l_value << "\t";
			return l_value;
		}

		//////////////////////////////////////////////////////////////////////////
		void printGraph(
			std::string const& _filename,
			std::string const& _title,
			std::string const& _xLabel,
			std::string const& _yLabel,
			std::string const& _curveLabel,
			std::vector<double>& _ascissa,
			std::vector<double>& _ordinata,
			bool _printOnFile)
		{
			bool l_showpoints = true;
			bool l_printOnFile = _printOnFile; 
			std::string l_title = _filename;
			std::string l_yLabel = l_yLabel;

			//	Draw Graphics:
			int n_graph = 1;
			int flags= 0;
			//if(!_printOnFile)
			//	flags = IDSG_MULTITHREAD;
			std::vector<int> np;
			np.push_back( (int)_ascissa.size() );

			CARTESIAN_GRAPH_DP(&n_graph, &flags, &np[0], &_ascissa[0], &_ordinata[0],"");

			char buffer[1024];
			sprintf(buffer, "%s", _title.c_str());
			GRAPH_COMMAND(buffer);

			// x Label
			std::string cmd = "xaxis label '";
			cmd += _xLabel;
			cmd += "'";
			GRAPH_COMMAND(const_cast<char*>(cmd.c_str()));
			// y Label
			cmd = "yaxis label '";
			cmd += _yLabel;
			cmd += "'";
			GRAPH_COMMAND(const_cast<char*>(cmd.c_str()));

			GRAPH_COMMAND("curve 0 color 0 0 1");
			GRAPH_COMMAND("curve 0 filled false");
			GRAPH_COMMAND("curve 0 fillcolor 0 0 1");

			// Curve Label
			cmd = "curve 0 label '";
			cmd += _curveLabel;
			cmd += "'";
			GRAPH_COMMAND(const_cast<char*>(cmd.c_str()));

			GRAPH_COMMAND("vdiv .9");
			GRAPH_COMMAND("show");

			if(_printOnFile)
			{
				char buffer[1024];
				sprintf(buffer,"hardcopy ./Graph/%s.jpg 99", _filename.c_str());

				std::cout << "Print Graph ";
				std::cout << _filename.c_str() << std::endl;

				//system("pause");
				GRAPH_COMMAND(buffer);
				GRAPH_COMMAND("close");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printPerformanceIndex(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Old Performance Index";
			std::string l_yLabel = "Old Performance Index";
			std::string l_xLabel = "Time";

			std::string l_curveLabel = "Old Index Curve";

			//std::vector<double> l_ascissa;
			//l_ascissa.insert(l_ascissa.end(), m_times.begin(), m_times.end());

			//std::vector<double> l_ordinata;
			//l_ordinata.insert(l_ordinata.end(), m_performanceIndex.begin(), m_performanceIndex.end());

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_oldPerformanceIndex,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printNewPerformanceIndex(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Performance Index";
			std::string l_yLabel = "Performance Index";
			std::string l_xLabel = "Time";

			std::string l_curveLabel = "Index Curve";

			//std::vector<double> l_ascissa;
			//l_ascissa.insert(l_ascissa.end(), m_times.begin(), m_times.end());

			//std::vector<double> l_ordinata;
			//l_ordinata.insert(l_ordinata.end(), m_performanceIndex.begin(), m_performanceIndex.end());

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_performanceIndex,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printNewPerformanceIndexVersusExplorationRate(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Performance Index";
			std::string l_yLabel = "Performance Index";
			std::string l_xLabel = "Experimental Rate";

			std::string l_curveLabel = "Index Curve";

			//std::vector<double> l_ascissa;
			//l_ascissa.insert(l_ascissa.end(), m_times.begin(), m_times.end());

			//std::vector<double> l_ordinata;
			//l_ordinata.insert(l_ordinata.end(), m_performanceIndex.begin(), m_performanceIndex.end());

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_explorationRate,
				m_performanceIndex,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printExplorationRate(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Exploration rate behaviour";
			std::string l_yLabel = "Exploration Rate";
			std::string l_xLabel = "Time";

			std::string l_curveLabel = "Index Curve";

			//std::vector<double> l_ascissa;
			//l_ascissa.insert(l_ascissa.end(), m_times.begin(), m_times.end());

			//std::vector<double> l_ordinata;
			//l_ordinata.insert(l_ordinata.end(), m_performanceIndex.begin(), m_performanceIndex.end());

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_explorationRate,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printBenefit(std::string const& _filename, bool _printOnFile)
		{
			std::string l_yLabel = "Benefit";
			std::string l_title = "Benefit";
			std::string l_xLabel = "Time";

			//std::vector<double> l_ascissa;
			//l_ascissa.insert(l_ascissa.end(), m_times.begin(), m_times.end());

			//std::vector<double> l_ordinata;
			//l_ordinata.insert(l_ordinata.end(), m_benefitValues.begin(), m_benefitValues.end());

			std::string l_curveLabel = "Benefit Curve";

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_benefitValues,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printPotential(std::string const& _filename, bool _printOnFile)
		{
			std::string l_yLabel = "Potential Function";
			std::string l_title = "Potential";
			std::string l_xLabel = "Time";

			//std::vector<double> l_ascissa;
			//l_ascissa.insert(l_ascissa.end(), m_times.begin(), m_times.end());

			//std::vector<double> l_ordinata;
			//l_ordinata.insert(l_ordinata.end(), m_potValues.begin(), m_potValues.end());

			std::string l_curveLabel = "Potential";

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_potValues,
				_printOnFile);
		}
	}
}