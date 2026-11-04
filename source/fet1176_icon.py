"""Original monochrome FET1176 artwork for the two MS+ icon frames."""

import math

from PIL import ImageDraw


_GLYPHS = {
    "F": ("11111", "10000", "10000", "11110", "10000", "10000", "10000"),
    "E": ("11111", "10000", "10000", "11110", "10000", "10000", "11111"),
    "T": ("11111", "00100", "00100", "00100", "00100", "00100", "00100"),
    "1": ("00100", "01100", "00100", "00100", "00100", "00100", "01110"),
    "7": ("11111", "00001", "00010", "00100", "01000", "01000", "01000"),
    "6": ("01110", "10000", "10000", "11110", "10001", "10001", "01110"),
}


def _bold_wordmark(draw, y, text, width):
    """Centered 5x7 block lettering with explicit tracking at pedal scale."""
    tracking = 1 if width <= 72 else 2
    glyph_width = 5
    total = len(text) * glyph_width + (len(text) - 1) * tracking
    x0 = (width - total) // 2
    for char in text:
        glyph = _GLYPHS[char]
        for gy, row in enumerate(glyph):
            for gx, pixel in enumerate(row):
                if pixel == "1":
                    draw.point((x0 + gx, y + gy), fill=255)
        x0 += glyph_width + tracking


def draw_icon(image, manifest):
    """Draw the minimal black-face FET1176/VU design at either native size."""
    w, h = image.size
    draw = ImageDraw.Draw(image)
    scale = w / 72.0
    line = 1

    # Minimal dark face and thin inset frame from the approved reference.
    draw.rectangle((0, 0, w - 1, h - 1), fill=0)
    inset = max(1, round(w * .015))
    draw.rectangle((inset, inset, w - inset - 1, h - inset - 1),
                   outline=255, width=line)

    _bold_wordmark(draw, round(h * .375), manifest["display"], w)

    # Large, uncluttered VU/gain-reduction window.
    left, top = round(w * .11), round(h * .515)
    right, bottom = round(w * .90), round(h * .94)
    draw.rectangle((left, top, right, bottom), outline=255, width=line)
    arc_box = (round(w * .19), round(h * .60),
               round(w * .82), round(h * 1.08))
    draw.arc(arc_box, 200, 340, fill=255, width=line)
    cx = (arc_box[0] + arc_box[2]) / 2.0
    cy = (arc_box[1] + arc_box[3]) / 2.0
    radius = (arc_box[2] - arc_box[0]) / 2.0
    for angle in (205, 230, 255, 280, 305, 330):
        a = math.radians(angle)
        p1 = (round(cx + (radius - scale) * math.cos(a)),
              round(cy + (radius - scale) * math.sin(a)))
        p2 = (round(cx + (radius - 4 * scale) * math.cos(a)),
              round(cy + (radius - 4 * scale) * math.sin(a)))
        draw.line((p1, p2), fill=255, width=line)
    pivot = (round(cx), bottom - max(1, round(2 * scale)))
    needle_a = math.radians(294)
    needle = (round(cx + radius * .92 * math.cos(needle_a)),
              round(cy + radius * .92 * math.sin(needle_a)))
    draw.line((pivot, needle), fill=255, width=line)
