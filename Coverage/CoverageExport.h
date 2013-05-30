//////////////////////////////////// 
/// CoverageExport.h: definition of export macro
//  Created on:      may-13-2013
//  Original author: s.nardi
//////////////////////////////////// 
#ifdef COVERAGE_EXPORTS
#define COVERAGE_API __declspec(dllexport)
#else
#define COVERAGE_API __declspec(dllimport)
#endif

#define COVERAGEMODULENAME "Coverage"

#pragma warning (disable:4251)
#pragma warning (disable:4275)
#pragma warning (disable:4996)
