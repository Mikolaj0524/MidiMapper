#pragma once

#include <vector>
#include "Shortcut.h"

namespace ConfigManager {
	extern std::vector<std::pair<Shortcut, Shortcut>> config;
	void Load();
	void Save();
}