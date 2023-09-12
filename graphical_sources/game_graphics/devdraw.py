import subprocess


class SVG:
    def __init__(self, w, h):
        self.inkscape_path = ''
        self.xml_begin = f"""<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!-- Created with Dev-Draw (https://github.com/TorsteinTenstad/dev-draw) -->

<svg
    width="{w}"
    height="{h}"
    xmlns="http://www.w3.org/2000/svg"
    xmlns:svg="http://www.w3.org/2000/svg">"""
        self.xml_body = ''
        self.xml_end = """
</svg>"""

    def add(self, xml_string):
        self.xml_body += xml_string

    def set_inkscape_path(self, path):
        self.inkscape_path = path

    def save(self, filename):
        xml = self.xml_begin + self.xml_body + self.xml_end
        with open(filename, 'w') as file:
            file.write(xml)

    def export_png(self, filename, output_w, output_h):
        xml = self.xml_begin + self.xml_body + self.xml_end
        subprocess.run([self.inkscape_path, '--export-type=png', f'--export-filename={filename}',
                        f'--export-width={output_w}', f'--export-height={output_h}', '--pipe'], input=xml.encode())


def rect(w, h, x=0, y=0, r=0, color='#ffffff', opacity=1):
    return f"""
<rect
    style="opacity:{opacity};fill:{color};"
    width="{w}"
    height="{h}"
    x="{x}"
    y="{y}"
    rx="{r}"
    ry="{r}" />"""


def circle(r, x=0, y=0, color='#ffffff', opacity=1):
    return f"""
<ellipse
    style="opacity:{opacity};fill:{color};"
    rx="{r}"
    ry="{r}"
    cx="{x}"
    cy="{y}"/>"""


def polygon(points, color='#ffffff', opacity=1):
    points = ' '.join([f'{p[0]},{p[1]}' for p in points])
    return f"""
<polygon
    style="opacity:{opacity};fill:{color};"
    points="{points}" />"""


def get_next_operation_id():
    get_next_operation_id.id += 1
    return get_next_operation_id.id


get_next_operation_id.id = 0


def mask(eliment, mask):
    operation_id = get_next_operation_id()
    return f"""
<defs>
    <mask id="{operation_id}">
        {mask}
    </mask>
</defs>
<g mask = "url(#{operation_id})">
    {eliment}
</g>"""


def transform(eliment, rotation=None, translation=None, scale=None):
    return f"""
<g
    transform="
		{f'rotate({" ".join([str(x) for x in rotation])})' if rotation is not None else ''}
		">
	{eliment}
</g>"""
