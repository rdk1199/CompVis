#include <iostream>

#include "Math/numbers.h"
#include "Math/linalg.h"

#include "ImagProc/image.h"

using std::cout;
using std::endl;

using std::vector;

int main()
{
	
	vector<complex> t1 = { complex{4.0, -1.0}, complex{1.5, -3.5} };
	vector<complex> t2 = { complex{-.75, 2.3}, complex{-8, .9} };

	//cout << (t1 * t2) + 5.0f * (t1 - t2) * (t2 + t1) << endl;

	complex c{ 1.0, 2.0 };

	//cout << c;

	//c = 0.0f;

	//cout << endl << c;

	Image sunset("Images/Sunset.png");
	Image galaxy("Images/Galaxy.png");
	Image colors("Images/Colors.png");
	//sunset.gain(2.0f);
	//sunset.gain({ 2, 1, 1, 1 });
//	sunset.bias(100.0f);

	Image padded = colors.clamp_pad(100);

	vector<ImgHistEntry> histogram = colors.histogram();
	vector<ImgHistEntry> cum_dist = colors.cum_dist();

	padded.save("Images/padded_colors.png");

	Image equalized = colors.hist_equalize();
	equalized.save("Images/equalized_colors.png");

	Image eq_sunset = sunset.hist_equalize();
	eq_sunset.save("Images/equalized_sunset.png");

	Image eq_galaxy = galaxy.hist_equalize();
	eq_galaxy.save("Images/equalized_galaxy.png");

	return 0;
}
