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

    cv::Mat frame;
    const std::string wName = "Video by frames";
    namedWindow(wName, cv::WINDOW_AUTOSIZE);

    while (true)
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


void Canny2_7(const std::string &filename)
{
    cv::Mat img_rgb, img_gry, img_cny;
    cv::namedWindow("Example Gray", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Example Canny", cv::WINDOW_AUTOSIZE);

    img_rgb = cv::imread(filename);
    cv::cvtColor(img_rgb, img_gry, cv::COLOR_BGR2GRAY);
    cv::imshow("Example Gray", img_gry);

    cv::Canny(img_gry, img_cny, 10, 100, 3, true);
    cv::imshow("Example Canny", img_cny);

    while (true)
    {
        const char c = (char)cv::waitKey(0);
        const char esc = 27;
        if (c == esc)
        {
            break;
        }
    }
}

void PaintBlackRegion(cv::Mat img, const std::size_t y, const std::size_t x, const std::size_t regionSize = 5)
{
    cv::Vec3b black(0,0,0);

    for (std::size_t i = (x-regionSize); i <= (x+regionSize); i++)
    {
        for (std::size_t j = (y-regionSize); j <= (y+regionSize); j++)
        {
            img.at<cv::Vec3b>(i,j) = black;
        }
    }
}


void ShowColoredRect(cv::Rect rect, cv::Vec3b color)
{
    std::cout << "Color is: " << color << std::endl;
    cv::Mat m(rect.width, rect.height, CV_8UC3, color);

    const std::string wName = "Colored rectangle";
    cv::namedWindow(wName, cv::WINDOW_AUTOSIZE);
    cv::imshow(wName, m);
}


void GettingSettingPixels2_9(const std::string &filename)
{
    const std::string wName = "Example";
    cv::namedWindow(wName, cv::WINDOW_AUTOSIZE);

    cv::Mat img_rgb = cv::imread(filename);
    std::cout << "image size: " << img_rgb.size << std::endl;

    std::size_t y = 32, x = 117;
    cv::Vec3b pixelIntensity = img_rgb.at<cv::Vec3b>(y, x);

    std::cout << "BGR code of pixel (" << y << ", " << x << ") is: (" \
              << (int) pixelIntensity[0] << ", " \
              << (int) pixelIntensity[1] << ", " \
              << (int) pixelIntensity[2] << ")" << std::endl;

    // PaintBlackRegion(img_rgb, y, x);
    ShowColoredRect(cv::Rect(1, 1, 100, 100), pixelIntensity);
    cv::imshow(wName, img_rgb);

    while (true)
    {
        const char c = (char)cv::waitKey(0);
        const char esc = 27;
        if (c == esc)
        {
            break;
        }
    }
}

void WaitEscKey()
{
    cv::waitKey(0);
    while (true)
    {
        const char c = (char)cv::waitKey(0);
        const char esc = 27;
        if (c == esc)
        {
            break;
        }
    }
}


cv::Vec3b DetectColorCentroid(const cv::Mat img)
{
    cv::Mat img_ = img.clone();
    img_.convertTo(img_, CV_32F);
    cv::Mat flattened_img = img_.reshape(0, 1);
    const int K = 1;
    const int attempts = 10;
    const cv::TermCriteria terminationCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0);
    cv::Mat centers, labels;
    double compactness = kmeans(flattened_img, K, labels, terminationCriteria, attempts, cv::KMEANS_RANDOM_CENTERS, centers);
    std::cout << "centers: " << centers << std::endl;

    // TODO: implicit type conversion...
    cv::Vec3b color = centers.at<cv::Vec3f>(0);
    return color;
}


void KMeansOfRegion(const std::string &filename)
{
    cv::Mat img_bgr = cv::imread(filename);

    const std::string wName = "Image";
    cv::namedWindow(wName, cv::WINDOW_AUTOSIZE);

    const std::size_t step = 200;
    const std::size_t w = img_bgr.size().width;
    const std::size_t h = img_bgr.size().height;
    for (std::size_t i = 0; i+step < w; i += step)
    {
        for (std::size_t j = 0; j+step < h; j += step)
        {
            cv::Rect r(i, j, step, step);
            cv::Mat cropped_img(img_bgr, r);
            cv::imshow(wName, cropped_img);

            cv::Vec3b color = DetectColorCentroid(cropped_img);

            ShowColoredRect(cv::Rect(1,1, 200, 200), color);
            WaitEscKey();
        }
    }
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: DisplayImage.out <Image_Path>" << std::endl;
        return -1;
    }

    KMeansOfRegion(argv[1]);

    return 0;
}
