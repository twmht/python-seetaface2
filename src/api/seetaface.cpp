#include <pybind11/pybind11.h>
#include <seeta/FaceDetector2.h>
#include <seeta/PointDetector2.h>
#include <seeta/Struct_cv.h>
// #include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <pybind11/stl_bind.h>
#include <ndarray_converter.h>

PYBIND11_MAKE_OPAQUE(std::vector<SeetaRect>);
PYBIND11_MAKE_OPAQUE(std::vector<SeetaPointF>);

namespace py = pybind11;

class SeetaImage {
  public:
    SeetaImage(const std::string &path) {
      cv::Mat mat = cv::imread(path.c_str());
      image.reset(new seeta::cv::ImageData(mat));
    }
    SeetaImage(const cv::Mat &mat) {
      image.reset(new seeta::cv::ImageData(mat));
    }
    seeta::cv::ImageData data() const {
      return *(image.get());
    }
    cv::Mat mat_data() const {
      return image->toMat();
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

class PointDetector {
  public:
    PointDetector(const std::string& model) {
      detector.reset(new seeta::PointDetector2(model.c_str()));
    }

    std::vector<SeetaPointF> detect(std::shared_ptr<SeetaImage> image, SeetaRect rect) {
      SeetaPointF *points = detector->Detect(image->data(), rect);
      std::vector<SeetaPointF> result;
      for (int i = 0; i < detector->LandmarkNum(); i++) {
        SeetaPointF point = points[i];
        result.push_back(point);
      }
      return result;
    }
  private:
    std::unique_ptr<seeta::PointDetector2> detector;
};

PYBIND11_MODULE(seetaface, m) {

  NDArrayConverter::init_numpy();

  py::class_<SeetaRect, std::unique_ptr<SeetaRect>>(m, "SeetaRect")
    .def(py::init<>())
    .def_readwrite("x", &SeetaRect::x)
    .def_readwrite("y", &SeetaRect::y)
    .def_readwrite("width", &SeetaRect::width)
    .def_readwrite("height", &SeetaRect::height);

  py::class_<SeetaPointF, std::unique_ptr<SeetaPointF>>(m, "SeetaPoint")
    .def(py::init<>())
    .def_readwrite("x", &SeetaPointF::x)
    .def_readwrite("y", &SeetaPointF::y);

  py::class_<SeetaImage, std::shared_ptr<SeetaImage>>(m, "SeetaImage")
    .def(py::init<const std::string &>())
    .def(py::init<const cv::Mat &>())
    .def("data", &SeetaImage::mat_data);

  py::bind_vector<std::vector<SeetaRect>>(m, "VectorSeetaRect");
  py::bind_vector<std::vector<SeetaPointF>>(m, "VectorSeetaPoint");

  py::class_<FaceDetector, std::unique_ptr<FaceDetector>>(m, "FaceDetector")
    .def(py::init<const std::string&>())
    .def("detect", &FaceDetector::detect);

  py::class_<PointDetector, std::unique_ptr<PointDetector>>(m, "PointDetector")
    .def(py::init<const std::string&>())
    .def("detect", &PointDetector::detect);
}
