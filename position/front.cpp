#include "front.h"
#include "common.h"

int getFrontCorner(int num) //num对应相机拍到的四张图的序号，对应不同的num，后面的计算也不同
{
	//Load raw image
	const char* path0 = "D:\\huangshun\\Documents\\Projects\\border positioning\\images\\front\\front 2.bmp";
	Mat img0 = imread(path0, IMREAD_GRAYSCALE);
	if (img0.empty())
	{
		cout << "!!!Empty image!!!" << endl;
		return -1;
	}
	namedWindow("raw image", WINDOW_NORMAL);
	imshow("raw image", img0);
	waitKey();

	//Dilate to fill gap, erode to erase glitch
	Mat element1 = getStructuringElement(MORPH_CROSS, Size(1, 51));
	Mat element2 = getStructuringElement(MORPH_CROSS, Size(51, 1));
	Mat element3 = getStructuringElement(MORPH_CROSS, Size(31, 1));
	Mat element4 = getStructuringElement(MORPH_CROSS, Size(1, 31));
	Mat img0_morph;
	morphologyEx(img0, img0_morph, MORPH_CLOSE, element1);
	//namedWindow("img0_morph1", WINDOW_NORMAL);
	//imshow("img0_morph1", img0_morph);
	//waitKey();
	morphologyEx(img0_morph, img0_morph, MORPH_CLOSE, element2);
	//namedWindow("img0_morph2", WINDOW_NORMAL);
	//imshow("img0_morph2", img0_morph);
	//waitKey();
	morphologyEx(img0_morph, img0_morph, MORPH_OPEN, element3);
	//namedWindow("img0_morph3", WINDOW_NORMAL);
	//imshow("img0_morph3", img0_morph);
	//waitKey();
	morphologyEx(img0_morph, img0_morph, MORPH_OPEN, element4);
	//namedWindow("img0_morph4", WINDOW_NORMAL);
	//imshow("img0_morph4", img0_morph);
	//waitKey();


	//Filter noise/Normalize/Blur
	//Mat img0_blur;
	//blur(img0_morph, img0_blur, Size(51, 51));//对各种噪声都有一定的抑制作用
	//medianBlur(img0, img0_blur, 51);//对椒盐噪声效果比较好
	//GaussianBlur(img0, img0_blur, Size(51, 51), 0);//对随机噪声比较好，对椒盐噪声效果不好
	//namedWindow("median blur", WINDOW_NORMAL);
	//imshow("median blur", img0_blur);
	//waitKey();

	//Calc thresh image
	Mat img0_thresh;
	int front_thresh = 150; //这个变量可能需要开放给用户设置
	threshold(img0_morph, img0_thresh, front_thresh, 255, THRESH_BINARY);
	namedWindow("thresh", WINDOW_NORMAL);
	imshow("thresh", img0_thresh);
	waitKey();

	//Calc roi mask
	vector<vector<Point> > contours_roi;
	vector<Vec4i> hier_roi;
	findContours(img0_thresh, contours_roi, hier_roi, RETR_TREE, CHAIN_APPROX_NONE);
	int id_contour_roi = -1;
	double area_max_roi = 0;
	for (int i = 0; i < contours_roi.size(); i++)
	{
		double area0 = contourArea(contours_roi[i]);
		if (area0 > area_max_roi)
		{
			area_max_roi = area0;
			id_contour_roi = i;
		}
	}
	Mat mask_img0 = Mat::zeros(img0.size(), CV_8UC1);
	drawContours(mask_img0, contours_roi, id_contour_roi, Scalar::all(255), -1);
	Rect rect_roi = boundingRect(mask_img0);
	namedWindow("mask_img0", WINDOW_NORMAL);
	imshow("mask_img0", mask_img0);
	waitKey();
	namedWindow("border", WINDOW_NORMAL);
	imshow("border", img0(rect_roi));
	waitKey();

	//Invert img0(rect_roi) to get the inner border
	Mat roi_inv_mask;
	threshold(mask_img0(rect_roi), roi_inv_mask, front_thresh, 255, THRESH_BINARY_INV);
	namedWindow("roi_inv_mask", WINDOW_NORMAL);
	imshow("roi_inv_mask", roi_inv_mask);
	waitKey();
	vector<vector<Point> > contours_inv;
	vector<Vec4i> hier_inv;
	findContours(roi_inv_mask, contours_inv, hier_inv, RETR_TREE, CHAIN_APPROX_NONE);
	int id_contour_inv = -1;
	double area_max_inv = 0;
	for (int i = 0; i < contours_inv.size(); i++)
	{
		double area0 = contourArea(contours_inv[i]);
		if (area0 > area_max_inv)
		{
			area_max_inv = area0;
			id_contour_inv = i;
		}
	}
	Mat mask_inv = roi_inv_mask.clone();
	drawContours(mask_inv, contours_inv, id_contour_inv, Scalar::all(0), -1);
	threshold(mask_inv, mask_inv, 200, 255, THRESH_BINARY_INV);
	namedWindow("mask_inv", WINDOW_NORMAL);
	imshow("mask_inv", mask_inv);
	waitKey();

	//Calc canny border
	Mat img0_roi_inv = Mat::zeros(img0.size(), CV_8UC1);
	mask_inv.copyTo(img0_roi_inv(rect_roi));
	namedWindow("img0_roi_inv", WINDOW_NORMAL);
	imshow("img0_roi_inv", img0_roi_inv);
	waitKey();
	Mat canny0 = Mat::zeros(img0.size(), CV_8UC1);
	Canny(img0_roi_inv, canny0, 0, 10, 3);
	namedWindow("canny", WINDOW_NORMAL);
	imshow("canny", canny0);
	waitKey();
	imwrite("./canny.bmp", canny0);

	//Hough Line Transform
	vector<Vec4i> lines0, linesf;
	Vec4i lines_v = Vec4i(0, 0, 0, 0);
	Vec4i lines_h = Vec4i(0,0,0,0); // will hold the results of the detection
	HoughLinesP(canny0, lines0, 1, CV_PI / 180, 150, 500, 500);
	//Filter and Draw the lines
	Mat img0_empty = Mat::zeros(img0.size(), CV_8UC1);
	for (size_t i = 0; i < lines0.size(); i++)
	{
		int x1 = lines0[i][0];
		int y1 = lines0[i][1];
		int x2 = lines0[i][2];
		int y2 = lines0[i][3];
		if (abs(x1 - x2) == 0 || (y1 - y2) / (x1 - x2) > 5)
		{
			if ((lines_v[0] == 0) || (abs(y1 - y2) > abs(lines_v[1] - lines_v[3])))
			{
				lines_v = lines0[i];
			}
		}
		if (abs(y1 - y2) == 0 || (y1 - y2) / (x1 - x2) < 0.2)
		{
			if ((lines_h[0] == 0) || (abs(x1 - x2) > abs(lines_h[1] - lines_h[3])))
			{
				lines_h = lines0[i];
			}
		}
		line(img0_empty, Point(lines0[i][0], lines0[i][1]), Point(lines0[i][2], lines0[i][3]), Scalar(255), 3, LINE_AA);
		//linesf.push_back(Vec4i(lines0[i][0], lines0[i][1], lines0[i][2], lines0[i][3]));
	}
	namedWindow("lines", WINDOW_NORMAL);
	imshow("lines", img0_empty);
	waitKey();
	imwrite("./lines.bmp", img0_empty);
	Point2f corner = getCrossPoint(lines_v, lines_h);
	cout << corner << endl;

	Mat img0_display = img0.clone();
	line(img0_display, Point(lines_v[0], lines_v[1]), Point(lines_v[2], lines_v[3]), Scalar(200), 10, LINE_AA);
	line(img0_display, Point(lines_h[0], lines_h[1]), Point(lines_h[2], lines_h[3]), Scalar(200), 10, LINE_AA);
	circle(img0_display, corner, 40, Scalar(200), -1);
	//add text
	string corner_x(to_string(int(corner.x)));
	string corner_y(to_string(int(corner.y)));
	string text = "(" + corner_x + "," + " " + corner_y + ")";
	Point2i org = Point2i(int(corner.x), int(corner.y) + 100);
	putText(img0_display, text, org, FONT_HERSHEY_COMPLEX, 2, Scalar(200), 5);
	//display result
	namedWindow("Result", WINDOW_NORMAL);
	imshow("Result", img0_display);
	waitKey();
	imwrite("./back corner.bmp", img0_display);

	getchar();
	return 0;
}
