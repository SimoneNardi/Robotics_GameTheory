#include "Statistics.h"
#include "IDSMath.h"
#include <iostream>

#include "IDSGraphicsC.h"

namespace Robotics
{
	namespace GameTheory 
	{
		//////////////////////////////////////////////////////////////////////////
		const int g_medianNumberObject = 100;
		const int g_steadyValueCompared = 10;
		//////////////////////////////////////////////////////////////////////////
		void computeSteadyValue(std::vector<double> & _result_container, double& _steadyValue , int &_steadyIndex)
		{
			_steadyIndex = _result_container.size();
			_steadyValue = 0.;
			for(int i = 0; i < _result_container.size(); ++i)
			{
				if (_steadyValue < _result_container[i])
					_steadyValue = _result_container[i];
			}

			for(int j = 0; j < _result_container.size(); ++j)
			{
				double l_steadyValuePartialMean=0;
				int num = 0;
				for(int i = 0; i < g_steadyValueCompared; ++i)
				{
					int index = i+j;
					if(index >= _result_container.size())
						break;

					num++;
					l_steadyValuePartialMean += _result_container[index];

				}
				l_steadyValuePartialMean /= double(num);

				if(l_steadyValuePartialMean > _steadyValue * .95)
				{
					_steadyIndex = j;
					break;
				}
			}
		}

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
			m_potentialIndex.clear();
			m_benefitIndex.clear();
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
			m_potentialIndex.push_back(potValue/NonCooperativeSteadyValue);
			m_benefitIndex.push_back( ( maxBenefitValue-benefitValue ) / maxBenefitValue );
			m_explorationRate.push_back(_explorationRate);
		}
		
		//////////////////////////////////////////////////////////////////////////
		//	Benefit Value
		double Statistics::getBenefitIndexMediumValue()
		{
			double tot = 0;
			for(size_t i = 0; i < m_benefitIndex.size(); ++i)
				tot+= m_benefitIndex[i];

			double l_value = 1.;
			if(m_benefitIndex.size() != 0)
				l_value = tot / double(m_benefitIndex.size());
			else
				std::cout << "NoData" << "\t";

			if(fabs(l_value) < IDSMath::TOLERANCE2)
				l_value = 0;

			//std::cout << "Value: " << l_value << "\t";
			return l_value;
		}


		//////////////////////////////////////////////////////////////////////////
		//	Steady Benefit Value
		double Statistics::getBenefitIndexSteadyValue()
		{
			double l_steadyValue;
			int l_steadyIndex;
			computeSteadyValue(m_benefitIndex, l_steadyValue, l_steadyIndex);

			return l_steadyValue;
		}

		//////////////////////////////////////////////////////////////////////////
		//	Non Cooperative Index
		double Statistics::getPotentialIndexMediumValue()
		{
			double tot = 0;
			for(size_t i = 0; i < m_potentialIndex.size(); ++i)
				tot+= m_potentialIndex[i];

			double l_value = 1.;
			if(m_potentialIndex.size() != 0)
				l_value = tot / double(m_potentialIndex.size());
			else
				std::cout << "NoData" << "\t";

			if(fabs(l_value) < IDSMath::TOLERANCE2)
				l_value = 0;

			//std::cout << "Value: " << l_value << "\t";
			return l_value;
		}

		//////////////////////////////////////////////////////////////////////////
		//	Steady Benefit Value
		double Statistics::getPotentialIndexSteadyValue()
		{
			double l_steadyValue;
			int l_steadyIndex;
			computeSteadyValue(m_potentialIndex, l_steadyValue, l_steadyIndex);

			return l_steadyValue;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

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
		void Statistics::printPotentialIndex(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Potential Index";
			std::string l_yLabel = "Potential Index";
			std::string l_xLabel = "Time";

			std::string l_curveLabel = "Old Index Curve";

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_potentialIndex,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printBenefitIndex(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Benefit Index";
			std::string l_yLabel = "Benefit Index";
			std::string l_xLabel = "Time";

			std::string l_curveLabel = "Index Curve";

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_times,
				m_benefitIndex,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printPotentialIndexVersusExplorationRate(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Potential Index";
			std::string l_yLabel = "Potential Index";
			std::string l_xLabel = "Experimental Rate";

			std::string l_curveLabel = "Index Curve";

			printGraph(
				_filename, 
				l_title, l_xLabel, l_yLabel, l_curveLabel, 
				m_explorationRate,
				m_potentialIndex,
				_printOnFile);
		}

		//////////////////////////////////////////////////////////////////////////
		void Statistics::printExplorationRate(std::string const& _filename, bool _printOnFile)
		{
			std::string l_title = "Exploration rate behaviour";
			std::string l_yLabel = "Exploration Rate";
			std::string l_xLabel = "Time";

			std::string l_curveLabel = "Index Curve";

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