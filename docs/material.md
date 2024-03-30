# Material files (.mtl)

Although we support most of the material file spec, our engine cannot render
all of the information contained in the usual `.mtl` file.

## Transmission Filter Color
- This only supports the RGB mode.
- Does not allow use of `.rfl` files
- Does not allow using CIEXYZ

## (planned) Supported Illumination Models:

- 0. Color on and Ambient off
- 1. Color on and Ambient on
- 9. Transparency: Glass on, Reflection: Ray trace off

## (planned) Supported Texture Options:

