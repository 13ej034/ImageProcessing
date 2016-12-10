/**
* @file detectSquare.cpp
* @brief 画像内に存在する四角形を検出する
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

	auto startTime = chrono::system_clock::now();	// 処理開始時間

	while (true){

		// 画像取得
		Mat frame;
		cap >> frame;

		// グレースケール化
		Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);

		// 二値化
		Mat binaly;
		threshold(gray, binaly, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);

		// 輪郭抽出
		vector<vector<Point>> contours;
		findContours(binaly, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		// 輪郭描画
		for (auto i = contours.begin(); i != contours.end(); i++){
			polylines(frame, *i, true, Scalar(0, 255, 0), 2);
		}

		// 四角形を検出
		for (auto i = contours.begin(); i != contours.end(); i++){

			// 輪郭を直線近似
			vector<Point> approx;
			approxPolyDP(Mat(*i), approx, 0.01 * arcLength(*i, true), true);

			// 近似した直線内の面積を算出
			double area = contourArea(approx);

			// 頂点座標が4つ かつ 面積が1000以上で四角形であると判定する
			if (approx.size() == 4 && area > 1000.0){
				auto checkTime = chrono::system_clock::now();	// 四角形を検出した時間
				double elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(checkTime - startTime).count();	// 処理開始時間から四角形検出までの経過時間

				// 画像を保存
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