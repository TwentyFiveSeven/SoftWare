#include <iostream>

using namespace std;

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

int main()
{
	cv::Mat image;
	image = cv::imread("C:/Users/opo/Desktop/image2/test3.jpg", 1);

	medianBlur(image, image,7);

	while (true)
	{
		cv::namedWindow("image_001");
		cv::imshow("image_001", image);

		cv::Mat gray_image;
		cv::cvtColor(image, gray_image, CV_BGR2GRAY); // 흑백영상으로 변환

		cv::namedWindow("Gray_image");
		cv::imshow("Gray_image", gray_image);  // 흑백 영상 출력

		// get a adaptive threshold image
		cv::Mat athimage;
		cv::adaptiveThreshold(gray_image, athimage, 255,
			cv::ADAPTIVE_THRESH_MEAN_C,
			cv::THRESH_BINARY, 401, 60);

		cv::namedWindow("Threshold Image");
		cv::imshow("Threshold Image2", athimage);

		int key = cv::waitKey(10);

		if (key == 27)      //ESC key
			break;
	}

	return 0;
}
