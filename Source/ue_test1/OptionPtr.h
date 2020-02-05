#pragma once

#include <exception>

class MyNullPointerException : public std::exception
{
	const char * what() const throw ();
};

template <typename T>
class OptionPtr {
	T* pointer;

	OptionPtr(T* set_pointer) {
		pointer = set_pointer;
	}

public:
	static OptionPtr<T> new_some(T* pointer) {
		if (pointer == nullptr) {
			throw MyNullPointerException();
		}

		return OptionPtr(pointer);
	}

	static OptionPtr<T> new_none() {
		return OptionPtr(nullptr);
	}

	static OptionPtr<T> new_unchecked(T* pointer) {
		return OptionPtr(pointer);
	}

	T& unwrap() {
		if (pointer == nullptr) {
			throw MyNullPointerException();
		}

		return *pointer;
	}

	bool is_some() {
		return pointer != nullptr;
	}

	bool is_none() {
		return pointer == nullptr;
	}

	T* match() {
		return pointer;
	}

};