#include <iostream>

#include "Math/numbers.h"
#include "Math/linalg.h"
#include "Math/functions.h"
#include "Math/sparse.h"
#include "Math/math_tests.h"
#include "Math/rand_util.h"

#include "ImagProc/image.h"

#include "ModelFitting/kernel_regression.h"
#include "ModelFitting/demin.h"

using std::cout;
using std::endl;

using std::vector;

int main()
{
	rand_init(time(0));

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
	/*
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

	Image bird_median = bird.median(2);
	bird_median.save("Images/bird_median.png");

	Image color_median = colors.median(3);
	color_median.save("Images/color_median.png");

	
	Image color_bilat = colors.bilateral(4, 1.0, 1.0);
	color_bilat.save("Images/color_bilat.png");

	Image bird_bilat = bird.bilateral(4, 1.0, 1.0);
	bird_bilat.save("Images/bird_bilat.png");
	

	Image bird_iter_bilateral = bird.iter_bilateral(3, 4, 1.0, 1.0);
	bird_iter_bilateral.save("Images/bird_iter_bilat.png");
	
	

	
	Image bird_max_mono = bird.max_monochrome();	
	Image bird_mean_mono = bird.mean_monochrome();	
	Image bird_grayscale = bird.grayscale();
	Image bird_binarize = bird_grayscale.binarize(0.5f);

	bird_max_mono.save("Images/bird_max_mono.png");
	bird_mean_mono.save("Images/bird_mean_mono.png");
	bird_grayscale.save("Images/bird_grayscale.png");
	bird_binarize.save("Images/bird_binary.png");

	Image col_invert = colors.invert();
	Image bird_invert = bird.invert();

	col_invert.save("Images/colors_inverted.png");
	bird_invert.save("Images/bird_inverted.png");
	

	Image bird_dilate = bird_binarize.dilate(4);
	bird_dilate.save("Images/bird_dilate.png");

	Image bird_erode = bird_binarize.erode(4);
	bird_erode.save("Images/bird_erode.png");

	Image bird_maj = bird_binarize.majority(4);
	bird_maj.save("Images/bird_maj.png");

	Image bird_open = bird_binarize.open(4);
	Image bird_close = bird_binarize.close(4);

	bird_open.save("Images/bird_open.png");
	bird_close.save("Images/bird_close.png");

	Image bird_dist = bird_binarize.manhattan_dist_trans();
	bird_dist.save("Images/bird_dist.png");*/

	//Image bird_upscale = bird.bicubic_upscale(3);
	//bird_upscale.save("Images/bird_upscale.png");

	//Image bird_downscale = bird.bicubic_decimate(3, -0.5);
	//bird_downscale.save("Images/bird_downscale.png");

	//Image bird_scaled = bird_upscale.bicubic_decimate(3);
	//bird_scaled.save("Images/bird_scaled.png");

	//Image bird_scale_diff = bird - bird_scaled;
	//bird_scale_diff.abs();
	//bird_scale_diff.save("Images/bird_scale_diff.png");

	//Image bird_trans = bird.translate(12, 12);
//	bird_trans.save("Images/bird_translate.png");
/*
	Image bird_rot_45 = bird.rotate(45);
	bird_rot_45.save("Images/bird_rot_45.png");

	Image bird_scale = bird.scale(0.5, 1.0f);
	bird_scale.save("Images/bird_scale.png");
	*/

/*	Matrix<float> mat({

		{2, -1, 0},
		{-1, 2, -1},
		{0, -1, 2}
		});


	//cout << mat.row_ech() << endl;
	cout << mat.red_row_ech() << endl;
	cout << mat.id_augment() << endl;
	cout << mat.id_augment().red_row_ech() << endl;
	cout << mat.inverse() << endl;
	cout << mat.det() << endl;*/

/*
vector<vector<float>> in = { {0.0, 0.0}, {0.0, 1000.0}, {1000.0, 0.0}, {1000.0, 1000.0} };
vector<Color> out = { Color::black(), Color::red(), Color::green(), Color::blue() };

ExactKernelRegression<vector<float>, Color> f(in, out, 1000, gaussian_basis);
RidgeKernelRegression<vector<float>, Color> ridge_f(in, out, 1, 1000, gaussian_basis);
NWKernelRegression<vector<float>, Color> nw_f(in, out, 500, gaussian_basis);

Image orig(1001, 1001, Color::black());
orig[0][0] = { 0.0, 0.0, 0.0, 255.0 };
orig[0][1000] = { 255.0, 0.0, 0.0, 255.0 };
orig[1000][0] = { 0.0, 255.0, 0.0, 255.0 };
orig[1000][1000] = { 0.0, 0.0, 255.0, 255.0 };

Image ridge_interp(1001, 1001);
Image interp(1001, 1001);
Image nw_interp(1001, 1001);

for (int i = 0; i < 1001; i++)
{
	for (int j = 0; j < 1001; j++)
	{
		nw_interp[i][j] = nw_f({ i,j });
		nw_interp[i][j].a = 255.0f;
		//interp[i][j] = f({ i,j });
	}
}


orig.save("Images/regression_orig.png");
//interp.save("Images/regression_interp.png");
//ridge_interp.save("Images/regression_ridge_interp.png");

nw_interp.save("Images/regression_nw_interp.png");
*/

Image bird_sample(bird.width(), bird.height(), Color::black());

vector<std::pair<int, int>> bird_in_int;
vector<vector<float>> bird_in;
vector<Color> bird_out;

for (int i = 0; i < bird_sample.width(); i += 1)
{
	for (int j = 0; j < bird_sample.height(); j += 1)
	{
		if (g_rand.flip(.3f))
		{
			bird_sample[i][j] = bird[i][j];
			bird_in.push_back({ i,j });
			bird_in_int.push_back({ i,j });
			bird_out.push_back(bird[i][j]);
		}
	}
}

/*
ExactKernelRegression<vector<float>, Color>bird_f(bird_in, bird_out, 100, gaussian_basis);

Image bird_regress(bird.width(), bird.height(), Color::black());

for (int i = 0; i < bird.width(); i++)
{
	for (int j = 0; j < bird.height(); j++)
	{
		bird_regress[i][j] = bird_f({ i,j });
	}
}*/


/*
RidgeKernelRegression<vector<float>, Color> bird_ridge_f(bird_in, bird_out, 1, 100, gaussian_basis);

Image bird_ridge_regress(bird.width(), bird.height(), Color::black());

for (int i = 0; i < bird.width(); i++)
{
	for (int j = 0; j < bird.height(); j++)
	{
		bird_ridge_regress[i][j] = bird_ridge_f({ i,j });
	}
}*/

/*
NWKernelRegression<vector<float>, Color> bird_nw_f(bird_in, bird_out, 100, gaussian_basis);

Image bird_nw_regress(bird.width(), bird.height(), Color::black());

for (int i = 0; i < bird.width(); i++)
{
	for (int j = 0; j < bird.height(); j++)
	{
		bird_nw_regress[i][j] = bird_nw_f({ i,j });
		bird_nw_regress[i][j].a = 255.0f;
	}
}*/

DEMinimizer<Color> bird_demin(bird.width(), bird.height(), bird_in_int, bird_out, 1, 4, EnergyFunction::membrane, .0000001, 1000);

Image bird_demin_img(bird.width(), bird.height());


for (int i = 0; i < bird.width(); i++)
{
	for (int j = 0; j < bird.height(); j++)
	{
		bird_demin_img[i][j] = bird_demin(i, j);
		//bird_demin_img[i][j].a = 255.0f;
	}
}

bird_demin_img.save("Images/bird_demin_low_c.png");

Image::abs_diff(bird, bird_demin_img).save("Images/bird_demin_diff.png");


/*
int corner_size = 100;
vector<std::pair<int, int>> in = { {10, 10}, {10, corner_size - 10}, { corner_size - 10, 10 }, { corner_size - 10, corner_size - 10 }};
vector<Color> out =  { Color::black(), Color::red(), Color::green(), Color::blue() };

Image small_col_corner(corner_size + 1, corner_size + 1, Color::zero());
small_col_corner[10][10] = { 255.0, 0.0, 0.0, 255.0 };
small_col_corner[10][corner_size-10] = { 255.0, 0.0, 0.0, 255.0 };
small_col_corner[corner_size-10][10] = { 0.0, 255.0, 0.0, 255.0 };
small_col_corner[corner_size-10][corner_size-10] = { 0.0, 0.0, 255.0, 255.0 };

DEMinimizer<Color> demin(corner_size + 1, corner_size + 1, in, out, 1, 1, EnergyFunction::membrane, .00000001, 120000);

Image demin_interp(corner_size + 1, corner_size + 1);

for (int i = 0; i < corner_size + 1; i++)
{
	for (int j = 0; j < corner_size + 1; j++)
	{
		demin_interp[i][j] = demin( i,j );
	}
}


small_col_corner.save("Images/corner_orig.png");
demin_interp.save("Images/corner_demin.png");

*/
bird_sample.save("Images/bird_sample.png");
//bird_regress.save("Images/bird_regress.png");
//bird_ridge_regress.save("Images/bird_ridge_regress.png");
//bird_nw_regress.save("Images/bird_nw_regress.png");


//cout << sum_mat[400][400] << endl;
//cout << sum_mat[399][400] << endl;

//test_gauss_elimination();
//test_gauss_seidel();

	return 0;
}
