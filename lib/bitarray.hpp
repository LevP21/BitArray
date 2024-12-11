#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string>

class BitArray
{
private:
  unsigned long *array{nullptr};
  int length{0};
  int capacity{0};
  const int dim{sizeof(unsigned long) * 8};

public:
  // default constructor, creates an empty object of BitArray class
  BitArray();
  // default destructor, frees the alocated memory
  ~BitArray();

  // parameterized constructor, creates an object of class BitArray with an array of length num_bits, the first values are filled with value
  explicit BitArray(int num_bits, unsigned long value = 0);
  // copy constructor, creates an object of class BitArray by copying object b
  BitArray(const BitArray &b);

  // swaps the values of two arrays
  void swap(BitArray &b);

  // assignment operator, assigns the values of one array to another array
  BitArray &operator=(const BitArray &b);

  // resizes the array, if the array is incremented, the new values are filled with value
  void resize(int num_bits, bool value = false);
  // frees the alocated memory
  void clear();
  // adds a new value to the end of the array
  void push_back(bool bit);

  // bitwise multiplication, works only when array sizes match, result is assigned to the object
  BitArray &operator&=(const BitArray &b);
  // bitwise addition, works only when array sizes match, result is assigned to the object
  BitArray &operator|=(const BitArray &b);
  // exclusive-or, works only when array sizes match, result is assigned to the object
  BitArray &operator^=(const BitArray &b);

  // bit shift to the left by n, the freed cells are filled with the value false, result is assigned to the object
  BitArray &operator<<=(int n);
  // bit shift to the right by n, the freed cells are filled with the value false, result is assigned to the object
  BitArray &operator>>=(int n);
  // bit shift to the left by n, the freed cells are filled with the value false, returns a new object
  BitArray operator<<(int n) const;
  // bit shift to the right by n, the freed cells are filled with the value false, returns a new object
  BitArray operator>>(int n) const;

  // sets the n-index bit to val
  BitArray &set(int n, bool val = true);
  // fills the array with true values
  BitArray &set();

  // sets the n-index bit to the value false
  BitArray &reset(int n);
  // fills the array with false values
  BitArray &reset();

  // return true if the array contains one or more true bits
  bool any() const;
  // returns true if all bits of the array are false
  bool none() const;
  // bitwise inversion, returns a new object
  BitArray operator~() const;
  // counts the number of true bits
  int count() const;

  // returns the value of the i-index bit
  bool operator[](int i) const;

  // returns the array size
  int size() const;
  // returns true if memory for the array is not allocated
  bool empty() const;

  // returns the array as a string
  std::string to_string() const;
};

// equality operator, return true if the arrays are the same, works only when array sizes match
bool operator==(const BitArray &a, const BitArray &b);
// inequality operator, return true if the arrays are not the same, works only when array sizes match
bool operator!=(const BitArray &a, const BitArray &b);

// bitwise multiplication, works only when array sizes match, returns a new object
BitArray operator&(const BitArray &b1, const BitArray &b2);
// bitwise addition, works only when array sizes match, returns a new object
BitArray operator|(const BitArray &b1, const BitArray &b2);
// exclusive-or, works only when array sizes match, returns a new object
BitArray operator^(const BitArray &b1, const BitArray &b2);