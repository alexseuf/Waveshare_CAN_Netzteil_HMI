# 1 Zweck und Geltungsbereich

Dieses Dokument spezifiziert den magnetischen, elektrischen, mechanischen und thermischen Entwurfsstand einer PFC-Drossel für einen dreiphasigen 20-kW-Interleaved-PFC.

Es enthält die Berechnungsmodelle, Kennlinien, Fertigungsanforderungen und Prüfmerkmale für einen Aufbau aus drei gestapelten High-Flux-Ringkernen.

## 1.1 Magnetisches Berechnungsmodell

Die B(H)-Kennlinie und die daraus abgeleitete differentielle Induktivität werden mit der in der bereitgestellten Formelsammlung dokumentierten Magnetics-Herstellerfitfunktion berechnet.

Für den Stromrippel um einen DC-Arbeitspunkt ist die differentielle Induktivität maßgebend:

$$
L_{\mathrm{diff}} = \frac{\mathrm{d}\Psi}{\mathrm{d}I}
$$

Die Sekanteninduktivität dient ergänzend zur Bewertung der gespeicherten Flussverkettung:

$$
L_{\mathrm{sec}} = \frac{\Psi}{I}
$$

## 1.2 Gültigkeit

Die Spezifikation beschreibt einen Entwurfs- und Berechnungsstand. Die endgültige Bauteilfreigabe setzt die messtechnische Verifikation der magnetischen Kennlinie, der Wicklungsverluste, des thermischen Verhaltens und der mechanischen Ausführung voraus.

## 1.3 Abgrenzung

Die dokumentierten Werte gelten für die in den folgenden Kapiteln festgelegten Systemdaten, Kernabmessungen und Wicklungsdaten. Änderungen an Topologie, Modulation, Zwischenkreisspannung, Schaltfrequenz, Kernmaterial oder Wicklung erfordern eine erneute Bewertung.
