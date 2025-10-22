import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

# --- Dados brutos (copie tudo que tiver) ---
dados_str = """
6775 ms | V_R: 5.00 | V_C: 0.00
7690 ms | V_R: 4.79 | V_C: 0.21
8146 ms | V_R: 4.57 | V_C: 0.43
8603 ms | V_R: 4.37 | V_C: 0.63
9060 ms | V_R: 4.17 | V_C: 0.83
9518 ms | V_R: 3.99 | V_C: 1.01
9974 ms | V_R: 3.81 | V_C: 1.19
10431 ms | V_R: 3.66 | V_C: 1.34
10889 ms | V_R: 3.49 | V_C: 1.51
11345 ms | V_R: 3.34 | V_C: 1.66
11802 ms | V_R: 3.19 | V_C: 1.81
12260 ms | V_R: 3.05 | V_C: 1.95
12717 ms | V_R: 2.91 | V_C: 2.09
13174 ms | V_R: 2.78 | V_C: 2.22
13631 ms | V_R: 2.66 | V_C: 2.34
14088 ms | V_R: 2.54 | V_C: 2.46
14545 ms | V_R: 2.42 | V_C: 2.58
15002 ms | V_R: 2.32 | V_C: 2.68
15460 ms | V_R: 2.21 | V_C: 2.79
15917 ms | V_R: 2.12 | V_C: 2.88
16373 ms | V_R: 2.02 | V_C: 2.98
16831 ms | V_R: 1.93 | V_C: 3.07
17288 ms | V_R: 1.84 | V_C: 3.16
17744 ms | V_R: 1.76 | V_C: 3.24
18202 ms | V_R: 1.68 | V_C: 3.32
18659 ms | V_R: 1.61 | V_C: 3.39
19116 ms | V_R: 1.53 | V_C: 3.47
19573 ms | V_R: 1.47 | V_C: 3.53
20030 ms | V_R: 1.40 | V_C: 3.60
20488 ms | V_R: 1.34 | V_C: 3.66
20944 ms | V_R: 1.28 | V_C: 3.72
21401 ms | V_R: 1.22 | V_C: 3.78
21859 ms | V_R: 1.17 | V_C: 3.83
22316 ms | V_R: 1.11 | V_C: 3.89
22772 ms | V_R: 1.07 | V_C: 3.93
23230 ms | V_R: 1.02 | V_C: 3.98
23687 ms | V_R: 0.97 | V_C: 4.03
24144 ms | V_R: 0.93 | V_C: 4.07
24601 ms | V_R: 0.89 | V_C: 4.11
25058 ms | V_R: 0.85 | V_C: 4.15
25516 ms | V_R: 0.81 | V_C: 4.19
25972 ms | V_R: 0.77 | V_C: 4.23
26429 ms | V_R: 0.74 | V_C: 4.26
26887 ms | V_R: 0.71 | V_C: 4.29
27343 ms | V_R: 0.67 | V_C: 4.33
27801 ms | V_R: 0.65 | V_C: 4.35
28258 ms | V_R: 0.62 | V_C: 4.38
28715 ms | V_R: 0.59 | V_C: 4.41
29172 ms | V_R: 0.56 | V_C: 4.44
29629 ms | V_R: 0.54 | V_C: 4.46
30086 ms | V_R: 0.51 | V_C: 4.49
30543 ms | V_R: 0.49 | V_C: 4.51
31000 ms | V_R: 0.47 | V_C: 4.53
31458 ms | V_R: 0.45 | V_C: 4.55
31915 ms | V_R: 0.43 | V_C: 4.57
32371 ms | V_R: 0.41 | V_C: 4.59
32829 ms | V_R: 0.39 | V_C: 4.61
33286 ms | V_R: 0.37 | V_C: 4.63
33742 ms | V_R: 0.36 | V_C: 4.64
34200 ms | V_R: 0.34 | V_C: 4.66
34657 ms | V_R: 0.32 | V_C: 4.68
35115 ms | V_R: 0.31 | V_C: 4.69
35571 ms | V_R: 0.30 | V_C: 4.70
36028 ms | V_R: 0.28 | V_C: 4.72
36486 ms | V_R: 0.27 | V_C: 4.73
36942 ms | V_R: 0.26 | V_C: 4.74
37399 ms | V_R: 0.25 | V_C: 4.75
37857 ms | V_R: 0.23 | V_C: 4.77
38313 ms | V_R: 0.22 | V_C: 4.78
38770 ms | V_R: 0.22 | V_C: 4.78
39228 ms | V_R: 0.21 | V_C: 4.79
39685 ms | V_R: 0.20 | V_C: 4.80
40142 ms | V_R: 0.19 | V_C: 4.81
40599 ms | V_R: 0.18 | V_C: 4.82
41056 ms | V_R: 0.17 | V_C: 4.83
41513 ms | V_R: 0.16 | V_C: 4.84
41970 ms | V_R: 0.15 | V_C: 4.85
42427 ms | V_R: 0.14 | V_C: 4.86
42884 ms | V_R: 0.13 | V_C: 4.87
43341 ms | V_R: 0.12 | V_C: 4.88
43798 ms | V_R: 0.11 | V_C: 4.89
44255 ms | V_R: 0.10 | V_C: 4.90
44712 ms | V_R: 0.09 | V_C: 4.91
45169 ms | V_R: 0.08 | V_C: 4.92
45626 ms | V_R: 0.07 | V_C: 4.93
46083 ms | V_R: 0.06 | V_C: 4.94
46540 ms | V_R: 0.05 | V_C: 4.95
47455 ms | V_R: 0.04 | V_C: 4.96
49284 ms | V_R: 0.03 | V_C: 4.97
55682 ms | V_R: 0.02 | V_C: 4.98
63909 ms | V_R: 0.01 | V_C: 4.99
"""

# --- Tratamento dos dados ---
linhas = [l for l in dados_str.strip().split("\n") if l]
tempos, vr, vc = [], [], []

for linha in linhas:
    partes = linha.replace(" ms | V_R: ", ",").replace(" | V_C: ", ",").split(",")
    if len(partes) == 3:
        t, r, c = partes
        tempos.append(int(t.strip()))
        vr.append(float(r.strip()))
        vc.append(float(c.strip()))

# Criar DataFrame
df = pd.DataFrame({
    "tempo_ms": tempos,
    "V_R": vr,
    "V_C": vc
})

# Corrigir tempo (iniciando do zero e convertendo para segundos)
df["tempo_s"] = (df["tempo_ms"] - df["tempo_ms"].min()) / 1000

# Suavizar (média móvel)
df["V_R_suave"] = df["V_R"].rolling(5, center=True).mean()
df["V_C_suave"] = df["V_C"].rolling(5, center=True).mean()

# Criar pasta de saída
os.makedirs("assets", exist_ok=True)

# --- Gráfico 1: Carga do capacitor ---
plt.figure(figsize=(8, 5))
plt.plot(df["tempo_s"], df["V_C_suave"], label="Carga do Capacitor (V_C)", color="blue")
plt.title("Carga do Capacitor")
plt.xlabel("Tempo (s)")
plt.ylabel("Tensão (V)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("assets/carga_capacitor.png")
plt.close()

# --- Gráfico 2: Descarga do resistor ---
plt.figure(figsize=(8, 5))
plt.plot(df["tempo_s"], df["V_R_suave"], label="Descarga do Resistor (V_R)", color="red")
plt.title("Descarga do Resistor")
plt.xlabel("Tempo (s)")
plt.ylabel("Tensão (V)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("assets/descarga_resistor.png")
plt.close()

# --- Gráfico 3: Ambos juntos ---
plt.figure(figsize=(8, 5))
plt.plot(df["tempo_s"], df["V_C_suave"], label="V_C (Carga do Capacitor)", color="blue")
plt.plot(df["tempo_s"], df["V_R_suave"], label="V_R (Descarga do Resistor)", color="red")
plt.title("Comportamento do Circuito RC")
plt.xlabel("Tempo (s)")
plt.ylabel("Tensão (V)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("assets/rc_completo.png")
plt.close()
