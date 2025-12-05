#include "include/arena.h"
#include "include/factory.h"
#include "include/console_observer.h"
#include "include/file_observer.h"
#include <iostream>
#include <memory>

int main() {
    try {
        std::cout << "=== Balagur Fate 3 - RPG Arena ===" << std::endl;
        std::cout << std::endl;

        Arena arena(500, 500);

        auto consoleObserver = std::make_shared<ConsoleObserver>();
        auto fileObserver = std::make_shared<FileObserver>("battle_log.txt");
        
        arena.addObserver(consoleObserver);
        arena.addObserver(fileObserver);

        std::cout << "1. Spawning NPCs..." << std::endl;

        arena.createAndAddNpc("Knight", "Lancelot", 100, 100);
        arena.createAndAddNpc("Knight", "Arthur", 150, 150);
        arena.createAndAddNpc("Pegasus", "Windrunner", 120, 120);
        arena.createAndAddNpc("Squirrel", "Nutty", 110, 110);
        arena.createAndAddNpc("Squirrel", "Chipper", 200, 200);
        std::cout << "Total NPCs created: " << arena.getNpcCount() << std::endl;
        std::cout << std::endl;

        std::cout << "2. Current NPCs in arena:" << std::endl;
        arena.printAllNpcs();
        std::cout << std::endl;

        std::cout << "3. Saving NPCs to file 'npcs_data.txt'..." << std::endl;
        arena.saveToFile("npcs_data.txt");
        std::cout << "Saved successfully!" << std::endl;
        std::cout << std::endl;

        std::cout << "4. Starting combat mode (range: 100 meters)..." << std::endl;
        std::cout << "--- BATTLE BEGINS ---" << std::endl;
        arena.startBattle(100.0);
        std::cout << "--- BATTLE ENDS ---" << std::endl;
        std::cout << std::endl;

        std::cout << "5. Survivors list:" << std::endl;
        arena.printAllNpcs();
        std::cout << "NPCs remaining: " << arena.getNpcCount() << std::endl;
        std::cout << std::endl;

        std::cout << "6. Clearing arena and reloading from file..." << std::endl;
        arena.clear();
        arena.loadFromFile("npcs_data.txt");
        std::cout << "Loaded NPCs: " << arena.getNpcCount() << std::endl;
        arena.printAllNpcs();
        std::cout << std::endl;

        std::cout << "=== Program completed successfully ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
