#include <iostream>
#include <string>

using namespace std;

class reference_counter {
    int count_;
    using self = reference_counter;

   public:
    reference_counter() : count_(0) {}

    reference_counter(const self& other) : count_(other.count_) {}

    self& operator=(const self& other) {
        count_ = other.count_;
        return *this;
    }

    template <typename U>
    void add_ref(U msg) {
        ++count_;
        cout << "add ref " << msg << endl;
    }

    template <typename U>
    void remove_ref(U msg) {
        --count_;
        cout << "remove ref " << msg << endl;
    }

    bool empty() { return count_ == 0; }

    int get() { return count_; }
};

template <typename T>
class shared_ptr {
    T* ptr_;
    reference_counter* counter_;
    using self = shared_ptr<T>;

   public:
    shared_ptr(T* ptr) : ptr_(ptr), counter_(new reference_counter()) {
        counter_->add_ref(*ptr);
    }

    shared_ptr(const self& other) : ptr_(other.ptr_), counter_(other.counter_) {
        counter_->add_ref(*ptr_);
    }

    self& operator=(const self& other) {
        if (counter_) {
            counter_->remove_ref(*ptr_);
        }
        ptr_ = other.ptr_;
        counter_ = other.counter_;
        counter_->add_ref(*ptr_);
        return *this;
    }

    self& operator=(T* ptr) {
        if (counter_) {
            counter_->remove_ref(*ptr_);
        }
        ptr_ = ptr;
        counter_ = new reference_counter();
        counter_->add_ref(*ptr_);
        return *this;
    }

    explicit operator bool() { return !counter_->empty(); }

    T& operator*() { return *ptr_; }

    const T& operator*() const { return *ptr_; }

    ~shared_ptr() {
        if (counter_->get() == 1) {
            cout << "delete ptr = " << *ptr_ << endl;
            delete ptr_;
            delete counter_;
        } else {
            counter_->remove_ref(*ptr_);
        }
    }
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto obj = new T(args...);
    return shared_ptr<T>(obj);
}

void f(shared_ptr<int> p) {
    p = make_shared<int>(8);
}

int main() {
    auto ptr = make_shared<int>(4);
    f(ptr);
    std::cout << "plm\n";
}
