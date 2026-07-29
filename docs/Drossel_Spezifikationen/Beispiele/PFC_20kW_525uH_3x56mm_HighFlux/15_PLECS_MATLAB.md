# 15 PLECS-/MATLAB-Parametersatz

```matlab
D_a_mm = 57.15;
D_i_mm = 35.56;
h_mm = 3*13.97;
N_Wdg = 48;
N_Litze = 630;
d_Litze_mm = 0.10;
A_mag = 3*144e-6;
l_mag_mm = 143;
mu_r_0 = 60;
mu_r_sat = 1;
B_sat = 1.78;

% Magnetics High Flux 60 µ:
% Pv[mW/cm^3] = a_StMetz * B_T^b_StMetz * f_kHz^c_StMetz
a_StMetz = 246.54;
b_StMetz = 2.218;
c_StMetz = 1.311;

U_LL_rms = 400;
U_DC = 750;
f_sw = 70e3;
```

Die Steinmetzgleichung verwendet in dieser Fassung $B$ in Tesla und $f$ in kHz.