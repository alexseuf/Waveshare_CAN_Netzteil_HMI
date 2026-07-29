# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

```math
L_0=\mu_0\mu_{r,0}N^2\frac{A_e}{l_e}
```

| Größe | Wert | Einheit |
|---|---:|---|
| $L_0$ |  | µH |
| $A_L=L_0/N^2$ |  | nH/N² |

## 6.2 Feldstärke

```math
H=\frac{NI}{l_e}
```

Für Angaben in Oersted ist die verwendete Umrechnung eindeutig zu dokumentieren.

## 6.3 Hersteller-B(H)-Fit

```math
B(H)=\left(\frac{a+bH+cH^2}{1+dH+eH^2}\right)^x
```

| Parameter | Wert |
|---|---:|
| $a$ |  |
| $b$ |  |
| $c$ |  |
| $d$ |  |
| $e$ |  |
| $x$ |  |

## 6.4 Differentielle Induktivität

```math
L_\mathrm{diff}(I)=N^2\frac{A_e}{l_e}\frac{\mathrm dB}{\mathrm dH}
```

Für Stromrippel um einen Arbeitspunkt ist die differentielle Induktivität maßgebend.

## 6.5 Sekanteninduktivität

```math
L_\mathrm{sec}(I)=\frac{N A_e\,[B(H)-B(0)]}{I}
```

Die Sekanteninduktivität dient ergänzend zur Bewertung der Flussverkettung und gespeicherten Energie.

## 6.6 Einheitensystem

Alle verwendeten Einheiten, insbesondere für B(H)-Fit und Steinmetzparameter, sind mit der Herstellerquelle abzugleichen und im Dokument festzuhalten.
