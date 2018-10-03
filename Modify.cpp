#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<cstdio>


using namespace cv;
using namespace std;


int main() {

	cv::Mat image = cv::imread("C:/Users/opo/Desktop/image2/case5.jpg", cv::IMREAD_COLOR);
	cv::Mat dst; //HSV2BGR 로 변환한 사진 저장
	cv::Mat bgr[3];
	cv::Mat hist; //Histogram 계산값 저장

	int channel[] = { 0,1,2 };
	int histSize = 255; //Histogram 가로값의 수
	int count = 0;
	float range[] = { 0,255.0 };
	const float * ranges = range;
	int hist_w = 512; int hist_h = 400;
	int number_bins = 255;
	int bin_w = cvRound((double)hist_w / number_bins);

	int row = image.rows;//470 세로 427

	int col = image.cols;//624 가로 398

	int row_start = (row / 2) - 2;

	int col_start = (col / 2) - 2;

	int red = 0, green = 0, blue = 0;

	for (int i = 0; i < 5; i++) {

		for (int j = 0; j < 5; j++) {

			red += image.at<cv::Vec3b>(i + row_start, j + col_start)[2];

			green += image.at<cv::Vec3b>(i + row_start, j + col_start)[1];

			blue += image.at<cv::Vec3b>(i + row_start, j + col_start)[0];

		}

	}

	red /= 25;

	green /= 25;

	blue /= 25;

	cout << "rgb : " << red * 0.9 << ", " << green * 0.9 << ", " << blue * 0.9 << endl;

	cout << "rgb : " << red * 1.5 << ", " << green * 1.5 << ", " << blue * 1.5 << endl;



	cv::Mat black;

	medianBlur(image, image, 7);
	cv::Mat athimage2;
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
			cv::THRESH_BINARY, 51, 11);
	
		cv::adaptiveThreshold(gray_image, athimage2, 255,
			cv::ADAPTIVE_THRESH_MEAN_C,
			cv::THRESH_BINARY, 401, 60);

		cv::namedWindow("Threshold Image");
		cv::imshow("Threshold Image", athimage);

		cv::namedWindow("Threshold Image2");
		cv::imshow("Threshold Image2", athimage2);

		int key = cv::waitKey(10);


	cv::inRange(athimage2, cv::Scalar((blue*0.1), (green*0.1), (red*0.1)), cv::Scalar((blue*1.3), (green*1.3), (red*1.3)), black);

	//cv::Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));//마스킹

		//cv::erode(athimage2, athimage2,  , cv::Point(-1, -1), 1);//감산연산

	//자르기

	int left = 0, right = 0, top = 0, bottom = 0;

	int read = -1;

	bool flag = false;

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < col; j++) {

			read = athimage2.at<uchar>(i, j);

			if (read == 0) {

				top = i;

				read = -1;

				flag = true;

				break;

			}

		}

		if (flag) {

			flag = false;

			break;

		}

	}

	for (int i = row - 1; i >= 0; i--) {

		for (int j = 0; j < col; j++) {

			read = athimage2.at<uchar>(i, j);

			if (read == 0) {

				bottom = i;

				read = -1;

				flag = true;

				break;

			}

		}

		if (flag) {

			flag = false;

			break;

		}

	}

	for (int i = 0; i < col; i++) {

		for (int j = 0; j < row; j++) {

			read = athimage2.at<uchar>(j, i);

			if (read == 0) {

				left = i;

				read = -1;

				flag = true;

				break;

			}

		}

		if (flag) {

			flag = false;

			break;

		}

	}

	for (int i = col - 1; i >= 0; i--) {

		for (int j = 0; j < row; j++) {

			read = athimage2.at<uchar>(j, i);

			if (read == 0) {

				right = i;

				read = -1;

				flag = true;

				break;

			}

		}

		if (flag) {

			flag = false;

			break;

		}

	}



	//cout << "left" << left << "right" << right << endl;

	//cout << "top" << top << "bottom" << bottom << endl;

	cv::Mat capture = image;

	if (!(left == 0 || right == 0 || top == 0 || bottom == 0))

		capture = image(cv::Range(top, bottom), cv::Range(left, right));


	//색조의 다양성 검사


	unsigned row2 = capture.rows; unsigned col2 = capture.cols; //자른 사진의 크기 저장

	cvtColor(capture, dst, CV_HSV2BGR); //Color 변경
	
	calcHist(&dst, 3, channel, Mat(), hist, 1, &histSize, &ranges,true,false); //Histogram 계산
	
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	
	for (int i = 1; i < number_bins; i++) {	//Histogram 선 그리기
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
	}
	
	for (int i = 0; i < histSize; i++) { //색의 다양성 검출
		if (hist.at<float>(i) > 229) {
			count++;
		}
	}

	Mat blackcapture;
	blackcapture = athimage2(cv::Range(top, bottom), cv::Range(left, right));
	int halfRow = blackcapture.rows/2;
	int halfCol = blackcapture.cols/2;
	int countk = 0;
	int size = halfCol * 2 + halfRow * 2;
	int* savei = new int[1000];
	int* savej = new int[1000];
	for (int j = 0; j < halfRow * 2; j++) {
		for (int i = 0; i < halfCol * 2; i++) {
			read = blackcapture.at<uchar>(j, i);
			if (read == 0) {
				savej[countk] = j;
				savei[countk++] = i;
				//cout << "저장된 값은 : " << j << ",  " << i << endl;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		if (savej[i] != 0) {
			cout <<i<<"번 째"<< " 중심으로 부터 거리는 : " << abs((halfRow - savej[i]))*abs((halfRow - savej[i])) + abs((halfCol - savei[i]))*abs((halfCol - savei[i])) << endl;
		}
	}


	printf("카운트 수 : %d\n", count);
	
	if (count > 10) {
		printf("다양한 색조를 보입니다.");
	}
	else {
		printf("다양한 색조를 보이지 않습니다.");
	}
	cv::namedWindow("HSV2BGR");
	
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	
	cv::namedWindow("blackcapture", CV_WINDOW_AUTOSIZE);
	
	cv::imshow("blackcapture", blackcapture);
	
	cv::imshow("HSV2BGR", dst);
	
	cv::imshow("Histogram",histImage);
	
	cv::imshow("original", image);

	cv::imshow("masking", black);

	cv::imshow("slice", capture);

	
	waitKey();

	return 0;

}
