#!/usr/bin/env python

import cv2
import time
import imutils

is_pi = True
try:
    import RPi.GPIO as GPIO
except ImportError:
    is_pi = False

# ---------------------------------------------
# classifiers
# ---------------------------------------------

# Load cascade files
casc_frontalface_1 = "./data/haarcascades/haarcascade_frontalface_default.xml"
casc_frontalface_2 = "./data/lbpcascades/lbpcascade_frontalface_improved.xml"
casc_catface_1 = "./data/haarcascades/haarcascade_frontalcatface_extended.xml"
casc_catface_2 = "./data/lbpcascades/lbpcascade_frontalcatface.xml"

# Create some classifiers
classifiers = [
    # cv2.CascadeClassifier(casc_frontalface_1),
    cv2.CascadeClassifier(casc_frontalface_2),
    cv2.CascadeClassifier(casc_catface_1),
    cv2.CascadeClassifier(casc_catface_2)
]

class_names = [
    # 'notacat',
    'notacat',
    'cat',
    'cat',
]
classifier_index = 0

# ---------------------------------------------
# Rotations
# ---------------------------------------------

angles = [-30, -20, -10, 0, 10, 20, 30]
angle_index = 0

# ---------------------------------------------
# Start capture video
# ---------------------------------------------

video_capture = cv2.VideoCapture(0)
try:
    video_capture.set(cv2.cv.CV_CAP_PROP_FPS, 8)
    video_capture.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, 320)
    video_capture.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, 160)
except AttributeError:
    video_capture.set(cv2.CAP_PROP_FPS, 8)
    video_capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    video_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 320)

# ---------------------------------------------
# Init GPIO
# ---------------------------------------------

if is_pi:
    GPIO.setmode(GPIO.BCM)

# ---------------------------------------------
# Globals
# ---------------------------------------------

tfound = 0
kitty_counter = 0
state = 0


# Pull up pin 4
def new_state(s, gray=None, force=False):
    global state

    if s == state and not force:
        return

    # set new state
    state = s

    print('New state: %d - %d' % (s, time.time()))
    with open('cat.txt', 'w') as the_file:
        the_file.write("%d\n" % s)

    if s == 2:
        if is_pi:
            GPIO.setup(4, GPIO.OUT)

        if gray is not None:
            now = time.time()
            cv2.imwrite('./imgs/cat-%d.png' % now, gray)
    else:
        if is_pi:
            GPIO.setup(4, GPIO.IN)


# Init state
new_state(0, force=True)

# Main loop
while True:
    # Check for break
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Capture frame-by-frame
    ret, frame = video_capture.read()

    # convert image to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    if not is_pi:
        cv2.imshow('eye', gray)

    # look for faces in image
    classifier = classifiers[classifier_index]
    name = class_names[classifier_index]

    angle = angles[angle_index]
    rotated = imutils.rotate(gray, angle)
    found = classifier.detectMultiScale(
        rotated,
        flags=cv2.CASCADE_SCALE_IMAGE)

    # if we found a face, remember it
    now = time.time()
    if len(found) > 0:
        if name == 'cat':
            kitty_counter = kitty_counter + 1
        else:
            kitty_counter = 0

        if kitty_counter > 1:
            new_state(2, gray)

        if kitty_counter == 0:
            new_state(1)

        # wait for 5 sec after found a new cat/non-cat
        tfound = now + 5

    elif tfound < now:
        kitty_counter = 0
        new_state(0)

    # Next classifier
    classifier_index = classifier_index + 1
    if classifier_index >= len(classifiers):
        classifier_index = 0

    # Next rotation
    angle_index = angle_index + 1
    if angle_index >= len(angles):
        angle_index = 0

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
