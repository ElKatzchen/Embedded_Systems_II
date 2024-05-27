import cv2
import numpy as np
import time
import serial as sl

#----------PREPROCESSING-----------
ser=sl.Serial("/dev/ttyACM0",9600)
ser.reset_input_buffer()
cap = cv2.VideoCapture(4)

#----------OPEN CAMERA----------
if not cap.isOpened():
    print("----------CAMERA ERROR----------")
    exit()

#----------SUBSTRACTION----------
fbg = cv2.createBackgroundSubtractorMOG2(history=500, varThreshold=150, detectShadows=True)

#----------INFINITE WHILE----------
while True:
    comp, frame = cap.read()
    if not comp:
        break

    fgmask = fbg.apply(frame)

    #----------KERNEL GENERATION----------
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
    fgmask = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel)
    fgmask = cv2.morphologyEx(fgmask, cv2.MORPH_CLOSE, kernel)

    #----------CONTOURS GENERATION----------
    contours, _ = cv2.findContours(fgmask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    bbs = 0

    #----------CONTOURS DETECT----------
    for contour in contours:
        if cv2.contourArea(contour) < 500:
            continue
        
        #----------BBS VISUALIZATION----------
        bbs += 1
        x1, y1, x2, y2 = cv2.boundingRect(contour)
        cv2.rectangle(frame, (x1, y1), (x1 + x2, y1 + y2), (255, 0, 255), 1)

    #----------CAMERA UPDATER----------
    cv2.imshow("----------CAMERA----------", frame)

    #----------CLOSE X----------
    if cv2.waitKey(1) & 0xFF == ord('x'):
        break

    #----------UART IMPLEMENTATION----------
    try:
	    #----------SEND DATA----------
        ser.write(bytearray(f"{bbs}\n",encoding='utf-8'))
        time.sleep(0.001)
    except Exception as e:
        print(e)

#----------RELEASE----------
cap.release()
cv2.destroyAllWindows()