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
	Image bird("Images/Bird.png");

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

	Image gauss_text = text.gauss_blur();
	gauss_text.save("Images/gauss_text.png");

	Image sobel_colors = 10*colors.sobel();
	sobel_colors.save("Images/sobel_colors.png");

	Image corner_colors = 10 * colors.corner();
	corner_colors.save("Images/corner_colors.png");

	Image corner_sunset = 10 * sunset.corner();
	corner_sunset.save("Images/corner_sunset.png");

	Image box_colors = colors.box_filter(11);
	box_colors.save("Images/box_colors.png");

	Image corner_galaxy = 10 * galaxy.corner();
	corner_galaxy.save("Images/corner_galaxy.png");

	Image color_integral = colors.integral().normalize();
	color_integral.save("Images/color_integral.png");

	Image galaxy_integral = galaxy.integral().normalize();
	galaxy_integral.save("Images/galaxy_integral.png");
	
	Image bird_sharpen = bird.sharpen(3.0f);
	bird_sharpen.save("Images/bird_sharpen.png");


	return 0;
}
