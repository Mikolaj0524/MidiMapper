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
        std::ifstream inFile(configName);

        if (inFile.is_open()) {
            try {
                json j;
                inFile >> j;

                config = j.get<std::vector<std::pair<Shortcut, Shortcut>>>();
                inFile.close();
                return;
            }
            catch (const json::exception& e) {
                std::cerr << "Blad parsowania pliku config: " << e.what() << " Generuje domyslny.\n";
            }
        }

        config.clear();
        for (int i = 0; i < 25; i++) {
            config.push_back(std::make_pair(Shortcut(), Shortcut()));
        }
    }

    void Save() {
        std::ofstream outFile(configName);
        if (outFile.is_open()) {
            json j = config;

            outFile << j.dump(4);
            outFile.close();
        }
        else {
            std::cerr << "Nie udalo sie otworzyc pliku do zapisu konfiguracji!\n";
        }
    }
}