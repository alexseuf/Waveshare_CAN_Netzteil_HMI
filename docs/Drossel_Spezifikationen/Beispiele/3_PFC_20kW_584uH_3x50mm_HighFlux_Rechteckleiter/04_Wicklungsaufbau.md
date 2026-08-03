# 4 Wicklungsaufbau

| Parameter | Wert |
|---|---:|
| Windungszahl | 80 |
| Leiterart | Rechteckkupferleiter |
| Leiterabmessung | 0,72 mm × 4,00 mm |
| Kupferquerschnitt | 2,88 mm² |
| Ersatz-Runddurchmesser | 1,915 mm |
| PLECS-Ersatz | $N_{Litze}=1$, $d_{Litze}=\sqrt{4A/\pi}$ |

Der Rechteckleiter besitzt den Kupferquerschnitt

$$A_{Cu}=0{,}72\,\mathrm{mm}\cdot4{,}00\,\mathrm{mm}=2{,}88\,\mathrm{mm^2}.$$

Für Modelle, die nur einen Runddrahtdurchmesser akzeptieren, wird ein flächengleicher Ersatzdurchmesser verwendet:

$$d_{eq}=\sqrt{\frac{4A_{Cu}}{\pi}}=\sqrt{\frac{4\cdot0{,}72\cdot4}{\pi}}\approx1{,}915\,\mathrm{mm}.$$

Dieser Ersatzdurchmesser erhält den Gleichstromquerschnitt, bildet Skin- und Proximity-Effekt des Rechteckleiters jedoch nicht geometrisch korrekt ab. Für die Wicklungsfertigung sind Isolationsdicke, Biegeradius, Lagenaufteilung und tatsächlich nutzbare Fenstergeometrie separat zu verifizieren.
