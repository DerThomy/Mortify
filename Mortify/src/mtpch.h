#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <utility>
#include <sstream>
#include <algorithm>
#include <optional>

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include "Mortify/Core/Log.h"
#include "Mortify/Debug/Instrumentor.h"

#ifdef MT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif