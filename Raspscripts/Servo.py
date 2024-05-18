import gpiozero as gz
from time import sleep
import serial as killer

#----------DECLARATION----------
sensor=gz.DistanceSensor(23,24)
servo=gz.Servo(17)
ledred=gz.LED(22)
ledgreen=gz.LED(27)

#----------WHILE----------
while(True):
        try:
                distance=sensor.distance*100
                distance=int(distance)
                if(distance>50):
                        distance=50

                if(distance>=5):
                        nv=1
                        ledgreen.off()
                        ledred.on()
                else:
                        nv=-1
                        ledgreen.on()
                        ledred.off()
                servo.value=nv
                print(nv)
        except Exception as e:
                sensor.close()
                servo.close()
                ledgreen.close()
                ledred.close()