# 15 PLECS-/MATLAB-Parametersatz

```matlab
D_a_mm = 63.09;
D_i_mm = 31.70;
h_mm = 3*25.91;
N_Wdg = 26;
N_Litze = 680;
d_Litze_mm = 0.10;
A_mag = 3*360e-6;
l_mag_mm = 144;
mu_r_0 = 60;
mu_r_sat = 1;
B_sat = 1.78;

% B(H)-Fit High Flux Toroids, 60 u
a_BH = 3.8280E-02;
b_BH = 1.8000E-02;
c_BH = 7.0120E-04;
d_BH = 7.0630E-02;
e_BH = 4.5020E-04;
x_BH = 1.630;

% Steinmetz High Flux 60 u
a_StMetz = 246.54;
b_StMetz = 2.218;
c_StMetz = 1.311;
```

## Verwendungshinweise

- `A_mag` ist in m² einzusetzen.
- `l_mag_mm` ist in der vorliegenden Parametrierung in mm angegeben.
- Für die B(H)-Fitfunktion wird die Feldstärke in Oe verwendet.
- Die Einheitensetzung der Steinmetzparameter muss vor Freigabe anhand der verwendeten Magnetics-Katalogquelle bestätigt werden.
- Für die Kernverlustberechnung ist bevorzugt der reale Flussdichteverlauf aus der PLECS-Simulation zu verwenden.