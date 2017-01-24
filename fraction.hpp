#ifndef FRACTION_H_INCLUDES
#define FRACTION_H_INCLUDES

#include "alg.hpp"

namespace
{
	class Fraction {
	public:
		using value_type = int;

		explicit Fraction() : _numarator(1), _numitor(1) { }

		explicit Fraction(value_type numarator) : _numarator(numarator), _numitor(1) { }
		
		explicit Fraction(value_type numarator, value_type numitor)
		{
			value_type divizorComun = alg::get_cmmdc(numarator, numitor);
			_numarator = numarator / divizorComun;
			_numitor = numitor / divizorComun;
		}


		value_type numarator() const { return this->_numarator; }
		value_type numitor() const { return this->_numitor; }

		friend std::istream& operator >> (std::istream&, Fraction&);
	private:
		value_type _numarator;
		value_type _numitor;
	}; // class Fraction


	Fraction operator + (const Fraction& fractie1, const Fraction& fractie2) noexcept
	{
		return Fraction(fractie1.numarator() * fractie2.numitor() + fractie2.numarator() * fractie1.numitor(),
						fractie1.numitor() * fractie2.numitor());
	}

	Fraction operator - (const Fraction& fractie1, const Fraction& fractie2) noexcept
	{
		return Fraction(fractie1.numarator() * fractie2.numitor() - fractie2.numarator() * fractie1.numitor(),
						fractie1.numitor() * fractie2.numitor());
	}

	Fraction operator * (const Fraction& fractie1, const Fraction& fractie2) noexcept
	{
		return Fraction(fractie1.numarator() * fractie2.numarator(),
						fractie2.numitor() * fractie2.numitor());
	}

	Fraction operator / (const Fraction& fractie1, const Fraction& fractie2) noexcept
	{
		return Fraction(fractie1.numarator() * fractie2.numitor(),
						fractie2.numitor() * fractie2.numarator());
	}

	std::ostream& operator << (std::ostream& stream, const Fraction& fractie)
	{
		return stream << fractie.numarator() << "/" << fractie.numitor();
	}

	std::istream& operator >> (std::istream& stream, Fraction& fractie)
	{
		char caracterDeSeparat;
		stream >> fractie._numarator >> caracterDeSeparat >> fractie._numitor;
		if (caracterDeSeparat == '/') {
			return stream;
		} else {
			std::cout << " Eroare in citirea fractiei, citirea trebuie facuta cu separatorul ""/""\n";
		}
	}

}

#endif // !FRACTION_H_INCLUDES