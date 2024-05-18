import cv2
import time
#import serial as sl

verde=1
morado=2
#comunicacion serial
#ser=sl.Serial("/dev/ttyACM0",9600)
#ser.reset_input_buffer()
# Configura los pines GPIO

def detect_color(hsv, color):
        # Rangos de colores en HSV
        if color == 'red':
                lower = (160, 100, 100)
                upper = (180, 255, 255)
        elif color == 'yellow':
                lower = (20, 100, 100)
                upper = (30, 255, 255)
        elif color == 'green':
                lower = (50, 100, 100)
                upper = (70, 255, 255)
        elif color == 'purple':
                lower = (100, 100, 100)
                upper = (250, 150, 255)

        mask = cv2.inRange(hsv, lower, upper)
        return mask

def activate_led(mask,pin):
        if cv2.countNonZero(mask) > 400 and pin==1:
                #ser.write(bytearray("1\n",encoding='utf-8'))
                print("1")
        elif cv2.countNonZero(mask) > 400 and pin==2:
                #ser.write(bytearray("2\n",encoding='utf-8'))
                print("2")
        else:
                #ser.write(bytearray("0\n",encoding='utf-8'))
                print("0")

# Inicializa la cámara
cap = cv2.VideoCapture(0)

try:
        while True:
                ret, frame = cap.read()
                if ret:

                        # Convierte la imagen a HSV
                        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

                        # Detecta colores y activa LEDs
                        #activate_led(detect_color(hsv, 'red'),0)
                        #activate_led(detect_color(hsv, 'yellow'),165)
                        activate_led(detect_color(hsv, 'green'),verde)
                        activate_led(detect_color(hsv, 'purple'),morado)

                # Muestra la imagen
                cv2.imshow('Frame', frame)
                if cv2.waitKey(1) & 0xFF == ord('q'):
                        break
                time.sleep(0.1)

finally:
        cap.release()
        cv2.destroyAllWindows()