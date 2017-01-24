#ifndef _BCT_LIB_INCLUDE
#define _BCT_LIB_INCLUDE


#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <list>
#include <map>

#define TIMES(n) \
    for(int i = 0; i <= n; ++i)

#define FOR(init,n) \
    for(int i = init; i <= n; ++i)

#define END_MAIN std::cout << "\n"; system("pause"); return 0;

// namespace traits, util, fc, alg, bct
#include "traits.hpp"
#include "utility.hpp"
#include "functional.hpp"
#include "alg.hpp"
#include "io.hpp"
#include "matrix.hpp"
#include "list.hpp"
#include "fraction.hpp"
//#include "numberIterator.hpp"

template<typename> struct cout_compile;

template <typename T>
void print(T t)
{
	std::cout << " " << t;
}

template <typename F>
void DEBUG_FUNCTION(F f)
{
	while (true) {
		size_t nr;
		std::cout << "\n ";
		std::cin >> nr;
		std::cout << "\n nr = " << f(nr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		system("pause");
		system("cls");
	}
}

#endif // BCTLIB

