#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
template <typename>
class Matrix;

namespace detail {
template <typename T>
class MatrixIter {
    using iterator_row = typename Matrix<T>::iterator_row;
    using iterator_col = typename Matrix<T>::iterator_col;
    using self = MatrixIter<T>;
    iterator_row row_;
    iterator_col curr_;

   public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    MatrixIter() = default;
    MatrixIter(iterator_row Rit, iterator_col Cit) : row_(Rit), curr_(Cit) {}

    MatrixIter& operator=(const self&) = default;
    MatrixIter(const self&) = default;

    reference operator*() { return *curr_; }

    iterator_row& row() noexcept { return row_; }

    iterator_col& curr() noexcept { return curr_; }

    friend bool operator==(const self& lhs, const self& rhs) {
        return (lhs.curr_ == rhs.curr_ && lhs.row_ == rhs.row_);
    }

    friend bool operator!=(const self& lhs, const self& rhs) {
        return (lhs.curr_ != rhs.curr_ && lhs.row_ != rhs.row_);
    }

    self& operator++() {
        if (curr_ != row_.end() - 1) {
            ++curr_;
            return *this;
        } else {
            ++row_;
            curr_ = row_.begin();
            return *this;
        }
    }

    self& operator--() {
        if (curr_ != row_.begin()) {
            --curr_;
            return *this;
        } else {
            --row_;
            curr_ = row_.end() - 1;
            return *this;
        }
    }

    self operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    self operator--(int) {
        auto tmp = *this;
        --(*this);
        return tmp;
    }
};

template <typename T>
class MatrixConstIter {
    using iterator_row = typename Matrix<T>::const_iterator_row;
    using iterator_col = typename Matrix<T>::const_iterator_col;
    using self = MatrixConstIter<T>;
    iterator_row row_;
    iterator_col curr_;

   public:
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    MatrixConstIter() = default;
    MatrixConstIter(const iterator_row Rit, const iterator_col Cit)
        : row_(Rit), curr_(Cit) {}

    MatrixConstIter& operator=(const self&) = default;
    MatrixConstIter(const self&) = default;

    reference operator*() { return *curr_; }

    const iterator_row& row() noexcept { return row_; }

    const iterator_col& curr() noexcept { return curr_; }

    friend bool operator==(const self& lhs, const self& rhs) {
        return (lhs.curr_ == rhs.curr_ && lhs.row_ == rhs.row_);
    }

    friend bool operator!=(const self& lhs, const self& rhs) {
        return (lhs.curr_ != rhs.curr_ && lhs.row_ != rhs.row_);
    }

    self& operator++() {
        if (curr_ != row_.end() - 1) {
            ++curr_;
            return *this;
        } else {
            ++row_;
            curr_ = row_.begin();
            return *this;
        }
    }

    self& operator--() {
        if (curr_ != row_.begin()) {
            --curr_;
            return *this;
        } else {
            --row_;
            curr_ = row_.end() - 1;
            return *this;
        }
    }

    self operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    self operator--(int) {
        auto tmp = *this;
        --(*this);
        return tmp;
    }
};

template <typename T>
class RowIteratorAdaptor {
    using self = RowIteratorAdaptor;
    using iter_type = T;
    iter_type iter_;

   public:
    RowIteratorAdaptor() = default;
    RowIteratorAdaptor(self const&) = default;
    self& operator=(self const&) = default;
    RowIteratorAdaptor(iter_type x) : iter_(x) {}

    iter_type& operator*() { return *iter_; }

    self* operator->() { return iter_; }

    iter_type data() noexcept { return iter_; }

    operator iter_type() { return iter_; }

    friend bool operator!=(const self& lhs, const self& rhs) {
        return (lhs.iter_ != rhs.iter_);
    }

    friend bool operator==(const self& lhs, const self& rhs) {
        return (lhs.iter_ == rhs.iter_);
    }

    self& operator++() {
        ++this->iter_;
        return *this;
    }

    self operator++(int) {
        self tmp(*this);
        ++this->iter_;
        return *this;
    }

    self& operator--() {
        --this->iter_;
        return *this;
    }

    self operator--(int) {
        self tmp(*this);
        --this->iter_;
        return *this;
    }

    self& operator-=(int x) {
        iter_ -= x;
        return *this;
    }

    self& operator+=(int x) {
        iter_ += x;
        return *this;
    }

    self operator-(int value) { return iter_ - value; }

    self operator+(int value) { return iter_ + value; }

    self operator+(self& rhs) { return iter_ + rhs; }

    self operator-(self& rhs) { return iter_ - rhs; }

    auto begin() noexcept { return iter_->begin(); }

    auto end() noexcept { return iter_->end(); }

    auto cbegin() noexcept { return iter_->cbegin(); }

    auto cend() noexcept { return iter_->cend(); }

    auto rbegin() noexcept { return iter_->rbegin(); }

    auto rend() noexcept { return iter_->rend(); }

    auto crbegin() noexcept { return iter_->crbegin(); }

    auto crend() noexcept { return iter_->crend(); }
};

template <typename T>
struct proxy_print_info {
    const Matrix<T>& _matrix;
    explicit proxy_print_info(const Matrix<T>& x) : _matrix(x) {}
};
}

/**
 *   Clasa generica de matrice alocata dinamic
 *   In Ctor se ia marimea matricii, elementele sunt initializate cu 0 daca nu
 * se specifica val de initializare
 *   Ctor-ul poate fi omis dar matricea trebuie initializata prin functia membru
 * resize() sau prin citirea ei dintr-un fisier
 *   Elementele pot fi accesate prin intermediul operatorul [0][0] / (0,0) sau
 * prin functia prieten for_each(*this,Func),
 *   Functia at(0,0) este doar pentru citirea elementelor
*/

template <typename T>
class Matrix {
   public:
    template <typename Tp>
    using t_matrix = std::vector<Tp>;
    using row_t = t_matrix<T>;
    using matrix_t = t_matrix<row_t>;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    using iterator_col = typename row_t::iterator;
    using const_iterator_col = typename row_t::const_iterator;
    using reverse_iterator_col = typename row_t::reverse_iterator;
    using const_reverse_iterator_col = typename row_t::const_reverse_iterator;

    using iterator_row =
        detail::RowIteratorAdaptor<typename matrix_t::iterator>;
    using const_iterator_row =
        detail::RowIteratorAdaptor<typename matrix_t::const_iterator>;
    using reverse_iterator_row =
        detail::RowIteratorAdaptor<typename matrix_t::reverse_iterator>;
    using const_reverse_iterator_row =
        detail::RowIteratorAdaptor<typename matrix_t::const_reverse_iterator>;

    using iterator = detail::MatrixIter<T>;
    using const_iterator = detail::MatrixConstIter<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

   private:
    matrix_t _matrix;
    size_type _line;
    size_type _coll;
    std::string _name;
    bool _zero_is_elem = true;

    struct proxy;
    struct const_proxy;

   public:
    explicit Matrix() : _matrix(), _line(0), _coll(0), _name("") {}

    explicit Matrix(size_type line, size_type coll, value_type init = 0,
                    std::string name = "")
        : _matrix(line, row_t(coll, init)),
          _line(line),
          _coll(coll),
          _name(name) {}

    Matrix(std::initializer_list<T> List)
        : _matrix(), _line(0), _coll(0), _name() {
        size_type marime = 0;
        while (marime * marime < List.size()) ++marime;
        this->resize(marime, marime);

        auto iter = List.begin();
        for (auto& vec : _matrix)
            for (auto& elem : vec)
                if (iter != List.end()) elem = *(iter++);
    }

    Matrix(const Matrix& other, std::string name) : Matrix(other) {
        _name = name;
    }

    Matrix(const Matrix&) = default;
    Matrix(Matrix&& other) noexcept = default;

    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) noexcept = default;

    ~Matrix() { /*static_assert(std::is_arithmetic<T>::value,
            " Matricea trebuie sa contina un tip de data aritmetica"); */
    }

    template <typename TChar, typename CharTraits>
    static void generate(std::basic_ostream<TChar, CharTraits>& stream,
                         size_type line = 10, size_type coll = 10,
                         value_type li = 1, value_type ls = 100) {
        auto width = alg::get_nr_cif(ls) + 1;
        std::mt19937 rng(util::random_seed());

        // chestia de mai jos este pentru a decide daca
        // se foloseste distributie de nr 'int' sau 'float'
        /*using distribution_type =
        typename std::conditional<
            std::is_floating_point<T>::value,
            std::uniform_real_distribution<T>,
            typename std::conditional<
                std::is_integral<T>::value,
                std::uniform_int_distribution<T>,
                void
            >::type
        >::type;*/
        std::uniform_int_distribution<T> dis(li, ls);

        for (size_type i = 0; i < line; i++) {
            for (size_type j = 0; j < coll; j++)
                stream << std::setw(width) << dis(rng);
            stream << '\n';
        }
    }

    iterator begin() noexcept {
        return iterator(_matrix.begin(), _matrix.begin()->begin());
    }

    const_iterator begin() const noexcept {
        return const_iterator(_matrix.cbegin(), _matrix.cbegin()->cbegin());
    }

    iterator end() noexcept {
        return iterator(_matrix.end(), (_matrix.end() - 1)->end());
    }

    const_iterator end() const noexcept {
        return const_iterator(_matrix.cend(), (_matrix.cend() - 1)->cend());
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(
            iterator(_matrix.end() - 1, (_matrix.end() - 1)->end()));
    }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(
            const_iterator(_matrix.cend() - 1, (_matrix.cend() - 1)->cend()));
    }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(_matrix.cbegin(), _matrix.cbegin()->cbegin());
    }

    const_iterator cend() const noexcept {
        return const_iterator(_matrix.cend(), (_matrix.cend() - 1)->cend());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(
            const_iterator(_matrix.cend() - 1, (_matrix.cend() - 1)->cend()));
    }
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    iterator_row begin1() noexcept { return iterator_row(_matrix.begin()); }

    const_iterator_row begin1() const noexcept {
        return const_iterator_row(_matrix.cbegin());
    }

    iterator_row end1() noexcept { return iterator_row(_matrix.end()); }

    const_iterator_row end1() const noexcept {
        return const_iterator_row(_matrix.cend());
    }

    reverse_iterator_row rbegin1() noexcept {
        return reverse_iterator_row(_matrix.rbegin());
    }

    const_reverse_iterator_row rbegin1() const noexcept {
        return const_reverse_iterator_row(_matrix.crbegin());
    }

    reverse_iterator_row rend1() noexcept {
        return reverse_iterator_row(_matrix.rend());
    }

    const_reverse_iterator_row rend1() const noexcept {
        return const_reverse_iterator_row(_matrix.crend());
    }

    const_iterator_row cbegin1() const noexcept {
        return const_iterator_row(_matrix.cbegin());
    }

    const_iterator_row cend1() const noexcept {
        return const_iterator_row(_matrix.cend());
    }

    const_reverse_iterator_row crbegin1() const noexcept {
        return const_reverse_iterator_row(_matrix.crbegin());
    }

    const_reverse_iterator_row crend1() const noexcept {
        return const_reverse_iterator_row(_matrix.crend());
    }

    proxy operator[](size_type line) noexcept { return proxy(_matrix[line]); }

    const_proxy operator[](size_type line) const noexcept {
        return const_proxy(_matrix[line]);
    }

    reference operator()(size_type line, size_type coll) {
        return _matrix.at(line).at(coll);
    }

    const_reference operator()(size_type line, size_type coll) const {
        return _matrix.at(line).at(coll);
    }

    size_type line() const noexcept { return _line; }

    size_type coll() const noexcept { return _coll; }

    size_type capacity() const noexcept {
        return _matrix.capacity() * _matrix.front().capacity();
    }

    size_type size() const noexcept { return _line * _coll; }

    const_reference at(size_type line, size_type coll) const {
        return _matrix.at(line).at(coll);
    }

    reference front() noexcept { return _matrix.front().front(); }

    const_reference front() const noexcept { return _matrix.front().front(); }

    reference back() noexcept { return _matrix.back().back(); }

    const_reference back() const noexcept { return _matrix.back().back(); }

    size_type get_output_width() const;

    matrix_t& data() noexcept { return _matrix; }

    const matrix_t& data() const noexcept { return _matrix; }

    const std::string& name() const noexcept { return _name; }

    bool empty() const noexcept {
        return (this->_line == 0 || this->_coll == 0 || _matrix.empty());
    }

    void zero_is_element(bool ok) noexcept { _zero_is_elem = ok; }

    bool is_zero_elem() const noexcept { return _zero_is_elem; }

    template <typename Func>
    void filter(Func conditie) noexcept {
        for (auto& vec : _matrix)
            for (auto& elem : vec)
                if (!conditie(elem)) elem = 0;
    }

    void clear() noexcept {
        _matrix.clear();
        _line = 0;
        _coll = 0;
    }

    template <typename Func>
    void remove_if(Func conditie);

    template <typename Func>
    void sort(Func compare);

    void resize(size_type line, size_type coll);

    void resize();

    void remove(value_type value) {
        this->remove_if([value](value_type const& x) { return x == value; });
    }

    detail::proxy_print_info<T> info() const noexcept {
        return detail::proxy_print_info<T>(*this);
    }

    Matrix& fill_line(size_type index, value_type value) noexcept {
        std::fill(_matrix.at(index).begin(), _matrix.at(index).end(), value);
        return *this;
    }

    Matrix& fill_coll(size_type index, value_type value) noexcept {
        for (auto& vec : _matrix) vec.at(index) = value;
        return *this;
    }

    Matrix& push_line() {
        _matrix.push_back(row_t(_coll, 0));
        _line++;
        return *this;
    }

    Matrix& push_coll() {
        for (auto& vec : _matrix) vec.push_back(0);
        _coll++;
        return *this;
    }

    Matrix& pop_line() {
        _line--;
        _matrix.pop_back();
        return *this;
    }

    Matrix& pop_coll() {
        for (auto& vec : _matrix) vec.pop_back();
        return *this;
    }

    Matrix& insert_line(size_type index) {
        _line++;
        _matrix.insert(_matrix.begin() + index, row_t(_coll, 0));
        return *this;
    }

    Matrix& insert_line(iterator_row pozitie) {
        _line++;
        _matrix.insert(pozitie, row_t(_coll, 0));
        return *this;
    }

    Matrix& insert_coll(size_type index) {
        _coll++;
        for (auto iter = _matrix.begin(); iter != _matrix.end(); ++iter)
            iter->insert(iter->begin() + index, 0);
        return *this;
    }

    Matrix& erase_line(size_type index) {
        _line--;
        _matrix.erase(_matrix.begin() + index);
        return *this;
    }

    Matrix& erase_line(iterator_row pozitie) {
        _line--;
        _matrix.erase(pozitie);
        return *this;
    }

    iterator erase(iterator first, iterator last) {
        _line -= std::distance(first.row().data(), last.row().data());
        auto distanta = std::distance(first.row().begin(), first.curr() - 1);
        std::fill(first, last, 0);
        auto it = _matrix.erase(first.row().data(), last.row().data());
        iterator newPos(it, it->begin());
        std::advance(newPos, distanta);
        return newPos;
    }

    Matrix& erase_coll(size_type index) {
        _coll--;
        for (auto iter = _matrix.begin(); iter != _matrix.end(); ++iter)
            iter->erase(iter.begin() + index);
        return *this;
    }

    Matrix& operator+=(const Matrix& rhs) {
        for (size_type i = 0; i < _line; i++)
            for (size_type j = 0; j < _coll; j++)
                this->_matrix.at(i).at(j) += rhs._matrix.at(i).at(j);
        return *this;
    }

    Matrix& operator-=(const Matrix<T>& rhs) {
        for (size_type i = 0; i < _line; i++)
            for (size_type j = 0; j < _coll; j++)
                this->_matrix.at(i).at(j) -= rhs._matrix.at(i).at(j);
        return *this;
    }

    Matrix& operator*=(const Matrix<T>& rhs) {
        for (size_type i = 0; i < _line; i++)
            for (size_type j = 0; j < _coll; j++)
                this->_matrix.at(i).at(j) *= rhs._matrix.at(i).at(j);
        return *this;
    }

    Matrix& operator/=(const Matrix<T>& rhs) {
        for (size_type i = 0; i < _line; i++)
            for (size_type j = 0; j < _coll; j++)
                this->_matrix.at(i).at(j) /= rhs._matrix.at(i).at(j);
        return *this;
    }

    template <typename Tp, typename TChar, typename CharTraits>
    friend std::basic_istream<TChar, CharTraits>& operator>>(
        std::basic_istream<TChar, CharTraits>& stream, Matrix<Tp>& matrix);

   private:
    // verifica daca exista linii "moarte"(linii care au doar elem de 0) in
    // matrice
    void _clean_end() {
        while (_matrix.back().front() == 0 and _matrix.back().at(1) == 0) {
            _matrix.pop_back();
            this->_line--;
        }
    }

    void _set_line(size_type line) {
        if (this->_line != line) {
            this->_line = line;
            this->_matrix.resize(_line);
        }
    }

    void _set_coll(size_type coll) {
        if (this->_coll != coll) {
            this->_coll = coll;
            for (auto& vec : _matrix) vec.resize(_coll);
        }
    }

};  // CLASS MATRIX < T >

template <typename T>
struct Matrix<T>::proxy {
    row_t& _vec;
    proxy(row_t& x) : _vec(x) {}

    reference operator[](size_type coll) { return _vec[coll]; }
};

template <typename T>
struct Matrix<T>::const_proxy {
    const row_t& _vec;
    const_proxy(const row_t& x) : _vec(x) {}

    const_reference operator[](size_type coll) const { return _vec[coll]; }
};

template <typename T>
typename Matrix<T>::size_type Matrix<T>::get_output_width() const {
    std::vector<T> max_val;
    max_val.reserve(this->size());
    bool ok = false;
    size_type _output_width = 2;
    for (auto& vec : _matrix)
        for (auto& elem : vec) {
            max_val.push_back(abs(elem));
            if (!ok && elem < 0) {
                ok = true;
                _output_width++;
            }
        }
    auto _value = *std::max_element(max_val.begin(), max_val.end());
    while (_value > 9) {
        _output_width++;
        _value /= 10;
    }
    return _output_width;
}

template <typename T>
template <typename F>
void Matrix<T>::sort(F comp) {
    // put elements in vector
    std::vector<T> sort_vector;
    for (auto& vec : _matrix)
        for (auto& elem : vec)
            if (elem != 0) sort_vector.push_back(elem);
    for (auto& vec : _matrix)
        for (auto& elem : vec) elem = 0;
    // sort the vector
    std::sort(sort_vector.begin(), sort_vector.end(), comp);

    // put elements back in matrix
    auto counter = sort_vector.begin();
    for (auto& vec : _matrix)
        for (auto& elem : vec) elem = *(counter++);
}

template <typename T>
template <typename Func>
void Matrix<T>::remove_if(Func conditie) {
    std::vector<T> filter_vec;
    for (auto& vec : _matrix)
        for (auto& elem : vec)  // filter elem
            if (!conditie(elem)) filter_vec.push_back(elem);

    for (auto& vec : _matrix)
        for (auto& elem : vec) elem = 0;

    // make the matrix square
    size_type _size = 0;
    while (_size * _size < filter_vec.size()) _size++;
    this->resize(_size, _size);

    // insert elem in matrix
    auto counter = filter_vec.begin();
    for (auto& vec : _matrix)
        for (auto& elem : vec)
            if (counter != filter_vec.end()) elem = *counter++;
    _clean_end();
}

//*///////////////////////////////////////////////
///////////////// MODIFIERS SIZE /////////////////
//////////////////////////////////////////////////

template <typename T>  // resize
void Matrix<T>::resize(size_type _line, size_type _coll) {
    if (_line == 0 or _coll == 0)
        this->clear();
    else if (this->_line == _line)
        _set_coll(_coll);
    else if (this->_coll == _coll)
        _set_line(_line);
    else {
        std::vector<T> vec_aux;
        for (auto& vec : _matrix)
            for (auto& elem : vec) vec_aux.push_back(elem);
        _set_line(_line);
        _set_coll(_coll);
        size_type counter = 0;
        for (auto& vec : _matrix)
            for (auto& elem : vec)
                if (counter < vec_aux.size())
                    elem = vec_aux.at(counter++);
                else
                    break;
    }
}

template <typename T>     // resize
void Matrix<T>::resize()  // face matricea patratica
{
    std::vector<T> vec;
    for (auto& v : _matrix)
        for (auto& e : v) vec.push_back(e);
    while (vec.back() == 0) vec.pop_back();

    size_type _size = 1;
    while (_size * _size < vec.size()) _size++;
    this->resize(_size, _size);

    this->_clean_end();
}

//*/////////////////////////////////////////////////////
///////////////// OPERATOR OVERLOADING /////////////////
////////////////////////////////////////////////////////

template <typename T>
inline bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return (lhs.size() == rhs.size);
}

template <typename T>
inline bool operator<(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return (lhs.size() < rhs.size());
}

template <typename T>
inline bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
inline bool operator>(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return (rhs < lhs);
}

template <typename T>
inline bool operator<=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs > rhs);
}

template <typename T>
inline bool operator>=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs < rhs);
}

template <typename T>
inline Matrix<T> operator+(Matrix<T> lhs, Matrix<T> const& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
inline Matrix<T> operator-(Matrix<T> lhs, Matrix<T> const& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T>
inline Matrix<T> operator*(Matrix<T> lhs, Matrix<T> const& rhs) {
    lhs *= rhs;
    return lhs;
}

template <typename T>
inline Matrix<T> operator/(Matrix<T> lhs, Matrix<T> const& rhs) {
    lhs /= rhs;
    return lhs;
}

//*///////////////////////////////////////////////
///////////////// READ FUNCTIONS /////////////////
//////////////////////////////////////////////////

template <typename T, typename TChar, typename CharTraits>
std::basic_istream<TChar, CharTraits>& operator>>(
    std::basic_istream<TChar, CharTraits>& stream, Matrix<T>& matrix) {
    if (matrix.empty()) {
        T value;
        std::string line;
        while (getline(stream, line)) {
            matrix.data().push_back(typename Matrix<T>::row_t());
            std::stringstream split(line);
            while (split >> value) matrix.data().back().push_back(value);
        }

        std::vector<T> min_max_vec;
        for (auto& vec : matrix.data()) min_max_vec.push_back(vec.size());

        using size_type = typename Matrix<T>::size_type;
        size_type _min =
            *std::min_element(min_max_vec.begin(), min_max_vec.end());
        size_type _max =
            *std::max_element(min_max_vec.begin(), min_max_vec.end());
        if (_min != _max)
            for (auto& vec : matrix.data())
                while (vec.size() < _max) vec.push_back(0);

        if (matrix.data().back().front() == 0) matrix.data().pop_back();

        matrix._line = matrix.data().size();
        matrix._coll = matrix.data().front().size();
    } else
        for (auto& vec : matrix.data())
            for (auto& elem : vec) stream >> elem;
    return stream;
}

//*////////////////////////////////////////////////
///////////////// PRINT FUNCTIONS /////////////////
///////////////////////////////////////////////////

template <typename T, typename TChar, typename CharTraits>
std::basic_ostream<TChar, CharTraits>& operator<<(
    std::basic_ostream<TChar, CharTraits>& stream, const Matrix<T>& matrix) {
    if (!matrix.empty()) {
        auto _output_width = matrix.get_output_width();

        stream << '\n'
               << " Matricea " << matrix.name() << ": " << matrix.line()
               << " x " << matrix.coll() << '\n';
        if (matrix.is_zero_elem())
            for (auto& vec : matrix.data()) {
                for (auto& elem : vec)
                    stream << std::setw(_output_width) << elem;
                stream << '\n';
            }
        else
            for (auto& vec : matrix.data()) {
                for (auto& elem : vec)
                    if (elem != 0)
                        stream << std::setw(_output_width) << elem;
                    else
                        stream << std::setw(_output_width) << ' ';
                stream << '\n';
            }
        stream << '\n';
    } else
        stream << " Matricea " << matrix.name() << " este goala " << '\n';
    return stream;
}

template <typename T, typename TChar, typename CharTraits>
std::basic_ofstream<TChar, CharTraits>& operator<<(
    std::basic_ofstream<TChar, CharTraits>& file, const Matrix<T>& matrix) {
    if (!matrix.empty()) {
        auto _output_width = matrix.get_output_width();
        for (auto& vec : matrix.data()) {
            for (auto& elem : vec) file << std::setw(_output_width) << elem;
            file << '\n';
        }
        file << '\n';
    }
    return file;
}

template <typename T, typename TChar, typename CharTraits>
std::basic_ostream<TChar, CharTraits>& operator<<(
    std::basic_ostream<TChar, CharTraits>& stream,
    const detail::proxy_print_info<T> info) {
    const auto& matrix = info._matrix;
    using size_type = typename Matrix<T>::size_type;
    if (!matrix.empty()) {
        auto _output_width = matrix.get_output_width();

        stream << '\n'
               << " Matricea " << matrix.name() << ": " << matrix.line()
               << " x " << matrix.coll() << '\n';
        stream << std::setw(_output_width) << "X";

        for (size_type i = 1; i <= matrix.coll(); i++)
            stream << std::setw(_output_width) << i;

        stream << '\n';
        size_type counter = 0;

        if (matrix.is_zero_elem())
            for (auto& vec : matrix.data()) {
                stream << std::setw(_output_width) << ++counter;
                for (auto& elem : vec)
                    stream << std::setw(_output_width) << elem;
                stream << '\n';
            }
        else
            for (auto& vec : matrix.data()) {
                stream << std::setw(_output_width) << ++counter;
                for (auto& elem : vec)
                    if (elem != 0)
                        stream << std::setw(_output_width) << elem;
                    else
                        stream << std::setw(_output_width) << ' ';
                stream << '\n';
            }
        stream << '\n';
    } else
        stream << " Matricea " << matrix.name() << " este goala " << '\n';
    return stream;
}

}  // NAMESOACE BCT

namespace std {
template <typename T, typename Func>
void for_each(Matrix<T>& matrix, Func func) noexcept {
    for (auto& vec : matrix.data())
        for (auto& elem : vec)
            if (elem != 0) func(elem);
}

template <typename T, typename Func>
void for_each(const Matrix<T>& matrix, Func func) noexcept {
    for (auto& vec : matrix.data())
        for (auto& elem : vec)
            if (elem != 0) func(elem);
}
}  // NAMESOACE STD

#endif  // MATRIX_H_INCLUDED
