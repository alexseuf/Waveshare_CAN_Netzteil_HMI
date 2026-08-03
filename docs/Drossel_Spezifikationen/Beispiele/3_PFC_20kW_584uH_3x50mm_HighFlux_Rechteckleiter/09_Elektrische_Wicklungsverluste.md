# 9 Elektrische Wicklungsverluste

## 9.1 Gleichstromwiderstand

Der Gleichstromwiderstand ist aus der tatsächlichen mittleren Windungslänge $l_{Cu}$ und dem Kupferquerschnitt zu bestimmen:

$$R_{DC,25}=\rho_{Cu,25}\frac{l_{Cu}}{A_{Cu}},$$

mit $A_{Cu}=2{,}88\,\mathrm{mm^2}$. Da die endgültige Wicklungsgeometrie und damit die mittlere Windungslänge noch nicht festgelegt sind, wird in diesem Entwurfsstand kein unbelegter Widerstandswert vorgegeben.

Die Temperaturabhängigkeit wird mit

$$R(T)=R_{25}\left[1+\alpha_{Cu}(T-25\,^{\circ}\mathrm C)\right]$$

und $\alpha_{Cu}=0{,}00393\,\mathrm{K^{-1}}$ berechnet.

## 9.2 Kupferverluste

$$P_{Cu}=I_{RMS}^2R_{AC}.$$

Für den Grundstrom darf zunächst $R_{AC}\approx R_{DC}$ verwendet werden. Die hochfrequenten Zusatzverluste des 0,72 mm dicken Rechteckleiters sind wegen Skin- und Proximity-Effekt separat zu bestimmen. Der flächengleiche Runddrahtdurchmesser im PLECS-Parametersatz ist dafür nur eine Näherung.
