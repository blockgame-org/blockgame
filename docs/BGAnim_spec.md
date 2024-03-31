# BlockGame Animation (.bganim) spec

The spec for the BlockGame Animation format is very very similar to the
specification for .obj and .mtl

## All Ops

```
# This is a comment!

# ! UNIMPLEMENTED YET
# # specifies the existence of an animation variable
# var <varname>

# * IMPLEMENTED

# set animation duration
duration <seconds
# set animation repeat mode (play once | repeat | hold)
rmode <0|1|2>

# select an object to animate
o <object name>

# keyframe for an object
k <timestamp in seconds>

# set the current pivot point at a keyframe (x, y, z)
piv <num or var> <num or var> <num or var>

# set rotation at a keyframe (x, y, z)
r <num or var> <num or var> <num or var>
# set translation at keyframe (x, y, z)
t <num or var> <num or var> <num or var>
# set scale at keyframe (x, y, z)
s <num or var> <num or var> <num or var>

# The following ops are used for interpolation, the default is linear

# set bezier mode (channel, v1, t1, v2, t2)
# interpolation channels:
#  - rx: rotation.x
#  - ry: rotation.y
#  - rz: rotation.z
#  - tx: translation.x
#  - ty: translation.y
#  - tz: translation.z
#  - sx: scale.x
#  - sy: scale.y
#  - sz: scale.z
#  - r: rotation
#  - t: translation
#  - s: scale

# Note, values and times are relative to the current value and time.

ibez <channel> <num or var> <num or var> <num or var> <num or var>

# interpolation mode for this keyframe (channel, linear | bezier | catmullrom)
imode <channel> <0|1|2>
```