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
	Image text("Images/Text.png");

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
	Image::blend(sunset,eq_sunset,.8f).save("Images/equalized_sunset.png");

	Image eq_galaxy = galaxy.hist_equalize();
	Image::blend(galaxy, eq_galaxy, .8f).save("Images/equalized_galaxy.png");


	Matrix<float> gauss_filter(
		{ {1,4,6,4,1},
		{4,16,24,16,4},
		{6,24,36,24,6},
		{4,16,24,16,4},
		{1,4,6,4,1} }

	);

	Matrix<float> id_filter(
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		}
	);

	gauss_filter = (1.0f / 256.0f) * gauss_filter;

	Image gauss_text = text.lin_filter(gauss_filter);

	gauss_text.save("Images/gauss_text.png");
	Image id_text = text.lin_filter(id_filter);

	id_text.save("Images/id_text.png");

	Image id_colors = colors.lin_filter(id_filter);
	id_colors.save("Images/id_colors.png");


	return 0;
}
