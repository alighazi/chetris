#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "core/boilerplate.h"
#include "core/sigil.h"

Boilerplate& boileplate = Boilerplate::instance(); //important to create one to initialize dependencies;

TEST_CASE("sigil width and height"){
    SECTION( "width and height of bar is 4 and 1" ) {
        Sigil s = Sigil(Sigil::blocks_Bar, glm::ivec2(0,0));
        REQUIRE( s.bounds()->width() == 4 * Sigil::SCALE);
        REQUIRE( s.bounds()->height() == 1 * Sigil::SCALE);
    }
    SECTION( "width and height of box is 2 and 2" ) {
        Sigil s = Sigil(Sigil::blocks_Box, glm::ivec2(0,0));
        REQUIRE( s.bounds()->width() == 2 * Sigil::SCALE);
        REQUIRE( s.bounds()->height() == 2 * Sigil::SCALE);
    }
    SECTION( "width and height of L is 3 and 2" ) {
        Sigil s = Sigil(Sigil::blocks_L, glm::ivec2(0,0));
        REQUIRE( s.bounds()->width() == 3 * Sigil::SCALE);
        REQUIRE( s.bounds()->height() == 2 * Sigil::SCALE);
    }
    SECTION( "width and height of LR is 3 and 2" ) {
        Sigil s = Sigil(Sigil::blocks_LR, glm::ivec2(0,0));
        REQUIRE( s.bounds()->width() == 3 * Sigil::SCALE);
        REQUIRE( s.bounds()->height() == 2 * Sigil::SCALE);
    }
    SECTION( "width and height of S is 3 and 2" ) {
        Sigil s = Sigil(Sigil::blocks_S, glm::ivec2(0,0));
        REQUIRE( s.bounds()->width() == 3 * Sigil::SCALE);
        REQUIRE( s.bounds()->height() == 2 * Sigil::SCALE);
    }
    SECTION( "width and height of SR is 3 and 2" ) {
        Sigil s = Sigil(Sigil::blocks_SR, glm::ivec2(0,0));
        REQUIRE( s.bounds()->width() == 3 * Sigil::SCALE);
        REQUIRE( s.bounds()->height() == 2 * Sigil::SCALE);
    }
}

TEST_CASE("iRect Collision"){
    iRect r1 = iRect(glm::ivec2(10,10), 4, 4);

    SECTION( "With another at top (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(10,15), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at top (touching)" ) {
        iRect r2 = iRect(glm::ivec2(10,14), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at top (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(10,13), 4, 4);
        REQUIRE(r1.collides(r2));
    }
    SECTION( "With another at bottom (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(10,5), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at bottom (touching)" ) {
        iRect r2 = iRect(glm::ivec2(10,6), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at bottom (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(10,7), 4, 4);
        REQUIRE(r1.collides(r2));
    }
    SECTION( "With another at left (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(5,10), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at left (touching)" ) {
        iRect r2 = iRect(glm::ivec2(6,10), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at left (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(7,10), 4, 4);
        REQUIRE(r1.collides(r2));
    }
    SECTION( "With another at right (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(15,10), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at right (touching)" ) {
        iRect r2 = iRect(glm::ivec2(14,10), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at right (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(13,10), 4, 4);
        REQUIRE(r1.collides(r2));
    }
    SECTION( "With another at top right (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(15,15), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at top right (touching)" ) {
        iRect r2 = iRect(glm::ivec2(14,14), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at top right (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(13,13), 4, 4);
        REQUIRE(r1.collides(r2));
    }
    SECTION( "With another at bottom right (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(15,5), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at bottom right (touching)" ) {
        iRect r2 = iRect(glm::ivec2(14,6), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at bottom right (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(13,7), 4, 4);
        REQUIRE(r1.collides(r2));
    }

    SECTION( "With another at top left (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(5,15), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at top left (touching)" ) {
        iRect r2 = iRect(glm::ivec2(6,14), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at top left (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(7,13), 4, 4);
        REQUIRE(r1.collides(r2));
    }

    SECTION( "With another at bottom left (no collision)" ) {
        iRect r2 = iRect(glm::ivec2(5,5), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at bottom left (touching)" ) {
        iRect r2 = iRect(glm::ivec2(6,6), 4, 4);
        REQUIRE(r1.collides(r2) == false);
    }
    SECTION( "With another at bottom left (colliding)" ) {
        iRect r2 = iRect(glm::ivec2(7,7), 4, 4);
        REQUIRE(r1.collides(r2));
    }
}
