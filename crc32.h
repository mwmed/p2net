#pragma once
#include <Windows.h>
#include <iostream>
#include "c_array.h"









namespace Util
{
	std::uint32_t ComputeHash(p2net::core::data::c_array Message);
	std::uint32_t ComputeHash(std::string Message);
}