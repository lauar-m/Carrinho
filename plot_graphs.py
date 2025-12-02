import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("dados.csv")

plt.figure()
plt.plot(df["t"], df["velL"], label="Velocidade L")
plt.plot(df["t"], df["velR"], label="Velocidade R")
plt.plot(df["t"], df["pwmL"], label="PWM")
plt.legend()
plt.xlabel("Tempo (ms)")
plt.ylabel("Vel / PWM")
plt.show()
