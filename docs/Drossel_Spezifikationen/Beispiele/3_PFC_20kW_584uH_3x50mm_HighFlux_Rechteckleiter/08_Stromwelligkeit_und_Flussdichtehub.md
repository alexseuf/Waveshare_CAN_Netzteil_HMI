# 8 Stromwelligkeit und Flussdichtehub

Für eine lokale Schaltperiode wird die Stromwelligkeit näherungsweise mit

$$\Delta I_{pp}=\frac{V_L}{L_{diff}f_s}$$

berechnet. Wegen der stromabhängigen Permeabilität ist dabei die differentielle Induktivität am jeweiligen Arbeitspunkt einzusetzen.

Der Flussdichtehub folgt aus

$$\Delta B_{pp}=\frac{V_L}{NA_ef_s}.$$

Mit $N=80$, $A_e=375\,\mathrm{mm^2}$ und $f_s=70\,\mathrm{kHz}$ reduziert die hohe Windungszahl den hochfrequenten Flussdichtehub. Der maximale Wert ist über die vollständige Netzperiode und die real verwendete SVM zu bestimmen.

Die Summe aus niederfrequentem Arbeitspunkt und halbem hochfrequentem Hub muss unterhalb der zulässigen Flussdichte bleiben:

$$|B_{LF}|+\frac{\Delta B_{pp}}{2}<B_{zul}.$$
