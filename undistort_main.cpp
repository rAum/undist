#include <iostream>
#include <array>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

cv::Mat image;

constexpr const char* kMainWindow = "Undistort";
constexpr int max_slide = 2000;
constexpr int slide_center = max_slide / 2;
std::array<int, 8> dist_coeffs;
std::vector<float> coeffs(dist_coeffs.size());

constexpr float slide_to_float(int val) {
  return (2.f * static_cast<float>(val) / max_slide) - 1.f;
}

bool redraw = false;
void display(int value, void* other) {
  std::cout << "dist_coeffs = { ";
  for (int i = 0; i < dist_coeffs.size(); ++i) {
    coeffs[i] = slide_to_float(dist_coeffs[i]);
    if (i != 0)
      std::cout << ", ";
    std::cout << coeffs[i];
  }
  std::cout << " }" << std::endl;
  redraw = true;
}

int main(int argc, char**argv) {
  if ( argc != 2 ) {
    std::cout << "usage: undistort <image>" << std::endl;
    return -1;
  }

  image = cv::imread(argv[1]);
  if ( !image.data ) {
    std::cout << "failed to open image " << argv[1] << std::endl;
    return -1;
  }

  for (int i = 0; i < dist_coeffs.size(); ++i)
    dist_coeffs[i] = slide_center;

  cv::namedWindow("Undistortion", cv::WINDOW_AUTOSIZE );
  cv::imshow(kMainWindow, image);

  for (int i = 0; i < dist_coeffs.size(); ++i) {
    cv::createTrackbar("Coefficent " + std::to_string(i+1), kMainWindow, &dist_coeffs[i], max_slide, display);
  }

  cv::Mat undistorted;
  const float width  = image.cols;
  const float height = image.rows;
  // assuming very simple model
  const  cv::Matx33f K = {width,  0.f,    width/2,
                          0.f,    height, height/2,
                          0.f,    0.f,    1.f};
  while (true) {
    if (redraw) {
      cv::undistort(image, undistorted, K, coeffs);
      cv::imshow(kMainWindow, undistorted);
      redraw = false;
    }
    if (cv::waitKey(50) == 'q') break;
  }

  return 0;
}
