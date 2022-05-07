// for gpu version test

#include <opencv2/opencv.hpp>
#include <torch/script.h>
#include <torch/torch.h>
#include <iostream>

int main()
{

    std::cout << torch::cuda::is_available() << std::endl;

    torch::DeviceType device_type = at::kCUDA;

    std::cout << device_type << std::endl;

    torch::Tensor tensor = torch::rand({5, 3}).to(device_type);

    std::cout << tensor << std::endl;

    cv::Mat img = cv::imread("../image.jpg");
    cv::imshow("img", img);
    cv::waitKey(0);
    return 0;
}