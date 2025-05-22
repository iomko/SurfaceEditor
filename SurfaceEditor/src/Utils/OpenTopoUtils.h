#pragma once
#include <string>
#include "../Params/OperationParams.h"

namespace utils::opentopo
{
	inline std::string buildUrl(const OpenTopoParams& input)
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
