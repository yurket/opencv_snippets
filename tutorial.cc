#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>



void ShowImage2_1(const std::string &filename)
{
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    cv::Mat crop(image, cv::Rect(1, 1, 100, 100));

    if (!image.data)
    {
        std::cout << "No image data \n";
        return;
    }

    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("All Image", image);
    cv::imshow("Cropped ", crop);

    cv::waitKey(0);
}



void ShowVideo2_3(const std::string &filename)
{
    cv::VideoCapture capture(filename);
    // capture.open(filename);

    cv::Mat frame;
    const std::string wName = "Video by frames";
    namedWindow(wName, cv::WINDOW_AUTOSIZE);

    while(true)
    {
        capture >> frame;
        if (frame.empty())
        {
            std::cout << "No frame data -> end of video \n";
            break;
        }
        cv::imshow(wName, frame);
        if (cv::waitKey(33) >= 0)
        {
            break;
        }
    }
}


int main(int argc, char** argv)
{
    if ( argc != 2 )
    {
        std::cout << "usage: DisplayImage.out <Image_Path>" << std::endl;
        return -1;
    }

    // ShowImage2_1(argv[1]);
    ShowVideo2_3(argv[1]);

    return 0;
}
