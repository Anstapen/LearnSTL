/*
Author: Ferdinand Stapenhorst 2022
// https://github.com/FerdinandStapenhorst/LearnSTL
Using STL algorithms with STL containers efficiently
Pleasae try to use STL algorythms and other STL features to solve the below exercises
*/

#include "pch.h" //used for precompiled headers

#pragma region HelperStuff

//Printable concept
template<typename T>
concept Printable = requires(std::ostream & os, T & p)
{
	os << p;		//nee an << operator
	{os << p} -> std::same_as<std::ostream&>; //Return of operator << must be an std::ostream
	p.Print(os);	//need a print methor
};

//Our enhanced Regular Type concept
template<class T>
concept ExtendedRegularType = std::regular<T> and std::totally_ordered<T>; //Our extended regular type adds totally_ordered to the requirement

//Product type.
class Product
{
public:
	Product() noexcept = default;
	Product(const Product& other) noexcept
		: _Name{ other._Name }, _Price{ other._Price }, _FreeDelivery{ other._FreeDelivery } {}
	Product(std::string const name, double const price, bool const freeDelivery) noexcept
		: _Name{ name }, _Price{ price }, _FreeDelivery{ freeDelivery } {}

	//These operators are needed for totally_ordering and equality_comparable
	auto operator<=>(const Product& other) const noexcept = default;
	bool operator==(Product const& other) const noexcept = default;

	//Printing a product
	void Print(std::ostream& os) const
	{
		os << std::format("Name:{}\t Price:{}\t Shipping:{}\n", _Name, _Price, (_FreeDelivery ? "free" : "not free"));
	}

	std::string Name() const {
		return _Name;
	}
	double Price() const
	{
		return _Price;
	}
	bool FreeDelivery() const
	{
		return _FreeDelivery;
	}

private:
	friend std::ostream& operator<<(std::ostream& os, const Product& product);
	std::string _Name{};
	double _Price{ 0 };
	bool _FreeDelivery{ false };
};

static_assert(ExtendedRegularType<Product>); //Make sure Product is am (extended) Regular Type (about Regular Type see https://abseil.io/blog/20180531-regular-types)
static_assert(Printable<Product>); //make sure Product is models the Printable concept

//Print function for a product
std::ostream& operator<<(std::ostream& os, const Product& product)
{
	product.Print(os);
	return os;
}

//Concept for numerics
template <typename T>
concept IsNumeric = std::integral<T> or std::floating_point<T>;

//Bool concept
template <typename T>
concept IsBool = std::common_with<T, bool>;

//Combinde PrintableItem concept
template<typename T>
concept PrintableItem = IsNumeric<T> or std::common_with<T, std::string> or std::is_same_v<std::remove_cv_t<T>, Product>;

//Print formatted string
template<typename... Args>
void PrintF(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

//Print single item
template<PrintableItem T>
void PrintItem(T item) noexcept
{
	if constexpr (IsNumeric<T>) //Numerics and bools are separated by a space
	{
		if constexpr (IsBool<T>)
			std::cout << std::boolalpha;
		std::cout << item;
		std::cout << ' ';
		std::cout << std::noboolalpha;
	}
	else
	{
		//Everything else is just sent to cout
		std::cout << item;
	}
}

//Print vertically two vectors
void PrintTable(std::vector<std::string> v1, std::vector<std::string> v2)
{
	for (int i = 0; i < v1.size(); i++)
	{
		std::cout << v1.at(i) << "\t" << v2.at(i) << std::endl;
	}
}

//printing single items, views and all STL containers
template <typename T>
void Print(T item) {
	if constexpr (std::ranges::input_range<T>)
	{
		using ValueType = std::ranges::range_value_t<decltype(item)>;
		std::for_each(std::begin(item), std::end(item), PrintItem<ValueType>);
	}
	else
	{
		using ValueType = decltype(item);
		PrintItem<ValueType>(item);
	}
}

//Used to print the current exercise to cout
struct ExerciseStart
{
	ExerciseStart(std::string name) : Name{ name }
	{
		std::cout << "" << Name << "" << std::endl << std::endl;
	}
	virtual ~ExerciseStart()
	{
		std::cout << std::endl << "------------------" << std::endl << std::endl;
	}

	std::string Name;
};

#pragma endregion

namespace ContainerAlgorithm {
	void Exercise1()
	{
		// Copy all element from v1 to the end of v2
		ExerciseStart t{ "ContainerAlgorithm:Exercise 1" };
		std::vector<int> v1{ 1,2,3,4,5,6,7,8 };
		std::vector<int> v2{ 10,11,12,13,14,15,16,17,18,19 };

		//Implementation here
		
		// the suboptimal version
		// std::copy(v1.begin(), v1.end(), std::back_inserter(v2));

		// this is the fastest way (I think)
		v2.insert(v2.end(), v1.begin(), v1.end());

		Print(v2);
	}

	void Exercise2()
	{
		// Copy all elements from v1 that are greater than 5 to the end of v2
		ExerciseStart t{ "ContainerAlgorithm:Exercise 2" };

		std::vector<int> v1{ 3,1,2,6,7,8,5,7,9 };
		std::vector<int> v2{ 10,11,12,13,14,15,16,17,18,19 };

		//Implementation here
		std::copy_if(v1.begin(), v1.end(), std::back_inserter(v2), [](int x) {return x > 5;  });

		Print(v2);
	}

	void Exercise3()
	{
		// Move all elements from v1 to the end of v2
		ExerciseStart t{ "ContainerAlgorithm:Exercise 3" };

		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };
		std::vector<int> v2{ 10,11,12,13,14,15,16,17,18,19 };

		//Implementation here
		std::move(std::begin(v1), std::end(v1), std::back_inserter(v2));
		v1.clear();
		Print(v1);
		Print(v2);
	}

	void Exercise4()
	{
		// Copy all elements from v1 in reverse order to the end of v2
		ExerciseStart t{ "ContainerAlgorithm:Exercise 4" };

		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };
		std::vector<int> v2{ 10,11,12,13,14,15,16,17,18,19 };

		//Implementation here
		std::reverse_copy(v1.begin(), v1.end(), std::back_inserter(v2));

		Print(v1);
		Print(v2);
	}

	void Exercise5()
	{
		// Copy the first 5 elements of v1 to the position after position 3 of the same vector v1,
		//             so that{ 1,2,3,4,5,6,7,8,9 } will become{ 1,2,3,1,2,3,4,5,9 }

		ExerciseStart t{ "ContainerAlgorithm:Exercise 5" };
		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };


		// in separate lines
		// std::vector<int>::iterator pos3 = v1.begin() + 3;
		// std::copy_n(v1.begin(), 5, pos3);

		// al in one line
		std::copy_n(v1.begin(), 5, ((std::vector<int>::iterator)v1.begin() + 3));

		//Implementation here

		Print(v1);
	}

	void Exercise6()
	{
		// Increment each number in v1 by 1 so that v1 becomes { 2,3,4,5,6,7,8,9,10 }
		ExerciseStart t{ "ContainerAlgorithm:Exercise 6" };
		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };

		//Implementation here

		// What comes in mind first (probably slow and too complex)
		/*for (auto i = v1.begin(); i < v1.end(); i++)
		{
			*i += 1;
		}*/

		// Maybe transform?
		std::transform(v1.begin(), v1.end(), v1.begin(), [](int x) {return ++x; });

		Print(v1);
	}

	void Exercise7()
	{
		// Count how many elements in v1 are even numbers
		ExerciseStart t{ "ContainerAlgorithm:Exercise 7" };
		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };

		//Implementation here
		unsigned int num = std::count_if(v1.begin(), v1.end(), [](int x) { return x % 2 == 0; });
		Print(num);
	}

	void Exercise8()
	{
		//Calculate the items of v1 that are not in v2 and put those items into a new vector v3
		ExerciseStart t{ "ContainerAlgorithm:Exercise 8" };
		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };
		std::vector<int> v2{ 4,5,6,7,8,9,10,11,12 };

		//Implementation here
		std::vector<int> v3;


		// most complex and hard to read
		/*
		for (auto i = v1.begin(); i < v1.end(); i++)
		{
			// Check if the current element exists in v2
			auto exists = std::find(v2.begin(), v2.end(), *i);
			if (exists == v2.end())
			{
				// The element was found
				v3.push_back(*i);
			}
		}
		*/
		// Better but not the best i guess (is there a dedicated function for stuff like this?)
		std::copy_if(v1.begin(), v1.end(), std::back_inserter(v3), [&v2](int x) { return std::find(v2.begin(), v2.end(), x) == v2.end(); });

		Print(v3);
	}

	void Exercise9()
	{
		//Create a vector<int> v containing the numbers from 10 to 100
		ExerciseStart t{ "ContainerAlgorithm:Exercise 9" };

		//Implementation here


		// wihtout STL
		/*
		std::vector<int> v;
		for (int i = 10; i <= 100; i++)
		{
			v.push_back(i);
		}*/

		// with STL but complex
		/*
		std::vector<int> v(91);
		std::generate(v.begin(), v.end(), [n = 10] ()  mutable { return n++; });
		*/

		// Easiest way imo
		std::vector<int> v(91);
		std::iota(v.begin(), v.end(), 10);

		Print(v);
	}

	void Exercise10()
	{
		//Change the elements in the vector v to reverse order
		ExerciseStart t{ "ContainerAlgorithm:Exercise 10" };
		std::vector<int> v{ 1,2,3,4,5,6,7,8,9 };
		//Implementation here

		Print(v);
	}

	void Exercise11()
	{
		//See this list v of elements in a vector: { "-", "-", "-", "-" ,"-", "-", "-", "-", "#", "#", "#", "#" ,"-", "-", "-", "-" }
		//The list has a size of 16 elements. Four of these elements are consecutively selected - marked with the #-character.
		//The exercise is to move these four elements forwards and backwards within the list v as follows:
		//	- move towards the end to position 15
		//	  Outcome: -----------####-
		//	- move towards the start to position 3
		//	  Outcome: ---####---------
		//	- move to the beginning of the list (= position 0)
		//	  Outcome: ####------------
		//The loop below prepares the list of elements and sets the variable 'newBeginOfRange' to each value 15, 3 and 0.
		//Please implement your solution in the body of the loop below.

		ExerciseStart t{ "ContainerAlgorithm:Exercise 11" };
		std::vector<std::string> v{ "-", "-", "-", "-" ,"-", "-", "-", "-", "#", "#", "#", "#" ,"-", "-", "-", "-" };
		int const sizeOfRange{ 4 };  //The number of consecutive selected items in the range (the # elements)
		int const beginOfRange{ 9 }; //The starting point of the original range
		int const endOfRange{ beginOfRange + sizeOfRange - 1 }; //The end of the range

		Print("Original " + std::to_string(beginOfRange) + ":\t");
		Print(v);

		std::vector<int> newBeginOfRangeList{ 15, 3, 0 }; //three new starting points of the range
		for (auto& newBeginOfRange : newBeginOfRangeList)
		{
			//Re-init the vector
			v = { "-", "-", "-", "-" ,"-", "-", "-", "-", "#", "#", "#", "#" ,"-", "-", "-", "-" };
			Print("Starting at " + std::to_string(newBeginOfRange) + ":\t");

			//Implement here

			Print(v);
		}
	}

	void Exercise12()
	{
		// See this vector: std::vector<std::string> v {"-2","#3","#1","-4","#5","-6","#7","-8","-18","-16","-20","#9","#11","-12","#13","#15","-22"};
		//
		// Reorder the elemnts in v1 so that all selected elements (the # elements) in the range from 1 through 8
		// are moved to the end of that range, and all selected elements in the range from 9 to the end
		// are moved to the top of that range. Keep the relative order of the elemnts preserved.
		// Before		After
		// -2			-2
		// -3			-4
		// #1  <-		-6
		// -4			-8
		// #5  <-		#3  <-
		// -6			#1  <-
		// #7  <-		#5  <-
		// -8			#7  <-
		// -----		-----
		// -18			#9  <-
		// -16			#11 <-
		// -20			#13 <-
		// #9  <-		#15 <-
		// #11 <-		-18
		// -12			-16
		// #13 <-		-20
		// #15 <-		-12
		// -22			-22
		ExerciseStart t{ "ContainerAlgorithm:Exercise 12" };
		std::vector<std::string> shouldBe{ "-2","-4","-6","-8","#3","#1","#5","#7","#9","#11","#13","#15","-18","-16","-20","-12","-22" };
		std::vector<std::string> v{ "-2","#3","#1","-4","#5","-6","#7","-8","-18","-16","-20","#9","#11","-12","#13","#15","-22" };

		//Implement here

		PrintTable(shouldBe, v);
	}

	void Exercise13()
	{
		//See sub tasks below
		ExerciseStart t{ "ContainerAlgorithm:Exercise 13" };

		//The list of existing products
		std::vector<Product> products;
		products.emplace_back(Product{ "P1", 10, true });
		products.emplace_back(Product{ "P5", 5, false });
		products.emplace_back(Product{ "P6", 2, true });
		products.emplace_back(Product{ "P3", 23, false });
		products.emplace_back(Product{ "P4", 69, true });
		products.emplace_back(Product{ "P7", 11, true });
		products.emplace_back(Product{ "P2", 44, false });

		//Exercise 10a: Sort the product list by price

		Print(products);

		//Exercise 10b: Put all elements that are free shipping at the top of the list without breaking the ordering by price

		Print(products);

		//Tast 10c: Find all products that are free shipping and cost less than 20$ and copy them to the vector FreeUnder20
		std::vector<Product> FreeUnder20;
		int const MaxPrice{ 20 };

		Print(FreeUnder20);
	}

	void Exercise14()
	{
		//Remove all uneven numbers from the vector v so that the size of the vector shrinks accordingly
		ExerciseStart t{ "ContainerAlgorithm:Exercise 14" };
		std::vector<int> v{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 };

		Print(v);
	}

	void Exercise15()
	{
		//Add the element newItem into the vector while keeping the vector ordered.
		//Do not use a static insert position but calculate the position where the new item should be inserted
		ExerciseStart t{ "ContainerAlgorithm:Exercise 15" };
		using Vstr = std::vector<std::string>;
		Vstr v{ "A","B","C","D","F","G","H" };
		std::string newItem{ "E" };

		Print(v);
		assert(std::ranges::is_sorted(v));
	}

	void Exercise16()
	{
		ExerciseStart t{ "ContainerAlgorithm:Exercise 16" };

		// Check out this programmking challenge: https://www.hackerrank.com/contests/hourrank-31/challenges/hanging-posters/problem
		// See the solution below and try to find at least one solution that uses STL algorithm(s)

		int h = 6;
		std::vector<int> wallPoints{ 22,33,19,74 };
		std::vector<int> lengths{ 2,3,5,6 };

		int accumulator = 0;
		for (int i = 0; i < wallPoints.size(); ++i)
			accumulator = std::max(accumulator, wallPoints[i] - lengths[i] / 4);
		auto result = std::max(0, accumulator - h);

		//Implement your solution here

	}
}

namespace STLRanges
{
	namespace views = std::ranges::views;

	void Exercise1()
	{
		ExerciseStart t{ "STLViews:Exercise 1" };
		std::vector<int> v{ 1,2,3,4,5,6,7,8,9 };

		//Use views to solve the following exercises.

		//Example: create a view of all elements in v:
		auto view = v | views::all;
		Print(view);

		//1): create a view1 of the first 5 elements of v in reverse order.

		//Print(view1);

		//2): create a view2 that filters out all uneven numbers out of v.

		//Print(view2);

		//3): create a view3 that calculates the square of all even numbers in v.

		//Print(view3);
	}
}

namespace Misc {
	void Exercise1()
	{
		//Assuming int x = -1 and unsigned y = 1: write code of the comparison between both
		ExerciseStart t{ "Misc:Exercise 1" };
		int x{ -1 };
		unsigned y{ 1 };
		bool b{ false };

		//implement here, like:
		// b = comparison( x , y );

		//Compare equal

		Print(b);

		//Compare not-equal

		Print(b);

		//compare  x is less than y

		Print(b);

		//compare x is less or equal to y

		Print(b);

		//compare x is greater than y

		Print(b);

		//compare x is greater or equal than y

		Print(b);
	}

	void Exercise2()
	{
		//See the following class Fraction. It represents a Fraction type that stores the denominator and divider.
		//Implement the operator overloads so that the comparisons below prints correctly (uncomment to verify your code).
		//Try to implement as less operator overloads as possible.

		ExerciseStart t{ "Misc:Exercise 2" };

		class Fraction {
		public:
			Fraction() = default;
			Fraction(const Fraction& other) noexcept = default;
			Fraction(int denominator, int divisor) : Denominator{ denominator }, Divisor{ divisor } {}

			//Implement the following comparison operators, so that they return the correct values
			auto operator<=>(const Fraction& rhs) const noexcept = default;
			bool operator==(const Fraction& rhs) const noexcept = default;

		private:

			long Denominator{ 0 };
			long Divisor{ 1 };
		};

		Fraction a{ 10, 15 };
		Fraction b{ 2, 3 };
		Fraction c{ 5, 3 };

		Fraction d{ 1, 3 };
		Fraction e{ 2, 6 };

		Fraction f{ 1, 5 };
		Fraction g{ 2, 10 };

		//Un-comment the following to check your results
		return;

		PrintF("a < c  should be true and is: {}\n", (a < c)); assert((a < c));
		PrintF("a > c  should be false and is: {}\n", (a > c)); assert(!(a > c));
		PrintF("c < a  should be false and is: {}\n", (c < a)); assert(!(c < a));
		PrintF("a == b should be true and is: {}\n", (a == b)); assert((a == b));
		PrintF("a != b should be false and is: {}\n", (a != b)); assert(!(a != b));
		PrintF("a <= b should be true and is: {}\n", (a <= b)); assert((a <= b));
		PrintF("a <= c should be true and is: {}\n", (a <= c)); assert((a <= c));
		PrintF("a >= c should be false and is: {}\n", (a >= c)); assert(!(a >= c));
		PrintF("c >= a should be true and is: {}\n", (c >= a)); assert((c >= a));
		PrintF("c <= a should be false and is: {}\n", (c <= a)); assert(!(c <= a));
		PrintF("a != c should be true and is: {}\n", (a != c)); assert((a != c));
		PrintF("d == e should be true and is: {}\n", (d == e)); assert((d == e));
		PrintF("f == g should be true and is: {}\n", (f == g)); assert((f == g));

		assert(ExtendedRegularType<Fraction>);  //Uncomment to check if the Fraction is an extended Regular Type
	}

	//See the template function signature below. Write a binary search in the body of that function which returns an iterator
	//pointing to the first element in the range [first, last) that satisfies element >= value,
	//or last if no such element is found. The range[first, last) must be a sorted range.

	template <std::forward_iterator ForwardIterator, typename ValueType>
	ForwardIterator BinarySearch(ForwardIterator first, const ForwardIterator last, const ValueType& value) {
		//Implement here

		return first;
	}

	void Exercise3()
	{
		ExerciseStart t{ "Misc:Exercise 3" };
		//Implement the binary search above
		std::vector<int> v{ 1,2,3,4,5,6,7,8,9,10 };
		auto pos = BinarySearch(std::begin(v), std::end(v), 5);
	}
}

int main()
{
	{ //Container stuff
		using namespace ContainerAlgorithm;
		Exercise1();
		Exercise2();
		Exercise3();
		Exercise4();
		Exercise5();
		Exercise6();
		Exercise7();
		Exercise8();
		Exercise9();
		Exercise10();
		Exercise11();
		Exercise12();
		Exercise13();
		Exercise14();
		Exercise15();
		Exercise16();
	}

	{  //Misc stuff
		using namespace Misc;
		Exercise1();
		Exercise2();
		Exercise3();
	}

	{
		//views
		using namespace STLRanges;
		Exercise1();
	}
}