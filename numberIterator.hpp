#ifndef NUMBER_ITERATOR
#define NUMBER_ITERATOR

namespace
{
	class NumberIterator {
	public:
		using value_type = int;
		NumberIterator(value_type num) : _number(num), _size(alg::get_nr_cif(num)) { }

		value_type begin() { return  }

	private:
		value_type _number;
		value_type _size;
	};

}


#endif // ! NUMBER_ITERATOR
