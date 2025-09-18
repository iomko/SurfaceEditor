#pragma once
#include <string>
#include <sstream>
#include "../Params/OperationParams.h"

namespace utils::third_api
{
	inline std::string buildOpenTopoUrl(const OpenTopoParams& input)
	{
		std::ostringstream oss;
		oss << "https://portal.opentopography.org/API/globaldem?"
			<< "demtype=NASADEM"
			<< "&south=" << input.m_lowerLeftLat
			<< "&north=" << input.m_upperRightLat
			<< "&west=" << input.m_lowerLeftLon
			<< "&east=" << input.m_upperRightLon
			<< "&outputFormat=GTiff"
			<< "&API_Key=" << input.m_apiKey;

		return oss.str();
	}
}
