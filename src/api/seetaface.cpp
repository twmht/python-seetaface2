#include <pybind11/pybind11.h>
#include <seeta/FaceDetector2.h>
#include <seeta/Struct_cv.h>
// #include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <pybind11/stl_bind.h>

PYBIND11_MAKE_OPAQUE(std::vector<SeetaRect>);

namespace py = pybind11;

class SeetaImage {
  public:
    SeetaImage(const std::string &path) {
      cv::Mat mat = cv::imread(path.c_str());
      image.reset(new seeta::cv::ImageData(mat));
  }
    seeta::cv::ImageData data() const {
      return *(image.get());
    }
  private:
    std::unique_ptr<seeta::cv::ImageData> image;
};

class FaceDetector {
  public:
    FaceDetector(const std::string& model) {
      detector.reset(new seeta::FaceDetector2(model.c_str()));
    }

    std::vector<SeetaRect> detect(std::shared_ptr<SeetaImage> image) {
      int num;
      SeetaRect *rects = detector->Detect(image->data(), &num);
      std::vector<SeetaRect> result;
      for (int i = 0; i < num; i++) {
        SeetaRect rect = rects[i];
        result.push_back(rect);
      }
      return result;
    }
  private:
    std::unique_ptr<seeta::FaceDetector2> detector;
};

// class PointDetector {
  // public:
    // PointDetector(const std::string& model) {
      // detector.reset(new seeta::FaceDetector2(model.c_str()));
    // }

    // std::vector<SeetaRect> detect(const std::string &path) {
      // cv::Mat mat = cv::imread(path.c_str());
      // seeta::cv::ImageData image = mat;
      // int num;
      // SeetaRect *rects = detector->Detect(image, &num);
      // std::vector<SeetaRect> result;
      // for (int i = 0; i < num; i++) {
        // SeetaRect rect = rects[i];
        // result.push_back(rect);
      // }
      // return result;
    // }
  // private:
    // std::unique_ptr<seeta::PointDetector2> detector;
// };

PYBIND11_MODULE(seetaface, m) {

  py::class_<SeetaRect, std::unique_ptr<SeetaRect>>(m, "SeetaRect")
    .def(py::init<>())
    .def_readwrite("x", &SeetaRect::x)
    .def_readwrite("y", &SeetaRect::y)
    .def_readwrite("width", &SeetaRect::width)
    .def_readwrite("height", &SeetaRect::height);

  py::class_<SeetaImage, std::shared_ptr<SeetaImage>>(m, "SeetaImage")
    .def(py::init<const std::string &>());

  py::bind_vector<std::vector<SeetaRect>>(m, "VectorSeetaRect");

  py::class_<FaceDetector, std::unique_ptr<FaceDetector>>(m, "FaceDetector")
    .def(py::init<const std::string&>())
    .def("detect", &FaceDetector::detect);
}
