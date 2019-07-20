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

cv2.imwrite('example2_result.jpg', im)
