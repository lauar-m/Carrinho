import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ======= CARREGAR CSV =======
df = pd.read_csv("motor-esquerdo.csv")

t = df["t"].values / 1000.0  # ms → s
v = df["velL"].values             # escolha vL ou vR

# ======= CALCULAR K =======
c = v[-1]

# ======= CALCULAR τ =======
target = 0.6321 * c
idx_tau = (np.abs(v - target)).argmin()
tau = t[idx_tau]

print(f"K estimado = {c:.2f}")
print(f"tau estimado = {tau:.3f} s")

# ======= CURVA TEÓRICA =======
v_teo = c * (1 - np.exp(-t / tau))

# ======= PLOTAR =======
plt.figure(figsize=(10,6))

plt.plot(t, v, label="Resposta Real (encoder)")
plt.plot(t, v_teo, "--", label="Modelo Teórico 1ª Ordem")

plt.xlabel("Tempo (s)")
plt.ylabel("Velocidade (pulsos/intervalo)")
plt.title("Resposta ao Degrau do Motor Esquerdo")
plt.legend()
plt.grid(True)

plt.savefig("comparacao_resposta_motor_esquerdo.png", dpi=300)
print("Figura salva como comparacao_resposta_motor_esquerdo.png")

a = 1/tau
A = 200/255 ## PWM DE 200 normalizado para a faixa 0-255
alpha = c*a/A

print("esquerdo")
print(alpha)
print("-----------")
print(f"s + {a}")