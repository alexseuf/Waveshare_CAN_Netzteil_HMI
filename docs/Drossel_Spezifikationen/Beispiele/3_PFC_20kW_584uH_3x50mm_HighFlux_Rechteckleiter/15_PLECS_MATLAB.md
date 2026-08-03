# 15 PLECS-/MATLAB-Parametersatz

```matlab
D_a_mm = 50.80;
D_i_mm = 31.75;
h_mm = 3*13.46;
N_Wdg = 80;
N_Litze = 1;
d_Litze_mm = sqrt(4*0.72*4/pi);
A_mag = 3*125e-6;
l_mag_mm = 127;
mu_r_0 = 26;
mu_r_sat = 1;
B_sat = 1.65;

% Geometrischer Leiterquerschnitt und flächengleicher Runddurchmesser
A_Cu_mm2 = 0.72*4;
d_eq_mm = sqrt(4*A_Cu_mm2/pi);

U_LL_rms = 400;
U_DC = 750;
f_sw = 70e3;
L_target = 584e-6;
```

Der Parameter `d_Litze_mm` ist ein flächengleicher Ersatzdurchmesser für den Rechteckleiter. Er bildet den Gleichstromquerschnitt ab, nicht jedoch die tatsächliche HF-Stromverteilung.

Für Kernverlustberechnungen sind die zum Magnetics-High-Flux-26-µ-Material gehörenden Steinmetzparameter einzusetzen. Parameter anderer Permeabilitätsvarianten dürfen nicht ungeprüft übernommen werden.
