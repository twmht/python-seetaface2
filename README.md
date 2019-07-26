# python-seetaface2

A python API interface for [SeetaFaceEngine2](https://github.com/seetaface/SeetaFaceEngine2)

## Installation

### Prerequisite

Download binary models and libraries from [SeetaFaceEngine2](https://github.com/seetaface/SeetaFaceEngine2/blob/master/attachment/README.md) 

``` bash
git clone --recursive https://github.com/twmht/python-seetaface2
cd python-seetaface2 
cp $seetaface_lib ./
python setup.py install
```
$seetaface_lib is a lib directory after you unzip the library SDK.

If you want to build with opencv3

``` bash
USE_OPENCV3=ON python setup.py build
```

## Usage

### detect faces and their landmarks

``` python
import seetaface
import cv2

root = 'example1.jpg'

im = cv2.imread(root)
image = seetaface.SeetaImage(im)
# or you can pass the image path to build SeetaImage
# image = seetaface.SeetaImage(root)

fd = seetaface.FaceDetector('/home/tumh/python-seetaface2/SeetaFaceDetector2.0.ats')
pd = seetaface.PointDetector('/home/tumh/python-seetaface2/SeetaPointDetector2.0.pts5.ats')

rects = fd.detect(image)
for rect in rects:
    cv2.rectangle(im, (rect.x, rect.y), (rect.x + rect.width, rect.y + rect.height), (0,0,255), 2)
    points = pd.detect(image, rect)
    for p in points:
        cv2.circle(im, (int(p.x), int(p.y)), 2, (0,0,255), -1)
        
```

![Result](/example/example1_result.jpg)

### given a bounding box and detect the landmark

``` python
import seetaface
import cv2

root = 'example2.jpg'

image = seetaface.SeetaImage(root)

pd = seetaface.PointDetector('/home/tumh/python-seetaface2/SeetaPointDetector2.0.pts5.ats')

# given a boudning box
rect = seetaface.SeetaRect()
rect.x = 221
rect.y = 130
rect.width = 232
rect.height = 278

im = cv2.imread(root)
cv2.rectangle(im, (rect.x, rect.y), (rect.x + rect.width, rect.y + rect.height), (0,0,255), 2)
points = pd.detect(image, rect)
for p in points:
    cv2.circle(im, (int(p.x), int(p.y)), 2, (0,0,255), -1)
```

![President of Taiwan](/example/example2_result.jpg)
