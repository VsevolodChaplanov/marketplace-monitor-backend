#include <iostream>

import Utility;

using utility::operator""_fs;

auto main() -> int {
    constexpr auto hello_world = "hello world"_fs;
    std::cout << hello_world.view() << std::endl;
    return 0;
}
