# 15 PLECS-/MATLAB-Parametersatz

```matlab
% Geometrie
D_a_mm = <Wert>;
D_i_mm = <Wert>;
h_mm = <Anzahl_Kerne>*<Hoehe_je_Kern>;

% Wicklung
N_Wdg = <Wert>;
N_Litze = <Wert>;
d_Litze_mm = <Wert>;

% Magnetischer Kreis
A_mag = <Wert>;      % m^2
l_mag_mm = <Wert>;  % mm
mu_r_0 = <Wert>;
mu_r_sat = <Wert>;
B_sat = <Wert>;      % T

% B(H)-Fit
a_BH = <Wert>;
b_BH = <Wert>;
c_BH = <Wert>;
d_BH = <Wert>;
e_BH = <Wert>;
x_BH = <Wert>;

% Steinmetzparameter
a_StMetz = <Wert>;
b_StMetz = <Wert>;
c_StMetz = <Wert>;

% Wicklungswiderstand
R_DC_25 = <Wert>;    % Ohm
alpha_Cu = 0.00393;  % 1/K
k_AC = <Wert>;
```

## 15.1 Parameterprüfung

| Parametergruppe | Quelle | geprüft am | Prüfer |
|---|---|---|---|
| Kerngeometrie | Datenblatt / Messung |  |  |
| B(H)-Fit | Herstellerquelle |  |  |
| Steinmetzparameter | Herstellerquelle |  |  |
| Wicklungsdaten | Zeichnung / Fertigung |  |  |
| Widerstand | Berechnung / Messung |  |  |

## 15.2 Modellhinweise

- Einheitensysteme müssen zwischen MATLAB, PLECS und Datenblatt konsistent sein.
- Differentielle und Sekanteninduktivität dürfen nicht verwechselt werden.
- Der reale Spannungsverlauf ist für Stromrippel und Kernverluste zu verwenden.
- Modellparameter sind mit Messwerten des Musters abzugleichen.
