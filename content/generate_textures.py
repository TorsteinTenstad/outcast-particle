from devdraw import *
import numpy as np


def generate_goal_texture():
    goal = SVG(2, 2)
    goal.add(rect(2, 2))
    goal.add(rect(1, 1, 0, 1, color='#000000'))
    goal.add(rect(1, 1, 1, 0, color='#000000'))
    goal.set_inkscape_path('C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    goal.export_png('content\\textures_generated\\goal.png', 120, 120)


def generate_particle_texture(name, outer_color, inner_color, sign=''):
    particle = SVG(30, 30)
    particle.add(circle(15, 15, 15, outer_color))
    particle.add(circle(12, 15, 15, inner_color))
    if sign == '-':
        particle.add(rect(18, 3, 15-18/2, 15-3/2, 3/2))
    if sign == '+':
        particle.add(rect(18, 3, 15-18/2, 15-3/2, 3/2))
        particle.add(rect(3, 18, 15-3/2, 15-18/2, 3/2))
    particle.set_inkscape_path(
        'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    particle.export_png(f'content\\textures_generated\\{name}.png', 120, 120)


def generate_magnetic_field_ring_texture(name, color):
    outer_radius = 10
    thickness = 5
    svg = SVG(30, 30)
    for x in [0, 30]:
        for y in [0, 30]:
            ring = circle(r=outer_radius, x=x, y=y, color=color)
            cut_shape = circle(r=outer_radius-thickness,
                               x=x, y=y, color="#000000")
            white_base = rect(w=30, h=30)
            ring = mask(ring, white_base+cut_shape)
            svg.add(ring)
    svg.set_inkscape_path(
        'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    svg.export_png(f'content\\textures_generated\\{name}.png', 120, 120)


def generate_magnetic_field_x_texture(name, color):
    length = 24
    thickness = 5
    svg_w, svg_h = 30, 30
    svg = SVG(svg_w, svg_h)
    stick = transform(rect(length, thickness, -length/2, -
                      thickness/2, thickness/2, color=color), rotation=[45])
    for x in [0, 90, 180, 270]:
        svg.add(transform(stick, rotation=[x, svg_w/2, svg_h/2]))
    svg.set_inkscape_path(
        'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    svg.export_png(f'content\\textures_generated\\{name}.png', 120, 120)


def generate_electric_field_textrue(name, color):
    svg_w, svg_h = 30, 30
    svg = SVG(svg_w, svg_h)
    tail_w = 6
    tail_l = 6
    head_w = 12
    head_l = 9
    margin_w = (svg_w-head_w)/2
    margin_h = (svg_h-tail_l-head_l)/2
    tail = rect(tail_w, tail_l, (svg_w-tail_w)/2, margin_h, color=color)
    head = polygon([[margin_w, margin_h+tail_l], [svg_w-margin_w,
                   margin_h+tail_l], [svg_w/2, svg_h-margin_h]], color=color)
    svg.add(tail+head)
    svg.set_inkscape_path(
        'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    svg.export_png(f'content\\textures_generated\\{name}.png', 120, 120)


def button_texture(w, h, borders_width):
    grays = ['#1a1a1a', '#363636', '#555555', '#767676', '#999999']
    outer_border_color = grays[0]
    dark_border_color = grays[1]
    light_border_color = grays[3]
    body_color = grays[2]
    for x in ['', 'pressed_']:
        if x == 'pressed_':
            dark_border_color, light_border_color = light_border_color, dark_border_color
        svg = SVG(w, h)
        t = borders_width
        svg.add(rect(w, h, color=outer_border_color))
        svg.add(rect(w-2*t, h-2*t, t, t, color=dark_border_color))
        svg.add(polygon(
            [[t, t], [w-t, t], [w-2*t, 2*t], [2*t, h-2*t], [t, h-t]], color=light_border_color))
        svg.add(rect(w-4*t, h-4*t, 2*t, 2*t, color=body_color))
        svg.set_inkscape_path(
            'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
        svg.export_png(
            f'content\\textures_generated\\{x}button_{w}_{h}.png', w, h)


def laser_texture():
    w = 10
    t = 3
    zag = 5
    h = 15
    svg_w = w+zag
    svg = SVG(svg_w, h)
    mid_line_xs = svg_w*np.ones(4)/2
    mid_line_xs[1] += zag/2
    mid_line_xs[2] -= zag/2
    for color, offset in zip(['#fcbf4a', '#ffe547'], [w/2, w/2-t]):
        xs = np.hstack((np.copy(mid_line_xs)+offset,
                        np.copy(mid_line_xs[::-1])-offset))
        mid_line_ys = [0, h/4, 3*h/4, h]
        ys = np.hstack((np.copy(mid_line_ys), mid_line_ys[::-1]))
        points = np.vstack((xs, ys)).T
        svg.add(polygon(points, color=color))
    svg.set_inkscape_path(
        'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    svg.export_png(
        f'content\\textures\\laser_vertical.png', svg_w*4, h*4)


if __name__ == '__main__':

    GENERATE_MAGNETIC_FELD_TEXTURES = False
    GENERATE_BUTTON_TEXTURES = False
    GENERATE_LASER_TEXTURE = True

    if GENERATE_LASER_TEXTURE:
        laser_texture()
    if GENERATE_BUTTON_TEXTURES:
        for (w, h) in [[3072, 432], [700, 432]]:
            button_texture(w, h, 24)
    if GENERATE_MAGNETIC_FELD_TEXTURES:
        purple_colors = ['#1b0044', '#441e6c',
                         '#6c4294', '#9467bd', '#be8ee8', '#ecbaff']
        for i in range(5):
            color = purple_colors[i]
            strength = 5-i
            generate_magnetic_field_x_texture(
                f'magnetic_field_x{strength}', color)
            generate_magnetic_field_ring_texture(
                f'magnetic_field_o{strength}', color)
