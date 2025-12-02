import pandas as pd
import matplotlib.pyplot as plt

df_esq = pd.read_csv("motor-esquerdo.csv")
df_dir = pd.read_csv("motor-direito.csv")

fig, ax1 = plt.subplots()

# --- EIXO 1: Velocidades ---
ax1.plot(df_esq["t"], df_esq["velL"], label="Velocidade L")
ax1.plot(df_dir["t"], df_dir["velR"], label="Velocidade R")
ax1.set_xlabel("Tempo (ms)")
ax1.set_ylabel("Velocidade (pulsos/step)")
ax1.legend(loc="upper left")

# --- EIXO 2: PWM (somente do motor testado) ---
ax2 = ax1.twinx()
ax2.plot(df_esq["t"], df_esq["pwmL"], color="orange", linestyle="--", label="PWM L")
ax2.plot(df_dir["t"], df_dir["pwmR"], color="red", linestyle="--", label="PWM R")
ax2.set_ylabel("PWM (0–255)")
ax2.legend(loc="upper right")

plt.title("Resposta ao Impulso — Motores L e R")
plt.show()
