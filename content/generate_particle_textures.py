import subprocess
import math
inkscape = 'C:\\Program Files\\Inkscape\\bin\\inkscape.exe'

colors = ['#00429d', '#2b57a7', '#426cb0', '#5681b9', '#6997c2', '#7daeca', '#93c4d2', '#abdad9', '#caefdf',
          '#ffffe0', '#ffe2ca', '#ffc4b4', '#ffa59e', '#f98689', '#ed6976', '#dd4c65', '#ca2f55', '#b11346', '#93003a']


with open('particle.svg', 'r') as f:
    svg_str = f.read()

for i in range(5):
    strength = 5-i
    for j in [-1, 1]:
        outer_color = colors[j*(i+1) - (1 if j == -1 else 0)]
        inner_color = colors[j*i - (1 if j == -1 else 0)]
        target = f'textures\\paricle_{-j*strength}.png'
        modified_svg_str = svg_str.replace('fill:#969696', f'fill:{outer_color}').replace(
            'fill:#646464', f'fill:{inner_color}')
        subprocess.run([inkscape, '--export-type=png', f'--export-filename={target}',
                        f'--export-width={240}', f'--export-height={240}', '--pipe'], input=modified_svg_str.encode())
