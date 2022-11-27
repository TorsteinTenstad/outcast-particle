import subprocess
import math
inkscape = 'C:\\Program Files\\Inkscape\\bin\\inkscape.exe'

colors = ['#0b3800', '#1f4c0b', '#346116', '#497721', '#5f8d31', '#76a446', '#8dbb5c', '#a4d372', '#cce5b1',
          '#f5f5f5', '#f9d4d2', '#feb2ac', '#f1958f', '#de7b77', '#ca615e', '#b44849', '#9b3137', '#821924', '#670010']

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

for i in range(5):
    strength = 5-i
    for j in [-1, 1]:
        outer_color = colors[-j*(i+1) - (1 if j == 1 else 0)]
        inner_color = colors[-j*i - (1 if j == 1 else 0)]
        target = f'textures\\particle_{j*strength}.png'

        modified_svg_str = svg_str.replace('fill:#969696', f'fill:{outer_color}').replace(
            'fill:#646464', f'fill:{inner_color}')

        if j == -1:
            modified_svg_str = modified_svg_str.replace(pluss_rect, '')

        subprocess.run([inkscape, '--export-type=png', f'--export-filename={target}',
                        f'--export-width={240}', f'--export-height={240}', '--pipe'], input=modified_svg_str.encode())
