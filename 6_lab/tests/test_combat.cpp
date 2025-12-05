#include <gtest/gtest.h>
#include "../include/combat_visitor.h"
#include "../include/arena.h"
#include "../include/factory.h"
#include "../include/console_observer.h"
#include "../include/file_observer.h"
#include <memory>
#include <fstream>

// Combat system tests
TEST(CombatTest, KnightVsKnight) {
    CombatVisitor visitor;
    auto knight1 = NpcFactory::createNpc("Knight", "Knight1", 0, 0);
    auto knight2 = NpcFactory::createNpc("Knight", "Knight2", 10, 10);
    
    // Knight doesn't attack knight
    EXPECT_FALSE(visitor.canKill(knight1.get(), knight2.get()));
    EXPECT_FALSE(visitor.canKill(knight2.get(), knight1.get()));
}

TEST(CombatTest, KnightVsSquirrel) {
    CombatVisitor visitor;
    auto knight = NpcFactory::createNpc("Knight", "Knight1", 0, 0);
    auto squirrel = NpcFactory::createNpc("Squirrel", "Squirrel1", 10, 10);
    
    // Knight kills squirrel
    EXPECT_TRUE(visitor.canKill(knight.get(), squirrel.get()));
    // Squirrel doesn't kill knight
    EXPECT_FALSE(visitor.canKill(squirrel.get(), knight.get()));
}

TEST(CombatTest, SquirrelVsPegasus) {
    CombatVisitor visitor;
    auto squirrel = NpcFactory::createNpc("Squirrel", "Squirrel1", 0, 0);
    auto pegasus = NpcFactory::createNpc("Pegasus", "Pegasus1", 10, 10);
    
    // Squirrel kills pegasus
    EXPECT_TRUE(visitor.canKill(squirrel.get(), pegasus.get()));
    // Pegasus doesn't attack
    EXPECT_FALSE(visitor.canKill(pegasus.get(), squirrel.get()));
}

TEST(CombatTest, PegasusVsAnyone) {
    CombatVisitor visitor;
    auto pegasus = NpcFactory::createNpc("Pegasus", "Pegasus1", 0, 0);
    auto knight = NpcFactory::createNpc("Knight", "Knight1", 10, 10);
    auto squirrel = NpcFactory::createNpc("Squirrel", "Squirrel1", 20, 20);
    
    // Pegasus doesn't attack anyone
    EXPECT_FALSE(visitor.canKill(pegasus.get(), knight.get()));
    EXPECT_FALSE(visitor.canKill(pegasus.get(), squirrel.get()));
    EXPECT_FALSE(visitor.canKill(pegasus.get(), pegasus.get()));
}

TEST(CombatTest, KnightVsPegasus) {
    CombatVisitor visitor;
    auto knight = NpcFactory::createNpc("Knight", "Knight1", 0, 0);
    auto pegasus = NpcFactory::createNpc("Pegasus", "Pegasus1", 10, 10);
    
    // Knight doesn't kill pegasus
    EXPECT_FALSE(visitor.canKill(knight.get(), pegasus.get()));
}

TEST(CombatTest, SquirrelVsKnight) {
    CombatVisitor visitor;
    auto squirrel = NpcFactory::createNpc("Squirrel", "Squirrel1", 0, 0);
    auto knight = NpcFactory::createNpc("Knight", "Knight1", 10, 10);
    
    // Squirrel doesn't attack knight
    EXPECT_FALSE(visitor.canKill(squirrel.get(), knight.get()));
}

TEST(CombatTest, SquirrelVsSquirrel) {
    CombatVisitor visitor;
    auto squirrel1 = NpcFactory::createNpc("Squirrel", "Squirrel1", 0, 0);
    auto squirrel2 = NpcFactory::createNpc("Squirrel", "Squirrel2", 10, 10);
    
    // Squirrel doesn't attack squirrel
    EXPECT_FALSE(visitor.canKill(squirrel1.get(), squirrel2.get()));
}

// Arena battle mode tests
TEST(CombatTest, BattleOutOfRange) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    // NPCs far apart
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 0, 0));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "Squirrel1", 400, 400));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Battle range 100 - NPCs can't reach each other
    arena.startBattle(100.0);
    
    // Both should survive
    EXPECT_EQ(arena.getNpcCount(), 2);
}

TEST(CombatTest, BattleInRange) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    // Knight and squirrel close together
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "Squirrel1", 110, 110));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Battle range 50 meters
    arena.startBattle(50.0);
    
    // Knight kills squirrel
    EXPECT_EQ(arena.getNpcCount(), 1);
}

TEST(CombatTest, ComplexBattle) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    // Create group of NPCs
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "Squirrel1", 110, 110));
    arena.addNpc(NpcFactory::createNpc("Pegasus", "Pegasus1", 105, 105));
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight2", 115, 115));
    
    EXPECT_EQ(arena.getNpcCount(), 4);
    
    // Start battle with large range
    arena.startBattle(100.0);
    
    // Knights kill squirrels, squirrels kill pegasi
    EXPECT_LE(arena.getNpcCount(), 3);
}

TEST(CombatTest, KnightsSurvive) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    // Only knights
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight2", 110, 110));
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight3", 120, 120));
    
    EXPECT_EQ(arena.getNpcCount(), 3);
    
    arena.startBattle(100.0);
    
    // Knights don't attack each other - all survive
    EXPECT_EQ(arena.getNpcCount(), 3);
}

TEST(CombatTest, FileObserverLogging) {
    std::string logfile = "test_combat_log.txt";
    
    Arena arena;
    auto observer = std::make_shared<FileObserver>(logfile);
    arena.addObserver(observer);
    
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "Squirrel1", 110, 110));
    
    arena.startBattle(50.0);
    
    // Check that file was created
    std::ifstream file(logfile);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    bool hasContent = false;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            hasContent = true;
            break;
        }
    }
    file.close();
    
    EXPECT_TRUE(hasContent);
    
    // Remove test file
    std::remove(logfile.c_str());
}
