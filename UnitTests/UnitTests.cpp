/*
  U n i t T e s t s . c p p
*/

// Modern C++
#include <span>

// Old fashion C
#include <cassert>

// Unit test framework
#include "CppUnitTest.h"

// Stuff being tested
#include "../OldRand.h"
#include "../MappedFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	// **********************************************************
	private:
		template<typename SeedT>
		void FillArrayRand(span<SeedT> result, SeedT firstSeed)
		{
			result[0] = RandTurboPascal(firstSeed);
			for (unsigned i = 1; i < result.size(); ++i)
				result[i] = RandTurboPascal(result[i - 1]);
		}

	public:
		TEST_METHOD(RandTurboPascal0A)
		{
			unsigned ans0[] =
				{ 1, 134775814, 3698175007, 870078620, 1172187917, 
					2884733762, 1368768587, 694906232, 1598751577, 1828254910 };
			constexpr unsigned numElem0 = sizeof(ans0) / sizeof(unsigned);

			unsigned ans100[] =
				{ 592679413, 4287376842, 61410547, 1016237248, 628080577, 
					1612071622, 2702009823, 885127516, 3074248397, 255606274 };
			constexpr unsigned numElem100 = sizeof(ans100) / sizeof(unsigned);

			unsigned result0[numElem0];
			FillArrayRand<unsigned>(result0, 0);
      if (!equal(begin(result0), end(result0), begin(ans0), end(ans0)))
				abort();

			unsigned result100[numElem100];
			FillArrayRand<unsigned>(result100, 100);
      if (!equal(begin(result100), end(result100), begin(ans100), end(ans100)))
				abort();
		}

		// **********************************************************
		TEST_METHOD(MappedFile0A)
		{
			constexpr size_t fileSize = 1024 * 100'000;
			const string fileName{"C:\\WorkSpace\\MapFile0A.data"};

			MappedFileT file(fileName.c_str(), fileSize);
			std::byte* fileByte = (std::byte*)file();

			for (size_t i{ 0 }; i < fileSize; ++i) {
				size_t value = (i % 256);
				assert(value < 256);
        fileByte[i] = (std::byte)value;
			}
		}
	};
}