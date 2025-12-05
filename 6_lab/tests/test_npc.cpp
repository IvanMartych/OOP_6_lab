#include <gtest/gtest.h>
#include "../include/npc.h"
#include "../include/knight.h"
#include "../include/pegasus.h"
#include "../include/squirrel.h"
#include <memory>

// NPC creation tests
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
