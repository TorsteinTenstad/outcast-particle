import subprocess
import math
inkscape = 'C:\\Program Files\\Inkscape\\bin\\inkscape.exe'

pluss_rect = """<rect
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

red_colors = ['#690000', '#8d0001', '#b30210', '#d22325',
              '#ee4039', '#fe6757', '#ff907b', '#ffb59c']
green_colors = ['#002b00', '#004600', '#006400',
                '#038308', '#2ea12d', '#52c04b', '#73e068', '#98ff89']
blue_colors = ['#002658', '#004177', '#035d97',
               '#237ab7', '#4d96d6', '#6fb3f5', '#92d4ff', '#b6f6ff']
for color_name, colors in zip(['red', 'green', 'blue'], [red_colors, green_colors, blue_colors]):
    for sign in ['-', '+']:
        for i in range(5):
            strength = 5-i
            outer_color = colors[i]
            inner_color = colors[i+1]
            target = f'textures\\particle_{color_name}_{sign}{strength}.png'

            modified_svg_str = svg_str.replace('fill:#969696', f'fill:{inner_color}').replace(
                'fill:#646464', f'fill:{outer_color}')

            if sign == '-':
                modified_svg_str = modified_svg_str.replace(pluss_rect, '')

            subprocess.run([inkscape, '--export-type=png', f'--export-filename={target}',
                            f'--export-width={240}', f'--export-height={240}', '--pipe'], input=modified_svg_str.encode())
