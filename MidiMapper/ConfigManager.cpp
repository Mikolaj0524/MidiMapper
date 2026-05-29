#include "ConfigManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include "Shortcut.h"

using json = nlohmann::json;

namespace ConfigManager {

    std::vector<std::pair<Shortcut, Shortcut>> config;
    const std::string configName = "config.json";

    void Load() {
        std::ifstream file(configName);

        if (file.is_open()) {
            try {
                json j;
                file >> j;
                config = j.get<std::vector<std::pair<Shortcut, Shortcut>>>();
                file.close();
                return;
            }
            catch (const json::exception& e) {}
        }

        config.clear();
        for (int i = 0; i < 25; i++) {
            config.push_back(std::make_pair(Shortcut(), Shortcut()));
        }
    }

    void Save() {
        std::ofstream file(configName);

        if (file.is_open()) {
            json j = config;
            file << j.dump(4);
            file.close();
        }
    }
}