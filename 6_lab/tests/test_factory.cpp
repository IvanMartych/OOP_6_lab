#include <gtest/gtest.h>
#include "../include/factory.h"
#include "../include/knight.h"
#include "../include/pegasus.h"
#include "../include/squirrel.h"
#include <memory>

// Factory tests
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

// Тесты загрузки из строки
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
