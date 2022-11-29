import subprocess
import math
import cairosvg
inkscape = 'C:\\Program Files\\Inkscape\\bin\\inkscape.exe'

USE_CAIRO = False
EDIT_OUTLINE = True

if False:
    with open('electric_field.svg', 'r') as f:
        svg_str = f.read()

    orange_colors = ['#4b0000', '#7b0c00',
                     '#a33300', '#cb5400', '#f4760b', '#ffa942']

    for i in range(5):
        strength = 5-i
        color = orange_colors[i+1]
        target = f'textures\\electric_field_{strength}.png'

        modified_svg_str = svg_str.replace('fill:#008080', f'fill:{color}')

        subprocess.run([inkscape, '--export-type=png', f'--export-filename={target}',
                        f'--export-width={120}', f'--export-height={120}', '--pipe'], input=modified_svg_str.encode())

pluss_rect = """      <rect
         style="fill:#ffffff;fill-opacity:1;stroke:#000000;stroke-width:0.0896775;stroke-opacity:0"
         id="rect4792"
         width="3"
         height="18"
         x="13.5"
         y="6"
         ry="1.5"
         rx="1.5" />"""


with open('particle.svg', 'r') as f:
    svg_str = f.read()

red_colors = ['#550000', '#610000', '#6e0000', '#7b0000', '#880000', '#960000', '#a40004', '#b1000f', '#be0518', '#ca151f',
              '#d42426', '#de2f2d', '#e83a34', '#f2443b', '#fb4e43', '#ff5e50', '#ff705f', '#ff7f6c', '#ff8e79', '#ff9c85', '#ffa991', '#ffb69d']
green_colors = ['#002500', '#002e00', '#003700', '#004100', '#004b00', '#005500', '#005f00', '#006a00', '#007500', '#007f03',
                '#038a14', '#1a9520', '#2b9f2b', '#39aa36', '#45b440', '#51bf4a', '#5dca54', '#68d55e', '#74e068', '#7feb72', '#8bf67d', '#a2ff93']
blue_colors = ['#001746', '#002051', '#00295c', '#003367', '#003d73', '#00477e', '#00518a', '#005b95', '#0066a1', '#0c70ad',
               '#257ab8', '#3585c3', '#448fce', '#5199d9', '#5da4e5', '#69aff0', '#75b9fc', '#83c5ff', '#90d2ff', '#9ddeff', '#aaebff', '#b7f7ff']

for color_name, colors in zip(['red', 'green', 'blue'], [red_colors, green_colors, blue_colors]):
    for sign in ['-', '+']:
        for i in range(6):
            strength = 5-i
            outer_color = colors[5+i]
            inner_color = colors[7+i]
            target = f'textures\\particle_{color_name}_{sign if strength != 0 else ""}{strength}.png'

            modified_svg_str = svg_str.replace(
                'fill:#969696', f'fill:{inner_color}').replace(
                'fill:#646464', f'fill:{outer_color}')

            if EDIT_OUTLINE:
                modified_svg_str = modified_svg_str.replace(
                    'r="12"', f'r="{12-i}"')

            if sign == '-':
                modified_svg_str = modified_svg_str.replace(pluss_rect, '')

            if strength == 0:
                modified_svg_str = modified_svg_str.replace(
                    'fill:#ffffff', f'fill:{inner_color}')

            scale = [1, 0.85, 0.7, 0.65, 0.5, 0][i]
            modified_svg_str = modified_svg_str.replace(
                'transform="matrix(0.21286172,0,0,0.21286172,11.807074,11.807074)"', f'transform="matrix({scale},0,0,{scale},{15*(1-scale)},{15*(1-scale)})"')

            if USE_CAIRO:
                cairosvg.svg2png(bytestring=modified_svg_str.encode(
                ), write_to=target, output_width=240, output_height=240)
                continue

            subprocess.run([inkscape, '--export-type=png', f'--export-filename={target}',
                            f'--export-width={240}', f'--export-height={240}', '--pipe'], input=modified_svg_str.encode())
