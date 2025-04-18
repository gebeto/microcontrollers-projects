from pprint import pprint


def read_bmp(filename):
    from PIL import Image
    import numpy as np

    path = filename
    image = Image.open(path)
    return np.asarray(image)


def extract_sprite(array2d, x, y, width, height):
    sprite = []
    for i in range(width):
        row = array2d[y + i][x:x + width]
        sprite.append(row)
    return sprite

def render_sprite(sp):
    print("{", end="")
    for row in sp:
        for cell in row:
            # print( ">>>> SSS", cell)
            formatted = "{g}, {r}, {b}, 0".format(r=cell[0], g=cell[1], b=cell[2])
            # formatted = "0, 255, 0, 0".format(r=cell[0], g=cell[1], b=cell[2])
            print("{" + formatted + "}", end=",")
    print("},", end="\n")


# print(len(pixels[0]), pixels[0][1])
# print(pixels[1][2])
# print(len(pixels), len(pixels[-1]), len(pixels[0]), len(pixels[-2]))
pixels = read_bmp('sprites.png')
render_sprite(extract_sprite(pixels, 13 * 8, 8, 8, 8))
render_sprite(extract_sprite(pixels, 14 * 8, 8, 8, 8))

# pixels = read_bmp('heart.png')
# render_sprite(extract_sprite(pixels, 0, 0, 8, 8))
# render_sprite(extract_sprite(pixels, 8, 0, 8, 8))
# render_sprite(extract_sprite(pixels, 16, 0, 8, 8))
