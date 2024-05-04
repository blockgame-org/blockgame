# Built to change blockbench files into .bganim files

import json
import sys

channels = {
    'rotation': 'r',
    'position': 't',
    'scale': 's'
}


class AnimMotion:
    def __init__(self, channel, values, interpolation):
        self.channel = channel
        self.interpolation = interpolation
        self.values = values
        self.bezier_left_value = [0, 0, 0]
        self.bezier_left_time = [0, 0, 0]
        self.bezier_right_value = [0, 0, 0]
        self.bezier_right_time = [0, 0, 0]

    def output_single_bezier(self, channel, component, v1, t1, v2, t2):
        return f'ibez {channel}{component} {v1} {t1} {v2} {t2}'

    def output_single_interp_mode(self, channel, component):
        interp_mode = 0
        match self.interpolation:
            case "linear":
                interp_mode = 1
            case "bezier":
                interp_mode = 2
            case "catmullrom":
                interp_mode = 3

        return f'imode {channel}{component} {interp_mode}'

    def output_interp_mode(self):
        channel = channels[self.channel]
        return self.output_single_interp_mode(channel, '')
        # code to use if blockbench ever support individual channel interpolation modes
        # x = self.output_single_interp_mode(channel, 'x')
        # y = self.output_single_interp_mode(channel, 'y')
        # z = self.output_single_interp_mode(channel, 'z')
        # return f'{x}\n{y}\n{z}'

    def output_bezier(self) -> str:
        channel = channels[self.channel]
        x = self.output_single_bezier(channel, 'x',
                                      self.bezier_left_value[0], self.bezier_left_time[0],
                                      self.bezier_right_value[0], self.bezier_right_time[0])
        y = self.output_single_bezier(channel, 'y',
                                      self.bezier_left_value[1], self.bezier_left_time[1],
                                      self.bezier_right_value[1], self.bezier_right_time[1])
        z = self.output_single_bezier(channel, 'z',
                                      self.bezier_left_value[2], self.bezier_left_time[2],
                                      self.bezier_right_value[2], self.bezier_right_time[2])
        return f'{x}\n{y}\n{z}'

    def output(self):
        channel = channels[self.channel]
        values = ' '.join((str(value) for value in self.values))
        out = f'{channel} {values}'
        if self.interpolation == "bezier":
            out += f'\n{self.output_bezier()}'
        out += f'\n{self.output_interp_mode()}'
        return out


class Keyframe:
    def __init__(self, timestamp, pivot: tuple[float, float, float]):
        self.timestamp = timestamp
        self.motions = []
        self.pivot = pivot

    def combine_keyframes(self, other):
        self.motions += other.motions

    def add_motion(self, motion):
        self.motions.append(motion)

    def output_motions(self) -> str:
        motions = '\n'.join((x.output() for x in self.motions))
        return motions

    def output(self) -> str:
        motions = self.output_motions()
        return f'k {self.timestamp}\n' + \
               motions


class AnimBone:
    def __init__(self, name, pivot):
        self.name = name
        self.objects = []
        self.pivot = pivot

    def add_keyframe(self, keyframe):
        for obj in self.objects:
            obj.add_keyframe(keyframe)

    def add_object(self, obj):
        obj.pivot = self.pivot
        self.objects.append(obj)

    def output(self) -> str:
        return '\n'.join((x.output() for x in self.objects))


class AnimObject:
    def __init__(self, name, pivot):
        self.name = name
        self.keyframes = {}  # time: keyframe
        self.pivot = pivot

    def add_keyframe(self, keyframe):
        if keyframe.timestamp in self.keyframes.keys():
            self.keyframes[keyframe.timestamp].combine_keyframes(keyframe)
        else:
            self.keyframes[keyframe.timestamp] = keyframe

    def output(self) -> str:
        keyframes = '\n'.join((x.output() for x in self.keyframes.values()))
        out = f'o {self.name}\n{keyframes}'
        return out


class Animation:
    def __init__(self, anim_length, loop_mode):
        self.objects = []
        self.anim_length = anim_length
        self.loop_mode = loop_mode

    def add_object(self, obj):
        self.objects.append(obj)

    def output(self) -> str:
        repeat_mode = 0
        match self.loop_mode:
            case "once":
                repeat_mode = 0
            case "repeat":
                repeat_mode = 1
            case "hold":
                repeat_mode = 2

        objects = '\n\n'.join((x.output() for x in self.objects))
        return f"duration {self.anim_length}\n" + \
               f"rmode {repeat_mode}\n" + \
               objects


def parse_bone(data: dict, objects):
    bone = AnimBone(data["name"], data["origin"])
    for obj in data["children"]:
        bone.add_object(objects[obj])
    return bone


def parse_cube(data: dict):
    return AnimObject(data["name"], data["origin"])


def parse_keyframe(data: dict):
    keyframe = Keyframe(float(data["time"]), data["interpolation"])
    motion = AnimMotion(data["channel"], data["data_points"][0].values(),
                        data["interpolation"])

    motion.bezier_left_time = data["bezier_left_time"]
    motion.bezier_left_value = data["bezier_left_value"]
    motion.bezier_right_time = data["bezier_right_time"]
    motion.bezier_right_value = data["bezier_right_value"]

    keyframe.add_motion(motion)
    return keyframe


# `animation` acts as an out param
def parse_animator(data: dict, animation, objects):
    for uuid, animator in data.items():
        obj = objects[uuid]
        for keyframe in animator["keyframes"]:
            obj.add_keyframe(parse_keyframe(keyframe))
        animation.add_object(obj)


def main():
    if len(sys.argv) < 2:
        print("You must specify a .bbmodel file to use")
        return

    with open(sys.argv[1]) as f:
        file_content = json.load(f)

    objects = {}
    elements = file_content["elements"]
    for element in elements:
        objects[element["uuid"]] = parse_cube(element)
    outliner = file_content["outliner"]
    for bone in outliner:
        if isinstance(bone, str) or not bone["export"]:
            continue
        objects[bone["uuid"]] = parse_bone(bone, objects)

    animations_raw = file_content["animations"]
    animations = {}
    for animation in animations_raw:
        name = animation["name"].split('.')[0]
        loop = animation["loop"]
        duration = animation["length"]
        anim = Animation(duration, loop)
        animations[animation['uuid']] = anim

        parse_animator(animation["animators"], anim, objects)

        with open(f'{name}.bganim', 'w+') as f:
            f.write(anim.output())


if __name__ == '__main__':
    main()
