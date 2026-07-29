# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

$$L_0 = \mu_0 \cdot \mu_{r,0} \cdot N^2 \cdot \frac{A_e}{l_e}$$

Für den vorliegenden Aufbau:

- $L_0 = 382{,}3\,\mu\text{H}$
- $A_L = 565{,}5\,\text{nH}/N^2$

## 6.2 Feldstärke

$$H[\text{Oe}] = \frac{4\pi \cdot N \cdot I[\text{A}]}{l_e[\text{mm}]}$$

Für diesen Aufbau gilt näherungsweise:

$$H \approx 2{,}2689 \cdot I$$

## 6.3 Magnetics-B(H)-Fit

$$B(H)=\left[\frac{a+bH+cH^2}{1+dH+eH^2}\right]^x$$

| Parameter | Wert |
|---|---:|
| $a$ | 3,8280E−02 |
| $b$ | 1,8000E−02 |
| $c$ | 7,0120E−04 |
| $d$ | 7,0630E−02 |
| $e$ | 4,5020E−04 |
| $x$ | 1,630 |

## 6.4 Differentielle Induktivität

$$L_{\mathrm{diff}}(I)=N^2\frac{A_e}{l_e}\left[\frac{dB}{dH_{\mathrm{Oe}}}\cdot\frac{4\pi}{1000}\right]$$

Die Umrechnung $4\pi/1000$ berücksichtigt $1\,\text{Oe}=1000/(4\pi)\,\text{A/m}$.

## 6.5 Sekanteninduktivität

$$L_{\mathrm{sec}}(I)=N\,A_e\,\frac{B(H)-B(0)}{I}$$

Das Abziehen des kleinen Fit-Offsets $B(0)$ verhindert eine unphysikalische Flussverkettung bei $I=0$.