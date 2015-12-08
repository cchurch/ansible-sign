#!/usr/bin/env python

# Python
import sys

# Pillow
from PIL import Image

# Jinja2
import jinja2


def get_img_data(filename):
    img = Image.open(filename)
    img = img.quantize(32).convert('RGBA')
    colors = [x[1] for x in img.getcolors()]
    data = list(img.getdata())
    rows = []
    for rowpixel in xrange(0, len(data), img.width):
        rowdata = data[rowpixel:rowpixel + img.width]
        rowmap = [colors.index(x) for x in rowdata]
        row = []
        for rowm in rowmap:
            try:
                if row[-1] == rowm:
                    row[-2] += 1
                    continue
            except IndexError:
                pass
            row.extend([1, rowm])
        rows.append(row)
    img_size = sum([len(r) + 1 for r in rows]) + len(rows) + 1
    if img_size > 22000:
        raise ValueError('given image pixels use %d bytes, max is %d' % (img_size, 22000))
    color_map = [dict(zip('rgba', x)) for x in colors]
    color_size = 4 * len(colors) + 4
    return {'color_map': color_map, 'rows': rows}


def output_ino(img_data):
    loader = jinja2.FileSystemLoader(searchpath='.')
    env = jinja2.Environment(loader=loader)
    template = env.get_template('sign_from_img.ino.j2')
    return template.render(img_data)


def main():
    img_data = get_img_data(sys.argv[1]) or {}
    print output_ino(img_data).encode('utf-8')


if __name__ == '__main__':
    main()
