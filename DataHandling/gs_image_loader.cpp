#include "gs_image_loader.h"

#include <fstream>

#include <cstdint>

using std::ifstream;
using std::fstream;
using std::cout;
using std::endl;
using std::vector;

Image GSImageLoader::load_as_image(const std::string& file, int& label)
{
	ifstream fi;

	try
	{
		fi.open(file.c_str());
	}
	
	catch(std::ios_base::failure)
	{
		cout << "GSImageLoader could not open " << file << endl;
		exit(1);
	}

	char label_rd;

	uint32_t width;
	uint32_t height;

	fi.read(&label_rd, 1);
	label = static_cast<int>(label_rd);

	//some C-style pointer deception
	fi.read(reinterpret_cast<char*>(&width), 4);
	fi.read(reinterpret_cast<char*>(&height), 4);

	Image out(width, height);


	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			unsigned char value;
			fi.read(reinterpret_cast<char*>(&value), 1);

			out[i][j] = Color::gray(static_cast<double>(value));
		}
	}

	fi.close();

	return out;
}

vector<double> GSImageLoader::load_as_vector(const std::string& file, int& label)
{
	ifstream fi;

	try
	{
		fi.open(file.c_str());
	}

	catch (std::ios_base::failure)
	{
		cout << "GSImageLoader could not open " << file << endl;
		exit(1);
	}

	char label_rd;

	uint32_t width;
	uint32_t height;

	fi.read(&label_rd, 1);
	label = static_cast<int>(label_rd);

	//some C-style pointer deception
	fi.read(reinterpret_cast<char*>(&width), 4);
	fi.read(reinterpret_cast<char*>(&height), 4);

	vector<double> out;

	for (int i = 0; i < width * height; i++)
	{
		unsigned char value;
		fi.read(reinterpret_cast<char*>(&value), 1);

		out.push_back(static_cast<double>(value));
	}
	

	fi.close();

	return out;
}