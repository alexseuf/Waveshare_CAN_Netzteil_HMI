"""Mathematisches Modell für Litzenpositionen auf einem gestapelten Ringkern.

Das Modell ordnet jeder Windung eine Winkelposition zu und berechnet die
Schnittpunkte derselben Windung auf der Innen- und Außenseite des Ringkerns.
Es unterstützt mehrere Wicklungslagen, prüft die geometrische Belegbarkeit und
berechnet die Litzenlänge über die Mittellinie um den Kernquerschnitt.

Ausgabe:
- wicklungspositionen_ringkern.svg
- wicklungspositionen_ringkern.png
- wicklungspositionen_ringkern.csv
- wicklungskennwerte.csv
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import argparse
import csv
import math

import matplotlib.pyplot as plt
from matplotlib.patches import Circle, Wedge


@dataclass(frozen=True)
class WindingGeometry:
    # Kern: Beispiel C058110A2, drei gestapelte Kerne
    core_outer_diameter_bare_m: float = 57.15e-3
    core_inner_diameter_bare_m: float = 35.56e-3
    core_outer_diameter_coated_m: float = 59.20e-3
    core_height_single_m: float = 13.97e-3
    core_count: int = 3

    # Wicklung: 48 Windungen = 28 + 20
    turns_per_layer: tuple[int, ...] = (28, 20)
    litz_outer_diameter_m: float = 4.00e-3

    # Fertigungsabstände
    core_insulation_clearance_m: float = 0.20e-3
    turn_gap_m: float = 0.10e-3
    interlayer_gap_m: float = 0.20e-3
    lead_length_total_m: float = 0.20

    # Darstellung
    layer_angular_offset_fraction: float = 0.5

    @property
    def stack_height_m(self) -> float:
        return self.core_count * self.core_height_single_m

    @property
    def total_turns(self) -> int:
        return sum(self.turns_per_layer)

    @property
    def core_inner_radius_m(self) -> float:
        # Für die Beschichtung ist nur der Gesamtaußendurchmesser bekannt.
        # Der Innenradius bleibt deshalb der unbeschichtete Datenblattwert.
        return self.core_inner_diameter_bare_m / 2.0

    @property
    def core_outer_radius_bare_m(self) -> float:
        return self.core_outer_diameter_bare_m / 2.0

    @property
    def core_outer_radius_coated_m(self) -> float:
        return self.core_outer_diameter_coated_m / 2.0

    @property
    def radial_core_width_m(self) -> float:
        return self.core_outer_radius_coated_m - self.core_inner_radius_m


@dataclass(frozen=True)
class TurnPoint:
    turn: int
    layer: int
    layer_turn: int
    theta_rad: float
    x_inner_m: float
    y_inner_m: float
    x_outer_m: float
    y_outer_m: float
    turn_length_m: float


def layer_center_offset(g: WindingGeometry, layer_index_zero: int) -> float:
    """Abstand der Litzenmittellinie von der Kernoberfläche."""
    d = g.litz_outer_diameter_m
    return (
        g.core_insulation_clearance_m
        + d / 2.0
        + layer_index_zero * (d + g.interlayer_gap_m)
    )


def layer_radii(g: WindingGeometry, layer_index_zero: int) -> tuple[float, float]:
    """Mittellinienradien der Innen- und Außenleiter einer Lage."""
    a = layer_center_offset(g, layer_index_zero)
    r_inner = g.core_inner_radius_m - a
    r_outer = g.core_outer_radius_coated_m + a
    if r_inner <= g.litz_outer_diameter_m / 2.0:
        raise ValueError(
            f"Lage {layer_index_zero + 1} passt radial nicht in die Kernöffnung."
        )
    return r_inner, r_outer


def turn_centerline_length(g: WindingGeometry, layer_index_zero: int) -> float:
    """Mittlere Länge einer Windung um den gestapelten Kernquerschnitt.

    Der Kernquerschnitt wird als Rechteck mit radialer Breite b und axialer
    Höhe h angenähert. Die Mittellinie einer Lage liegt im Abstand a von der
    Oberfläche. Für die parallel versetzte, abgerundete Kontur gilt nach der
    Steiner-Formel:

        l_Wdg = 2 (h + b) + 2 pi a

    Dadurch wächst die Windungslänge mit jeder weiteren Lage konsistent an.
    """
    a = layer_center_offset(g, layer_index_zero)
    h = g.stack_height_m
    b = g.radial_core_width_m
    return 2.0 * (h + b) + 2.0 * math.pi * a


def angular_pitch(g: WindingGeometry, layer_index_zero: int) -> float:
    return 2.0 * math.pi / g.turns_per_layer[layer_index_zero]


def available_inner_arc_pitch(g: WindingGeometry, layer_index_zero: int) -> float:
    r_inner, _ = layer_radii(g, layer_index_zero)
    return r_inner * angular_pitch(g, layer_index_zero)


def maximum_turns_on_inner_radius(g: WindingGeometry, layer_index_zero: int) -> int:
    r_inner, _ = layer_radii(g, layer_index_zero)
    required_pitch = g.litz_outer_diameter_m + g.turn_gap_m
    return math.floor(2.0 * math.pi * r_inner / required_pitch)


def validate_geometry(g: WindingGeometry) -> None:
    if g.core_outer_diameter_coated_m < g.core_outer_diameter_bare_m:
        raise ValueError("Beschichteter Außendurchmesser ist kleiner als der Rohkern.")
    if g.total_turns <= 0:
        raise ValueError("Windungszahl muss positiv sein.")
    for layer_idx, n_turns in enumerate(g.turns_per_layer):
        if n_turns <= 0:
            raise ValueError("Jede Wicklungslage muss mindestens eine Windung enthalten.")
        pitch = available_inner_arc_pitch(g, layer_idx)
        required = g.litz_outer_diameter_m + g.turn_gap_m
        if pitch < required:
            n_max = maximum_turns_on_inner_radius(g, layer_idx)
            raise ValueError(
                f"Lage {layer_idx + 1}: {n_turns} Windungen passen nicht. "
                f"Innenbogenabstand {pitch * 1e3:.2f} mm < benötigt "
                f"{required * 1e3:.2f} mm; maximal etwa {n_max} Windungen."
            )


def calculate_turn_points(g: WindingGeometry) -> list[TurnPoint]:
    validate_geometry(g)
    points: list[TurnPoint] = []
    global_turn = 1

    for layer_idx, n_turns in enumerate(g.turns_per_layer):
        r_inner, r_outer = layer_radii(g, layer_idx)
        dtheta = 2.0 * math.pi / n_turns
        offset = layer_idx * g.layer_angular_offset_fraction * dtheta
        length = turn_centerline_length(g, layer_idx)

        for local_idx in range(n_turns):
            theta = (local_idx * dtheta + offset) % (2.0 * math.pi)
            points.append(
                TurnPoint(
                    turn=global_turn,
                    layer=layer_idx + 1,
                    layer_turn=local_idx + 1,
                    theta_rad=theta,
                    x_inner_m=r_inner * math.cos(theta),
                    y_inner_m=r_inner * math.sin(theta),
                    x_outer_m=r_outer * math.cos(theta),
                    y_outer_m=r_outer * math.sin(theta),
                    turn_length_m=length,
                )
            )
            global_turn += 1
    return points


def plot_cross_section(g: WindingGeometry, points: list[TurnPoint], output: Path) -> None:
    fig, ax = plt.subplots(figsize=(10, 10))

    # Beschichteter Kern: nur Gesamtaußendurchmesser bekannt; Innenkontur = Rohkern-ID.
    annulus = Wedge(
        (0.0, 0.0),
        g.core_outer_radius_coated_m * 1e3,
        0,
        360,
        width=(g.core_outer_radius_coated_m - g.core_inner_radius_m) * 1e3,
        facecolor="none",
        edgecolor="black",
        hatch="///",
        linewidth=1.2,
    )
    ax.add_patch(annulus)

    # Rohkernkonturen als durchgezogene große Kreise.
    ax.add_patch(Circle((0, 0), g.core_outer_radius_bare_m * 1e3, fill=False, linewidth=1.5))
    ax.add_patch(Circle((0, 0), g.core_inner_radius_m * 1e3, fill=False, linewidth=1.5))

    radius_litz_mm = g.litz_outer_diameter_m * 1e3 / 2.0
    for p in points:
        # Derselbe Windungsindex erscheint innen und außen auf derselben Radiallinie.
        for x, y in ((p.x_inner_m, p.y_inner_m), (p.x_outer_m, p.y_outer_m)):
            ax.add_patch(
                Circle(
                    (x * 1e3, y * 1e3),
                    radius_litz_mm,
                    fill=False,
                    linewidth=1.0,
                )
            )

    limit = max(abs(p.x_outer_m) for p in points) * 1e3 + 1.5 * radius_litz_mm
    ax.set_xlim(-limit, limit)
    ax.set_ylim(-limit, limit)
    ax.set_aspect("equal", adjustable="box")
    ax.set_xlabel("x [mm]")
    ax.set_ylabel("y [mm]")
    ax.set_title(
        f"Maßstäblicher Ringkernquerschnitt: {g.total_turns} Windungen "
        f"({'+'.join(map(str, g.turns_per_layer))})"
    )
    ax.grid(True, linewidth=0.4)
    fig.tight_layout()
    fig.savefig(output.with_suffix(".svg"), bbox_inches="tight")
    fig.savefig(output.with_suffix(".png"), dpi=220, bbox_inches="tight")
    plt.close(fig)


def write_csv(g: WindingGeometry, points: list[TurnPoint], output_dir: Path) -> None:
    with (output_dir / "wicklungspositionen_ringkern.csv").open(
        "w", newline="", encoding="utf-8"
    ) as f:
        writer = csv.writer(f, delimiter=";")
        writer.writerow(
            [
                "Windung",
                "Lage",
                "Windung_in_Lage",
                "Winkel_deg",
                "x_innen_mm",
                "y_innen_mm",
                "x_aussen_mm",
                "y_aussen_mm",
                "Windungslaenge_mm",
            ]
        )
        for p in points:
            writer.writerow(
                [
                    p.turn,
                    p.layer,
                    p.layer_turn,
                    f"{math.degrees(p.theta_rad):.6f}",
                    f"{p.x_inner_m * 1e3:.6f}",
                    f"{p.y_inner_m * 1e3:.6f}",
                    f"{p.x_outer_m * 1e3:.6f}",
                    f"{p.y_outer_m * 1e3:.6f}",
                    f"{p.turn_length_m * 1e3:.6f}",
                ]
            )

    length_without_leads = sum(p.turn_length_m for p in points)
    total_length = length_without_leads + g.lead_length_total_m
    with (output_dir / "wicklungskennwerte.csv").open(
        "w", newline="", encoding="utf-8"
    ) as f:
        writer = csv.writer(f, delimiter=";")
        writer.writerow(["Kennwert", "Wert", "Einheit"])
        writer.writerow(["Windungszahl", g.total_turns, "-"])
        writer.writerow(["Lagen", len(g.turns_per_layer), "-"])
        writer.writerow(["Litzenlaenge_ohne_Anschluesse", f"{length_without_leads:.6f}", "m"])
        writer.writerow(["Anschlusslaenge_gesamt", f"{g.lead_length_total_m:.6f}", "m"])
        writer.writerow(["Litzenlaenge_gesamt", f"{total_length:.6f}", "m"])
        for idx, n_turns in enumerate(g.turns_per_layer):
            writer.writerow([f"Windungslaenge_Lage_{idx + 1}", f"{turn_centerline_length(g, idx):.6f}", "m"])
            writer.writerow([f"Innenbogenabstand_Lage_{idx + 1}", f"{available_inner_arc_pitch(g, idx):.6f}", "m"])
            writer.writerow([f"Max_Windungen_Lage_{idx + 1}", maximum_turns_on_inner_radius(g, idx), "-"])


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=Path("generated"))
    args = parser.parse_args()

    g = WindingGeometry()
    points = calculate_turn_points(g)
    args.output.mkdir(parents=True, exist_ok=True)
    plot_cross_section(g, points, args.output / "wicklungspositionen_ringkern")
    write_csv(g, points, args.output)

    length = sum(p.turn_length_m for p in points) + g.lead_length_total_m
    print(f"Windungen: {g.total_turns}")
    print(f"Lagen: {g.turns_per_layer}")
    print(f"Gesamte Litzenlänge inkl. Anschlüsse: {length:.3f} m")


if __name__ == "__main__":
    main()
