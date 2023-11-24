#pragma once

#include <iostream>

#define LOG(module) std::cout << __FILE__ << "[" << __LINE__ << "]" << "[" << #module << "] "
