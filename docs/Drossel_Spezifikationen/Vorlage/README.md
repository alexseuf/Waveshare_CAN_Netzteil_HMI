# Vorlage für Drossel-Spezifikationen

Dieser Ordner enthält eine modulare Markdown-Vorlage für neue Drosselprojekte. Die Struktur wurde aus der bestehenden Entwicklungsspezifikation einer PFC-Drossel abgeleitet und projektneutral aufbereitet.

## Kapitel

- [00 – Dokumentinformationen, Zweck und Geltungsbereich](00_Dokumentinformationen.md)
- [01 – Systemanforderungen](01_Systemanforderungen.md)
- [02 – Magnetischer Aufbau](02_Magnetischer_Aufbau.md)
- [03 – Wicklungsaufbau](03_Wicklungsaufbau.md)
- [04 – Mechanischer Aufbau](04_Mechanischer_Aufbau.md)
- [05 – Berechnungsgrundlagen](05_Berechnungsgrundlagen.md)
- [06 – Magnetische Kennlinien](06_Magnetische_Kennlinien.md)
- [07 – Stromrippel und Flussdichtehub](07_Stromrippel_und_Flussdichte.md)
- [08 – Kupferverluste](08_Kupferverluste.md)
- [09 – Kernverluste](09_Kernverluste.md)
- [10 – Gesamtverluste](10_Gesamtverluste.md)
- [11 – Thermische Anforderungen](11_Thermik.md)
- [12 – Fertigungsanforderungen](12_Fertigung.md)
- [13 – Prüf- und Abnahmekriterien](13_Pruefung.md)
- [14 – PLECS-/MATLAB-Parametersatz](14_PLECS_MATLAB.md)
- [15 – Bewertung, offene Verifikation und Quellen](15_Bewertung_und_Quellen.md)

## Zusätzliche Ordner

Für Bilder und Diagramme wird folgende Struktur empfohlen:

```text
Vorlage/
└── Bilder/
    ├── Kerngeometrie.png
    ├── Wicklungsaufbau.png
    ├── Mechanischer_Aufbau.png
    ├── BH_Kennlinie.png
    ├── L_I_Kennlinie.png
    ├── Mu_diff_Kennlinie.png
    ├── Stromrippel.png
    ├── Flussdichtehub.png
    ├── Kupferverluste.png
    ├── Kernverluste.png
    └── Gesamtverluste.png
```

## Verwendung

1. Den Ordner `Vorlage` für ein neues Drosselprojekt kopieren.
2. Platzhalter und leere Tabellenfelder projektspezifisch ausfüllen.
3. Nicht benötigte Kapitel entfernen oder als nicht zutreffend kennzeichnen.
4. Berechnungswerte, Diagramme, Zeichnungen und Prüfergebnisse ergänzen.
5. Herstellerkoeffizienten einschließlich Einheitensystem und Quellenstand prüfen.
6. Jede freigegebene Änderung im Änderungsverlauf dokumentieren.
