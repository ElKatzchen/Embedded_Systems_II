import cv2

droidcam_url = "http://10.193.86.147:4747/video"

cap = cv2.VideoCapture(droidcam_url)

if not cap.isOpened():
    print("Error: No se puede abrir el stream de DroidCam")
    exit()

while True:
    ret, frame = cap.read()

    if not ret:
        print("Error: No se puede recibir el frame")
        break

    cv2.imshow('DroidCam', frame)

    if cv2.waitKey(1) & 0xFF == ord('x'):
        break

cap.release()
cv2.destroyAllWindows()