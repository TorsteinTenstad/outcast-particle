from devdraw import *


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


if __name__ == '__main__':
    purple_colors = ['#1b0044', '#441e6c',
                     '#6c4294', '#9467bd', '#be8ee8', '#ecbaff']
    for i in range(5):
        color = purple_colors[i]
        strength = 5-i
        generate_magnetic_field_x_texture(f'magnetic_field_x{strength}', color)
        generate_magnetic_field_ring_texture(
            f'magnetic_field_o{strength}', color)
