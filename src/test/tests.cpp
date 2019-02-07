#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "core/boilerplate.h"
#include "core/sigil.h"

// Boilerplate& boileplate = Boilerplate::instance(); //important to create one to initialize dependencies;

// TEST_CASE("sigil width and height"){
//     SECTION( "width and height of bar is 4 and 1" ) {
//         Sigil s = Sigil(Sigil::blocks_Bar, glm::ivec2(0,0));
//         REQUIRE( s.width() == 4 * Sigil::SCALE);
//         REQUIRE( s.height() == 1 * Sigil::SCALE);
//     }
//     SECTION( "width and height of box is 2 and 2" ) {
//         Sigil s = Sigil(Sigil::blocks_Box, glm::ivec2(0,0));
//         REQUIRE( s.width() == 2 * Sigil::SCALE);
//         REQUIRE( s.height() == 2 * Sigil::SCALE);
//     }
//     SECTION( "width and height of L is 3 and 2" ) {
//         Sigil s = Sigil(Sigil::blocks_L, glm::ivec2(0,0));
//         REQUIRE( s.width() == 3 * Sigil::SCALE);
//         REQUIRE( s.height() == 2 * Sigil::SCALE);
//     }
//     SECTION( "width and height of LR is 3 and 2" ) {
//         Sigil s = Sigil(Sigil::blocks_LR, glm::ivec2(0,0));
//         REQUIRE( s.width() == 3 * Sigil::SCALE);
//         REQUIRE( s.height() == 2 * Sigil::SCALE);
//     }
//     SECTION( "width and height of S is 3 and 2" ) {
//         Sigil s = Sigil(Sigil::blocks_S, glm::ivec2(0,0));
//         REQUIRE( s.width() == 3 * Sigil::SCALE);
//         REQUIRE( s.height() == 2 * Sigil::SCALE);
//     }
//     SECTION( "width and height of SR is 3 and 2" ) {
//         Sigil s = Sigil(Sigil::blocks_SR, glm::ivec2(0,0));
//         REQUIRE( s.width() == 3 * Sigil::SCALE);
//         REQUIRE( s.height() == 2 * Sigil::SCALE);
//     }
// }