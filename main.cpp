#include <iostream>

#include "Math/numbers.h"
#include "Math/linalg.h"

using std::cout;
using std::endl;

using std::vector;

int main()
{
	
	vector<complex> t1 = { complex{4.0, -1.0}, complex{1.5, -3.5} };
	vector<complex> t2 = { complex{-.75, 2.3}, complex{-8, .9} };

	cout << (t1 * t2) + 5.0f * (t1 - t2) * (t2 + t1) << endl;

	return 0;
}
