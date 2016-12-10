/**
* @file detectSquare.cpp
* @brief �摜���ɑ��݂���l�p�`�����o����
* @author 13ej034
* @date 2016.12.11
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace cv;
using namespace std;

int main(int argc, const char* argv[])
{
	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;

	namedWindow("Capture", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	namedWindow("Gray", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	namedWindow("Binaly", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

	auto startTime = chrono::system_clock::now();	// �����J�n����

	while (true){

		// �摜�擾
		Mat frame;
		cap >> frame;

		// �O���[�X�P�[����
		Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);

		// ��l��
		Mat binaly;
		threshold(gray, binaly, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);

		// �֊s���o
		vector<vector<Point>> contours;
		findContours(binaly, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		// �֊s�`��
		for (auto i = contours.begin(); i != contours.end(); i++){
			polylines(frame, *i, true, Scalar(0, 255, 0), 2);
		}

		// �l�p�`�����o
		for (auto i = contours.begin(); i != contours.end(); i++){

			// �֊s�𒼐��ߎ�
			vector<Point> approx;
			approxPolyDP(Mat(*i), approx, 0.01 * arcLength(*i, true), true);

			// �ߎ������������̖ʐς��Z�o
			double area = contourArea(approx);

			// ���_���W��4�� ���� �ʐς�1000�ȏ�Ŏl�p�`�ł���Ɣ��肷��
			if (approx.size() == 4 && area > 1000.0){
				auto checkTime = chrono::system_clock::now();	// �l�p�`�����o��������
				double elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(checkTime - startTime).count();	// �����J�n���Ԃ���l�p�`���o�܂ł̌o�ߎ���

				// �摜��ۑ�
				ostringstream streamOutputFileName;
				streamOutputFileName <<  elapsedTime;
				string outputFileName = "data/detectSquare_" + streamOutputFileName.str() + "msec.png";
				polylines(frame, approx, true, Scalar(255, 0, 0), 2);
				imwrite(outputFileName, frame);
			}
		}

		imshow("Capture", frame);
		imshow("Gray", gray);
		imshow("Binaly", binaly);
		if (waitKey(30) >= 0){
			break;
		}
	}
	return 0;
}