#pragma once
#include "graphicSettings.h"
#include "dnnSettings.h"
#include "dnnMathSettings.h"

constexpr graphsettings_0_t GetGraphicSettings()
{
	return graphsettings;
}

constexpr dnnmathsettings_0_t GetDNNMathSettings()
{
	return dnnmathsettings;
}

constexpr dnnsettings_0_t GetDNNSettings()
{
	return dnnsettings;
}
