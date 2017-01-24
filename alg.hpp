#pragma once
#include <type_traits>
#include <cmath>
#include <vector>
//#define constexpr

///////////////////////////////////////////////////////////////////////////
///////////////// FUNCTII SCRISE DE BARA CONSTANTIN TUDOR /////////////////
///////////////////////////////////////////////////////////////////////////


namespace alg
{
	size_t constexpr get_nr_cif(size_t) noexcept;

	namespace detail
	{

		template <typename Iter_t>
		typename std::iterator_traits<Iter_t>::value_type
		_SumImpl(Iter_t li, Iter_t ls)
		{
			if (li + 1 != ls) {
				Iter_t mijl = li + ((ls - li) / 2);
				return (_SumImpl(li, mijl) + _SumImpl(mijl, ls));
			}
			else {
				return *li;
			}
		}

		template <typename T>
		inline auto
		SumDeI(const T& c)
		{
			return (_SumImpl(std::begin(c), std::end(c)));
		}

		constexpr double
		calcPIimpl(double x, double r1, double r2) noexcept
		{
			if (x > 0)
				return (4 / r1) - (4 / r2) + calcPIimpl(x - 1, r1 + 4, r2 + 4);
			else
				return (4 / r1) - (4 / r2);
		}

		constexpr double
		calcPI(double x) noexcept
		{
			return calcPIimpl(x, 1, 3);
		}
		/*
		size_t constexpr
		dubleaza_cifra(size_t nr, size_t cifra) noexcept
		{
			size_t nrCifre = std::pow(10, get_nr_cif(nr));
			for (size_t poz = 1; poz < nrCifre; poz *= 10)
				if (nr / poz % 10 == cifra) {
					poz *= 10;
					nr = nr%poz + cifra*poz + (nr / poz)*poz * 10;
				}
			return nr;
		}*/

	} // NAMESPACE DETAIL

	//*//////////////////////////////////////////////////
	///////////////// PRELUCRARE NUMERE /////////////////
	/////////////////////////////////////////////////////

	size_t constexpr
	get_oglindit(size_t nr) noexcept
	{
		size_t ogl = 0;
		while (nr > 0) {
			ogl = ogl * 10 + nr % 10;
			nr /= 10;
		}
		return ogl;
	}

	size_t constexpr
	get_cmmdc(size_t nr1, size_t nr2) noexcept
	{
		while (nr1 != nr2 && nr1*nr2 != 0)
			if (nr1 > nr2)
				nr1 -= nr2;
			else
				nr2 -= nr1;
		return nr1;
	}

	size_t constexpr
	get_cmmmc(size_t nr1, size_t nr2) noexcept
	{
		return (nr1 * nr2) / get_cmmdc(nr1, nr2);
	}

	size_t constexpr
	get_factorial(size_t nr) noexcept
	{
		size_t fac = 1;
		for (size_t i = 1; i <= nr; i++)
			fac *= i;
		return fac;
	}

	size_t constexpr
	get_sum_cif(size_t nr) noexcept
	{
		size_t s = 0;
		while (nr > 0) {
			s += nr % 10;
			nr /= 10;
		}
		return s;
	}

	size_t constexpr
	get_prod_cif(size_t nr) noexcept
	{
		size_t p = 1;
		while (nr > 0) {
			p *= nr % 10;
			nr /= 10;
		}
		return p;
	}

	size_t constexpr
	get_nr_fac_primi(size_t nr) noexcept
	{
		size_t nrFacPrimi = 0;
		size_t div = 2;
		while (nr > 1)
			if (nr % div == 0) {
				nrFacPrimi++;
				while (nr % div == 0)
					nr /= div;
			}
			else
				div++;
		return nrFacPrimi;
	}

	std::vector<int>
	get_fac_primi(size_t nr)
	{
		size_t div = 2;
		std::vector<int> facPrimi;
		while (nr > 1)
			if (nr % div == 0) {
				facPrimi.push_back(div);
				nr /= div;
			}
			else
				++div;
		return facPrimi;
	}

	size_t constexpr
	get_nr_cif(size_t nr) noexcept
	{
		size_t nrCif = 0;
		while (nr != 0) {
			nrCif++;
			nr /= 10;
		}
		return nrCif;
	}

	size_t constexpr
	get_nr_div(size_t nr) noexcept
	{
		size_t nrDiv = 0;
		for (size_t d = 1; d <= nr; ++d)
			if (nr % d == 0)
				nrDiv++;
		return nrDiv;
	}

	size_t constexpr
	get_aprox_nr(bool conditie(size_t), size_t nr) noexcept
	{
		if (conditie(nr))
			return nr;
		auto li = nr - 1;
		auto ls = nr + 1;
		while (!conditie(li) && !conditie(ls)) {
			li--;
			ls++;
		}
		if (conditie(li))
			return li;
		else
			return ls;
	};


	//////////////////////////////////////////////////////////
	///////////////// VERIFICARE DE CONDITII /////////////////
	//////////////////////////////////////////////////////////

	bool constexpr inline
	is_zero(bool b) noexcept
	{
		return b;
	}

	bool constexpr inline
	is_prim(size_t nr)
	{
		//int nr = std::abs(Nr);
		if (nr <= 1)
			return false;
		for (size_t d = 2; d*d <= nr; ++d)
			if (nr % d == 0)
				return false;
		return true;
	}

	bool constexpr inline
	is_par(size_t nr) noexcept
	{
		return (nr % 2 == 0);
	}

	bool constexpr inline
	is_impar(size_t nr) noexcept
	{
		return (nr % 2 != 0);
	}

	bool constexpr inline
	is_palindrom(size_t nr) noexcept
	{
		return(nr == get_oglindit(nr));
	}

	bool constexpr inline
	is_factorial(size_t nr) noexcept
	{
		size_t fac = 0;
		while (nr > get_factorial(fac))
			fac++;
		return (nr == get_factorial(fac));
	}

	bool constexpr inline
	is_patrat(size_t nr) noexcept
	{
		for (size_t patrat = 1; nr > patrat*patrat; ++patrat)
			if (nr == patrat*patrat)
				return true;
		return false;
	}

	bool constexpr inline
	is_cub(size_t nr) noexcept
	{
		for (size_t cub = 1; nr > cub*cub*cub; ++cub)
			if (nr == cub*cub*cub)
				return true;
		return false;
	}
	/*
		bool constexpr inline
		is_pow(size_t nr, uint8_t putere) noexcept
		{
			size_t p = 1;
			while(nr > std::pow(p,putere))
				++p;
			return (nr==std::pow(p,putere));
		}

		//*/////////////////////////////////////////
		///////////////// FUNCTORI /////////////////
		////////////////////////////////////////////


	template <typename T>
	auto add(T nr)
	{
		return [nr](auto x) { return  x + nr; };
	}

	template <typename T>
	auto diff(T nr)
	{
		return [nr](auto x) { return  x - nr; };
	}

	template <typename T>
	auto div(T nr)
	{
		return [nr](auto x) { return  x / nr; };
	}

	template <typename T>
	auto mult(T nr)
	{
		return [nr](auto x) { return  x * nr; };
	}

}; //  NAMESPACE ALG

namespace std
{
	template <typename container, typename F>
	const container& for_each(container& c, F&& f)
	{
		std::for_each(begin(c), end(c), forward<F>(f));
		return c;
	}

} // NAMESPACE STD

#undef constexpr
