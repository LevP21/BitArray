#include "bitarray.hpp"

// default constructor, creates an empty object of BitArray class
BitArray::BitArray() : length(0), capacity(0), array(nullptr) {}

// default destructor, frees the alocated memory
BitArray::~BitArray()
{
    delete[] this->array; // the array memory is freed
    this->array = nullptr;
}

// parameterized constructor, creates an object of class BitArray with an array of length num_bits, the first values are filled with value
BitArray::BitArray(int num_bits, unsigned long value) : length(num_bits)
{
    if (num_bits < 0) // the argument check
    {
        throw std::invalid_argument("Error: argument num_bits expects value > 0");
    }

    if (num_bits > 0)
    {
        if (num_bits % dim == 0 && num_bits > 0)
            this->capacity = num_bits; // the capacity = num_bits if num_bits can be integer-divided by the dimension
        else
            this->capacity = ((num_bits / dim) + 1) * dim; // else the capacity takes the size of full unsigned long cells that can hold all bits

        this->array = new unsigned long[this->capacity / dim]{}; // the array memory is allocated
        this->array[0] = value;
    }
}

// copy constructor, creates an object of class BitArray by copying object b
BitArray::BitArray(const BitArray &b) : length(b.length), capacity(b.capacity)
{
    if (b.array != nullptr)
    {
        this->array = new unsigned long[this->capacity / dim]{}; // the array memory is allocated

        std::copy(b.array, b.array + (this->capacity / dim), this->array); // the array is filled with elements of the array b
    }
}

// swaps the values of two arrays
void BitArray::swap(BitArray &b)
{
    std::swap(this->length, b.length);
    std::swap(this->capacity, b.capacity);
    std::swap(this->array, b.array);
}

// assignment operator, assigns the values of one array to another array
BitArray &BitArray::operator=(const BitArray &b)
{
    if (((*this).empty() && b.empty()) || (!(*this).empty() && !b.empty()))
    {
        if ((*this) == b)
        {
            return *this; // self-attribution
        }
    }

    this->length = b.length;
    this->capacity = b.capacity;

    delete[] this->array; // old array memory is freed

    if (!b.empty())
    {
        this->array = new unsigned long[this->capacity / dim]{}; // new array memory is allocated

        std::copy(b.array, b.array + (this->capacity / dim), this->array); // the array is filled with elements of the array b
    }
    else
    {
        this->array = nullptr;
    }

    return *this;
}

// resizes the array, if the array is incremented, the new values are filled with value
void BitArray::resize(int num_bits, bool value)
{
    if (num_bits < 0) // the argument check
    {
        throw std::invalid_argument("Error: argument num_bits expects value > 0");
    }
    else if (num_bits == 0)
    {
        (*this).clear(); // if the array resizes to 0, the array is cleared
    }
    else
    {
        if ((num_bits % dim == 0 && this->capacity / dim != num_bits / dim) || (num_bits % dim != 0 && this->capacity / dim != num_bits / dim + 1))
        {
            if (num_bits % dim == 0)
            {
                this->capacity = num_bits; // the capacity = num_bits if num_bits can be integer-divided by the dimension
            }
            else
            {
                this->capacity = (num_bits / dim + 1) * dim; // else the capacity takes the size of full unsigned long cells that can hold all bits
            }

            unsigned long *new_arr(new unsigned long[this->capacity / dim]{}); // a new array is created and its memory is alocated

            if (!(*this).empty()) // the array empty check
            {
                if (this->length < num_bits)
                {
                    if (this->length % dim == 0)
                    {
                        std::copy(this->array, this->array + (this->length / dim), new_arr); // if the length can be integer-divided by the dimensionon, the new array is filled with full unsigned long cells only
                    }
                    else
                    {
                        std::copy(this->array, this->array + (this->length / dim + 1), new_arr); // else the new array is also filled with an incomplete unsigned long cell
                    }
                }
                else
                {
                    if (num_bits % dim == 0)
                    {
                        std::copy(this->array, this->array + (num_bits / dim), new_arr); // if the length can be integer-divided by the dimensionon, the new array is filled with full unsigned long cells only
                    }
                    else
                    {
                        std::copy(this->array, this->array + (num_bits / dim + 1), new_arr); // else the new array is also filled with an incomplete unsigned long cell
                    }
                }
            }

            delete[] this->array;  // the array memory is freed
            this->array = new_arr; // the new array is became the array of this object
        }

        int old_length = this->length;

        this->length = num_bits;

        for (int i = old_length; i < this->length; ++i)
        {
            (*this).set(i, value); // free cells are set with the value argument
        }
    }
}

// frees the alocated memory
void BitArray::clear()
{
    this->length = 0;
    this->capacity = 0;

    delete[] this->array;  // the array memory is freed
    this->array = nullptr; // the array pointer = nullptr
}

// adds a new value to the end of the array
void BitArray::push_back(bool bit)
{
    if (this->length == this->capacity)
    {
        this->capacity += dim;

        unsigned long *new_arr(new unsigned long[this->capacity / dim]{}); // if the array is full, new array memory is allocated

        if (!(*this).empty()) // the array empty check
        {
            std::copy(this->array, this->array + (this->length / dim), new_arr); // the new array is filled with elements of the old array
        }

        delete[] this->array;  // the array memory is freed
        this->array = new_arr; // the new array is became the array of this object
    }

    this->length++;

    (*this).set(this->length - 1, bit); // the last sell is set with the bit argument
}

// bitwise multiplication, works only when array sizes match, result is assigned to the object
BitArray &BitArray::operator&=(const BitArray &b)
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: this array is empty");
    }

    if (b.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: other array is empty");
    }

    if (this->length != b.length) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    if (this->length % dim == 0)
    {
        for (int i = 0; i < this->length / dim; ++i)
        {
            this->array[i] &= b.array[i]; // applying the operation to full unsigned long cells only
        }
    }
    else
    {
        for (int i = 0; i < this->length / dim + 1; ++i)
        {
            this->array[i] &= b.array[i]; // applying the operation also to an incomplete unsigned long cell
        }
    }

    return *this;
}

// bitwise addition, works only when array sizes match, result is assigned to the object
BitArray &BitArray::operator|=(const BitArray &b)
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: this array is empty");
    }

    if (b.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: other array is empty");
    }

    if (this->length != b.length) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    if (this->length % dim == 0)
    {
        for (int i = 0; i < this->length / dim; ++i)
        {
            this->array[i] |= b.array[i]; // applying the operation to full unsigned long cells only
        }
    }
    else
    {
        for (int i = 0; i < this->length / dim + 1; ++i)
        {
            this->array[i] |= b.array[i]; // applying the operation also to an incomplete unsigned long cell
        }
    }

    return *this;
}

// exclusive-or, works only when array sizes match, result is assigned to the object
BitArray &BitArray::operator^=(const BitArray &b)
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: this array is empty");
    }

    if (b.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: other array is empty");
    }

    if (this->length != b.length) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    if (this->length % dim == 0)
    {
        for (int i = 0; i < this->length / dim; ++i)
        {
            this->array[i] ^= b.array[i]; // applying the operation to full unsigned long cells only
        }
    }
    else
    {
        for (int i = 0; i < this->length / dim + 1; ++i)
        {
            this->array[i] ^= b.array[i]; // applying the operation also to an incomplete unsigned long cell
        }
    }

    return *this;
}

// bit shift to the left by n, the freed cells are filled with the value false, result is assigned to the object
BitArray &BitArray::operator<<=(int n)
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    if (n < 0) // the argument check
    {
        throw std::invalid_argument("Error: argument n expects value > 0");
    }

    if (n >= this->length)
    {
        (*this).reset(); // if the argument is greater than the array length, the array is filled with the value false
    }
    else
    {
        for (int i = n; i < this->length; ++i)
        {
            (*this).set(i - n, (*this)[i]); // the array is shifted to the left by n positions
        }

        for (int i = this->length - n; i < this->length; ++i)
        {
            (*this).set(i, false); // empty cells are filled with the value false
        }
    }

    return *this;
}

// bit shift to the right by n, the freed cells are filled with the value false, result is assigned to the object
BitArray &BitArray::operator>>=(int n)
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    if (n < 0) // the argument check
    {
        throw std::invalid_argument("Error: argument n expects value > 0");
    }

    if (n >= this->length)
    {
        (*this).reset(); // if the argument is greater than the array length, the array is filled with the value false
    }
    else
    {
        for (int i = this->length - n - 1; i >= 0; --i)
        {
            (*this).set(i + n, (*this)[i]); // the array is shifted to the right by n positions
        }

        for (int i = 0; i < n; ++i)
        {
            (*this).set(i, false); // empty cells are filled with the value false
        }
    }

    return *this;
}

// bit shift to the left by n, the freed cells are filled with the value false, returns a new object
BitArray BitArray::operator<<(int n) const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    if (n < 0) // the argument check
    {
        throw std::invalid_argument("Error: argument n expects value > 0");
    }

    BitArray new_object(this->length);

    if (n < this->length) // if n is greater than the array lenght, the array remains untouched with the value false in each cell
    {
        for (int i = n; i < this->length; ++i)
        {
            new_object.set(i - n, (*this)[i]); // the array is shifted to the left by n positions
        }

        for (int i = this->length - n; i < this->length; ++i)
        {
            new_object.set(i, false); // empty cells are filled with the value false
        }
    }

    return new_object;
}

// bit shift to the right by n, the freed cells are filled with the value false, returns a new object
BitArray BitArray::operator>>(int n) const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    if (n < 0) // the argument check
    {
        throw std::invalid_argument("Error: argument n expects value > 0");
    }

    BitArray new_object(this->length);

    if (n < this->length) // if n is greater than the array lenght, the array remains untouched with the value false in each cell
    {
        for (int i = this->length - n - 1; i >= 0; --i)
        {
            new_object.set(i + n, (*this)[i]); // the array is shifted to the right by n positions
        }

        for (int i = 0; i < n; ++i)
        {
            new_object.set(i, false); // empty cells are filled with the value false
        }
    }

    return new_object;
}

// sets the n-index bit to val
BitArray &BitArray::set(int n, bool val)
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    if (n < 0 || n >= this->length) // the index validitation check
    {
        throw std::out_of_range("Error: index is out of range");
    }

    if (val)
    {
        this->array[n / dim] |= 1UL << (dim - ((n + 1) % dim)); // if argument value is true, the unsigned long cell containing the n-index is bitwise added with the bitmask consisting of the true bit shifted to the left
    }
    else
    {
        this->array[n / dim] &= ~(1UL << (dim - ((n + 1) % dim))); // if argument value is false, the unsigned long cell containing the n-index is bitwise multiplied with the bitmask consisting of the negated true bit shifted to the left
    }

    return *this;
}

// fills the array with true values
BitArray &BitArray::set()
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    for (int i = 0; i < this->capacity / dim; ++i)
    {
        this->array[i] |= ~0UL; // the array is filled with negated bits false
    }

    return *this;
}

// sets the n-index bit to the value false
BitArray &BitArray::reset(int n)
{
    (*this).set(n, false); // the n-index cell is set with the value false

    return *this;
}

// fills the array with false values
BitArray &BitArray::reset()
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    for (int i = 0; i < this->capacity / dim; ++i)
    {
        this->array[i] &= 0UL; // the array is filled with bits false
    }

    return *this;
}

// return true if the array contains one or more true bits
bool BitArray::any() const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    for (int i = 0; i < this->length / dim; ++i)
    {
        if (this->array[i] > 0UL) // if at least one element of the array is greater than 0, return true
            return true;
    }

    if (this->length % dim == 0)
    {
        return false; // if the array consists of full unsigned long cells only and all unsigned long cells are already checked, return false
    }
    else
    {
        for (int i = this->length - this->length % dim; i < this->length; ++i)
        {
            if ((*this)[i]) // else check also an incomplete unsigned long cell, if at least one bit is true, return true
                return true;
        }
    }

    return false; // if all bits are already checked and none of them are true, return false
}

// returns true if all bits of the array are false
bool BitArray::none() const
{
    return !(*this).any(); // returns negated any
}

// bitwise inversion, returns a new object
BitArray BitArray::operator~() const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    BitArray new_object(this->length);

    for (int i = 0; i < this->capacity / dim; ++i)
    {
        new_object.array[i] = ~this->array[i]; // the new array is filled with negated elements of the old array
    }

    return new_object;
}

// counts the number of true bits
int BitArray::count() const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    int count = 0;

    for (int i = 0; i < this->length; ++i)
    {
        if ((*this)[i]) // counting true values in the array
            ++count;
    }

    return count;
}

// returns the value of the i-index bit
bool BitArray::operator[](int i) const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    if (i < 0 || i >= this->length) // the index validitation check
    {
        throw std::out_of_range("Error: index is out of range");
    }

    return (this->array[i / dim] & (1UL << (dim - ((i + 1) % dim)))) > 0UL; // the unsigned long cell containing the i-index is bitwise multiplied with the bitmask consisting of the true bit shifted to the left, if the i-index bit is true, unsigned long cell is greater than 0, returns true
}

// returns the array size
int BitArray::size() const
{
    return this->length;
}

// returns true if memory for the array is not allocated
bool BitArray::empty() const
{
    if (this->array == nullptr) // the allocated memory check
        return true;
    else
        return false;
}

// returns the array as a string
std::string BitArray::to_string() const
{
    if ((*this).empty()) // the array empty check
    {
        throw std::invalid_argument("Error: array is empty");
    }

    std::string str("");

    for (int i = 0; i < this->length; ++i)
    {
        if ((*this)[i]) // if the value is true, appends 1 to the end of the string, else appends 0
            str.append("1");
        else
            str.append("0");
    }

    return str;
}

// equality operator, return true if the arrays are the same, works only when array sizes match
bool operator==(const BitArray &a, const BitArray &b)
{
    if (a.empty() && b.empty())
        return true;

    if (a.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: first array is empty");
    }

    if (b.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: second array is empty");
    }

    if (a.size() != b.size()) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i]) // checking equality of each pair of bits in arrays
            return false;
    }

    return true;
}

// inequality operator, return true if the arrays are not the same, works only when array sizes match
bool operator!=(const BitArray &a, const BitArray &b)
{
    return !(a == b); // returns negated a == b
}

// bitwise multiplication, works only when array sizes match, returns a new object
BitArray operator&(const BitArray &b1, const BitArray &b2)
{
    if (b1.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: first array is empty");
    }

    if (b2.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: second array is empty");
    }

    if (b1.size() != b2.size()) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    BitArray new_object(b1.size());

    for (int i = 0; i < b1.size(); ++i)
    {
        new_object.set(i, b1[i] & b2[i]); // the new array is filled with the result of the & operation with each bit in 2 input arrays
    }

    return new_object;
}

// bitwise addition, works only when array sizes match, returns a new object
BitArray operator|(const BitArray &b1, const BitArray &b2)
{
    if (b1.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: first array is empty");
    }

    if (b2.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: second array is empty");
    }

    if (b1.size() != b2.size()) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    BitArray new_object(b1.size());

    for (int i = 0; i < b1.size(); ++i)
    {
        new_object.set(i, b1[i] | b2[i]); // the new array is filled with the result of the | operation with each bit in 2 input arrays
    }

    return new_object;
}

// exclusive-or, works only when array sizes match, returns a new object
BitArray operator^(const BitArray &b1, const BitArray &b2)
{
    if (b1.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: first array is empty");
    }

    if (b2.empty()) // the array empty check
    {
        throw std::invalid_argument("Error: second array is empty");
    }

    if (b1.size() != b2.size()) // the array sizes check
    {
        throw std::runtime_error("Error: array sizes do not match");
    }

    BitArray new_object(b1.size());

    for (int i = 0; i < b1.size(); ++i)
    {
        new_object.set(i, b1[i] ^ b2[i]); // the new array is filled with the result of the ^ operation with each bit in 2 input arrays
    }

    return new_object;
}