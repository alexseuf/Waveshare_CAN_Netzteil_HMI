"""Parametrisches Positions- und Längenmodell für Ringkernwicklungen.

Das Modell verwendet die in der Formelsammlung dokumentierte Abschätzung
für den Litzenaußendurchmesser:

    A_Cu = n * pi*d_einz^2/4
    A_ges = A_Cu / eta_pack
    d_Litze = sqrt(4*A_ges/pi) = d_einz*sqrt(n/eta_pack)

Jede Windung besitzt im maßstäblichen Draufschnitt je einen Schnittpunkt auf
der Innen- und Außenseite. Beide Punkte liegen auf derselben Radiallinie.
Mehrere Lagen werden radial versetzt und gegeneinander um einen halben
Winkelabstand gestaffelt.
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
class ToroidWinding:
    # Rohkern C058110A2
    d_outer_bare_m: float = 57.15e-3
    d_inner_bare_m: float = 35.56e-3
    h_single_m: float = 13.97e-3
    core_count: int = 3

    # Für die Beschichtung wird entsprechend der Vorgabe nur der
    # Gesamtaußendurchmesser verwendet. Diesen Wert ggf. durch Messwert ersetzen.
    d_outer_coated_total_m: float = 59.20e-3

    # Wicklung
    turns_per_layer: tuple[int, ...] = (28, 20)
    strand_count: int = 630
    strand_diameter_m: float = 0.10e-3
    packing_factor: float = 0.68

    # Abstände
    core_clearance_m: float = 0.10e-3
    turn_gap_m: float = 0.05e-3
    interlayer_gap_m: float = 0.10e-3
    lead_length_total_m: float = 0.20

    @property
    def turns(self) -> int:
        return sum(self.turns_per_layer)

    @property
    def h_stack_m(self) -> float:
        return self.core_count * self.h_single_m

    @property
    def r_inner_m(self) -> float:
        return self.d_inner_bare_m / 2

    @property
    def r_outer_bare_m(self) -> float:
        return self.d_outer_bare_m / 2

    @property
    def r_outer_coated_m(self) -> float:
        return self.d_outer_coated_total_m / 2

    @property
    def copper_area_m2(self) -> float:
        return self.strand_count * math.pi * self.strand_diameter_m**2 / 4

    @property
    def litz_area_m2(self) -> float:
        return self.copper_area_m2 / self.packing_factor

    @property
    def litz_diameter_m(self) -> float:
        return math.sqrt(4 * self.litz_area_m2 / math.pi)

    @property
    def radial_core_width_m(self) -> float:
        return self.r_outer_coated_m - self.r_inner_m


@dataclass(frozen=True)
class Turn:
    number: int
    layer: int
    number_in_layer: int
    theta_rad: float
    inner_x_m: float
    inner_y_m: float
    outer_x_m: float
    outer_y_m: float
    length_m: float


def centerline_offset(g: ToroidWinding, layer0: int) -> float:
    """Normalabstand der Litzenmittellinie von der Kernoberfläche."""
    return (
        g.core_clearance_m
        + g.litz_diameter_m / 2
        + layer0 * (g.litz_diameter_m + g.interlayer_gap_m)
    )


def conductor_radii(g: ToroidWinding, layer0: int) -> tuple[float, float]:
    a = centerline_offset(g, layer0)
    return g.r_inner_m - a, g.r_outer_coated_m + a


def turn_length(g: ToroidWinding, layer0: int) -> float:
    """Mittellinienlänge einer Windung.

    Der gestapelte Kern wird im meridionalen Schnitt als Rechteck mit Höhe h
    und radialer Breite b angesetzt. Die Leiter-Mittellinie ist eine parallel
    versetzte, an den Ecken abgerundete Kontur. Damit gilt:

        l_Wdg,k = 2*(h+b) + 2*pi*a_k

    mit a_k als Abstand der Mittellinie von der Kernoberfläche.
    """
    a = centerline_offset(g, layer0)
    return 2 * (g.h_stack_m + g.radial_core_width_m) + 2 * math.pi * a


def max_turns_inner(g: ToroidWinding, layer0: int) -> int:
    r_i, _ = conductor_radii(g, layer0)
    pitch_required = g.litz_diameter_m + g.turn_gap_m
    return math.floor(2 * math.pi * r_i / pitch_required)


def calculate(g: ToroidWinding) -> list[Turn]:
    if g.d_outer_coated_total_m < g.d_outer_bare_m:
        raise ValueError("Gesamtaußendurchmesser mit Beschichtung ist zu klein.")

    result: list[Turn] = []
    n_global = 1
    for layer0, n_layer in enumerate(g.turns_per_layer):
        r_i, r_o = conductor_radii(g, layer0)
        if r_i <= g.litz_diameter_m / 2:
            raise ValueError(f"Lage {layer0+1} passt nicht in die Kernöffnung.")

        delta = 2 * math.pi / n_layer
        # Zweite und weitere Lagen liegen versetzt in den Lücken der vorherigen.
        theta_offset = 0 if layer0 == 0 else delta / 2
        arc_pitch_inner = r_i * delta
        pitch_required = g.litz_diameter_m + g.turn_gap_m
        if arc_pitch_inner < pitch_required:
            raise ValueError(
                f"Lage {layer0+1}: {n_layer} Windungen nicht möglich; "
                f"Innenbogenabstand={arc_pitch_inner*1e3:.3f} mm, "
                f"erforderlich={pitch_required*1e3:.3f} mm, "
                f"maximal={max_turns_inner(g, layer0)}."
            )

        l_turn = turn_length(g, layer0)
        for j in range(n_layer):
            theta = (j * delta + theta_offset) % (2 * math.pi)
            result.append(
                Turn(
                    number=n_global,
                    layer=layer0 + 1,
                    number_in_layer=j + 1,
                    theta_rad=theta,
                    inner_x_m=r_i * math.cos(theta),
                    inner_y_m=r_i * math.sin(theta),
                    outer_x_m=r_o * math.cos(theta),
                    outer_y_m=r_o * math.sin(theta),
                    length_m=l_turn,
                )
            )
            n_global += 1
    return result


def draw(g: ToroidWinding, turns: list[Turn], out: Path) -> None:
    fig, ax = plt.subplots(figsize=(10, 10))

    # Schraffierte Gesamtgeometrie mit Beschichtung. Da nur der beschichtete
    # Außendurchmesser vorliegt, wird innen die Rohkernöffnung beibehalten.
    ax.add_patch(
        Wedge(
            (0, 0),
            g.r_outer_coated_m * 1e3,
            0,
            360,
            width=(g.r_outer_coated_m - g.r_inner_m) * 1e3,
            fill=False,
            hatch="///",
            linewidth=1.1,
        )
    )
    # Durchgezogene Rohkernkonturen.
    ax.add_patch(Circle((0, 0), g.r_outer_bare_m * 1e3, fill=False, linewidth=1.8))
    ax.add_patch(Circle((0, 0), g.r_inner_m * 1e3, fill=False, linewidth=1.8))

    r_wire_mm = g.litz_diameter_m * 500
    for t in turns:
        for x, y in ((t.inner_x_m, t.inner_y_m), (t.outer_x_m, t.outer_y_m)):
            ax.add_patch(
                Circle((x * 1e3, y * 1e3), r_wire_mm, fill=False, linewidth=1.0)
            )
        # Dünne Hilfslinie verbindet Innen- und Außenschnitt derselben Windung.
        ax.plot(
            [t.inner_x_m * 1e3, t.outer_x_m * 1e3],
            [t.inner_y_m * 1e3, t.outer_y_m * 1e3],
            linewidth=0.25,
            alpha=0.35,
        )

    max_r = max(math.hypot(t.outer_x_m, t.outer_y_m) for t in turns)
    lim = (max_r + g.litz_diameter_m) * 1e3
    ax.set_xlim(-lim, lim)
    ax.set_ylim(-lim, lim)
    ax.set_aspect("equal")
    ax.set_xlabel("x [mm]")
    ax.set_ylabel("y [mm]")
    ax.set_title(
        f"Ringkernwicklung maßstäblich: N={g.turns}, "
        f"Lagen={'+'.join(str(v) for v in g.turns_per_layer)}, "
        f"d_Litze={g.litz_diameter_m*1e3:.2f} mm"
    )
    ax.grid(True, linewidth=0.4)
    fig.tight_layout()
    fig.savefig(out.with_suffix(".svg"), bbox_inches="tight")
    fig.savefig(out.with_suffix(".png"), dpi=220, bbox_inches="tight")
    plt.close(fig)


def export_csv(g: ToroidWinding, turns: list[Turn], out_dir: Path) -> None:
    with (out_dir / "wicklungspositionen_ringkern.csv").open(
        "w", newline="", encoding="utf-8"
    ) as f:
        w = csv.writer(f, delimiter=";")
        w.writerow([
            "Windung", "Lage", "Windung_in_Lage", "Winkel_deg",
            "x_innen_mm", "y_innen_mm", "x_aussen_mm", "y_aussen_mm",
            "Windungslaenge_mm"
        ])
        for t in turns:
            w.writerow([
                t.number, t.layer, t.number_in_layer,
                f"{math.degrees(t.theta_rad):.6f}",
                f"{t.inner_x_m*1e3:.6f}", f"{t.inner_y_m*1e3:.6f}",
                f"{t.outer_x_m*1e3:.6f}", f"{t.outer_y_m*1e3:.6f}",
                f"{t.length_m*1e3:.6f}"
            ])

    total_no_leads = sum(t.length_m for t in turns)
    with (out_dir / "wicklungskennwerte.csv").open(
        "w", newline="", encoding="utf-8"
    ) as f:
        w = csv.writer(f, delimiter=";")
        w.writerow(["Kennwert", "Wert", "Einheit"])
        w.writerow(["Kupferquerschnitt", f"{g.copper_area_m2*1e6:.6f}", "mm2"])
        w.writerow(["Litzenquerschnitt_geometrisch", f"{g.litz_area_m2*1e6:.6f}", "mm2"])
        w.writerow(["Litzenaußendurchmesser", f"{g.litz_diameter_m*1e3:.6f}", "mm"])
        w.writerow(["Litzenlänge_ohne_Anschlüsse", f"{total_no_leads:.6f}", "m"])
        w.writerow(["Litzenlänge_gesamt", f"{total_no_leads+g.lead_length_total_m:.6f}", "m"])
        for layer0, n_layer in enumerate(g.turns_per_layer):
            r_i, r_o = conductor_radii(g, layer0)
            w.writerow([f"Windungszahl_Lage_{layer0+1}", n_layer, "-"])
            w.writerow([f"Radius_innen_Lage_{layer0+1}", f"{r_i*1e3:.6f}", "mm"])
            w.writerow([f"Radius_außen_Lage_{layer0+1}", f"{r_o*1e3:.6f}", "mm"])
            w.writerow([f"Windungslänge_Lage_{layer0+1}", f"{turn_length(g, layer0):.6f}", "m"])
            w.writerow([f"Max_Windungen_innen_Lage_{layer0+1}", max_turns_inner(g, layer0), "-"])


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=Path("generated"))
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)

    g = ToroidWinding()
    turns = calculate(g)
    draw(g, turns, args.output / "wicklungspositionen_ringkern")
    export_csv(g, turns, args.output)

    total = sum(t.length_m for t in turns) + g.lead_length_total_m
    print(f"d_Litze = {g.litz_diameter_m*1e3:.3f} mm")
    print(f"N = {g.turns}, Aufteilung = {g.turns_per_layer}")
    print(f"Litzenlänge gesamt = {total:.3f} m")


if __name__ == "__main__":
    main()
