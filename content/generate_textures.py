from devdraw import *
import numpy as np


def generate_goal_texture():
    goal = SVG(2, 2)
    goal.add(rect(2, 2))
    goal.add(rect(1, 1, 0, 1, color='#000000'))
    goal.add(rect(1, 1, 1, 0, color='#000000'))
    goal.set_inkscape_path('C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    goal.export_png('content\\textures_generated\\goal.png', 120, 120)


def generate_particle_texture(name, outer_color, inner_color, sign='', scale = 1):
    inner_r = 12*scale
    particle = SVG(30, 30)
    particle.add(circle(15, 15, 15, outer_color))
    particle.add(circle(inner_r, 15, 15, inner_color))
    sign_size = 18*scale
    sight_thickness = 3*scale
    if sign == '-':
        particle.add(rect(sign_size, sight_thickness, 15-sign_size/2, 15-sight_thickness/2, sight_thickness/2))
    if sign == '+':
        particle.add(rect(sign_size, sight_thickness, 15-sign_size/2, 15-sight_thickness/2, sight_thickness/2))
        particle.add(rect(sight_thickness, sign_size, 15-sight_thickness/2, 15-sign_size/2, sight_thickness/2))
    particle.set_inkscape_path(
        'C:\\Program Files\\Inkscape\\bin\\inkscape.exe')
    particle.export_png(f'content\\textures_generated\\{name}.png', 240, 240)


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
    GENERATE_LASER_TEXTURE = False
    GENERATE_PARTICLE_TEXTURES = False
    GENERATE_NEUTRAL_PLAYER_TEXTURES = True

    if GENERATE_PARTICLE_TEXTURES:
        red_colors = ['#550000', '#610000', '#6e0000', '#7b0000', '#880000', '#960000', '#a40004', '#b1000f', '#be0518', '#ca151f',
              '#d42426', '#de2f2d', '#e83a34', '#f2443b', '#fb4e43', '#ff5e50', '#ff705f', '#ff7f6c', '#ff8e79', '#ff9c85', '#ffa991', '#ffb69d']
        green_colors = ['#002500', '#002e00', '#003700', '#004100', '#004b00', '#005500', '#005f00', '#006a00', '#007500', '#007f03',
                '#038a14', '#1a9520', '#2b9f2b', '#39aa36', '#45b440', '#51bf4a', '#5dca54', '#68d55e', '#74e068', '#7feb72', '#8bf67d', '#a2ff93']
        blue_colors = ['#001746', '#002051', '#00295c', '#003367', '#003d73', '#00477e', '#00518a', '#005b95', '#0066a1', '#0c70ad',
               '#257ab8', '#3585c3', '#448fce', '#5199d9', '#5da4e5', '#69aff0', '#75b9fc', '#83c5ff', '#90d2ff', '#9ddeff', '#aaebff', '#b7f7ff']

        for color_name, colors in zip(['red', 'green', 'blue'], [red_colors, green_colors, blue_colors]):
            for sign in ['', '-', '+']:
                for i in range(6):
                    scale = [1, 0.85, 0.7, 0.65, 0.5, 0][i]
                    strength = 5-i
                    outer_color = colors[5+i]
                    inner_color = colors[7+i]
                    name = f'particle_{color_name}_{sign}{strength}'
                    generate_particle_texture(name, outer_color, inner_color, sign, scale)
    if GENERATE_LASER_TEXTURE:
        laser_texture()
    if GENERATE_BUTTON_TEXTURES:
        for (w, h) in [[3072, 432], [700, 432],[6344,432]]:
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
