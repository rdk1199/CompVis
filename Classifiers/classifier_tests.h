

#include "bayes_classifier.h"
#include "../DataHandling/gs_image_loader.h"


void test_naive_bayes_mnist(std::string data_path) //path to parsed mnist data
{
	std::string train_path = data_path + "/training/tr_img_";
	std::string test_path = data_path + "/test/test_img_";

	std::vector<std::vector<double>> tr_data;
	std::vector<int> tr_labels;

	for (int i = 0; i < 60000; i++) //60000 = size of mnist training set
	{
		std::cout << "\r loading mnist training image " << i + 1 << "/60000         ";
		int label = 0;
		tr_data.push_back(GSImageLoader::load_as_vector(train_path + std::to_string(i), label));
		tr_labels.push_back(label);
	}

	std::cout << std::endl;
	NaiveBayesClassifier<double, int> classifier(28 * 28, { 0,1,2,3,4,5,6,7,8,9 }, tr_data, tr_labels, { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 });

	int matches = 0;

	for (int i = 0; i < 10000; i++) //10000 = size of mnist test set
	{
		std::cout << "\r running Naive Bayes classifier on test image " << i + 1 << "/10000         ";
		int correct_label = 0;
		std::vector<double> img_vec = GSImageLoader::load_as_vector(test_path + std::to_string(i), correct_label);

		int output = classifier(img_vec);

		if (output == correct_label)
		{
			matches++;
		}
	}
	std::cout << std::endl;

	std::cout << "Naive Bayes Classifier correctly classified " << matches << " out of 10000 test images" << std::endl;
}


