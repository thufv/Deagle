/*
 * bit_int.h
 *
 *  Created on: Jul 18, 2016
 *      Author: ylz86
 */

#ifndef CBMC_BIT_INT_H_
#define CBMC_BIT_INT_H_

#include <vector>
#include <cassert>
#include <iostream>

class bit_int {
public:
	int len;        // number of bits
	int int_len;	// number of ints
	int* data;		// data

public:
	// constructor, null
	bit_int(): len(0), int_len(0), data(0){}

	// constructor, bit_int(len), data = 0 initially
	bit_int(int length) {
		len = length;
		int_len = (len + 31) >> 5;
		if (int_len > 0)
			data = new int[int_len];
		for (int i = 0; i < int_len; i++)
			data[i] = 0;
	}

	// constructor bit_int(a)
	bit_int(const bit_int& a) {
		len = a.len;
		int_len = a.int_len;
		if (int_len > 0)
			data = new int[int_len];
		for (int i = 0; i < int_len; i++)
			data[i] = a.data[i];
	}

	// destructor, delete data
	virtual ~bit_int() {
		if(data != NULL) {
			delete[] data;
			data = NULL;
		}
	}

	// initialize
	void init(int length) {
		len = length;
		int_len = (len + 31) >> 5;
		if(data != NULL) {
			delete[] data;
			data = NULL;
		}
		if (int_len > 0)
			data = new int[int_len];
		for (int i = 0; i < int_len; i++)
			data[i] = 0;
	}

	void clear() {
		for (int i = 0; i < int_len; i++)
			data[i] = 0;
	}

	// this = a;
	void operator=(const bit_int& a) {
		len = a.len;
		int_len = a.int_len;
		if (data != NULL) {
			delete[] data;
			data = NULL;
		}
		if (int_len > 0)
			data = new int[int_len];

		for (int i = 0; i < int_len; i++)
			data[i] = a.data[i];
	}

	// r = a | b
	bit_int operator|(const bit_int& a) {
		assert(len == a.len);
		bit_int r(len);
		for (int i = 0; i < int_len; i++)
			r.data[i] = data[i] | a.data[i];
		return r;
	}

	void unionc(const bit_int& a) {
		assert(len == a.len);
		for (int i = 0; i < int_len; i++)
			if (a.data[i])
				data[i] = data[i] | a.data[i];
	}

	// r = a - b, set difference, r is set that remove a & b from a
	bit_int operator-(const bit_int& a) {
		assert(len == a.len);
		bit_int r(len);
		for (int i = 0; i < int_len; i++)
			r.data[i] = data[i]^(data[i] & a.data[i]);
		return r;
	}

	void diff(const bit_int& a, bit_int& r) {
		assert(len == a.len);
		r.init(len);
		for (int i = 0; i < int_len; i++)
			r.data[i] = data[i]^(data[i] & a.data[i]);
	}

	// set the i-th bit to 1, i < len
	void set_true (int i) {
		assert(i < len);
		data[i >> 5] |= (1 << (i & 0x0000001f));
	}

	// set the i-th bit to 0, i < len
	void set_false (int i) {
		assert(i < len);
		data[i >> 5] &= ~(1 << (i & 0x0000001f));
	}

	// judge if the i-th bit is 1, i < len
	bool is_true (int i) {
		assert(i < len);
		return data[i >> 5] & (1 << (i & 0x0000001f));
	}

	// get all the bits of 1
	void get_one_bits(std::vector<int>& ones) {
		ones.clear();
		for (int i = 0; i < int_len; i++) {
			if (data[i] & 0xffffffff) {
				if (data[i] & 0x0000ffff) {
					for (int j = 0; j < 16; j++) {
						if (data[i] & (1 << j)) {
							ones.push_back((i << 5) | j);
						}
					}
				}
				if (data[i] & 0xffff0000) {
					for (int j = 16; j < 32; j++) {
						if (data[i] & (1 << j)) {
							ones.push_back((i << 5) | j);
						}
					}
				}
			}
		}
	}

	// print ones;
	void print_ones(std::ostream& out) {
		std::vector<int> ones;
		get_one_bits(ones);
		for (unsigned i = 0; i < ones.size(); i++)
			std::cout << ones[i] << " ";
		std::cout << "\n";
	}
};

#endif /* CBMC_BIT_INT_H_ */
