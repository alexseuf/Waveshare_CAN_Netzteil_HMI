# C9-Skripte – reproduzierbare Diagrammerzeugung

Dieses Verzeichnis enthält den vollständigen, parametrierbaren Python-Referenzstand zur Neuerzeugung der Diagramme einer PFC-Drossel-Dokumentation.

## Dateien

- `parameter.py`: zentraler Parametersatz.
- `generate_all_figures.py`: Berechnung und Erzeugung aller SVG-/PNG-Diagramme sowie der Kennwert-CSV.

## Installation

```bash
python -m pip install numpy matplotlib
```

## Ausführung

```bash
python generate_all_figures.py
```

Die Ausgabe wird standardmäßig im Unterverzeichnis `generated` abgelegt.

Ausgabe direkt in ein Drosselbeispiel:

```bash
python generate_all_figures.py \
  --output ../../../../Drossel_Spezifikationen/Beispiele/PFC_20kW_525uH_3x56mm_HighFlux/Bilder
```

## Neue Drosselvariante

1. `parameter.py` kopieren oder ändern.
2. Kerngeometrie, Windungszahl, Litze, Netzparameter und Lastfälle eintragen.
3. Die Stützstellen für `current_support_a`, `ldiff_support_h`, `b_current_support_a` und `b_support_t` ersetzen.
4. Steinmetz- und Thermikparameter kontrollieren.
5. Skript ausführen.
6. `berechnete_kennwerte.csv` auf Plausibilität prüfen.

## Automatische Prüfungen

Das Skript prüft vor der Berechnung:

- positive Geometrie und Windungszahl,
- gleiche Länge der Strom- und Induktivitätsstützstellen,
- streng monotone Stromstützstellen,
- positive differentielle Induktivitäten,
- Übereinstimmung von `Ldiff(0)` und geometrisch berechnetem `L0` innerhalb einer Toleranz.

## Erzeugte Dateien

Das Skript erzeugt die mechanischen und magnetischen Darstellungen, Induktivitäts- und Permeabilitätskennlinien, Kupfer-, Kern- und Gesamtverluste, Thermik, Stromwelligkeit, zeitdiskreten 50-Hz-/70-kHz-Stromverlauf, Flussdichtehub und Materialausnutzung. Jede Abbildung wird als SVG und PNG gespeichert.

## Modellgrenzen

Die Skripte sind eine reproduzierbare Referenzimplementierung für die dokumentierten vereinfachten Modelle. Sie ersetzen keine PLECS-Schaltsimulation, keine FEM-Feldberechnung und keine Messung am Muster. Insbesondere müssen B(I)- und Ldiff(I)-Stützstellen für jede neue Kern-/Wicklungskombination neu validiert werden.
