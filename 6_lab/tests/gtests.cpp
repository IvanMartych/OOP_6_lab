#include <gtest/gtest.h>
#include "../include/arena.h"
#include "../include/factory.h"
#include "../include/combat_visitor.h"
#include "../include/console_observer.h"
#include "../include/file_observer.h"
#include "../include/npc.h"
#include "../include/knight.h"
#include "../include/pegasus.h"
#include "../include/squirrel.h"
#include <memory>
#include <fstream>

// тесты создания npc
TEST(NpcTest, CreateKnight) {
    Knight knight(100, 200, "Lancelot");
    
    EXPECT_EQ(knight.getX(), 100);
    EXPECT_EQ(knight.getY(), 200);
    EXPECT_EQ(knight.getName(), "Lancelot");
    EXPECT_EQ(knight.getType(), "Knight");
}

TEST(NpcTest, CreatePegasus) {
    Pegasus pegasus(150, 250, "Windrunner");
    
    EXPECT_EQ(pegasus.getX(), 150);
    EXPECT_EQ(pegasus.getY(), 250);
    EXPECT_EQ(pegasus.getName(), "Windrunner");
    EXPECT_EQ(pegasus.getType(), "Pegasus");
}

TEST(NpcTest, CreateSquirrel) {
    Squirrel squirrel(50, 75, "Nutty");
    
    EXPECT_EQ(squirrel.getX(), 50);
    EXPECT_EQ(squirrel.getY(), 75);
    EXPECT_EQ(squirrel.getName(), "Nutty");
    EXPECT_EQ(squirrel.getType(), "Squirrel");
}

// Distance calculation tests
TEST(NpcTest, DistanceCalculation) {
    Knight knight1(0, 0, "Knight1");
    Knight knight2(3, 4, "Knight2");
    
    // Distance should be 5 (Pythagorean theorem: 3^2 + 4^2 = 25, sqrt(25) = 5)
    EXPECT_DOUBLE_EQ(knight1.distanceTo(knight2), 5.0);
}

TEST(NpcTest, DistanceSamePosition) {
    Squirrel squirrel1(100, 100, "Squirrel1");
    Squirrel squirrel2(100, 100, "Squirrel2");
    
    EXPECT_DOUBLE_EQ(squirrel1.distanceTo(squirrel2), 0.0);
}

TEST(NpcTest, DistanceSymmetric) {
    Pegasus pegasus(10, 20, "Pegasus1");
    Knight knight(50, 80, "Knight1");
    
    // Distance should be same in both directions
    EXPECT_DOUBLE_EQ(pegasus.distanceTo(knight), knight.distanceTo(pegasus));
}

// Boundary coordinates tests
TEST(NpcTest, BoundaryCoordinates) {
    Knight knight1(0, 0, "MinCorner");
    EXPECT_EQ(knight1.getX(), 0);
    EXPECT_EQ(knight1.getY(), 0);
    
    Knight knight2(500, 500, "MaxCorner");
    EXPECT_EQ(knight2.getX(), 500);
    EXPECT_EQ(knight2.getY(), 500);
}

// тесты фабрики
TEST(FactoryTest, CreateKnight) {
    auto knight = NpcFactory::createNpc("Knight", "TestKnight", 100, 200);
    
    ASSERT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(knight->getName(), "TestKnight");
    EXPECT_EQ(knight->getX(), 100);
    EXPECT_EQ(knight->getY(), 200);
}

TEST(FactoryTest, CreatePegasus) {
    auto pegasus = NpcFactory::createNpc("Pegasus", "TestPegasus", 150, 250);
    
    ASSERT_NE(pegasus, nullptr);
    EXPECT_EQ(pegasus->getType(), "Pegasus");
    EXPECT_EQ(pegasus->getName(), "TestPegasus");
}

TEST(FactoryTest, CreateSquirrel) {
    auto squirrel = NpcFactory::createNpc("Squirrel", "TestSquirrel", 50, 75);
    
    ASSERT_NE(squirrel, nullptr);
    EXPECT_EQ(squirrel->getType(), "Squirrel");
    EXPECT_EQ(squirrel->getName(), "TestSquirrel");
}

TEST(FactoryTest, CreateInvalidType) {
    EXPECT_THROW({
        auto npc = NpcFactory::createNpc("Dragon", "TestDragon", 100, 100);
    }, std::invalid_argument);
}

// РўРµСЃС‚С‹ Р·Р°РіСЂСѓР·РєРё РёР· СЃС‚СЂРѕРєРё
TEST(FactoryTest, CreateFromStringKnight) {
    std::string line = "Knight Lancelot 100 200";
    auto knight = NpcFactory::createFromString(line);
    
    ASSERT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(knight->getName(), "Lancelot");
    EXPECT_EQ(knight->getX(), 100);
    EXPECT_EQ(knight->getY(), 200);
}

TEST(FactoryTest, CreateFromStringPegasus) {
    std::string line = "Pegasus Windrunner 300 400";
    auto pegasus = NpcFactory::createFromString(line);
    
    ASSERT_NE(pegasus, nullptr);
    EXPECT_EQ(pegasus->getType(), "Pegasus");
    EXPECT_EQ(pegasus->getName(), "Windrunner");
    EXPECT_EQ(pegasus->getX(), 300);
    EXPECT_EQ(pegasus->getY(), 400);
}

TEST(FactoryTest, CreateFromStringSquirrel) {
    std::string line = "Squirrel Nutty 50 75";
    auto squirrel = NpcFactory::createFromString(line);
    
    ASSERT_NE(squirrel, nullptr);
    EXPECT_EQ(squirrel->getType(), "Squirrel");
    EXPECT_EQ(squirrel->getName(), "Nutty");
}

TEST(FactoryTest, CreateFromStringInvalidFormat) {
    std::string line = "Knight Lancelot";  // No coordinates
    EXPECT_THROW({
        auto npc = NpcFactory::createFromString(line);
    }, std::runtime_error);
}

TEST(FactoryTest, CreateFromStringInvalidType) {
    std::string line = "Wolf Wolfie 100 200";
    EXPECT_THROW({
        auto npc = NpcFactory::createFromString(line);
    }, std::invalid_argument);
}

TEST(FactoryTest, CreateFromStringEmptyLine) {
    std::string line = "";
    EXPECT_THROW({
        auto npc = NpcFactory::createFromString(line);
    }, std::runtime_error);
}

// тесты арены
TEST(ArenaTest, CreateArena) {
    Arena arena(500, 500);
    EXPECT_EQ(arena.getNpcCount(), 0);
}

TEST(ArenaTest, CreateArenaInvalidSize) {
    EXPECT_THROW({
        Arena arena(600, 600);  // Р‘РѕР»СЊС€Рµ РјР°РєСЃРёРјСѓРјР°
    }, std::out_of_range);
}

TEST(ArenaTest, AddNpc) {
    Arena arena;
    auto Knight = NpcFactory::createNpc("Knight", "Lancelot", 100, 100);
    arena.addNpc(std::move(Knight));
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

TEST(ArenaTest, AddMultipleNpcs) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Pegasus", "Pegasus1", 200, 200));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "Squirrel1", 300, 300));
    
    EXPECT_EQ(arena.getNpcCount(), 3);
}

TEST(ArenaTest, AddNpcDuplicateName) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Knight", "Duplicate", 100, 100));
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Squirrel", "Duplicate", 200, 200));
    }, std::invalid_argument);
}

TEST(ArenaTest, AddNpcOutOfBounds) {
    Arena arena(500, 500);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Knight", "OutKnight", 600, 100));
    }, std::out_of_range);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Knight", "OutKnight2", 100, 600));
    }, std::out_of_range);
    
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Knight", "NegativeKnight", -10, 100));
    }, std::out_of_range);
}

TEST(ArenaTest, ClearArena) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Knight", "Knight1", 100, 100));
    arena.addNpc(NpcFactory::createNpc("Pegasus", "Pegasus1", 200, 200));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    arena.clear();
    EXPECT_EQ(arena.getNpcCount(), 0);
}

// РўРµСЃС‚С‹ СЃРѕС…СЂР°РЅРµРЅРёСЏ/Р·Р°РіСЂСѓР·РєРё
TEST(ArenaTest, SaveToFile) {
    Arena arena;
    
    arena.addNpc(NpcFactory::createNpc("Knight", "Lancelot", 100, 200));
    arena.addNpc(NpcFactory::createNpc("Pegasus", "Vypochka", 150, 250));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "Vyhuhol", 50, 75));
    
    std::string filename = "test_save.txt";
    arena.saveToFile(filename);
    
    // РџСЂРѕРІРµСЂСЏРµРј, С‡С‚Рѕ С„Р°Р№Р» СЃРѕР·РґР°РЅ Рё СЃРѕРґРµСЂР¶РёС‚ РґР°РЅРЅС‹Рµ
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lineCount++;
        }
    }
    file.close();
    
    EXPECT_EQ(lineCount, 3);
    
    // РЈРґР°Р»СЏРµРј С‚РµСЃС‚РѕРІС‹Р№ С„Р°Р№Р»
    std::remove(filename.c_str());
}

TEST(ArenaTest, LoadFromFile) {
    // РЎРѕР·РґР°С‘Рј С‚РµСЃС‚РѕРІС‹Р№ С„Р°Р№Р»
    std::string filename = "test_load.txt";
    std::ofstream outfile(filename);
    outfile << "Knight Lancelot 100 200\n";
    outfile << "Pegasus Vypochka 150 250\n";
    outfile << "Squirrel Vyhuhol 50 75\n";
    outfile.close();
    
    // Р—Р°РіСЂСѓР¶Р°РµРј РёР· С„Р°Р№Р»Р°
    Arena arena;
    arena.loadFromFile(filename);
    
    EXPECT_EQ(arena.getNpcCount(), 3);
    
    // РЈРґР°Р»СЏРµРј С‚РµСЃС‚РѕРІС‹Р№ С„Р°Р№Р»
    std::remove(filename.c_str());
}

TEST(ArenaTest, LoadFromNonExistentFile) {
    Arena arena;
    
    EXPECT_THROW({
        arena.loadFromFile("nonexistent_file.txt");
    }, std::runtime_error);
}

TEST(ArenaTest, SaveAndLoadRoundtrip) {
    std::string filename = "test_roundtrip.txt";
    
    // РЎРѕР·РґР°С‘Рј Р°СЂРµРЅСѓ Рё РґРѕР±Р°РІР»СЏРµРј NPC
    {
        Arena arena1;
        arena1.addNpc(NpcFactory::createNpc("Knight", "Knight1", 111, 222));
        arena1.addNpc(NpcFactory::createNpc("Pegasus", "Pegasus1", 333, 444));
        arena1.saveToFile(filename);
    }
    
    // Р—Р°РіСЂСѓР¶Р°РµРј РІ РЅРѕРІСѓСЋ Р°СЂРµРЅСѓ
    {
        Arena arena2;
        arena2.loadFromFile(filename);
        EXPECT_EQ(arena2.getNpcCount(), 2);
    }
    
    // РЈРґР°Р»СЏРµРј С‚РµСЃС‚РѕРІС‹Р№ С„Р°Р№Р»
    std::remove(filename.c_str());
}

// РўРµСЃС‚С‹ Observer
TEST(ArenaTest, AddObserver) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    
    EXPECT_NO_THROW({
        arena.addObserver(observer);
    });
}

TEST(ArenaTest, AddFileObserver) {
    Arena arena;
    auto observer = std::make_shared<FileObserver>("test_observer.txt");
    
    EXPECT_NO_THROW({
        arena.addObserver(observer);
    });
    
    // РЈРґР°Р»СЏРµРј С‚РµСЃС‚РѕРІС‹Р№ С„Р°Р№Р» РµСЃР»Рё СЃРѕР·РґР°Р»СЃСЏ
    std::remove("test_observer.txt");
}

// тесты боевой системы
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

// тесты загрузки из файла
TEST(FileLoadingTest, LoadRealFileAndVerify) {
    Arena arena;
    
    // Р—Р°РіСЂСѓР¶Р°РµРј С„Р°Р№Р» СЃ С‚РµСЃС‚РѕРІС‹РјРё РґР°РЅРЅС‹РјРё
    ASSERT_NO_THROW({
        arena.loadFromFile("test_data_npcs.txt");
    });
    
    // РџСЂРѕРІРµСЂСЏРµРј РєРѕР»РёС‡РµСЃС‚РІРѕ Р·Р°РіСЂСѓР¶РµРЅРЅС‹С… NPC
    EXPECT_EQ(arena.getNpcCount(), 5);
}

TEST(FileLoadingTest, LoadAndPrintNpcs) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    std::cout << "\n=== Р—Р°РіСЂСѓР·РєР° NPC РёР· С„Р°Р№Р»Р° test_data_npcs.txt ===" << std::endl;
    
    arena.loadFromFile("test_data_npcs.txt");
    
    std::cout << "Р—Р°РіСЂСѓР¶РµРЅРѕ NPC: " << arena.getNpcCount() << std::endl;
    std::cout << "\nРЎРїРёСЃРѕРє Р·Р°РіСЂСѓР¶РµРЅРЅС‹С… NPC:" << std::endl;
    arena.printAllNpcs();
    
    EXPECT_EQ(arena.getNpcCount(), 5);
}

TEST(FileLoadingTest, LoadAndTestBattle) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    // Р—Р°РіСЂСѓР¶Р°РµРј NPC
    arena.loadFromFile("test_data_npcs.txt");
    EXPECT_EQ(arena.getNpcCount(), 5);
    
    std::cout << "\n=== РўРµСЃС‚ Р±РѕСЏ СЃ Р·Р°РіСЂСѓР¶РµРЅРЅС‹РјРё NPC ===" << std::endl;
    std::cout << "РќР°С‡Р°Р»СЊРЅРѕРµ РєРѕР»РёС‡РµСЃС‚РІРѕ NPC: " << arena.getNpcCount() << std::endl;
    
    // Р—Р°РїСѓСЃРєР°РµРј Р±РѕР№ СЃ Р±РѕР»СЊС€РѕР№ РґР°Р»СЊРЅРѕСЃС‚СЊСЋ
    std::cout << "\nР—Р°РїСѓСЃРє Р±РѕСЏ СЃ РґР°Р»СЊРЅРѕСЃС‚СЊСЋ 200 РјРµС‚СЂРѕРІ..." << std::endl;
    arena.startBattle(200.0);
    
    std::cout << "РљРѕР»РёС‡РµСЃС‚РІРѕ РІС‹Р¶РёРІС€РёС…: " << arena.getNpcCount() << std::endl;
    std::cout << "\nР’С‹Р¶РёРІС€РёРµ:" << std::endl;
    arena.printAllNpcs();
    
    // РџРѕСЃР»Рµ Р±РѕСЏ РґРѕР»Р¶РЅРѕ РѕСЃС‚Р°С‚СЊСЃСЏ РјРµРЅСЊС€Рµ NPC
    EXPECT_LT(arena.getNpcCount(), 5);
}

TEST(FileLoadingTest, LoadSaveAndReload) {
    std::string tempFile = "temp_test_file.txt";
    
    // РџРµСЂРІР°СЏ Р°СЂРµРЅР° - Р·Р°РіСЂСѓР¶Р°РµРј Рё СЃРѕС…СЂР°РЅСЏРµРј
    {
        Arena arena1;
        arena1.loadFromFile("test_data_npcs.txt");
        size_t originalCount = arena1.getNpcCount();
        
        EXPECT_EQ(originalCount, 5);
        
        // РЎРѕС…СЂР°РЅСЏРµРј РІ РґСЂСѓРіРѕР№ С„Р°Р№Р»
        arena1.saveToFile(tempFile);
    }
    
    // Р’С‚РѕСЂР°СЏ Р°СЂРµРЅР° - Р·Р°РіСЂСѓР¶Р°РµРј РёР· СЃРѕС…СЂР°РЅС‘РЅРЅРѕРіРѕ С„Р°Р№Р»Р°
    {
        Arena arena2;
        arena2.loadFromFile(tempFile);
        
        EXPECT_EQ(arena2.getNpcCount(), 5);
        
        std::cout << "\n=== РџСЂРѕРІРµСЂРєР° СЃРѕС…СЂР°РЅРµРЅРёСЏ/Р·Р°РіСЂСѓР·РєРё ===" << std::endl;
        std::cout << "Р”Р°РЅРЅС‹Рµ СѓСЃРїРµС€РЅРѕ СЃРѕС…СЂР°РЅРµРЅС‹ Рё Р·Р°РіСЂСѓР¶РµРЅС‹ РѕР±СЂР°С‚РЅРѕ!" << std::endl;
        arena2.printAllNpcs();
    }
    
    // РЈРґР°Р»СЏРµРј РІСЂРµРјРµРЅРЅС‹Р№ С„Р°Р№Р»
    std::remove(tempFile.c_str());
}

TEST(FileLoadingTest, VerifySpecificNpcData) {
    Arena arena;
    arena.loadFromFile("test_data_npcs.txt");
    
    // РџСЂРѕРІРµСЂСЏРµРј С‡С‚Рѕ РІСЃРµ 5 NPC Р·Р°РіСЂСѓР·РёР»РёСЃСЊ
    EXPECT_EQ(arena.getNpcCount(), 5);
    
    // РЎРѕР·РґР°С‘Рј NPC СЃ РґСѓР±Р»РёРєР°С‚РѕРј РёРјРµРЅРё (Lancelot СѓР¶Рµ СЃСѓС‰РµСЃС‚РІСѓРµС‚) - РґРѕР»Р¶РЅР° РІС‹Р±СЂРѕСЃРёС‚СЊ РёСЃРєР»СЋС‡РµРЅРёРµ
    EXPECT_THROW({
        arena.addNpc(NpcFactory::createNpc("Knight", "Lancelot", 300, 300));
    }, std::invalid_argument);
}

TEST(FileLoadingTest, BattleWithLoadedNpcs) {
    Arena arena;
    auto observer = std::make_shared<ConsoleObserver>();
    arena.addObserver(observer);
    
    arena.loadFromFile("test_data_npcs.txt");
    
    std::cout << "\n=== Р”РµС‚Р°Р»СЊРЅС‹Р№ С‚РµСЃС‚ Р±РѕСЏ ===" << std::endl;
    std::cout << "Р—Р°РіСЂСѓР¶РµРЅРѕ NPC: " << arena.getNpcCount() << std::endl;
    
    // Mishka (100,200) Рё Vyhuhol (50,75) - СЂР°СЃСЃС‚РѕСЏРЅРёРµ ~136 РјРµС‚СЂРѕРІ
    // Р’РёРїРѕС‡РєР° (150,250) Р±Р»РёР·РєРѕ Рє РњРёС€РєРµ - ~71 РјРµС‚СЂ
    
    std::cout << "\nР‘РѕР№ СЃ РґР°Р»СЊРЅРѕСЃС‚СЊСЋ 100 РјРµС‚СЂРѕРІ (С‚РѕР»СЊРєРѕ Р±Р»РёР·РєРёРµ NPC):" << std::endl;
    arena.startBattle(100.0);
    
    std::cout << "\nР’С‹Р¶РёРІС€РёРµ:" << std::endl;
    arena.printAllNpcs();
    std::cout << "РћСЃС‚Р°Р»РѕСЃСЊ NPC: " << arena.getNpcCount() << std::endl;
    
    EXPECT_GE(arena.getNpcCount(), 1);  // РҐРѕС‚СЏ Р±С‹ РєС‚Рѕ-С‚Рѕ РґРѕР»Р¶РµРЅ РІС‹Р¶РёС‚СЊ
}
