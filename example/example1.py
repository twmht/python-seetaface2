import seetaface
import cv2

root = 'example1.jpg'

image = seetaface.SeetaImage(root)

fd = seetaface.FaceDetector('/home/tumh/python-seetaface2/SeetaFaceDetector2.0.ats')
pd = seetaface.PointDetector('/home/tumh/python-seetaface2/SeetaPointDetector2.0.pts5.ats')

rects = fd.detect(image)
im = cv2.imread(root)
for rect in rects:
    cv2.rectangle(im, (rect.x, rect.y), (rect.x + rect.width, rect.y + rect.height), (0,0,255), 2)
    points = pd.detect(image, rect)
    for p in points:
        cv2.circle(im, (int(p.x), int(p.y)), 2, (0,0,255), -1)


cv2.imwrite('example1_result.jpg', im)
#  cv2.imshow('win', im)
#  cv2.waitKey(0)
