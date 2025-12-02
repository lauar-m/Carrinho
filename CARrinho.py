ser = serial.Serial('COM17',9600)
                    
data = ser.readline().decode('utf-8').strip()